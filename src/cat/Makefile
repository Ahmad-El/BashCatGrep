CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11


all: s21_cat

s21_cat:
	${CC} ${CFLAGS} cat.c -o s21_cat

test: s21_cat
	cp test_cat/* ../cat/
	sh test_func_cat.sh
	

test_leak: s21_cat
	cp test_cat/* ../cat/
	sh test_leak_cat.sh

clean:
	rm -rf s21_cat
	rm -rf fail.txt
	rm -rf .clang-format
	rm -f test_*
rebuild: cl