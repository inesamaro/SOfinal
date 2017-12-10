#include "header.h"

Paciente* criarQueuePacientes() {
  //criar cabeçalho da lista de pacientes (queue)
  aux = (Paciente*)malloc(sizeof(Paciente));
  if (aux != NULL) {
    aux->mtype = 0;
    strcpy(aux->info.nome, "oiiiiii galera");
    aux->info.numChegada = 0;
    aux->info.tempoTriagem = 0;
    aux->info.tempoAtend = 0;
    aux->info.inicio = 0;
    aux->info.inicioTriagem = 0;
    aux->info.inicioAtend = 0;
    aux->info.fimAtend = 0;
    aux->info.next = NULL;
  }
  printf("cria queuePacientes com sucesso!\n");
  return aux;
}
