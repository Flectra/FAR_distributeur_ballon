#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//télécherger et installer la librairie Openssl 
#include "time.h"


// à vérifier !
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


int sock;


int main(void) {
  
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
 
    //Connect to remote server
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
	/*Demande ballon Dispo*/

  /* Création d'un ballon a envoyé au robot s'il n'en possède pas de valide*/
  if (BallonValide == 0){
    ball BallonEnvoi = GenerateBall(IP);
    code++; // on incrémente code pour le prochain ballon
    printf("%s \n", BallonEnvoi.IPjoueur);
    char *BallonClePublique = Serialize(BallonEnvoi);
    printf("%s\n",BallonClePublique );
    /*char *BallonClePrive;
    Encrypt(BallonClePublique, BallonClePrive );*/
  /* Envoi de donnees au serveur */
    /*char *pos = strchr(BallonClePrive, '\n');
    *pos = '\0';*/
    send(sock, BallonClePublique, 32, 0);
  }

  return EXIT_SUCCESS;

}//main

