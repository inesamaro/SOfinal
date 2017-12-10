#include "header.h"
void lerInfoPacientes() {
  FILE *fich = fopen("infoPacientes.txt", "r");
  char linha[400], info[50];
  //envia cada uma das linhas do ficheiro pelo pipe. LE TODAS AS LINHAS
  while (fscanf(fich, " %[^\n]", linha) != EOF) {
    write(fdpipe, linha, 400);
    printf("linha lida: %s\n", linha);

    char line[400], c;
    int count = 0;
    int ind = 0;

    read(fdpipe, line, 400);
    Paciente* p;
    p = (Paciente *)malloc(sizeof(Paciente));
    //nome
    while ((c = line[ind]) != ' ') {
      info[count] = c;
      count ++;
      ind ++;
    }
    info[count] = '\0';
    strcpy(p->info.nome, info);
    c = line[ind++];
    count = 0;

    //tempoTriagem
    while ((c = line[ind]) != ' ') {
      info[count] = c;
      count ++;
      ind ++;
    }
    p->info.tempoTriagem = atoi(info);
    c = line[ind++];
    count =0;

    //tempoAtend
    while ((c = line[ind]) != ' ') {
      info[count] = c;
      count ++;
      ind ++;
    }
    p->info.tempoAtend = atoi(info);
    c = line[ind++];
    count =0;

    //prioridade
    while ((c = line[ind]) != EOF) {
      info[count] = c;
      count ++;
      ind ++;
    }
    p->mtype = atoi(info);
    count =0;

    p->info.inicio = clock();
    p->info.inicioAtend = 0;
    p->info.inicioTriagem = 0;
    p->info.fimAtend = 0;

    Node_paciente aux = queuePacientes;
    while (aux->info.next != NULL) {
      aux = aux->info.next;
    }
    aux->info.next = p;
    printf("[lerInfo] nome na queue: %s\n", (aux->info.next)->info.nome);
    p->info.next = NULL;
    sem_post(&full);
  }
}
