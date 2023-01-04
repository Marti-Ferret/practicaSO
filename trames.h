// Define Guard
#define _GNU_SOURCE

// Llibreries del sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

// Define
#define escriure(x) write(1, x, strlen(x))
// Tipus propis
typedef struct
{
    char tipo;
    char *header;
    short longitud;
    char *data;

} Trames;

void tramaConectarServidor(char *name, char *ip, int port, int pid, int fd);
Trames tramaRebreConnexio(int fd);
void enviarTrama(int fd, Trames t);
Trames llegirTrama(int fd);