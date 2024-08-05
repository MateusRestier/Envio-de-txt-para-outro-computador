#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h> // biblioteca para mexer com data
#define SIZE 1024

long calcularTamanhoArquivo(FILE *arquivo)
{

  // guarda o estado ante de chamar a função fseek
  long posicaoAtual = ftell(arquivo);

  // guarda tamanho do arquivo
  long tamanho;

  // calcula o tamanho
  fseek(arquivo, 0, SEEK_END);
  tamanho = ftell(arquivo);

  // recupera o estado antigo do arquivo
  fseek(arquivo, posicaoAtual, SEEK_SET);

  return tamanho;
}

int main()
{
  char *ip = "127.0.0.1"; // ip utilizado
  int port = 8080;        // porta utilizada
  int e, n;

  int sockfd;
  struct sockaddr_in server_addr; // informacoes do servidor
  FILE *fp;
  char *filename = "arquivo.txt"; // nome do arquivo

  // criacao do socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("Erro no socket");
    exit(1);
  }
  printf("Socket do servidor foi criado com sucesso.\n");

  // conectar ao servidor
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port; // configura a porta do servidor para conexao
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e == -1)
  {
    perror("Erro no socket.");
    exit(1);
  }
  printf("Conectado ao servidor.\n");

  // abrir o arquivo
  fp = fopen(filename, "r");
  long tamanho = calcularTamanhoArquivo(fp);
  if (fp == NULL)
  {
    perror("Erro ao ler o arquivo.");
    exit(1);
  }

  // enviar o arquivo
  char data[SIZE] = {0};

  struct timeval start, end;

  gettimeofday(&start, NULL); // primeira captura do horário

  while (fgets(data, SIZE, fp) != NULL)
  {
    if (send(sockfd, data, sizeof(data), 0) == -1)
    {
      perror("Erro ao enviar o arquivo.");
      exit(1);
    }
    bzero(data, SIZE);
  }

  // segunda captura do horário
  gettimeofday(&end, NULL);
  double segundos = (double)((((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec) - (start.tv_usec))/1000000;
  
  double mbps = (((double)tamanho * 8) / segundos) / 1000000;
  printf("\nVelocidade: %lf\n", mbps);

  printf("Arquivo enviado com sucesso.\n");

  printf("Fechando a conexao.\n");
  close(sockfd);

  return 0;
}
