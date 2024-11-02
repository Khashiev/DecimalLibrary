CC=gcc
CFLAGS=-Wall -Wextra -Werror
AFLAGS=ar rc

SOURCES=$(wildcard s21_*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

TEST_SRC=$(wildcard run_test.c)
TEST_OBJ = $(patsubst %.c,%.o, ${TEST_SRC})

GCOVFLAGS=-fprofile-arcs -ftest-coverage

ifeq ($(shell uname -s),Linux)
OFLAGS=-lcheck -lsubunit -lm -lrt -lpthread
endif
ifeq ($(shell uname -s),Darwin)
OFLAGS=-lcheck -lm -lpthread
endif


all: s21_decimal.a gcov_report clean

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

s21_decimal.a: $(OBJECTS)
	ar rc s21_decimal.a $^
	ranlib s21_decimal.a

test: s21_decimal.a 
	$(CC) $(CFLAGS) ${TEST_SRC} $(GCOVFLAGS) $< -o test $(OFLAGS)
	./test

retest: clean test

gcov_report: test
	lcov -t "./gcov" -o gcov.info --no-external -c -d .
	genhtml -o gcov gcov.info
	open ./gcov/index.html

style_check:
	clang-format -style=google -n *.c
	clang-format -style=google -n *.h

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info 
	rm -rf test 

clean_gcov:
	rm -rf gcov

rebuild: clean clean_gcov all
