// Define Guard
#define _GNU_SOURCE

// Llibreries del sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

// Tipus propis
typedef struct
{
	char *nom;
	char *ip;
	int port;
	int pid;
	int fd;
} Usuaris;

char *read_untilEnter(int fd, char character, int i);
char *read_until(int fd, char character);
char *read_md5sum(char *nomF);
