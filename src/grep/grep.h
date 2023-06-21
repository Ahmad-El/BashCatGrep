#ifndef GREP_H_
#define GREP_H_

#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1024

struct option {
  int e;
  int f;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int option_counter;
  int file_counter;
  int template_counter;
  char **template;
  char **file_name;
};

int check_options(int argc, char *argv[], struct option *option);
void search_from_files(struct option option);
int option_f(char *stroka, struct option *option);
int init_fuction(int argc, char *argv[]);
void option_o(char *word, char *line, int i_mode);

#endif  // GREP_H_