#ifndef _S21_GREP_H_
#define _S21_GREP_H_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1024

typedef struct Flag {
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
  int flag_count;
  int file_name_count;
  int shablon_count;
  int error;
  char **shablon;
  char **file_name;
} Flag;

int check_flags(int argc, char *argv[], Flag *flags);
int check_flag_e_f(size_t *a, int *b, char *argv[], Flag *flags,
                   int flag_checker);
void invalid_opt(Flag *flags, char ch);
void check_files(Flag flags);
int start_search(FILE *file, Flag flags, int i, int *cnt);
void flag_o(char *pattern, char *stroka, int flag_i);
int check_icase_sym(char ch1, char ch2);
int flag_f(char *str, Flag *flags);
void print_info();
#endif  // _S21_GREP_H_
