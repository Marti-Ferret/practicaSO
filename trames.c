#include "trames.h"
#include "reads.h"
#include "arda.h"

void tramaConectarServidor(char *name, char *ip, int port, int pid, int fd)
{
    char *t;

    char tipo;
    char *header;
    short longitud;
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

void enviarTrama(int fd, Trames t)
{

    escriure(t.data);
    char *enviar;
    int j = 0;
    int i = 0;

    int size = 1 + 2 + strlen(t.header) + t.longitud;

    int headerSize = strlen(t.header);
    int dataSize = strlen(t.data);

    enviar = (char *)malloc(sizeof(char) * size);

    enviar[0] = t.tipo;

    for (i = 1; i < headerSize + 1; i++)
    {
        enviar[i] = t.header[i - 1];
    }

    enviar[i] = (t.longitud >> 8) & 0xFF;
    i++;
    enviar[i] = (t.longitud) & 0xFF;
    i++;
    enviar[0] = t.tipo;
    j = 0;
    for (i = i; i < dataSize + 1; i++)
    {
        enviar[i] = t.data[j];
        j++;
    }

    write(fd, enviar, size);
}

Trames llegirTrama(int fd)
{
    Trames t;
    char *aux;

    aux = read_until(fd, '[');

    t.tipo = aux[0];

    t.header = read_until(fd, ']');

    read(fd, &t.longitud, sizeof(short));

    t.data = (char *)malloc(sizeof(char) * t.longitud);

    read(fd, t.data, t.longitud);

    return t;
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