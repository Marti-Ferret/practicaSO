#include "iluvatarSon.h"
#include "reads.h"

int fdSocket;
Usuaris *usuaris;
int totalUsuaris=0;

void llegirUsuaris(int totalUsuaris);
void llistarUsuaris();
void controlC(void){
	escriure("\nDisconnecting from Arda. See you soon, son of Iluvatar\n");
	signal(SIGINT, SIG_DFL);
    raise(SIGINT);

}

/**
void enviarMissatge(char *nomDest, char *msg){
	
}
*/
int comprobarNom(char *nom){
	int i,correcte=0;
	for(i=0; i<totalUsuaris; i++){
		if(strcasecmp(nom,usuaris[i].nom)==0){
			correcte=1;
		}
	}
	return correcte;
}

void enviarMissatge(){
	char *buffer;
	int i;
	for(i=0; i<totalUsuaris; i++){
		asprintf(&buffer,"%d. %s\t%s\t%d\t%d\n",i+1,usuaris[i].nom,usuaris[i].ip,usuaris[i].port,usuaris[i].pid);
		escriure(buffer);
		free(buffer);


	}

}

char *validarNom(char *nom){
	int i=0,j=0;
	
	char *aux = (char*)malloc(sizeof(char));
	
	while(nom[j] != '\0'){
		
		if(nom[j] != '&'){
			aux = (char*) realloc(aux,sizeof(char) * (i+2));
			aux[i++] = nom[j];

		}
		
		j++;
	}
	aux[i] = '\0';
	return aux;

}


Config llegirConfig(Config config, char *nomF){
	int fd;
	char *nom;

	fd=open(nomF,O_RDONLY);

	if(fd != 0){
		nom = read_until(fd,'\n');
		config.nom = validarNom(nom);
		config.directori = read_until(fd,'\n');
		config.ipS = read_until(fd,'\n');
		config.portS = atoi(read_until(fd,'\n'));
		config.ipC = read_until(fd,'\n');
		config.portC = atoi(read_until(fd,'\n'));

	}else{
		
		escriure("ERROR, no s'ha pogut llegir el fitxer de configuració\n");
		close(fd);
		exit(0);
				

	}
	free(nom);
	close(fd);
	return config;
}

int validarComanda (int numParaules, char **arrayComanda){
	
	if(strcasecmp(arrayComanda[0],"UPDATE")== 0){
		if(numParaules == 2){
			if(strcasecmp(arrayComanda[1],"USERS") == 0){
				return 0;
			}else {
				return 1;
			}
		}


	}else if(strcasecmp(arrayComanda[0],"LIST") == 0){
		if(numParaules == 2){
			if(strcasecmp(arrayComanda[1],"USERS") == 0){
				return 0;
			}
		}

	}else if(strcasecmp(arrayComanda[0],"EXIT")==0){
		if(numParaules == 1){
			return 0;
		}
	}else if(strcasecmp(arrayComanda[0],"SEND")==0){
		if(numParaules == 4){
			if(strcasecmp(arrayComanda[1],"MSG") == 0){
				return 0;
			}else if(strcasecmp(arrayComanda[1],"FILE") == 0){
				return 0;
			}
		}
	}

	return 2;
}

int gestionarComanda(){
	char * comanda;
	char * (* arrayComanda);
	int numParaules=0;
	int correcte;
	int i=0;

	escriure("$ ");

	comanda = read_untilEnter(STDIN_FILENO, '\n', 0);
	
	if (comanda[0] == '\0') {
        free(comanda);
        return 0;
    }
		
	arrayComanda = (char **) malloc(sizeof(char *));
    arrayComanda[i] = strtok(comanda, " ");

	while(arrayComanda[i] != NULL){
    	numParaules++;
    	arrayComanda= realloc(arrayComanda, (numParaules +1) * sizeof(char * ));
		arrayComanda[++i] = strtok(NULL, " ");
	}
	
	correcte = validarComanda(numParaules,arrayComanda);

	if(correcte == 0){
		if(strcasecmp(arrayComanda[0],"UPDATE") == 0){
			write(fdSocket,"U",sizeof(char));
			read(fdSocket,&totalUsuaris,sizeof(int));
			llegirUsuaris(totalUsuaris);
			escriure("Users list updated\n");

		}else if(strcasecmp(arrayComanda[0],"LIST") == 0){
			llistarUsuaris();

		}else if(strcasecmp(arrayComanda[0],"EXIT") == 0){
			write(fdSocket,"S",sizeof(char));
			free(comanda);
			free(arrayComanda);
			raise(SIGINT);

		}else if(strcasecmp(arrayComanda[0],"SEND") == 0 && strcasecmp(arrayComanda[1],"MSG") == 0){
			if(comprobarNom(arrayComanda[2])==1){
					escriure("CORRECTE");
			}else{
				escriure("User not found in the list\n");
			}
			

		}else if(strcasecmp(arrayComanda[0],"SEND") == 0 && strcasecmp(arrayComanda[1],"FILE") == 0){
			escriure("Send FILE\n");
		}

		free(comanda);
		free(arrayComanda);
		return 0;

	}else if(correcte == 1){
		free(comanda);
		free(arrayComanda);
		return 0;

	}else{
		int pid;
		
		pid = fork();
		if(pid < 0){
			escriure("Error al crear el fork");
			exit(1);
		}else if(pid == 0){
			if(execvp(arrayComanda[0],arrayComanda) < 0){
				escriure("Unknown command\n");
				
				
			}
		}else{
			wait(NULL);
			free(arrayComanda);
			free(comanda);
		}
		return 0;
	}
	
}

int connectarServidor(Config config){
	
	int socketFD;
	struct sockaddr_in servidor;

	if( (socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    	escriure("Error creant el socket\n");
  	}

  	bzero(&servidor, sizeof(servidor));
  	servidor.sin_family = AF_INET;
  	servidor.sin_port = htons(config.portS);

  	if(inet_pton(AF_INET, config.ipS, &servidor.sin_addr) < 0){
    	escriure("Error configurant IP\n");
  	}

  	if(connect(socketFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
    	escriure("Error fent el connect\n");
	}
	return socketFD;
}

void llistarUsuaris(){
	int i;
	char *buffer;

	asprintf(&buffer,"There are %d children of Iluvatar connected:\n",totalUsuaris);
	escriure(buffer);
	free(buffer);

	for(i=0; i<totalUsuaris; i++){
		asprintf(&buffer,"%d. %s\t%s\t%d\t%d\n",i+1,usuaris[i].nom,usuaris[i].ip,usuaris[i].port,usuaris[i].pid);
		escriure(buffer);
		free(buffer);


	}

}

void enviarInfo(Config config){
	int pid;
	
	pid = getpid();

	write(fdSocket,config.nom,strlen(config.nom));
	write(fdSocket,"\n",sizeof(char));
	write(fdSocket,config.ipC,strlen(config.ipC));
	write(fdSocket,"\n",sizeof(char));
	write(fdSocket,&config.portC,sizeof(int));
	write(fdSocket,&pid,sizeof(int));

}

void llegirUsuaris(int totalUsuaris){
	
	int i;
	char *nom, *ip;
	int port,pid;
	usuaris = (Usuaris*) malloc (sizeof(Usuaris) * totalUsuaris);

	for(i=0;i<totalUsuaris;i++){
		nom = read_until(fdSocket,'\n');
		ip = read_until(fdSocket,'\n');
		read(fdSocket,&port,sizeof(int));
		read(fdSocket,&pid,sizeof(int));

		usuaris[i].nom = (char *) malloc ((strlen(nom)+1) * sizeof(char));
		usuaris[i].ip = (char *) malloc ((strlen(ip)+1) * sizeof(char));

		strcpy(usuaris[i].nom,nom);
		strcpy(usuaris[i].ip,ip);
		usuaris[i].port = port;
		usuaris[i].pid = pid;
		
		
		free(nom);
		free(ip);
	}


}

int main(int argc, char ** argv){
	Config config;
	char* buffer;
	int comanda = 0;
	
	if(argc != 2){
		escriure("ERROR, falta el fitxer de configuració\n");
		return -1;
	}else{

		signal(SIGINT, (void * ) controlC);		
		config = llegirConfig(config,argv[1]);
		fdSocket = connectarServidor(config);
		enviarInfo(config);
		read(fdSocket,&totalUsuaris,sizeof(int));
		llegirUsuaris(totalUsuaris);	

		asprintf(&buffer,"Welcome %s, son of Iluvatar\n",config.nom);
		escriure(buffer);
		free(buffer);

		while(comanda == 0){
			comanda = gestionarComanda();
		}
		

		raise(SIGINT);
		
	}

}
