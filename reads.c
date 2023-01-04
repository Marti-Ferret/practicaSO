#include "reads.h"

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

char *read_untilEnter(int fd, char character, int i)
{
    char *buffer = NULL;

    while (character != '\n' || i == 0)
    {
        read(fd, &character, sizeof(char));

        if (buffer == NULL)
        {
            buffer = (char *)malloc(1 * sizeof(char));
        }
        else
        {
            buffer = (char *)realloc(buffer, i + 1);
        }
        buffer[i] = character;

        if (character == '\n' || character == '\0')
        {
            buffer[i] = '\0';
            return buffer;
        }
        i++;
    }

    return buffer;
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

char *read_until(int fd, char character)
{
    int i = 0, size;
    char c = '\0';
    char *string = (char *)malloc(sizeof(char));

    while (1)
    {
        size = read(fd, &c, sizeof(char));

        if (c != character && size > 0)
        {
            string = (char *)realloc(string, sizeof(char) * (i + 2));
            string[i++] = c;
        }
        else
        {
            break;
        }
    }

    string[i] = '\0';
    return string;
}

char *llegirFins(char *cadena, char character)
{
    int i = 0;

    char *string = (char *)malloc(sizeof(char));

    while (cadena[i] != character)
    {
        string = (char *)realloc(string, sizeof(char) * (i + 2));
        string = string + cadena[i];
        i++;
    }

    string[i] = '\0';
    return string;
}

char *read_md5sum(char *nomF)
{
    int link[2];
    pid_t pid;

    if (pipe(link) == -1)
        perror("pipe");

    if ((pid = fork()) == -1)
        perror("fork");

    // codi del fill
    if (pid == 0)
    {

        // generem el codi del md5sum amb execl i el passem a stdout
        dup2(link[1], STDOUT_FILENO);
        close(link[0]);
        close(link[1]);
        execl("/bin/md5sum", "md5sum", nomF, (char *)0);
        perror("execl");
    }
    else
    {
        wait(NULL);
        close(link[1]);

        // aloquem memòria per la cadena
        char *md5_out = (char *)malloc(256 * sizeof(char));
        // llegim pel pipe la cadena
        read(link[0], md5_out, 256);
        strtok(md5_out, " ");

        close(link[0]);

        return md5_out;
    }
    return NULL;
}
