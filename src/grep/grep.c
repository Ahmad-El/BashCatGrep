#include "grep.h"

int init_fuction(int argc, char *argv[]) {
  struct option option = {0};
  option.template = (char **)malloc(SIZE * sizeof(char *));
  option.file_name = (char **)malloc(SIZE * sizeof(char *));
  for (int i = 0; i < SIZE; i++) {
    option.template[i] = (char *)malloc(SIZE * sizeof(char));
    option.file_name[i] = (char *)malloc(SIZE * sizeof(char));
  }
  int error_flag = check_options(argc, argv, &option);
  if (!error_flag) {
    search_from_files(option);
  }
  for (int i = 0; i < SIZE; i++) {
    free(option.template[i]);
    free(option.file_name[i]);
  }
  free(option.template);
  free(option.file_name);
  return 0;
}

int check_options(int argc, char *argv[], struct option *option) {
  size_t index_e = 1;
  char symbol_error;
  int error = 0;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      int flag = 0;
      for (size_t j = 1; j < strlen(argv[i]) && flag == 0; ++j) {
        if (argv[i][j] == 'e') {
          option->e = 1;
          option->option_counter++;
          if (j == strlen(argv[i] + 1)) {
            i++;
            snprintf(option->template[option->template_counter++], SIZE, "%s",
                     argv[i]);
            flag++;
          } else if (index_e != strlen(argv[i]) - 1) {
            snprintf(option->template[option->template_counter++], SIZE, "%s",
                     &argv[i][++j]);
            flag++;
          }
        } else if (argv[i][j] == 'f') {
          option->f = 1;
          option->option_counter++;
          if (j == strlen(argv[i] + 1)) {
            i++;
            error = option_f(argv[i], option);
            flag++;
          } else if (index_e != strlen(argv[i]) - 1) {
            error = option_f(&argv[i][++j], option);
            flag++;
          }
        } else if (argv[i][j] == 'i') {
          option->i = 1;
          option->option_counter++;
        } else if (argv[i][j] == 'v') {
          option->v = 1;
          option->option_counter++;
        } else if (argv[i][j] == 'c') {
          option->c = 1;
          option->option_counter++;
        } else if (argv[i][j] == 'l') {
          option->l = 1;
          option->option_counter++;
        } else if (argv[i][j] == 'n') {
          option->n = 1;
          option->option_counter++;
        } else if (argv[i][j] == 'h') {
          option->h = 1;
          option->option_counter++;
        } else if (argv[i][j] == 'o') {
          option->o = 1;
          option->option_counter++;
        } else if (argv[i][j] == 's') {
          option->s = 1;
          option->option_counter++;
        } else {
          symbol_error = argv[i][j];
          printf("grep: invalid option -- %c\nusage: grep [-eivclnhs]\n",
                 symbol_error);
          option->option_counter = -1;
          error = -1;
        }
      }
    } else if (option->template_counter == 0) {
      snprintf(option->template[option->template_counter++], SIZE, "%s",
               argv[i]);
    } else {
      snprintf(option->file_name[option->file_counter++], SIZE, "%s", argv[i]);
    }
  }
  return error;
}

void search_from_files(struct option option) {
  for (int i = 0; i < option.file_counter; ++i) {
    FILE *file = fopen(option.file_name[i], "rt");
    if (file == NULL) {
      if (!option.s) {
        printf("grep: %s: No such file or directory\n", option.file_name[i]);
      }
    } else {
      char *line = NULL;
      char get_line[SIZE];
      int cnt_line = 0, count_ex_file = 0, count_line = 1;
      int reg_match = 1;
      char *key_word;
      while (fgets(get_line, sizeof(get_line), file)) {
        line = get_line;
        for (int j = 0; j < option.template_counter; ++j) {
          regex_t reg;
          int reg_option = option.i ? REG_ICASE : REG_EXTENDED;
          regcomp(&reg, option.template[j], reg_option);
          key_word = option.template[j];
          reg_match = regexec(&reg, line, 0, 0, 0);
          if (reg_match == 0) {
            j = option.template_counter;
          }
          regfree(&reg);
        }
        if (option.v) {
          if (reg_match)
            reg_match = 0;
          else
            reg_match = 1;
        }
        if (!reg_match) {
          cnt_line++;
          count_ex_file = 1;
        }
        if (!reg_match && !option.l && !option.c) {
          if (option.file_counter > 1 && !option.h) {
            printf("%s:", option.file_name[i]);
          }
          if (option.n) printf("%d:", count_line);

          if (option.o && !option.v) {
            option_o(key_word, line, option.i);
          } else
            printf("%s", line);
          if (line[strlen(line) - 1] != '\n' && (!option.o || option.v)) {
            printf("\n");
          }
        }
        count_line++;
      }
      if (option.c) {
        if (option.file_counter > 1 && !option.h) {
          printf("%s:", option.file_name[i]);
        }
        if (option.l && cnt_line) {
          cnt_line = 1;
        }
        printf("%d\n", cnt_line);
        cnt_line = 0;
      }
      if (option.l && count_ex_file == 1) {
        printf("%s\n", option.file_name[i]);
        count_ex_file = 0;
      }
      fclose(file);
    }
  }
}

void option_o(char *word, char *line, int i_mode) {
  char temp[strlen(word)];
  char match[strlen(word)];
  if (word[0] == '^') {
    for (size_t i = 1; i < strlen(word); i++) temp[i - 1] = word[i];
    temp[strlen(word) - 1] = '\0';
    strcpy(match, temp);
  } else
    strcpy(match, word);

  for (size_t i = 0; i < strlen(line) - strlen(match) + 1; i++) {
    int flag = 1;
    for (size_t j = 0; j < strlen(match); j++) {
      temp[j] = line[i + j];
      if (i_mode) {
        if (tolower(match[j]) != tolower(line[i + j])) {
          flag = 0;
        }
      } else {
        if (match[j] != line[i + j]) {
          flag = 0;
        }
      }
    }
    temp[strlen(match)] = '\0';
    if (flag) printf("%s\n", temp);
  }
}

int option_f(char *str, struct option *option) {
  int error = 0;
  FILE *file = fopen(str, "rt");
  if (file == NULL) {
    printf("grep: %s: No such file or directory\n", str);
    error = -1;
  } else {
    char *line = NULL;
    char get_line[SIZE];
    while (fgets(get_line, sizeof(get_line), file)) {
      line = get_line;
      int tmp_len = strlen(line);
      int str_len = line[tmp_len - 1] == '\n' ? tmp_len : tmp_len + 1;
      snprintf(option->template[option->template_counter++], str_len, "%s",
               line);
    }
    fclose(file);
  }
  return error;
}

int main(int argc, char *argv[]) {
  init_fuction(argc, argv);
  return 0;
}