#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct option {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
};

int option_check(struct option *option, int argc, char *argv[]);
void print_result(int argc, char *argv[]);
int search_word_from_file(char *argv[], int cnt, int argc, char *word,
                          struct option option, int mode);
int word_count(char *str, char *word, int mode);
char *strcasestr(const char *haystack, const char *needle);

// ********************************************************
int main(int argc, char *argv[]) {
  print_result(argc, argv);
  return 0;
}
// ********************************************************

char *strcasestr(const char *haystack, const char *needle) {
  const char *h, *n;
  for (; *haystack; ++haystack) {
    for (h = haystack, n = needle; *h && *n && tolower(*h) == tolower(*n);
         ++h, ++n) {
    }
    if (*n == '\0') {
      return (char *)haystack;
    }
  }
  return NULL;
}

int word_count(char *str, char *word, int mode) {
  int count = 0, i, j, len1, len2;

  len1 = strlen(str);
  len2 = strlen(word);

  for (i = 0; i <= len1 - len2; i++) {
    for (j = 0; j < len2; j++) {
      if (tolower(str[i + j]) != tolower(word[j])) break;
      if (!mode && str[i + j] != word[j]) break;
    }
    if (j == len2) count++;
  }
  return count;
}

int search_word_from_file(char *argv[], int cnt, int argc, char *word,
                          struct option option, int mode) {
  char *file_name = argv[cnt];
  int line_counter = 0;
  int flag = 1;
  int error = 0;
  char o_word[strlen(word) + 1];
  strcpy(o_word, word);
  o_word[strlen(word)] = '\n';
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    flag = 0;
    error = 1;
  }

  char line[1024];
  char *line_to_print = NULL;
  int counter = 0;
  int not_exist = 0;
  int exist_i = 0;
  int exist = 0;
  while (flag && fgets(line, sizeof(line), file)) {
    line_counter++;
    if (strstr(line, word) != NULL) exist = 1;
    if (strcasestr(line, word) != NULL) exist_i = 1;

    if (option.e && !option.i && exist) {
      counter++;
      line_to_print = line;
    } else if (option.i && exist_i) {
      counter++;
      line_to_print = line;
    } else
      line_to_print = NULL;
    if (!option.e && !option.i && exist) {
      line_to_print = line;
      counter++;
    }

    if (option.o) {
      int size = word_count(line, word, option.i);
      if (size != 0) {
        char temp[100];
        int count_o = 0;
        for (int i = 0; i < size; i++) {
          for (size_t j = 0; j < strlen(word); j++) {
            temp[count_o] = word[j];
            count_o += 1;
          }
          temp[count_o] = '\n';
          count_o += 1;
        }
        temp[count_o] = '\0';
        line_to_print = temp;
      }
    }
    // printf("%d\n", option.v);
    if (option.v) {
      // printf("ljsfaljflaj");
      if (line_to_print != NULL) {
        line_to_print = NULL;
      } else {
        line_to_print = line;
        not_exist++;
      }
    }

    if (option.l) {
      if (option.i && exist_i) {
        flag = 0;
      } else if (exist) {
        flag = 0;
      } else if (not_exist) {
        flag = 0;
      }
    }
    if (!option.c && line_to_print != NULL && !option.l) {
      if (!mode && !option.h) {
        if (option.n) {
          printf("%s:%d:%s", file_name, line_counter, line_to_print);
          if (!strstr(line_to_print, "\n")) printf("\n");
        } else {
          printf("%s:%s", file_name, line_to_print);
          if (!strstr(line_to_print, "\n")) printf("\n");
        }
      } else if (option.n) {
        printf("%d:%s", line_counter, line_to_print);
        if (!strstr(line_to_print, "\n")) printf("\n");
      } else {
        printf("%s", line_to_print);
        if (!strstr(line_to_print, "\n")) printf("\n");
      }
    }
    exist_i = 0;
    exist = 0;
    line_to_print = NULL;
  }

  fclose(file);

  if (option.c && option.v && !error) {
    if (!mode && !option.h)
      printf("%s:%d\n", file_name, not_exist);
    else
      printf("%d\n", not_exist);
  } else if (option.c && !error) {
    if (!mode && !option.h)
      printf("%s:%d\n", file_name, counter);
    else
      printf("%d\n", counter);
  }

  if (!flag) {
    if (error && !option.s)
      printf("grep: %s: No such file or directory\n", file_name);
    else if (!option.s)
      printf("%s\n", file_name);
    else if (!error && option.l) {
      printf("%s\n", file_name);
    }
  }
  return argc++;
}

void print_result(int argc, char *argv[]) {
  int flag = 1;
  struct option option = {0};
  int cnt = option_check(&option, argc, argv);
  char *searched_word = argv[cnt++];
  FILE *file;
  if (option.f) {
    file = fopen(searched_word, "r");
    if (file == NULL) {
      printf("No such file or dirctory\n");

      flag = 0;
    }
  }

  if (flag) {
    int mode;
    for (int i = cnt; i < argc; i++) {
      if (cnt < argc - 1 && flag)
        mode = 0;
      else
        mode = 1;

      if (option.f) {
        char line[1024];
        file = fopen(searched_word, "r");
        while (fgets(line, sizeof(line), file)) {
          line[strlen(line) - 1] = '\0';
          search_word_from_file(argv, i, argc, line, option, mode);
        }
      } else
        search_word_from_file(argv, i, argc, searched_word, option, mode);
    }
  }
  fclose(file);
}

int option_check(struct option *option, int argc, char *argv[]) {
  int c;
  int flag = 1;
  int cnt = 1;
  for (int i = 1; i < argc; i++) {
    if ((c = getopt(argc, argv, ":eivclnhsfov:")) != -1) {
      if (c == 'e') {
        option->e = 1;
      } else if (c == 'i') {
        option->i = 1;
      } else if (c == 'v') {
        option->v = 1;
      } else if (c == 'c') {
        option->c = 1;
      } else if (c == 'l') {
        option->l = 1;
      } else if (c == 'n') {
        option->n = 1;
      } else if (c == 'h') {
        option->h = 1;
      } else if (c == 's') {
        option->s = 1;
      } else if (c == 'f') {
        option->f = 1;
      } else if (c == 'o') {
        option->o = 1;
      }
    }
  }

  while (flag && argv[cnt][0] == '-') {
    if (argv[cnt][1] == 'e') flag = 0;

    cnt++;
  }
  return cnt;
}
