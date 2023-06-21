#include <stdio.h>
#include <string.h>

void stdin_options(int argc, char* argv[]);
int check(char c, int argc, char* argv[]);
char legal_option_check(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  stdin_options(argc, argv);
  return 0;
}

void stdin_options(int argc, char* argv[]) {
  int b = 0, e = 0, n = 0, s = 0, t = 0, v;
  int new_line = 1, cnt = 1, skip = 1, flag = 1;
  char legality = legal_option_check(argc, argv);
  char c;

  FILE* file = fopen(argv[argc - 1], "r");
  if (file == NULL) {
    printf("s21_cat: %s: No such file or directory\n", argv[argc - 1]);
    flag = 0;
  } else if (legality != '1') {
    flag = 0;
    printf("s21_cat: illegal option -- %c", legal_option_check(argc, argv));
  } else {
    b = check('b', argc, argv);
    e = check('e', argc, argv);
    n = check('n', argc, argv);
    s = check('s', argc, argv);
    t = check('t', argc, argv);
    v = check('v', argc, argv);
    if (e == 1 || t == 1) v = 1;

    c = getc(file);
  }
  while (flag && !feof(file)) {
    if (s && c == '\n' && skip > 1) {
      c = getc(file);
      continue;
    } else if (s && c == '\n') {
      skip += 1;
    } else {
      skip = 0;
    }
    if ((b || n) && new_line) {
      new_line = 0;
      if (b && c != '\n')
        printf("%6d\t", cnt++);
      else if (n && !b)
        printf("%6d\t", cnt++);
    }
    if ((b || n) && c == '\n') {
      new_line = 1;
    }
    if (e && c == '\n') {
      printf("$");
    }

    if (v) {
      if (c < 32 && c != 9 && c != 10) {
        c += 64;
        printf("^");
      }

      if (c == 127) {
        printf("^");
        c = '?';
      }
    }

    if (t && c == 9) {
      printf("%c", '^');
      c = 'I';
    }
    printf("%c", c);
    c = getc(file);
  }
  fclose(file);
}

int check(char c, int argc, char* argv[]) {
  int flag = 0;
  for (int i = 1; i < argc - 1; i++) {
    if (c == argv[i][1]) flag = 1;
    if (c == 'b' && strcmp("--number-nonblank", argv[i]) == 0)
      flag = 1;
    else if (c == 'e' && argv[i][1] == 'E')
      flag = 1;
    else if (c == 'n' && strcmp("--number", argv[i]) == 0)
      flag = 1;
    else if (c == 's' && strcmp("--squeeze-blank", argv[i]) == 0)
      flag = 1;
    else if (c == 't' && argv[i][1] == 'T')
      flag = 1;
  }
  return flag;
}

char legal_option_check(int argc, char* argv[]) {
  char flag = '1';
  for (int i = 1; i < argc - 1; i++) {
    // char gnu[100] = "--number-nonblank --number --squeeze-blank
    // -b-e-n-s-t-v-T-E";
    if (strlen(argv[i]) <= 3) {
      if (strstr("-b-e-n-s-t-v-T-E", argv[i]) == NULL) {
        // printf("%c --> Error\n", argv[i][1]);
        flag = argv[i][1];
        i = argc;
      }
    } else {
      if (strcmp("--number-nonblank", argv[i]) != 0 &&
          strcmp("--number", argv[i]) != 0 &&
          strcmp("--squeeze-blank", argv[i]) != 0)
        flag = argv[i][2];
      i = argc;
    }
  }
  return flag;
}
