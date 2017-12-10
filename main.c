#include "header.h"

int main(int argc, char *argv[]){
  clock_gettime(CLOCK_REALTIME, &start);
  int idSemProcess;
  if ((idSemProcess = shmget(IPC_PRIVATE, sizeof(Sem), IPC_CREAT|0777)) > 0 ) {
    if ((sem = shmat(idSemProcess, NULL, 0)) == (Sem*) -1){
      perror("error in shmat");
    }
    sem_init(&sem->semProcess, 1, 1);
  }
  else {
    printf("Erro ao criar a memória partilhada!");
  }

  //signal(SIGUSR1, sigusr1);
  //signal(SIGINT, terminar);
  sem_init(&sem_sharedvar, 0, 1);
  sem_init(&sem_mq, 0, 1);
  sem_init(&full, 0, 0);
  sem_init(&sem_queueMax, 0, 0);

  //inicializacao de processos
  pthread_cond_init(&condNewTriagens, NULL);
  pthread_mutex_init(&condNewTriagens_mutex, NULL);

  pthread_cond_init(&condNewPacientes, NULL);
  pthread_mutex_init(&condNewPacientes_mutex, NULL);

  pthread_mutex_init(&mutex, NULL);

  criarMemPartilhada();
  lerFichConfig();
  mmf();
  queuePacientes = criarQueuePacientes();

  if ((mkfifo(PIPE_NAME, O_CREAT|O_EXCL|0600) < 0) & (errno!= EEXIST)) {
    perror("cannot open pipe!");
    exit(0);
  }
  else{
    printf("Criou o pipe\n");
  }

  if ((fdpipe = open(PIPE_NAME, O_RDWR)) < 0) {
    perror("Cannot open pipe for writing: ");
    exit(0);
  }
  printf("O pipe esta aberto\n");

  lerInfoPacientes(); //le primiero os pacientes do ficheiro

  int idThreadPipe;
  if (pthread_create(&threadPipe, NULL, namedPipe, 0) != 0) { //thread que se responsabiliza por estar sempre a ler o que entra pelo pipe
    perror("Erro a criar a thread do pipe\n");
    exit(1);
  }
  //pthread_join(threadPipe, NULL);


  criarMQ();

  //TEMPO MAS SO EM SEGUNDOS, SEM CASAS DECIMAIS
  //HA UMA ESTRUTURA DO TEMPO NO HEADER E ESSA ESTRUTURA VEM AUTOMATICAMENTE COM O .TV_SEC e .TV_NSEC QUE NAO ESTAMOS A USAR (AMANHA EXPLICO)

  /*clock_gettime(CLOCK_REALTIME, &start);
  clock_gettime(CLOCK_REALTIME, &finish);
  timeValue =(finish.tv_sec - start.tv_sec )
  printf("%f\n", timeValue );
  fprintf(f, "%llf\n", timeValue);
  fflush(f);*/

  criarTriagens();

  //RESUMINDO ESTA A TIRAR BEM AS PESSOAS DA MQ MAS COMO OS DOUTORES COMECAM A CORRER ANTES DE HAVER PESSOAS NA MQ MAL APARECEM PESSOAS NA MQ OS DOUTORES TIRAM LOGO AS PRIMEIRAS (config.NUMERO_DE_DOUTORES) PESSOAS A ENTRAREMM NA MQ E VAO SER LOGO ATENDIDAS INDEPENDENTEMENTE DA SUA PRIORIDADE. DE RESTO SAO PRINTS QUE ESTAO FORA DE ORDEM PORQUE ESTA A ACONTECER TUDO AO MESMO TEMPO MAS E SO POR ISSO.

  criarDoutor();
}
