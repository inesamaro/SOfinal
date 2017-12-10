#include "header.h"

void *namedPipe() {
  int num_chars;
  char *token;
  char space[2] = " ";
  newNumTriagens = 0;
  char string[30];
  double total_time;
  struct timespec helper, finish;

  while(1) {
    num_chars = read(fdpipe, string, 400);
    if (num_chars > 0) {
      printf("string: %s\n", string);
      token = strtok(string, space);
      printf("token: %s\n",token );

      if (strcmp(token, "TRIAGE") == 0) { //caso se esteja a enviar o comando para aumento de triagens
        printf("Pipe leu o comando TRIAGE\n");
        pthread_mutex_lock(&condNewTriagens_mutex);
        token = strtok(NULL, space);
        newNumTriagens = atoi(token);
        pthread_cond_signal(&condNewTriagens);
        pthread_mutex_unlock(&condNewTriagens_mutex);
      }

      else {
        printf("Pipe leu um novo paciente\n");
        clock_gettime(CLOCK_REALTIME, &helper);
        total_time = ((double)(helper.tv_sec-start.tv_sec) + ((helper.tv_nsec-start.tv_nsec)) / 1000000000.0);
        pthread_mutex_lock(&condNewPacientes_mutex);
        Node_paciente p;
        p = (Node_paciente)malloc(sizeof(Paciente));
        strcpy(p->info.nome, token);

        token = strtok(NULL, space);
        printf("token: %s\n",token );

        p->info.tempoTriagem = atoi(token);


        token = strtok(NULL, space);
        printf("token: %s\n",token );
        p->info.tempoAtend = atoi(token);

        token = strtok(NULL, "\n");
        printf("token: %s\n",token );
        p->mtype = atoi(token);

        p->info.inicio = total_time;
        p->info.inicioAtend = 0;
        p->info.inicioTriagem = 0;
        p->info.fimAtend = 0;

        Node_paciente aux = queuePacientes;
        while (aux->info.next != NULL) {
          aux = aux->info.next;
        }
        aux->info.next = p;
        printf("nome na queue: %s\n", (aux->info.next)->info.nome);
        printf("inicio do paciente: %f\n", (aux->info.next)->info.inicio);
        (aux->info.next)->info.next = NULL;

        Paciente* aux1 = queuePacientes;
        printf("queuePacientes:\n");
        while (aux1->info.next != NULL) {
          printf("nome: %s\n", aux1->info.nome);
          aux1 = aux1->info.next;
        }
        sem_post(&full);
        pthread_cond_signal(&condNewPacientes);
        pthread_mutex_unlock(&condNewPacientes_mutex);
      }
    }
  }
}
