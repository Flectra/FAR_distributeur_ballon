#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define PORT 10001



typedef struct ball ball;
struct ball{int ID; char IPjoueur[32]; int chrono};

char *IP="127.0.0.1"; // adresse provisoire, sera remplacée par l'IP du robot

int code = 1; // ne pas oublier de l'incrémenter, variable globale

/*function qui génère un ballon à distribuer*/
ball GenerateBall(char *IPjoueur){

  ball Ballon;
  strcpy(Ballon.IPjoueur,IPjoueur);
  /*Initialisation pour la date en seconde*/
  time_t temps;
  time(&temps);

 	Ballon.chrono = temps;

  Ballon.ID = code;
  return Ballon;
}

/*Sérialisation d'un ballon*/

char* Serialize(ball Ballon){

  char str1[1024];
  sprintf(str1,"%d",Ballon.ID);
  fflush(stdout);
  strcat(str1,"*");
  strcat(str1,Ballon.IPjoueur);
  strcat(str1,"*");
  char str3[128];
  sprintf(str3,"%d",Ballon.chrono);
  strcat(str1,str3);
  return str1;

}


// tableau des ballons

ball Ballons[4]; // à mettre dans le gestionnaire de stock de ballon

// 1- fonction récupération logs 





int main(){

	// réception des logs

	///parsage

	int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

	//boucle infinie
	 for(;;){
	 	

	 	//if passage RFID commment le matérialiser ?
	    //Connexion au serveur robot
	    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	    {
	        perror("connect failed. Error");
	        return 1;
	    }
	     
	    puts("Connected\n");

	  	/* Reception de donnees du serveur ( Réponse à la question : Avez vous un ballon ?)*/
	  	char buffer[1024];
	  	recv(sock, buffer, 1024, 0);
	  	printf("Recu : %s\n", buffer);

	  	/* Initialisation du test ballon */
	  	int BallonValide= 0;
	  	char* ballonRecu[256];

	  	/* Test ballon valide*/

	  	char buffer2[1024] = "pas de ballon";

	  	int res = strcmp(buffer, buffer2);

	  	if(res != 0){
	    	/*Decrypt(buffer, ballonRecu);*/
	    	const char s[2] = "*";

	    	char *token;
	    	token = strtok(ballonRecu,s); // récupération de L'ID
	    	int i = 0;
	    	while(i<=code && BallonValide == 0){
	      		char var = (char)i;
	      		if(strcmp(token,var)!=0){
	        		i++;
	      		}
	      		else{
		        	token = strtok(NULL,s); // récupération de l'IP
		        	token = strtok(NULL,s); // récupération de l'heure en seconde
		        	time_t tempsVerif;
		        	time(&tempsVerif);
		        	int tempsRecup = atoi(token);
	        		if( tempsVerif < tempsRecup+180){
	          			BallonValide = 1; 
	        		}
	      		}
	        }  
	    }
	}

	return 0;

}