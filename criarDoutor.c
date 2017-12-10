#include "header.h"

void atendEspecial(){
  struct msqid_ds buf;
  int num_pacientes;
  num_pacientes = buf.msg_qnum;
  Paciente paciente;

  while(num_pacientes > (int)(config->queueMax * 0.8 + 0.5)){
      if (msgrcv(mqid, &paciente, sizeof(paciente)-sizeof(long), -3, 0) == -1){
    	   perror("Oh boy u got a error recieving a msg from the messagequeue");
      }
      sleep(paciente.info.tempoAtend);

      sem_wait(&sem->semProcess);
      shared_var->countPacientesQueue --;
      shared_var->nAtendidos ++;
      sem_post(&sem->semProcess);

      printf("-----------------------------------> %s, %ld\n",paciente.info.nome, paciente.mtype);
  }

  exit(0);
}

void sigAlarm(int sig){
  exit(0);
}

void atendimento() {
  Paciente paciente; // = (Paciente)malloc(sizeof(struct pa
  struct timespec startProcess, finishProcess, helper1, helper2;

  double shift_time, left_time;
  clock_gettime(CLOCK_REALTIME, &startProcess);
  clock_gettime(CLOCK_REALTIME, &finishProcess);

  while (shift_time < config->tempoTurno) {
    signal(SIGALRM, sigAlarm);
    left_time = config->tempoTurno - shift_time;
    alarm(left_time);
    printf("%f\n", left_time);

    if (msgrcv(mqid, &paciente, sizeof(paciente)-sizeof(long), -3, 0) == -1){
      perror("Oh boy u got a error recieving a msg from the messagequeue");
    }
    printf("[%d]-------------------------------------------------> %s COM %ld\n",getpid(), paciente.info.nome, paciente.mtype);
    signal(SIGALRM, SIG_IGN);

    sem_wait(&sem->semProcess);
    (shared_var)->countPacientesQueue--;
    (shared_var)->nAtendidos ++;
    sem_post(&sem->semProcess);

    sleep(paciente.info.tempoAtend);

    clock_gettime(CLOCK_REALTIME, &finishProcess);
    shift_time = ((double)(finishProcess.tv_sec-startProcess.tv_sec) + ((finishProcess.tv_nsec-startProcess.tv_nsec)) / 1000000000.0);
    printf("[%d]TEMPO DO SHIFT %f\n", getpid(), shift_time);
  }
}

void criarDoutor() {
  struct msqid_ds buf;
  int i, num_pacientes;
  for (i=0; i<config->nDoutores; i++) {
    if (fork() == 0) {
      printf("[%d] INICIO DOUTOR\n", getpid());
      atendimento();//por fazer
      printf("[%d] FIM DOUTOR\n", getpid());
      exit(0);
    }
  }

  if(fork() == 0){ //NAO E E UM DOUTOR
    while(1){
      sem_wait(&sem_queueMax);
      if (fork() == 0) {
        printf("[%d] ATENDESPECIAL INICIO DOUTOR\n", getpid());
        //atendEspecial();
        printf("[%d] ATENDESPECIAL FIM DOUTOR\n", getpid());
        exit(0);
      }
    }
  }

  while(1){ //sempre que um dos processos morre um novo é criado
    wait(NULL);
    msgctl(mqid, IPC_STAT, &buf);
    num_pacientes = buf.msg_qnum;
    if(num_pacientes != 0){
      if(fork() == 0){
        printf("[%d] NEW INICIO DOUTOR\n", getpid());
        atendimento(); //por fazer
        printf("[%d] NEW FIM DOUTOR\n", getpid());
        exit(0);
      }
    }
    printf("numero de pacientes na queue: %d\n", num_pacientes);
  }
}
