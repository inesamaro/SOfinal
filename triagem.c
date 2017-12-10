#include "header.h"

void *triagem(){
  printf("Uma thread entrou na triagem!\n");
  //printf("TRIAGEM HAPPENING\n");
  struct msqid_ds buf;
  int num_pacientes;
  Paciente *paciente;

  while(1) {
    sem_wait(&full);
    printf("oi\n");
    Paciente* aux1 = queuePacientes;
    while (aux1->info.next != NULL) {
      printf("nome: %s\n", aux1->info.nome);
      aux1 = aux1->info.next;
    }
    pthread_mutex_lock(&mutex);
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
  /*pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  while(1){
    pthread_mutex_lock(&mutex);
    printf("estou prestes a ser triado(sleep): %s\n",paciente->info.nome );
    sleep((int)paciente->info.tempoTriagem);
    //printf("ESTA E A PROXIMA PESSOA A SER TRIADA %s\n",paciente->info.nome);
    //shared_var->nTriados += 1;
    //printf("numero de pessoas triadas: %d \n", (shared_var)->nTriados);
    sendMQ(*paciente);

    paciente = queuePacientes->info.next;
    //printf("Paciente que vai ser eliminado: %s\n", paciente->info.nome);
    while((queuePacientes->info.next) == NULL){
      continue;
    }
    queuePacientes->info.next = (queuePacientes->info.next)->info.next;

    pthread_mutex_unlock(&mutex);
  }

  return NULL;*/
