#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "config.h"

int isWholeNumber(char* num)
{
    // Check if the input is empty
    if (*num == '\0') return 0;

    // Ignore the '+' sign if it is explicitly present in the beginning of the number
    if (*num == '+') ++num;

    // Check if the input contains anything other than digits
    while (*num)
    {
        if (!isdigit(*num)) return 0;
        ++num;
    }

    // You can add other tests like
       // Ignoring the leading and trailing spaces
       // Other formats of whole number (1.0, 1.00, 001, 1+0i, etc.)
       // etc. (depends on your input format)

    // The input is a whole number if it passes these tests
	return 1;
}

void insert_symbol0(char *str, char *out) {
	char s = '#';
	strcpy(out, str);
	size_t i = strlen(out);
	memmove(out+1, out, i + 1);
	out[0] = s;
//	str[i+2] = 0;
}

void parse_item(char *str, char *out, int *index) {
	char* newString = strtok(str, " \n");
	int i = -1, k = 0;
	while (newString!= NULL)
	{
		if (k == 0 && isWholeNumber(newString) > 0 && atoi(newString) < NUM_COLORS) {
			i = atoi(newString);
		}
		else if (k == 1 && i != -1) {
			strcpy(out, newString);
			*index = i;
		}
		else {
			printf("Индекс вне диапазона\n");
			*index = -1;
			break;
		}
		// printf("parse_item%s\n", newString);
		newString = strtok(NULL, " \n");
		k++;
	}
}

void c_init_conf() {
	strcpy(kb_state_colors[0], "#FF0000");
	strcpy(kb_state_colors[1], "#007BFF");
}

void c_init_xf_colors(Display *dpy, XSetWindowAttributes *attr) {
	int i;
    for (i = 0; i < NUM_COLORS; i++) {
        strcpy(a_xf_colors[i].hcolor, c_get_kb_color(i));
        XParseColor(dpy, attr->colormap, a_xf_colors[i].hcolor, &a_xf_colors[i].scolor);
        XAllocColor(dpy, attr->colormap, &a_xf_colors[i].scolor);
    }	
}

unsigned long c_get_xcolor_pixel(int index) {
	return a_xf_colors[index].scolor.pixel;
}

int c_is_file_exists(char *filename) {
	if (access(filename, F_OK) == 0) {
		// file exists
		return 1;
	} else {
		// file doesn't exist
		return 0;
	}
}

void c_read_conf(char *filename/*, char kb_state_colors[NUM_COLORS][MAX_LEN]*/) {
	char path[200];
	char buf[200];
	int i;
	/* Open the file for reading. */
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error opening the config file\n" );
		exit(1);
	}
	while (fgets(path, sizeof(path), fp) != NULL) {
		if (path[0]!='#' && path[0]!=' ') {
			parse_item(path, buf, &i);

			if (i != -1) {
				// printf("i:%d\n", i);
				// printf("buf:%s\n", buf);
				// printf("aaa:%s\n", kb_state_colors[i]);
				//sprintf(*kb_state_colors[i],"%s",buf);
				strcpy(kb_state_colors[i],buf);
				//strcpy(kb_state_colors[i].color,buf);
			}

		}
	}
	/* close */
	fclose(fp);
}

void c_print_conf() {
	int i;
    for (i=0; i < NUM_COLORS ; i++) {
		printf("%d - %s\n", i, kb_state_colors[i]);
	}
}

// void set_conf() {
// }

void c_write_conf(char *filename, char *header/*, char kb_state_colors[NUM_COLORS][MAX_LEN]*/) {
    fp = fopen (filename,"w");
	if (fp == NULL) {
		printf("Ошибка создания файла\n" );
		exit(1);
	}

	char buff[200];
    char* newString= strtok(header, "\r\n");
    while (newString!= NULL)
    {
		insert_symbol0(newString, buff);
        // printf("%s\n", buff);
		fputs( buff, fp );
		fputc( '\n', fp );
        newString = strtok(NULL, "\r\n");
    }

	int i;
	char buf[MAX_LEN+2];
    for (i=0; i < NUM_COLORS ; i++) {
		sprintf(buf, "%d %s\n", i, kb_state_colors[i]);
		fputs(buf, fp);
	}
	// sprintf( buf, "0 #FF0000\n" );
	// fputs( buf, fp );
	// sprintf( buf, "1 #007BFF\n" );
	// fputs( buf, fp );
	fclose(fp);
}

char *c_get_kb_color(int i) {
	return kb_state_colors[i];
}



// int main(int argc, char *argv[])
// {
// 	if (!c_is_file_exists(filename)) {
// 		printf("Файл не существует!\nСоздание файла...\n");
// 		char header[] = "Line list 1\nLine list 2\nLine list 3\n";
// 		c_write_conf(filename, header/*,kb_state_colors*/);
// 		printf("Конфиг по умолчанию записан в файл\n");
// 	}
// 	else {
// 		printf("Файл конфигурации найден. Читаем конфиг...\n");
// 		c_read_conf(filename/*,kb_state_colors*/);
// 		printf("Конфиг:\n");
// 		c_print_conf(/*kb_state_colors, NUM_COLORS*/);
// 		printf("Цвет 1:%s\n", c_get_kb_color(1));
// 	}
//   	return 0;
// }