#include <stdio.h>
#include <string.h>
#include "kbutil.h"

int kb_get_group(Display *dpy, int gc) {
	XkbStateRec state[1];
	memset(state, 0, sizeof(state));
	XkbGetState(dpy, XkbUseCoreKbd, state);
	if ( state->group < 0 || state->group > (gc-1) ) { printf("kb layout group index that greater than :%d is not supported\n", gc); return 0; }
	return state->group;
}

// int get_layouts(Display *dpy, char **names) {
// 	XkbDescRec desc[1];
// 	int gc;
// 	memset(desc, 0, sizeof(desc));
// 	desc->device_spec = XkbUseCoreKbd;
// 	XkbGetControls(dpy, XkbGroupsWrapMask, desc);
// 	XkbGetNames(dpy, XkbGroupNamesMask, desc);
// 	XGetAtomNames(dpy, desc->names->groups, gc = desc->ctrls->num_groups, names);
// 	XkbFreeControls(desc, XkbGroupsWrapMask, True);
// 	XkbFreeNames(desc, XkbGroupNamesMask, True);
// 	return gc;
// }

void kb_get_layouts(Display *dpy/*, char **names*/, char *out) {

	char *names[XkbNumKbdGroups];
	char buf[200]="\0";
	XkbDescRec desc[1];
	int gc;
	memset(desc, 0, sizeof(desc));
	desc->device_spec = XkbUseCoreKbd;
	XkbGetControls(dpy, XkbGroupsWrapMask, desc);
	XkbGetNames(dpy, XkbGroupNamesMask, desc);
	XGetAtomNames(dpy, desc->names->groups, gc = desc->ctrls->num_groups, names);

    int i;
	for (i = 0; i < gc; i++) {
		sprintf(buf, "%d %s\n", i, names[i]);
		strcat(out, buf);
	}

	XkbFreeControls(desc, XkbGroupsWrapMask, True);
	XkbFreeNames(desc, XkbGroupNamesMask, True);
}

// void free_layouts(char **names, int gc) {
// 	for (; gc--; ++names)
// 		if (*names) {
// 			XFree(*names);
// 			*names = NULL;
// 		}
// }

// void show_layouts(Display *dpy, char **names, int gc) {
//     int i;
// 	for (i = 0; i < gc; i++)
// 		printf("%d - %s\n", i, names[i]);
// }



// int main(int argc, char *argv[])
// {
//     Display *display = XOpenDisplay(NULL);
// 	char layout_names[500]="\0";

// 	int g = kb_get_group(display); // должно выполняться периодически
//    	kb_get_layouts(display, layout_names); // выполняется один раз во время старта

//     printf("Active lang index:%d\n", g);
// 	printf("KB Layouts list:\n");
// 	char header[] = "Line list 1\nLine list 2\nLine list 3\n\n";
//     printf("%s%s", header, layout_names);
//     // show_layouts(display, names, gcl);

//     // free_layouts(names, gcl);
//     XCloseDisplay(display);

//     return 0;
// }
