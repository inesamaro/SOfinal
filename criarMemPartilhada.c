#include "header.h"

void criarMemPartilhada() {
  if ((shmid = shmget(IPC_PRIVATE, sizeof(Estat), IPC_CREAT|0777)) > 0 ) {

    if ((shared_var = shmat(shmid, NULL, 0)) == (Estat*) -1){
      perror("error in shmat");
    }
    (*shared_var).nTriados = 0;
    (*shared_var).nAtendidos = 0;
    (*shared_var).tempoMedioAntesTriagem = 0;
    (*shared_var).tempoMedioAteAtend = 0;
    (*shared_var).tempoMedioTotal = 0;
    (*shared_var).countPacientesQueue = 0;
  }
  else {
    printf("Erro ao criar a memória partilhada!");
  }
}
