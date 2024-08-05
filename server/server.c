#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
 
//funcao para reescrever o arquivo txt no destino
void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "arquivo.txt";
  char buffer[SIZE];
 
  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
} 
 
int main(){
  char *ip = "127.0.0.1"; //ip utilizado
  int port = 8080; //porta utilizada
  int e;
 
  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr; //informacoes do servidor
  socklen_t addr_size;
  char buffer[SIZE];

  //criacao do socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("Erro no socket");
    exit(1);
  }
  printf("Socket do servidor foi criado com sucesso.\n");
  //conectar ao servidor
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port; //configura a porta do servidor para conexao
  server_addr.sin_addr.s_addr = inet_addr(ip);
 
  //contrucao do bind
  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("Erro no bind.");
    exit(1);
  }
  printf("Bind sucedido.\n");
 
  //tratanto conexoes
  if(listen(sockfd, 10) == 0){
 printf("Esperando conexao...\n");
 }else{
 perror("Erro ao conectar");
    exit(1);
 }
 
  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

  //reescrever arquivo do cliente no destino (servidor)
  write_file(new_sock);
  
  printf("Arquivo transferido com sucesso.\n");
 
  return 0;
}