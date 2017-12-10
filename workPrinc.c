#include "header.h"

void *workPrinc() {
  printf("Na funcao workPrinc\n");
  pthread_t *vTriagens = (pthread_t*)malloc(sizeof(pthread_t)* config->nTriagens);
//  Paciente *paciente;
  int dif, i;

  for (i=0; i<config->nTriagens; i++) {
    writeMmf("Triagem criada\n");
    pthread_create((vTriagens + i), NULL, triagem, 0);
  }
  
  int triagensAtuais = config->nTriagens;
  while(1){
    pthread_mutex_lock(&condNewTriagens_mutex);
    while (newNumTriagens == 0) {
      pthread_cond_wait(&condNewTriagens, &condNewTriagens_mutex);
    }
    printf("O comando TRIAGE foi recebido pela funcao triagens\n");
    if (newNumTriagens > triagensAtuais) { //criar nova triagens
      dif = newNumTriagens - triagensAtuais;
      for (i=triagensAtuais; i<=newNumTriagens; i++) {
        vTriagens = (pthread_t*)realloc(vTriagens, sizeof(pthread_t)*newNumTriagens);
        //paciente = queuePacientes->info.next;
        //printf("Paciente que vai ser eliminado: %s\n", paciente->info.nome);
        //queuePacientes->info.next = (queuePacientes->info.next)->info.next;
        pthread_create((vTriagens + i), NULL, triagem, 0);

        Paciente* aux = queuePacientes;
        while(aux->info.next != NULL) {
          aux = aux->info.next;
        }
      }
    }

    else {  //eliminar triagens
      dif = triagensAtuais - newNumTriagens;
      for (i=1; i<=dif; i++) {
      //  pthread_join((vTriagens + i), NULL);
        printf("Tens que eliminar threads!\n");
      }
    }
    triagensAtuais = newNumTriagens;
    newNumTriagens = 0;
    pthread_mutex_unlock(&condNewTriagens_mutex);
  }
  return NULL;
}
