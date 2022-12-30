#include "iluvatarSon.h"
#include "reads.h"

int fdSocket;
Usuaris *usuaris;
int totalUsuaris = 0;
Config config;

void llegirUsuaris(int totalUsuaris);
void llistarUsuaris();


/******************************************************************
 * 
 * @Nom: controlC
 * @Finalitat: LLibera la memoria dle usuari que es desconecta del programa
 * @Parametres:
 * @Retorn:
 * 
 * *****************************************************************/

void controlC(void)
{
	escriure("\nDisconnecting from Arda. See you soon, son of Iluvatar\n");
	write(fdSocket, "S", sizeof(char));
	for (int i = 0; i < totalUsuaris; i++)
	{
		free(usuaris[i].nom);
		free(usuaris[i].ip);
	}

	free(usuaris);
	// free(config).nom i tots
	signal(SIGINT, SIG_DFL);
	raise(SIGINT);
}


/******************************************************************
 * 
 * @Nom: comprobarMissatge
 * @Finalitat: Comproba si el format del missatge es e correcte o no
 * @Parametres: char **arrayComanda : el argv del main, int paraules: el numero de paraules que hi ha al argv
 * @Retorn: Retorna 1 o 0 si el format és el correcte o no 
 * 
 * *****************************************************************/

int comprobarMissatge(char **arrayComanda, int numParaules)
{
	int size, sizeParaula;

	if (numParaules > 4)
	{
		size = strlen(arrayComanda[numParaules - 1]);
		if (arrayComanda[3][0] == '"' && arrayComanda[numParaules - 1][size - 1] == '"')
		{

			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		sizeParaula = strlen(arrayComanda[3]);
		if (arrayComanda[3][0] == '"' && arrayComanda[3][sizeParaula - 1] == '"')
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	return 1;
}


/******************************************************************
 * 
 * @Nom: comprobarNom
 * @Finalitat: comprobar que el nom del usuari destinatari existeix o no 
 * @Parametres: char nom: nom a comprobar
 * @Retorn: retorna 1 o 0 si el nom existeix o no
 * 
 * *****************************************************************/

int comprobarNom(char *nom)
{
	int i, correcte = 0;
	for (i = 0; i < totalUsuaris; i++)
	{
		if (strcasecmp(nom, usuaris[i].nom) == 0)
		{
			correcte = 1;
		}
	}
	return correcte;
}


/******************************************************************
 * 
 * @Nom: validarNom
 * @Finalitat: validar el nom del usuari del fitxer de configuració
 * @Parametres: char nom: Nom a comprobar
 * @Retorn: retorna el nom del usuari
 * 
 * *****************************************************************/

char *validarNom(char *nom)
{
	int i = 0, j = 0;

	char *aux = (char *)malloc(sizeof(char));

	while (nom[j] != '\0')
	{

		if (nom[j] != '&')
		{
			aux = (char *)realloc(aux, sizeof(char) * (i + 2));
			aux[i++] = nom[j];
		}

		j++;
	}
	aux[i] = '\0';
	free(nom);
	return aux;
}


/******************************************************************
 * 
 * @Nom:llegirConfig
 * @Finalitat: llegir la configuració del usuari del seu fitxer
 * @Parametres: char nomF: nom del file de configuració
 * @Retorn:
 * 
 * *****************************************************************/

void llegirConfig(char *nomF)
{
	int fd;
	char *nom;
	char *directoriTemp;
	int size;

	fd = open(nomF, O_RDONLY);

	if (fd != 0)
	{
		nom = read_until(fd, '\n');
		config.nom = validarNom(nom);
		directoriTemp = read_until(fd, '\n');
		config.ipS = read_until(fd, '\n');
		config.portS = atoi(read_until(fd, '\n'));
		config.ipC = read_until(fd, '\n');
		config.portC = atoi(read_until(fd, '\n'));

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

		escriure("ERROR, no s'ha pogut llegir el fitxer de configuració\n");
		close(fd);
		exit(0);
	}

	free(directoriTemp);
	close(fd);
}



/******************************************************************
 * 
 * @Nom: connectarSocketMsg
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 * 
 * *****************************************************************/

int connectarSocketMsg(int port, char *ip)
{

	int socketFD;
	struct sockaddr_in servidor;

	if ((socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		escriure("Error creant el socket\n");
	}

	bzero(&servidor, sizeof(servidor));
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(port);

	if (inet_pton(AF_INET, ip, &servidor.sin_addr) < 0)
	{
		escriure("Error configurant IP\n");
	}

	if (connect(socketFD, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
	{
		escriure("Error fent el connectAAA\n");
	}
	return socketFD;
}



/******************************************************************
 * 
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 * 
 * *****************************************************************/

char *generarMissatge(int numParaules, char **arrayComanda){
	//char *buffer;
	//char *msg;
	int tamanyLletres = 0;
	for (int i = 3; i < numParaules; i++)
	{

		for (int j = 0; arrayComanda[i][j] != '\0'; j++)
		{
			tamanyLletres++;
		}
	}

	tamanyLletres = tamanyLletres + numParaules - 4;

	//msg = (char *)malloc(sizeof(char) * tamanyLletres);

	// Falta ficar el contingut de **arrayComandes dintre de msg, sabem que el total de lletres es= tamanyLletres i el de paraules  a numParaules
}
 


/******************************************************************
 * 
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 * 
 * *****************************************************************/

int enviarMissatge(char *nom, int numParaules, char **arrayComanda)
{
	struct hostent *host_info;
	char *ip = (char *)malloc(sizeof(char));
	char *msg;
	int fdClient, port;

	msg = generarMissatge(numParaules, arrayComanda);

	for (int i = 0; i < totalUsuaris; i++)
	{

		if (strcasecmp(nom, usuaris[i].nom) == 0)
		{
			ip = strdup(usuaris[i].ip);
			port = usuaris[i].port;
		}
	}
	host_info = gethostbyname(ip);

	if (host_info == NULL)
	{
		escriure("Error al enviar el missatge");
		free(host_info);
		free(ip);
		return 0;
	}

	if (strcasecmp(ip, config.ipC) != 0)
	{

		fdClient = connectarSocketMsg(port, ip);
		write(fdClient, config.nom, strlen(config.nom));
		write(fdClient, "\n", sizeof(char));
		write(fdClient, config.ipC, strlen(config.ipC));
		write(fdClient, "\n", sizeof(char));
		write(fdClient, msg, strlen(msg));
		write(fdClient, "\n", sizeof(char));
	}
	else
	{

		// Mateixa maquina fer pipes o cues
	}
	close(fdClient);
	free(ip);
	return 1;
}



/******************************************************************
 * 
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 * 
 * *****************************************************************/

int validarComanda(int numParaules, char **arrayComanda)
{

	if (strcasecmp(arrayComanda[0], "UPDATE") == 0)
	{
		if (numParaules == 2)
		{
			if (strcasecmp(arrayComanda[1], "USERS") == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else if (strcasecmp(arrayComanda[0], "LIST") == 0)
	{
		if (numParaules == 2)
		{
			if (strcasecmp(arrayComanda[1], "USERS") == 0)
			{
				return 0;
			}
		}
	}
	else if (strcasecmp(arrayComanda[0], "EXIT") == 0)
	{
		if (numParaules == 1)
		{
			return 0;
		}
	}
	else if (strcasecmp(arrayComanda[0], "SEND") == 0)
	{
		if (numParaules >= 4)
		{
			if (strcasecmp(arrayComanda[1], "MSG") == 0)
			{
				return 0;
			}
			else if (strcasecmp(arrayComanda[1], "FILE") == 0)
			{
				return 0;
			}
		}
	}

	return 2;
}




/******************************************************************
 * 
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 * 
 * *****************************************************************/

int gestionarComanda()
{
	int enviat;
	char *comanda;
	char *(*arrayComanda);
	int numParaules = 0;
	int correcte;
	int i = 0;

	escriure("$ ");

	comanda = read_untilEnter(STDIN_FILENO, '\n', 0);

	if (comanda[0] == '\0')
	{
		free(comanda);
		return 0;
	}

	arrayComanda = (char **)malloc(sizeof(char *));
	arrayComanda[i] = strtok(comanda, " ");

	while (arrayComanda[i] != NULL)
	{
		numParaules++;
		arrayComanda = realloc(arrayComanda, (numParaules + 1) * sizeof(char *));
		arrayComanda[++i] = strtok(NULL, " ");
	}

	correcte = validarComanda(numParaules, arrayComanda);

	if (correcte == 0)
	{
		if (strcasecmp(arrayComanda[0], "UPDATE") == 0)
		{
			write(fdSocket, "U", sizeof(char));
			read(fdSocket, &totalUsuaris, sizeof(int));
			/*
			for (int i = 0; i < totalUsuaris; i++)
			{
				free(usuaris[i].nom);
				free(usuaris[i].ip);
			}

			free(usuaris);
			*/
			llegirUsuaris(totalUsuaris);
			escriure("Users list updated\n");
		}
		else if (strcasecmp(arrayComanda[0], "LIST") == 0)
		{
			llistarUsuaris();
		}
		else if (strcasecmp(arrayComanda[0], "EXIT") == 0)
		{
			write(fdSocket, "S", sizeof(char));
			free(comanda);
			free(arrayComanda);
			raise(SIGINT);
		}
		else if (strcasecmp(arrayComanda[0], "SEND") == 0 && strcasecmp(arrayComanda[1], "MSG") == 0)
		{
			if (comprobarNom(arrayComanda[2]) == 1)
			{
				if (comprobarMissatge(arrayComanda, numParaules) == 0)
				{
					enviat = enviarMissatge(arrayComanda[2], numParaules, arrayComanda);

					if (enviat == 1)
					{
						escriure("\nMessage correctly sent\n\n");
					}
				}
			}
			else
			{
				escriure("User not found in the list\n");
			}
		}
		else if (strcasecmp(arrayComanda[0], "SEND") == 0 && strcasecmp(arrayComanda[1], "FILE") == 0)
		{
			escriure("Send FILE\n");
		}

		free(comanda);
		free(arrayComanda);
		return 0;
	}
	else if (correcte == 1)
	{
		free(comanda);
		free(arrayComanda);
		return 0;
	}
	else
	{
		int pid;

		pid = fork();

		switch (pid)
		{
		case -1:
			escriure("Error al crear el fork\n");
			exit(-1);

		case 0:
			if (execvp(arrayComanda[0], arrayComanda) < 0)
			{
				escriure("Unknown command\n");
			}
			// alliberar memoria que estigui per sobre el fork
			_exit(0);

		default:
			wait(NULL);
		}

		free(arrayComanda);
		free(comanda);
		return 0;
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

int connectarServidor()
{

	int socketFD;
	struct sockaddr_in servidor;

	if ((socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		escriure("Error creant el socket\n");
	}

	bzero(&servidor, sizeof(servidor));
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(config.portS);

	if (inet_pton(AF_INET, config.ipS, &servidor.sin_addr) < 0)
	{
		escriure("Error configurant IP\n");
	}

	if (connect(socketFD, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
	{
		escriure("Error fent el connect\n");
	}

	return socketFD;
}



/******************************************************************
 * 
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 * 
 * *****************************************************************/

void llistarUsuaris()
{
	int i;
	char *buffer;

	asprintf(&buffer, "There are %d children of Iluvatar connected:\n", totalUsuaris);
	escriure(buffer);
	free(buffer);

	for (i = 0; i < totalUsuaris; i++)
	{
		asprintf(&buffer, "%d. %s\t%s\t%d\t%d\n", i + 1, usuaris[i].nom, usuaris[i].ip, usuaris[i].port, usuaris[i].pid);
		escriure(buffer);
		free(buffer);
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

void enviarInfo(Config config)
{
	int pid;

	pid = getpid();

	write(fdSocket, config.nom, strlen(config.nom));
	write(fdSocket, "\n", sizeof(char));
	write(fdSocket, config.ipC, strlen(config.ipC));
	write(fdSocket, "\n", sizeof(char));
	write(fdSocket, &config.portC, sizeof(int));
	write(fdSocket, &pid, sizeof(int));
}





/******************************************************************
 * 
 * @Nom:
 * @Finalitat:
 * @Parametres:
 * @Retorn:
 * 
 * *****************************************************************/

void llegirUsuaris(int totalUsuaris)
{

	int i;
	char *nom, *ip;
	int port, pid;
	usuaris = (Usuaris *)malloc(sizeof(Usuaris) * totalUsuaris);

	for (i = 0; i < totalUsuaris; i++)
	{
		nom = read_until(fdSocket, '\n');
		ip = read_until(fdSocket, '\n');
		read(fdSocket, &port, sizeof(int));
		read(fdSocket, &pid, sizeof(int));

		// usuaris[i].nom = (char *) malloc ((strlen(nom)+1) * sizeof(char));
		// usuaris[i].ip = (char *) malloc ((strlen(ip)+1) * sizeof(char));

		usuaris[i].nom = strdup(nom);
		usuaris[i].ip = strdup(ip);
		// strcpy(usuaris[i].nom,nom);
		// strcpy(usuaris[i].ip,ip);
		usuaris[i].port = port;
		usuaris[i].pid = pid;

		free(nom);
		free(ip);
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

int configSocketMsg()
{
	char *buffer;
	struct sockaddr_in servidor;
	int listenFD;

	if ((listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		escriure("Error creant el socket\n");
	}

	bzero(&servidor, sizeof(servidor));
	servidor.sin_port = htons(config.portC);
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

	asprintf(&buffer, "\nPort:------%d------\n", config.portC);
	escriure(buffer);
	free(buffer);

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

void *esperarMissatges()
{
	int listenFD, clientFD;

	listenFD = configSocketMsg();

	while (1)
	{
		char *nom, *ip, *msg;
		char *buffer;
		clientFD = accept(listenFD, (struct sockaddr *)NULL, NULL);

		nom = read_until(clientFD, '\n');
		ip = read_until(clientFD, '\n');
		msg = read_until(clientFD, '\n');
		asprintf(&buffer, "\n\nNew message recived!\n%s, from %s says:\n%s\n\n$", nom, ip, msg);
		escriure(buffer);
		free(buffer);
		free(nom);
		free(ip);
		free(msg);
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

int main(int argc, char **argv)
{
	pthread_t thread;
	char *buffer;
	int comanda = 0;

	if (argc != 2)
	{
		escriure("ERROR, falta el fitxer de configuració\n");
		return -1;
	}
	else
	{
		llegirConfig(argv[1]);
		int resultat = pthread_create(&thread, NULL, esperarMissatges, NULL);

		if (resultat != 0)
		{
			escriure("Error al crear el thread\n");
			exit(0);
		}
		signal(SIGINT, (void *)controlC);

		fdSocket = connectarServidor();
		enviarInfo(config);
		read(fdSocket, &totalUsuaris, sizeof(int));
		llegirUsuaris(totalUsuaris);

		asprintf(&buffer, "Welcome %s, son of Iluvatar\n", config.nom);
		escriure(buffer);
		free(buffer);

		while (comanda == 0)
		{
			comanda = gestionarComanda();
		}

		raise(SIGINT);
	}
}