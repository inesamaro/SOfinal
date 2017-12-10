#include "header.h"
void lerFichConfig(){
  FILE *fich = fopen("config.txt", "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *token;
  long x=0;
  int count = 0;

  config = (Config*)malloc(sizeof(Config));

  while((read = getline(&line, &len, fich)) != -1){
    token = strtok(line,"=");
    token = strtok(NULL,"\n");
    x = atoi(token);
    if (count == 0) {
      config->nTriagens = x;
    }
    else if (count == 1) {
      config->nDoutores = x;//atoi(numero);
    }
    else if (count == 2) {
      config->tempoTurno = x; //atoi(numero);
    }
    else if (count == 3) {
      config->queueMax = x; //atoi(numero);
    }
    count ++;
  }

  printf("nTriagens: %ld\n", config->nTriagens);
  printf("nDoutores: %ld\n", config->nDoutores);
  printf("tempoTurno: %ld\n", config->tempoTurno);
  printf("queueMax: %ld\n", config->queueMax);
}
