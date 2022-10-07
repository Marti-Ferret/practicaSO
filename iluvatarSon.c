#include "iluvatarSon.h"


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


int main(int argc, char ** argv){
	Config config;

	if(argc != 2){
		escriure("ERROR, falta el fitxer de configuració\n");
		return -1;
	}else{
		
		config = llegirConfig(config,argv[1]);	
		printf("%s %s %s %d %s %d",config.nom,config.directori,config.ipS,config.portS,config.ipC,config.portC);		
	}

}
