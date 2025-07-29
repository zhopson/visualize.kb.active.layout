#include <X11/Xlib.h>
#include <X11/XKBlib.h>

// char *names[XkbNumKbdGroups]; // имена раскладок
// int gcl; 					  // количество раскладок

int kb_get_group(Display *dpy, int gc);
void kb_get_layouts(Display *dpy/*, char **names*/, char *out);
// void show_layouts(Display *dpy/*, char **names, int gc*/);
//void free_layouts(char **names, int gc);