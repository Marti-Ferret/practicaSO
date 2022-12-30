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
