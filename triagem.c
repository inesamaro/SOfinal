#include "header.h"

void *triagem(void* a){
  printf("Uma thread entrou na triagem!\n");
  //printf("TRIAGEM HAPPENING\n");
  struct msqid_ds buf;
  int num_pacientes;
  Paciente *paciente;
  int id = *((int*)a);

  while(1) {
    sem_wait(&full);
    printf("oi\n");
    Paciente* aux1 = queuePacientes;
    while (aux1->info.next != NULL) {
      printf("nome: %s\n", aux1->info.nome);
      aux1 = aux1->info.next;
    }
    pthread_mutex_lock(&mutex);
    if (id > triagensParaApagar ) {
      pthread_mutex_unlock(&mutex);
      printf("Vou sair\n");
      pthread_exit(NULL);
    }
    if (queuePacientes->info.next != NULL) {
      printf("No if\n");
      paciente = queuePacientes->info.next; //vamos buscar o primeiro paciente da queuePacientes
      queuePacientes->info.next = (queuePacientes->info.next)->info.next;

      sleep(paciente->info.tempoTriagem);
      paciente->info.next = NULL;

      msgctl(mqid, IPC_STAT, &buf);
      num_pacientes = buf.msg_qnum;

      if (num_pacientes > config->queueMax) {
        sem_post(&sem_queueMax);
      }

      sendMQ(*paciente); //envia o paciente pelo MQ para que possa ser buscado pelos DOUTORES
      shared_var->nTriados ++;

      pthread_mutex_unlock(&mutex);

    }
    else {
      printf("Entrou no else\n");
      pthread_mutex_lock(&condNewPacientes_mutex);
      printf("A espera do condNewPacientes\n");
      pthread_cond_wait(&condNewPacientes, &condNewPacientes_mutex);
      printf("condNewPacientes ativo!\n");
      pthread_mutex_unlock(&condNewPacientes_mutex);
      printf("SAi do unlock\n");
      pthread_mutex_unlock(&mutex);
    }
  }
}
