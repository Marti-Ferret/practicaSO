#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

char * read_untilEnter(int fd, char character, int i);
char * read_until(int fd, char character);
