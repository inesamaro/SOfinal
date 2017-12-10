#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/msg.h>
#include <math.h>
#include <sys/mman.h>
#include <fcntl.h>
#define FILEMODE S_IRWXU | S_IRGRP | S_IROTH
#define MAX 150
#define _GNU_SOURCE

#define BILLION  1000000000L;
//define input_pipe
#define PIPE_NAME "input_pipe"


FILE *f;

struct timespec start, helper, finish;
double shift_time;


//define estrutura de info do paciente
typedef struct paciente *Node_paciente;
typedef struct infoPaciente {
  char nome[50];
  int numChegada;
  int tempoTriagem;
  int tempoAtend;
  double inicio; //para calcularmos qunato tempo e que cada paciente gastou desde que entrou no sistema atá que saiu
  clock_t inicioTriagem;;
  clock_t inicioAtend;
  clock_t fimAtend;
  Node_paciente next;
} InfoPaciente;

//define estrtura do paciente
typedef struct paciente {
  long int mtype;
  InfoPaciente info;
} Paciente;

//define estrutura para as estatisticas
typedef struct estat {
  int nTriados;
  int nAtendidos;
  int tempoMedioAntesTriagem;
  int tempoMedioAteAtend;
  int tempoMedioTotal;
  int countPacientesQueue;
} Estat;

typedef struct sem {
  sem_t semProcess;
} Sem;

Sem* sem;

int* triagensIds;
int triagensParaApagar;


//define estrutura para a configuraçao do sistema
typedef struct config {
  long int nTriagens;
  long int nDoutores;
  long int tempoTurno;
  long int queueMax;
} Config;

//
Estat *shared_var;
Config *config;
Paciente* queuePacientes;
Paciente* aux;
//vars for mq
int shmid;
int mqid;
int newNumTriagens;
//vars for pipe
int fdpipe;
int logfd, ret;

struct stat st;
char *addr;

size_t len_file;
//sem_t semAtend;
//sem_init(&semAtend, 0, 1);
sem_t semProc;
int idSemProc;
sem_t full;
sem_t sem_sharedvar;
sem_t sem_mq;
sem_t sem_queueMax;
pthread_t threadPipe;

pthread_cond_t condNewPacientes;
pthread_mutex_t condNewPacientes_mutex;

pthread_cond_t condNewTriagens;
pthread_mutex_t condNewTriagens_mutex;

pthread_mutex_t mutex;

void criarMemPartilhada();
void lerFichConfig();
void *namedPipe();
void criarMQ();
Paciente* criarQueuePacientes();
void criarTriagens();
void *triagem();
void sendMQ();
void criarDoutor();
void lerInfoPacientes();
void *workPrinc();
void sigusr1();
int mmf();
int writeMmf();
