CC=gcc
CFLAGS=-c -Wall -Wextra -Werror
AFLAGS=ar rc

SOURCES=$(wildcard *.c)
TESTS=$(wildcard ./tests/*test.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
TESTOBJ=$(patsubst %.c, %.o, $(TESTS))
LIBSRC=$(wildcard s21_*.c)
LIBOBJ=$(patsubst %.c, %.o, $(LIBSRC))

ifeq ($(shell uname -s),Linux)
OPEN_CMD=xdg-open
OFLAGS=-lcheck -lsubunit -lm -lrt -lpthread -lgcov -o 
endif
ifeq ($(shell uname -s),Darwin) 
OPEN_CMD=open
OFLAGS=-lcheck -lm -lpthread -lgcov -o
endif

all: s21_decimal.a $(SOURCES) test 

test: $(OBJECTS) $(TESTOBJ)
	$(CC) $(OBJECTS) $(TESTOBJ) $(OFLAGS) $@

retest: clean test

s21_decimal.a:
	$(CC) $(CFLAGS) $(LIBSRC)
	$(AFLAGS) s21_decimal.a $(LIBOBJ)
	rm -rf *.o

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info *.html *.css *.png *.out.dSYM main.c *.out
	rm -rf ./tests/*.o ./tests/*.gcda ./tests/*.gcno
	rm -rf test src

rebuild: clean all


