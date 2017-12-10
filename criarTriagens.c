#include "header.h"

void criarTriagens() {
  pthread_t triagemPrinc;
  int i;
  printf("A criar a triagem principal\n");
  pthread_create(&triagemPrinc, NULL, workPrinc, 0);


  /*//cria o numero de triagens necessarias
  for (i=1; i<=config->nTriagens; i++) {
    idsTriagens[i] = i;
    paciente = queuePacientes->info.next;
    //printf("Paciente que vai ser eliminado: %s\n", paciente->info.nome);
    queuePacientes->info.next = (queuePacientes->info.next)->info.next;


    pthread_create(&vTriagens[i], NULL, triagem, paciente);

    Paciente* aux = queuePacientes;
    //printf("QueuePacientes depois de eliminar: \n");
    while(aux->info.next != NULL) {
      aux = aux->info.next;
      //printf("nome: %s\n", aux->info.nome);
    }
  }

  //for (i=0; i<config->nTriagens; i++) {
    //pthread_join(vTriagens[i], NULL);
  //}*/
}
