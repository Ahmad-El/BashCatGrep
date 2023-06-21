#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc < 3)
    print_info();
  else {
    Flag flags = {0};
    flags.shablon = (char **)malloc(SIZE * sizeof(char *));
    flags.file_name = (char **)malloc(SIZE * sizeof(char *));
    for (int i = 0; i < SIZE; i++) {
      flags.shablon[i] = (char *)malloc(SIZE * sizeof(char));
      flags.file_name[i] = (char *)malloc(SIZE * sizeof(char));
    }
    int error_flag = check_flags(argc, argv, &flags);
    if (!error_flag) {
      check_files(flags);
    }
    for (int i = 0; i < SIZE; i++) {
      free(flags.shablon[i]);
      free(flags.file_name[i]);
    }
    free(flags.shablon);
    free(flags.file_name);
  }
  return 0;
}
void print_info() {
  printf(
      "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
      "[-C[num]]");
  printf("\n");
  printf("\t[-e pattern] [-f file] [--binary-files=value] [--color=when]");
  printf("\n");
  printf(
      "\t[--context[=num]] [--directories=action] [--label] [--line-buffered]");
  printf("\n");
  printf("\t[--null] [pattern] [file ...]");
  printf("\n");
}

int check_flags(int argc, char *argv[], Flag *flags) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      int flag = 0;
      for (size_t j = 1; j < strlen(argv[i]) && flag == 0; ++j) {
        if (argv[i][j] == 'e' || argv[i][j] == 'f') {
          flag = check_flag_e_f(&j, &i, argv, flags, flag);
        } else if (argv[i][j] == 'i') {
          flags->i = 1;
          flags->flag_count++;
        } else if (argv[i][j] == 'v') {
          flags->v = 1;
          flags->flag_count++;
        } else if (argv[i][j] == 'c') {
          flags->c = 1;
          flags->flag_count++;
        } else if (argv[i][j] == 'l') {
          flags->l = 1;
          flags->flag_count++;
        } else if (argv[i][j] == 'n') {
          flags->n = 1;
          flags->flag_count++;
        } else if (argv[i][j] == 'h') {
          flags->h = 1;
          flags->flag_count++;
        } else if (argv[i][j] == 'o') {
          flags->o = 1;
          flags->flag_count++;
        } else if (argv[i][j] == 's') {
          flags->s = 1;
          flags->flag_count++;
        } else
          invalid_opt(flags, argv[i][j]);
      }
    } else if (flags->shablon_count == 0) {
      snprintf(flags->shablon[flags->shablon_count++], SIZE, "%s", argv[i]);
    } else {
      snprintf(flags->file_name[flags->file_name_count++], SIZE, "%s", argv[i]);
    }
  }
  return flags->error;
}

int check_flag_e_f(size_t *a, int *b, char *argv[], Flag *flags,
                   int flag_checker) {
  int i = *b;
  size_t j = *a;
  if (argv[i][j] == 'e') {
    flags->e = 1;
    flags->flag_count++;
    if (j == strlen(argv[i] + 1)) {
      i++;
      snprintf(flags->shablon[flags->shablon_count++], SIZE, "%s", argv[i]);
      flag_checker++;
    } else if (1 != strlen(argv[i]) - 1) {
      snprintf(flags->shablon[flags->shablon_count++], SIZE, "%s",
               &argv[i][++j]);
      flag_checker++;
    }
  } else if (argv[i][j] == 'f') {
    flags->f = 1;
    flags->flag_count++;
    if (j == strlen(argv[i] + 1)) {
      i++;
      flags->error = flag_f(argv[i], flags);
      flag_checker++;
    } else if (1 != strlen(argv[i]) - 1) {
      flags->error = flag_f(&argv[i][++j], flags);
      flag_checker++;
    }
  }
  *a = j;
  *b = i;
  return flag_checker;
}

void invalid_opt(Flag *flags, char ch) {
  printf("grep: invalid flags -- %c\nusage: grep [-eivclnhs]\n", ch);
  flags->flag_count = -1;
  flags->error = -1;
}

void check_files(Flag flags) {
  for (int i = 0; i < flags.file_name_count; ++i) {
    FILE *file = fopen(flags.file_name[i], "rt");
    if (file == NULL) {
      if (!flags.s) {
        printf("grep: %s: No such file or directory\n", flags.file_name[i]);
      }
    } else {
      int line_count = 0,
          count_ex_file = start_search(file, flags, i, &line_count);
      if (flags.c) {
        if (flags.file_name_count > 1 && !flags.h) {
          printf("%s:", flags.file_name[i]);
        }
        if (flags.l && line_count) {
          line_count = 1;
        }
        printf("%d\n", line_count);
        line_count = 0;
      }
      if (flags.l && count_ex_file == 1) {
        printf("%s\n", flags.file_name[i]);
        count_ex_file = 0;
      }
      fclose(file);
    }
  }
}

int start_search(FILE *file, Flag flags, int i, int *cnt) {
  char *line = NULL;
  char stroka[SIZE];
  int line_count = 0, count_ex_file = 0, line_quantity = 1;
  int reg_match = 1;
  char *key_word;
  while (fgets(stroka, sizeof(stroka), file)) {
    line = stroka;
    for (int j = 0; j < flags.shablon_count; ++j) {
      regex_t reg;
      int reg_flags = flags.i ? REG_ICASE : REG_EXTENDED;
      regcomp(&reg, flags.shablon[j], reg_flags);
      key_word = flags.shablon[j];
      reg_match = regexec(&reg, line, 0, 0, 0);
      if (reg_match == 0) {
        j = flags.shablon_count;
      }
      regfree(&reg);
    }
    if (flags.v) {
      if (reg_match)
        reg_match = 0;
      else
        reg_match = 1;
    }
    if (!reg_match) {
      line_count++;
      count_ex_file = 1;
    }
    if (!reg_match && !flags.l && !flags.c) {
      if (flags.file_name_count > 1 && !flags.h) {
        printf("%s:", flags.file_name[i]);
      }
      if (flags.n) printf("%d:", line_quantity);
      if (flags.o && !flags.v) {
        flag_o(key_word, line, flags.i);
      } else
        printf("%s", line);
      if (line[strlen(line) - 1] != '\n' && (!flags.o || flags.v)) {
        printf("\n");
      }
    }
    line_quantity++;
  }
  *cnt = line_count;
  return count_ex_file;
}

void flag_o(char *pattern, char *stroka, int flag_i) {
  char buffer[strlen(pattern)];
  char match[strlen(pattern)];
  int counter = 1024;
  if (pattern[0] == '^') {
    counter = 1;
    for (size_t i = 1; i < strlen(pattern); i++) buffer[i - 1] = pattern[i];
    buffer[strlen(pattern) - 1] = '\0';
    strcpy(match, buffer);
  } else
    strcpy(match, pattern);

  for (size_t i = 0; i < strlen(stroka) - strlen(match) + 1; i++) {
    int flag = 1;
    for (size_t j = 0; j < strlen(match); j++) {
      buffer[j] = stroka[i + j];
      if (flag_i) {
        if (!check_icase_sym(match[j], stroka[i + j])) {
          flag = 0;
        }
      } else {
        if (match[j] != stroka[i + j]) {
          flag = 0;
        }
      }
    }
    buffer[strlen(match)] = '\0';
    if (flag && counter) {
      counter--;
      printf("%s\n", buffer);
    }
  }
}

int check_icase_sym(char ch1, char ch2) {
  int flag = 0;
  if (ch1 == ch2 || (ch1 - 32) == ch2 || (ch1 + 32) == ch2) flag = 1;
  return flag;
}

int flag_f(char *str, Flag *flags) {
  int error = 0;
  FILE *file = fopen(str, "rt");
  if (file == NULL) {
    printf("grep: %s: No such file or directory\n", str);
    error = -1;
  } else {
    char *stroka = NULL;
    char reader_buf[SIZE];
    while (fgets(reader_buf, sizeof(reader_buf), file)) {
      stroka = reader_buf;
      int tmp_len = strlen(stroka);
      int str_len = stroka[tmp_len - 1] == '\n' ? tmp_len : tmp_len + 1;
      snprintf(flags->shablon[flags->shablon_count++], str_len, "%s", stroka);
    }
    fclose(file);
  }
  return error;
}
