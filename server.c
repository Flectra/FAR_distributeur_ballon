#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define PORT 10001

int testFichierVide(FILE* fichier)
{
    int caracterePremier = 0;
 
    //On lit le prmeier caractère du fichier
    caracterePremier = fgetc(fichier);
    if(caracterePremier == EOF)
    {
        return 1;//le fichier est vide donc on retourne 1
    }
    return 0;//le fichier n'est pas vide donc on retourne 0
}

int main(void) {
    FILE *fichierBall = fopen("Ballon.txt","r"); 

    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    

  for(;;) {
    /* Attente d'une connexion client */
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    printf("connected\n");

    if (testFichierVide(fichierBall)){
      char pasBallon[1024] = "pas de ballon";
      send(client_sock, pasBallon,1024, 0);
    }
    else{
      char BallonEnvoi[1024];
      fgets(BallonEnvoi, 1024, fichierBall);
      send(client_sock, BallonEnvoi, 1024, 0);
    }
     fclose(fichierBall);
    /* Reception de donnees du client */
     char buffer[1024];
     printf("attente\n");
    int res = recv(client_sock, buffer, 1024, 0);
    printf("recv %s\n", buffer);
      if(strcmp( buffer, "pas de ballon")!=0){
        fichierBall = fopen( "Ballon.txt" , "w" );
        fwrite(buffer,1,strlen(buffer),fichierBall);
        fclose(fichierBall);
      }
     /* printf("Recu de la socket %d de %s:%d : %s\n", client_sock,
               inet_ntoa(csin.sin_addr), htons(csin.sin_port), buffer);*/

      /* Fermeture de la socket dans les deux sens */
      shutdown(client_sock, 2);

      /* Fermeture de la socket client */
      close(client_sock);
  }//for

  /* Fermeture de la socket serveur */
  close(socket_desc);

  return EXIT_SUCCESS;

}//main
