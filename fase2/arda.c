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

int main(int argc, char *agrv[]){
	Config config;

	if(argc != 2){
		escriure("Error, falten fichers\n");
		return 0;
	}
	
	signal(SIGINT, (void * ) controlC);
	
	escriure("\nARDA SERVER\n");
	config = llegirConfig(config,agrv[1]);
/*
	while(1){
		

	}
*/	
}
