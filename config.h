#include <X11/Xlib.h>

#define NUM_COLORS 2
#define MAX_LEN 20

FILE *fp;

// char *filename = "app.conf";

// struct c_colors
// {
//     char color[20];
//     int index;
// } kb_state_colors[NUM_COLORS];

char kb_state_colors[NUM_COLORS][MAX_LEN]; // = {"#FF0000", "#007BFF"};

struct xf_colors {
    XColor scolor;
    char   hcolor[MAX_LEN];
} a_xf_colors[NUM_COLORS];


void c_init_conf();
void c_init_xf_colors(Display *dpy, XSetWindowAttributes *attr);
unsigned long c_get_xcolor_pixel(int index);
char *c_get_kb_color(int i);
int  c_is_file_exists(char *filename);
void c_read_conf(char *filename/*, char kb_state_colors[NUM_COLORS][MAX_LEN]*/);
void c_write_conf(char *filename, char *header/*, char kb_state_colors[NUM_COLORS][MAX_LEN]*/);
void c_print_conf();
