#include "header.h"

/*void atendEspecial(){
  sem_wait(&sem_sharedvar);
  while((shared_var)->countPacientesQueue > (int)(config->queueMax * 0.8 + 0.5)){
    Paciente paciente;
    struct timespec start, finish;

    double shift_time, left_time;
    clock_gettime(CLOCK_REALTIME, &start);
    clock_gettime(CLOCK_REALTIME, &finish);
    shift_time = ((double)(finish.tv_sec-start.tv_sec) + ((finish.tv_nsec-start.tv_nsec)) / 1000000000.0);

    clock_gettime(CLOCK_REALTIME, &finish);
    shift_time = ((double)(finish.tv_sec-start.tv_sec) + ((finish.tv_nsec-start.tv_nsec)) / 1000000000.0);
    while (shift_time < config->tempoTurno) {
      sem_wait(&sem_mq);
      if (msgrcv(mqid, &paciente, sizeof(paciente)-sizeof(long), -3, 0) == -1){
    	   perror("Oh boy u got a error recieving a msg from the messagequeue");
      }
      sem_post(&sem_mq);

      sleep(paciente.info.tempoAtend);

      sem_wait(&sem_sharedvar);
      shared_var->countPacientesQueue --;
      shared_var->nAtendidos ++;
      sem_post(&sem_sharedvar);

      printf("-----------------------------------> %s, %ld\n",paciente.info.nome, paciente.mtype);

      end = clock();
      shift_time = (double)(end - start); // / CLOCKS_PER_SEC;
      printf("shift time %f // tempo doctor %ld\n", shift_time, config->tempoTurno);

    }
  }
  sem_post(&sem_sharedvar);
  exit(0);
}*/

void sigAlarm(int sig){
  exit(0);
}

void atendimento() {
  Paciente paciente; // = (Paciente)malloc(sizeof(struct pa
  struct timespec start, finish;

  double shift_time, left_time;
  clock_gettime(CLOCK_REALTIME, &start);
  clock_gettime(CLOCK_REALTIME, &finish);

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

    sem_wait(&sem_sharedvar);
    (shared_var)->countPacientesQueue--;
    (shared_var)->nAtendidos ++;
    sem_post(&sem_sharedvar);

    sleep(paciente.info.tempoAtend);

    clock_gettime(CLOCK_REALTIME, &finish);
    shift_time = ((double)(finish.tv_sec-start.tv_sec) + ((finish.tv_nsec-start.tv_nsec)) / 1000000000.0);
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

  while(1){ //sempre que um dos processos morre um novo é criado
      if(fork() == 0){
        sem_wait(&sem_queueMax);
        printf("[%d] ATENDESPECIAL INICIO DOUTOR\n", getpid());
        //atendEspecial();
        printf("[%d] ATENDESPECIAL FIM DOUTOR\n", getpid());
        exit(0);
      }
      wait(NULL);
      msgctl(mqid, IPC_STAT, &buf);
      num_pacientes = buf.msg_qnum;
      if(fork() == 0){
        printf("[%d] NEW INICIO DOUTOR\n", getpid());
        atendimento(); //por fazer
        printf("[%d] NEW FIM DOUTOR\n", getpid());
        exit(0);
      }
      printf("numero de pacientes na queue: %d\n", num_pacientes);
    }
 //de momento manter comentado para nao criar demasiados doutores
}
