#include "arda.h"
#include "reads.h"

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

void * threadClients(void *clientFD){
	
	int fd = *((int*) clientFD);



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

	while(1){
		escriure("Waiting for connections...");
		clientFD = accept(listenFD,(struct sockaddr*) NULL, NULL);

		pthread_t threadClient;
		pthread_create( &threadClients, NULL, threadClients, &clientFD);
	}

	raise(SIGINT);
}
