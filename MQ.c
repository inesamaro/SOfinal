#include "header.h"

//Criaçao da message message queue
void criarMQ(){
  if ((mqid = msgget(IPC_PRIVATE, IPC_CREAT|0777)) < 0) {
    perror("Error creating message queue");
  }
  else{
    printf("-------------> message queue criada\n");
  }
}

  //Por o paciente na message queue
void sendMQ(Paciente paciente){
  int msgsend;
  if ((msgsend = msgsnd(mqid, &paciente, sizeof(paciente)-sizeof(long), 0)) == -1){
    perror("Oh boy u got a error sending a msg to the messagequeue");
  }
  else {
    //printf("prioridade no send %ld\n", paciente.mtype);
    //printf("mqid no send %d\n", mqid);
    printf("%s <---------------------------------------entrei na mq\n", paciente.info.nome);
    //sem_wait(&sem_sharedvar);
    //(shared_var)->countPacientesQueue++;
    //sem_post(&sem_sharedvar);
    }
}
