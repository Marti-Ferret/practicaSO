#include "iluvatarSon.h"
#include "reads.h"

void controlC(void){
	escriure("\nDisconnecting from Arda. See you soon, son of Iluvatar\n");
	signal(SIGINT, SIG_DFL);
    raise(SIGINT);

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
		if(numParaules == 2){
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
			escriure("Update\n");

		}else if(strcasecmp(arrayComanda[0],"LIST") == 0){
			escriure("List\n");

		}else if(strcasecmp(arrayComanda[0],"EXIT") == 0){
			free(comanda);
			free(arrayComanda);
			raise(SIGINT);

		}else if(strcasecmp(arrayComanda[0],"SEND") == 0 && strcasecmp(arrayComanda[1],"MSG") == 0){
			escriure("Send MSG\n");

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
				escriure("Aquesta comanda no existeix\n");
				
				
			}
		}else{
			wait(NULL);
			free(arrayComanda);
			free(comanda);
		}
		return 0;
	}
	
}

void connectarServidor(Config config){
	
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
		connectarServidor(config);
		asprintf(&buffer,"Welcome %s, son of Iluvatar\n",config.nom);
		escriure(buffer);
		free(buffer);

		while(comanda == 0){
			comanda = gestionarComanda();
		}
		

		raise(SIGINT);
		
	}

}
