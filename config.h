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

void c_init_conf();
char *c_get_kb_color(int i);
int  c_is_file_exists(char *filename);
void c_read_conf(char *filename/*, char kb_state_colors[NUM_COLORS][MAX_LEN]*/);
void c_write_conf(char *filename, char *header/*, char kb_state_colors[NUM_COLORS][MAX_LEN]*/);
void c_print_conf();
