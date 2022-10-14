#include "iluvatarSon.h"

void controlC(void){
	escriure("\nDisconnecting from Arda. See you soon, son of Iluvatar\n");
	signal(SIGINT, SIG_DFL);
    raise(SIGINT);


}

char * read_untilEnter(int fd, char caracter, int i) {
    char * buffer = NULL;

    while (caracter != '\n' || i == 0) {
        read(fd, & caracter, sizeof(char));

  
        if (buffer == NULL) {
            buffer = (char * ) malloc(1 * sizeof(char));
        } else {
            buffer = (char * ) realloc(buffer, i + 1);
        }
        buffer[i] = caracter;

  
        if (caracter == '\n' || caracter == '\0') {
            buffer[i] = '\0';
            return buffer;
        }
        i++;
    }

    return buffer;
}

char* read_until(int fd, char end) {
     int i = 0, size;
      char c = '\0';
      char* string = (char*)malloc(sizeof(char));

     while (1) {
          size = read(fd, &c, sizeof(char));

          if (c != end && size > 0) {
              string = (char*)realloc(string, sizeof(char) * (i + 2));
              string[i++] = c;
          } else {
              break;
          }
      }

 		string[i] = '\0';
 	return string;
 }


Config llegirConfig(Config config, char *nomF){
	int fd;

	fd=open(nomF,O_RDONLY);

	if(fd != 0){
		config.nom = read_until(fd,'\n');
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
	}
	return 1;
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
			raise(SIGINT);
		}


	}

	return 0;
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
		printf("%s %s %s %d %s %d",config.nom,config.directori,config.ipS,config.portS,config.ipC,config.portC);

		asprintf(&buffer,"Welcome %s, son of Iluvatar\n",config.nom);
		escriure(buffer);
		free(buffer);

		while(comanda == 0){
			comanda = gestionarComanda();
		}

		raise(SIGINT);
	}

}
