#include "trames.h"
#include "reads.h"
#include "arda.h"

void tramaConectarServidor(char *name, char *ip, int port, int pid, int fd)
{
    char *t;

    char tipo;
    char *header;
    int longitud;
    char *aux;

    tipo = 0x01;

    header = strdup("[NEW_SON]");

    asprintf(&aux, "%s&%s&%d&%d", name, ip, port, pid);

    longitud = strlen(aux);

    asprintf(&t, "%c%s%d%s", tipo, header, longitud, aux);

    write(fd, t, strlen(t));

    free(header);
    free(aux);
    free(t);
}

Trames tramaRebreConnexio(int fd)
{
    Trames t;

    char tipo;
    char *header;
    char longitud[2];
    char *data;

    char caracter = ']';

    read(fd, &tipo, sizeof(char));
    t.tipo = tipo;

    header = read_until(fd, ']');
    header = (char *)realloc(header, strlen(header) + 2);
    header[strlen(header)] = caracter;
    header[strlen(header) + 1] = '\0';
    t.header = strdup(header);

    read(fd, longitud, sizeof(longitud));
    t.longitud = atoi(longitud);

    data = (char *)malloc(sizeof(char) * t.longitud);
    read(fd, data, sizeof(char) * t.longitud);
    t.data = strdup(data);

    free(data);
    free(header);

    return t;
}