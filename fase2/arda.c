#include "arda.h"
#include "reads.h"

Usuaris *usuaris;
int totalUsuaris=0;

void controlC(void){
	escriure("\nDisconnecting from Arda. See you soon, son of Iluvatar\n");
	signal(SIGINT, SIG_DFL);
    raise(SIGINT);
}


Config llegirConfig(Config config, char *nomF){
	int fd;

	fd = open(nomF,O_RDONLY);
	
	if(fd != 0){
		escriure("Reading configuration file\n");
		config.ip = read_until(fd,'\n');
		config.port = atoi(read_until(fd,'\n'));
		config.directori = read_until(fd,'\n');

	}else{
		escriure("Error, no s'ha pogut llegir el fixter de configuració\n");
		close(fd);
		exit(0);
	}

	close(fd);
	return config;


}

int configSocket(Config config){

	struct sockaddr_in servidor;	
	int listenFD;

	if( (listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        escriure("Error creant el socket\n");
	}
	
	bzero(&servidor, sizeof(servidor));
    servidor.sin_port = htons(config.port);
    servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listenFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
    	escriure("Error fent el bind\n");
    }

	if(listen(listenFD, 10) < 0){
    	escriure("Error fent el listen\n");
    }

	return listenFD;
}

void enviarUsuaris(int fd){
	int i;

	for(i=0; i<totalUsuaris; i++){
		write(fd,usuaris[i].nom,strlen(usuaris[i].nom));
		write(fd,"\n",sizeof(char));
		write(fd,usuaris[i].ip,strlen(usuaris[i].ip));
		write(fd,"\n",sizeof(char));
		write(fd,&usuaris[i].port,sizeof(int));
		write(fd,&usuaris[i].pid,sizeof(int));
	}

}

void afegirUsuari(char* nom, char* ip, int port,int pid, int fd){
	escriure("Updating user's list\n");

	totalUsuaris++;
	usuaris = (Usuaris*) realloc(usuaris, ((totalUsuaris) * sizeof(Usuaris)));
	usuaris[totalUsuaris-1].nom = (char *) malloc((strlen(nom) +1) * sizeof(char));
	usuaris[totalUsuaris-1].ip = (char *) malloc((strlen(ip) +1) * sizeof(char));
	
	strcpy(usuaris[totalUsuaris-1].nom,nom);
	strcpy(usuaris[totalUsuaris-1].ip,ip);
	usuaris[totalUsuaris-1].port = port;
	usuaris[totalUsuaris-1].pid = pid;
	usuaris[totalUsuaris-1].fd = fd;
}

///////////////////////////// Aixó no funciona, audio whats
void eliminarUsuari(int fd){
	Usuaris *usuarisCopia;
	usuarisCopia = (Usuaris *) malloc (sizeof(Usuaris) * totalUsuaris -1);

	for(int i=0; i<totalUsuaris; i++){
		if(usuaris[i].fd != fd){
			usuarisCopia[i].nom = (char *) malloc ((strlen(usuaris[i].nom) +1) * sizeof(char));
			usuarisCopia[i].ip = (char *) malloc ((strlen(usuaris[i].ip) +1) * sizeof(char));

			strcpy(usuarisCopia[i].nom,usuaris[i].nom);
			strcpy(usuarisCopia[i].ip,usuaris[i].ip);
			usuarisCopia[i].port = usuaris[i].port;
			usuarisCopia[i].pid = usuaris[i].pid;
			usuarisCopia[i].fd = usuaris[i].fd;
			
		}

	}

	for(int i=0;i<totalUsuaris; i++){
		escriure(usuarisCopia[i].nom);

	}
	
	totalUsuaris--;
		
	usuaris = (Usuaris *) realloc (usuaris, ((totalUsuaris) * sizeof(Usuaris)));

	for(int i=0; i<totalUsuaris; i++){
		memcpy(usuaris[i].nom,usuarisCopia[i].nom,strlen(usuarisCopia[i].nom)+1);
		memcpy(usuaris[i].ip,usuarisCopia[i].ip,strlen(usuarisCopia[i].ip)+1);
		usuaris[i].port = usuarisCopia[i].port;
		usuaris[i].pid = usuarisCopia[i].pid;
		usuaris[i].fd = usuarisCopia[i].fd;

		escriure(usuaris[i].nom);
	}
}

int funcions(int fd, char opcio, char* nom){
	char *buffer;
	if(opcio == 'U'){

		asprintf(&buffer,"New Petition: %s demands the user's list\n",nom);
		escriure(buffer);
		free(buffer);
		
		write(fd,&totalUsuaris,sizeof(int));
		enviarUsuaris(fd);
		
		asprintf(&buffer,"Sending user's list to %s\n\n",nom);
		escriure(buffer);
		free(buffer);
		return 0;
	
	}else if(opcio == 'S'){
		asprintf(&buffer,"New exit petition: %s has left Arda\n",nom);
		escriure(buffer);
		free(buffer);

		eliminarUsuari(fd);

		return 1;

	}
	
	return 0;
}

void * threadClients(void *clientFD){
	int fd = *((int*) clientFD);
	char *buffer;
	int pid, port;
	char *nom,*ip;
	char opcio;
	int sortir=0;

	nom = read_until(fd,'\n');

	ip = read_until(fd,'\n');
	
	read(fd,&port,sizeof(int));
	
	read(fd,&pid,sizeof(int));

	asprintf(&buffer,"New Login: %s, IP: %s, port: %d, PID %d\n",nom,ip,port,pid);
	escriure(buffer);
	free(buffer);
	
	afegirUsuari(nom,ip,port,pid,fd);
	escriure("Sending user’s list\n");
	
	write(fd,&totalUsuaris,sizeof(int));
	enviarUsuaris(fd);
	escriure("Response sent\n\n");


	while(sortir != 1){
		
		read(fd,&opcio,sizeof(char));
		sortir = funcions(fd,opcio,nom);
	}

	free(ip);
	free(nom);
	return NULL;

}


int main(int argc, char *agrv[]){
	Config config;
	
	int clientFD,listenFD;
	
	if(argc != 2){
		escriure("Error, falten fichers\n");
		return 0;
	}
	
	signal(SIGINT, (void * ) controlC);
	
	escriure("\nARDA SERVER\n");
	config = llegirConfig(config,agrv[1]);
	listenFD = configSocket(config);
	
	escriure("Waiting for connections... \n\n");
	
	while(1){
		clientFD = accept(listenFD,(struct sockaddr*) NULL, NULL);
		
		pthread_t threadClient;
		pthread_create( &threadClient, NULL, threadClients, &clientFD);
	}

	raise(SIGINT);
}
