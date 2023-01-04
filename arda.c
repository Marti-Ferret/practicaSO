#include "arda.h"
#include "reads.h"
#include "trames.h"

Usuaris *usuaris;
int totalUsuaris = 0;

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

void controlC(void)
{
	escriure("\nDisconnecting from Arda. See you soon, son of Iluvatar\n");
	signal(SIGINT, SIG_DFL);
	raise(SIGINT);
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

Config llegirConfig(Config config, char *nomF)
{
	int fd;
	char *directoriTemp;
	int size;

	fd = open(nomF, O_RDONLY);

	if (fd != 0)
	{
		escriure("Reading configuration file\n");
		config.ip = read_until(fd, '\n');
		config.port = atoi(read_until(fd, '\n'));
		directoriTemp = read_until(fd, '\n');

		size = strlen(directoriTemp);
		config.directori = (char *)malloc(sizeof(char) * size);
		memset(config.directori, 0, size * sizeof(char));

		for (int i = 1; directoriTemp[i] != '\0'; i++)
		{
			config.directori[i - 1] = directoriTemp[i];
		}

		struct stat st = {0};
		if (stat(config.directori, &st) == -1)
		{
			mkdir(config.directori, 0700);
		}
	}
	else
	{
		escriure("Error, no s'ha pogut llegir el fixter de configuració\n");
		close(fd);
		exit(0);
	}

	close(fd);
	return config;
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

int configSocket(Config config)
{

	struct sockaddr_in servidor;
	int listenFD;

	if ((listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		escriure("Error creant el socket\n");
	}

	bzero(&servidor, sizeof(servidor));
	servidor.sin_port = htons(config.port);
	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenFD, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
	{
		escriure("Error fent el bind\n");
	}

	if (listen(listenFD, 10) < 0)
	{
		escriure("Error fent el listen\n");
	}

	return listenFD;
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

void enviarUsuaris(int fd)
{
	int i;

	for (i = 0; i < totalUsuaris; i++)
	{
		write(fd, usuaris[i].nom, strlen(usuaris[i].nom));
		write(fd, "\n", sizeof(char));
		write(fd, usuaris[i].ip, strlen(usuaris[i].ip));
		write(fd, "\n", sizeof(char));
		write(fd, &usuaris[i].port, sizeof(int));
		write(fd, &usuaris[i].pid, sizeof(int));
	}
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

void afegirUsuari(char *nom, char *ip, int port, int pid, int fd)
{
	escriure("Updating user's list\n");

	totalUsuaris++;
	usuaris = (Usuaris *)realloc(usuaris, ((totalUsuaris) * sizeof(Usuaris)));
	usuaris[totalUsuaris - 1].nom = (char *)malloc((strlen(nom) + 1) * sizeof(char));
	usuaris[totalUsuaris - 1].ip = (char *)malloc((strlen(ip) + 1) * sizeof(char));

	strcpy(usuaris[totalUsuaris - 1].nom, nom);
	strcpy(usuaris[totalUsuaris - 1].ip, ip);
	usuaris[totalUsuaris - 1].port = port;
	usuaris[totalUsuaris - 1].pid = pid;
	usuaris[totalUsuaris - 1].fd = fd;
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

void eliminarUsuari(int fd)
{
	int pos;

	if (totalUsuaris > 0)
	{
		for (int i = 0; i < totalUsuaris; i++)
		{
			if (usuaris[i].fd == fd)
			{
				pos = i;
			}
		}

		for (int i = pos; i < totalUsuaris - 1; i++)
		{
			usuaris[i].nom = (char *)malloc((strlen(usuaris[i + 1].nom) + 1) * sizeof(char));
			usuaris[i].ip = (char *)malloc((strlen(usuaris[i + 1].ip) + 1) * sizeof(char));

			strcpy(usuaris[i].nom, usuaris[i + 1].nom);
			strcpy(usuaris[i].ip, usuaris[i + 1].ip);
			usuaris[i].port = usuaris[i + 1].port;
			usuaris[i].fd = usuaris[i + 1].fd;
			usuaris[i].pid = usuaris[i + 1].pid;
		}

		totalUsuaris--;

		usuaris = (Usuaris *)realloc(usuaris, ((totalUsuaris) * sizeof(Usuaris)));
	}
	else
	{
		usuaris = (Usuaris *)realloc(usuaris, ((0) * sizeof(Usuaris)));
	}
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

int funcions(int fd, char opcio, char *nom)
{
	char *buffer;
	if (opcio == 'U')
	{

		asprintf(&buffer, "New Petition: %s demands the user's list\n", nom);
		escriure(buffer);
		free(buffer);

		write(fd, &totalUsuaris, sizeof(int));
		enviarUsuaris(fd);

		asprintf(&buffer, "Sending user's list to %s\n\n", nom);
		escriure(buffer);
		free(buffer);
		return 0;
	}
	else if (opcio == 'S')
	{
		asprintf(&buffer, "New exit petition: %s has left Arda\n", nom);
		escriure(buffer);
		free(buffer);

		eliminarUsuari(fd);

		return 1;
	}

	return 0;
}

void enviarTramaConnexio(int fd)
{
	char *t;

	char tipo;
	char *header;
	int longitud;
	char *aux;
	char *buffer = NULL;

	tipo = 0x01;

	header = strdup("[CONOK]");

	for (int i = 0; i < totalUsuaris; i++)
	{
		asprintf(&aux, "%s&%s&%d&%d#", usuaris[i].nom, usuaris[i].ip, usuaris[i].port, usuaris[i].pid);

		buffer = (char *)realloc(buffer, sizeof(char) * strlen(aux) + 2);
		strcat(buffer, aux);
	}

	buffer[strlen(buffer) - 1] = '\0';

	longitud = strlen(buffer);

	asprintf(&t, "%c%s%d%s", tipo, header, longitud, buffer);

	write(fd, t, strlen(t));

	free(header);
	free(aux);
	free(t);
	free(buffer);
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

void *threadClients(void *clientFD)
{
	Trames t;
	int cont = 0;
	int fd = *((int *)clientFD);
	char *nom, *ip;
	int pid, port;
	char *buffer;
	char tipo;
	char *tConnexio;

	char opcio;
	int sortir = 0;

	Trames tramaProva;

	tramaProva = llegirTrama(fd);

	escriure(tramaProva.data);

	t = tramaRebreConnexio(fd);

	if (t.longitud < 0)
	{

		tipo = 0x01;

		asprintf(&tConnexio, "%c%s%d%s", tipo, "[CONKO]", 0, "");

		write(fd, tConnexio, strlen(tConnexio));

		free(tConnexio);
	}
	else
	{

		char *token = strtok(t.data, "&");

		while (token != NULL)
		{
			if (cont == 0)
			{
				nom = strdup(token);
			}
			else if (cont == 1)
			{
				ip = strdup(token);
			}
			else if (cont == 2)
			{
				port = atoi(token);
			}
			else if (cont == 3)
			{
				pid = atoi(token);
			}
			cont++;
			token = strtok(NULL, "&");
		}

		free(token);

		asprintf(&buffer, "New Login: %s, IP: %s, port: %d, PID %d\n", nom, ip, port, pid);
		escriure(buffer);
		free(buffer);

		afegirUsuari(nom, ip, port, pid, fd);

		enviarTramaConnexio(fd);
		escriure("Sending user’s list\n");

		while (sortir != 1)
		{

			read(fd, &opcio, sizeof(char));
			sortir = funcions(fd, opcio, nom);
		}

		free(ip);
		free(nom);
	}
	return NULL;
}

/******************************************************************
 *
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 *
 * *****************************************************************/

int main(int argc, char *agrv[])
{
	Config config;

	int clientFD, listenFD;

	if (argc != 2)
	{
		escriure("Error, falten fichers\n");
		return 0;
	}

	signal(SIGINT, (void *)controlC);

	escriure("\nARDA SERVER\n");
	config = llegirConfig(config, agrv[1]);
	listenFD = configSocket(config);

	escriure("Waiting for connections... \n\n");

	while (1)
	{
		clientFD = accept(listenFD, (struct sockaddr *)NULL, NULL);

		pthread_t threadClient;
		pthread_create(&threadClient, NULL, threadClients, &clientFD);
	}

	raise(SIGINT);
}
