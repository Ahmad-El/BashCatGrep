CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11


all: s21_grep

s21_grep:
	${CC} ${CFLAGS} grep.c -o s21_grep

second_test: s21_grep
	sh test.sh
test: s21_grep
	cp test_grep/* ../grep/
	sh test_func_grep.sh

clean:
	rm -f test_*.txt
	rm -f test_*.sh
	rm -f test_*.log
	rm -rf s21_grep*

clear:
	clear

rebuild: clean s21_grep

debug:
	@rm -rf s21_grep
	@${CC} ${CFLAGS} grep.c -o s21_grep