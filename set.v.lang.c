#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define BORDER_WIDTH 5
#define APP_PATH_SIZE 500
// #define SIGTERM_MSG "SIGTERM received.\n"
#define UTIL_PATH "/soft/v.color.lang/xkblayout-state print \"\%s\""

const char LOCKFILE[] = "/tmp/vlang.lock";

// Define a signal handler function
void sig_handler(int signum) {
    printf("\nПолучен сигнал номер %d\n", signum);
    printf("Завершение приложения.\n");
    unlink(LOCKFILE); // Удаляем файл блокировки перед выходом    
    exit(signum);  // Graceful shutdown
}

void get_active_lang(char* app_path, char* out)
{
    FILE *fp;
    char buffer[100];
    char c;
    size_t read_size;

    /* Open the command for reading. */
    fp = popen(app_path, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    // Читаем вывод команды большими блоками
    while((read_size = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        //fwrite(buffer, 1, read_size, stdout); // Выводим результат на экран
        strncat(out,buffer,read_size);
    }

    /* close */
    pclose(fp);
}

void DrawLineTopBottomDesktop(Display *dpy/*, Window target_window*/, char *app_path, int px, int py, int pwidth, int pheight) {
    XSetWindowAttributes attr = {0};
    XGCValues gcv = {0};
    XVisualInfo vinfo;
    GC gct, gcb;
    Window wt, wb;

    XMatchVisualInfo(dpy, DefaultScreen(dpy), 32, TrueColor, &vinfo);
    attr.colormap = XCreateColormap(dpy, DefaultRootWindow(dpy), vinfo.visual, AllocNone);
    attr.override_redirect = True; // Невмешивается в оконный менеджер

    XColor color_orange;
    // char orangeDark[] = "#FF8000";
    char orangeDark[] = "#FF0000";
    XParseColor(dpy, attr.colormap, orangeDark, &color_orange);
    XAllocColor(dpy, attr.colormap, &color_orange);

    XColor color_blue;
    char blueSky[] = "#007bff";
    XParseColor(dpy, attr.colormap, blueSky, &color_blue);
    XAllocColor(dpy, attr.colormap, &color_blue);

    wt = XCreateWindow(dpy, DefaultRootWindow(dpy), 0, 0,
                      pwidth, BORDER_WIDTH, BORDER_WIDTH, vinfo.depth,
                      CopyFromParent, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attr);
    wb = XCreateWindow(dpy, DefaultRootWindow(dpy), px, py+pheight,
                      pwidth, BORDER_WIDTH, BORDER_WIDTH, vinfo.depth,
                      CopyFromParent, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attr);

    gcv.line_width = BORDER_WIDTH;
    gct = XCreateGC(dpy, wt, GCLineWidth, &gcv);
    gcb = XCreateGC(dpy, wb, GCLineWidth, &gcv);

    XMapWindow(dpy, wt);
    XMapWindow(dpy, wb);
    XSync(dpy, False);

    char ppp[2000];
    char prev_lang[3];

    while (1) {

        sleep(3);
        ppp[0] = '\0';
        get_active_lang(app_path, ppp);

        if (strncmp(ppp, prev_lang, 3)!=0) {
    //        printf("%s\n", ppp);
            if (strcmp(ppp,"us")==0) {
                XSetForeground(dpy, gct, color_orange.pixel);
                XSetForeground(dpy, gcb, color_orange.pixel);
            }
            else {
                XSetForeground(dpy, gct, color_blue.pixel);
                XSetForeground(dpy, gcb, color_blue.pixel);
            }

            XDrawRectangle(dpy, wt, gct, 0, 0, pwidth, BORDER_WIDTH);
            XDrawRectangle(dpy, wb, gcb, BORDER_WIDTH, 0, pwidth, BORDER_WIDTH);
            XSync(dpy, False);

            snprintf(prev_lang, 3, "%s", ppp);
            // printf("Active language changed to %s\n", ppp);
        }
        else { 
            // printf("No changes in active language...\n");
            continue;
        }
    }

    // Обновляем содержимое окна
    XFlush(dpy);
    XFreeGC(dpy, gct);
    XFreeGC(dpy, gcb);
    XDestroyWindow(dpy, wt);
    XDestroyWindow(dpy, wb);
}

int GetWinClientArea(Display *display, int screen, int *coords) {

    // Получаем рабочую область экрана (там, где расположены окна приложений)
    Atom work_area_atom = XInternAtom(display, "_NET_WORKAREA", False);
    Atom actual_type;
    int actual_format;
    unsigned long n_items, bytes_after;
    unsigned char *work_area_data = NULL;
    long workarea_x, workarea_y, workarea_width, workarea_height;

    // Читаем рабочее пространство
    Status result = XGetWindowProperty(display, RootWindow(display, screen), work_area_atom, 0, 4 /* enough for 4 integers */, False, XA_CARDINAL, &actual_type, &actual_format, &n_items, &bytes_after, &work_area_data);

    if (result != Success || work_area_data == NULL || n_items < 4) {
        fprintf(stderr, "Не удалось прочитать рабочую область или она пустая.\n");
        return 1;
    }

    if (work_area_data != NULL) {
        //if (actual_type == None) 
        //    printf("work_area_data: %d\n",actual_format);
        memcpy(&workarea_x, work_area_data, sizeof(long));
        memcpy(&workarea_y, work_area_data + sizeof(long), sizeof(long));
        memcpy(&workarea_width, work_area_data + 2*sizeof(long), sizeof(long));
        memcpy(&workarea_height, work_area_data + 3*sizeof(long), sizeof(long));

        // Освобождаем ресурс
        XFree(work_area_data);
    } else {
        fprintf(stderr, "Не удалось прочитать рабочую область.\n");
        return 1;
    }

    coords[0] = workarea_x;
    coords[1] = workarea_y;
    coords[2] = workarea_width;
    coords[3] = workarea_height;

    // Выводим результат
//    printf("Рабочая область: %dx%d (+%d+%d)\n", workarea_width, workarea_height, workarea_x, workarea_y);

    return 0;
}

int main(int argc, char *argv[])
{
    int fd = open(LOCKFILE, O_RDWR | O_CREAT | O_EXCL, 0644);
    if (fd == -1) {
        if (errno == EEXIST) {
            fprintf(stderr, "Приложение уже запущено.\n");
            return 1;
        } else {
            perror("Ошибка открытия файла блокировки");
            return 1;
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////        
    // Регистрируем обработчики для сигналов SIGTERM and SIGINT
    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);
/////////////////////////////////////////////////////////////////////////////////////////////////////////        
    char *env_p = getenv("HOME");
    if (env_p) {
        printf("HOME DIR = %s\n", env_p);
    }
    else {
        printf("no HOME env var set\n");
        exit(1);
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////        
    char app_path[APP_PATH_SIZE];
    snprintf(app_path, sizeof app_path, "%s%s", env_p, UTIL_PATH);
    printf("Utility (get active lang) path: %s\n", app_path);
/////////////////////////////////////////////////////////////////////////////////////////////////////////        
    Display *display = XOpenDisplay(NULL);
    // Берём номер текущего экрана
    int screen = DefaultScreen(display);

    int dims[3];
    if (GetWinClientArea(display, screen, dims)) 
        return 1;
    int cx = dims[0], cy = dims[1], cwidth = dims[2], cheight = dims[3];
    printf(" > Rect of win client's work area: [%d, %d, %dX%d]\n", cx, cy, cwidth, cheight);        

    DrawLineTopBottomDesktop(display, app_path, cx, cy, cwidth, cheight);

    XCloseDisplay(display);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
    return EXIT_SUCCESS;
}