CC = gcc
CFLAGS =	 -D_REENTRANT -Wall -g -pthread
all:	 main.o criarMemPartilhada.o lerFichConfig.o namedPipe.o MQ.o criarTriagens.o triagem.o criarQueuePacientes.o criarDoutor.o lerInfoPacientes.o workPrinc.o sigusr1.o mmf.o writeMmf.o

	$(CC)	$(CFLAGS)	-o	exe main.o criarMemPartilhada.o lerFichConfig.o namedPipe.o MQ.o criarTriagens.o triagem.o criarQueuePacientes.o criarDoutor.o lerInfoPacientes.o workPrinc.o sigusr1.o mmf.o writeMmf.o
main.o: main.c header.h
	$(CC) $(CFLAGS) -c main.c

#ja estava comentado
#menu.o: menu.c header.h
#	        $(CC) $(CFLAGS) -c menu.c


lerFichConfig.o: lerFichConfig.c header.h
	$(CC) $(CFLAGS) -c lerFichConfig.c

namedPipe.o: namedPipe.c header.h
	$(CC) $(CFLAGS) -c namedPipe.c

criarQueuePacientes.o: criarQueuePacientes.c header.h
	$(CC) $(CFLAGS) -c criarQueuePacientes.c

criarMemPartilhada.o: criarMemPartilhada.c header.h
	$(CC) $(CFLAGS) -c criarMemPartilhada.c

MQ.o: MQ.c header.h
	$(CC) $(CFLAGS) -c MQ.c

criarTriagens.o: criarTriagens.c header.h
	$(CC) $(CFLAGS) -c criarTriagens.c

triagem.o: triagem.c header.h
	$(CC) $(CFLAGS) -c triagem.c

criarDoutor.o: criarDoutor.c header.h
	$(CC) $(CFLAGS) -c criarDoutor.c

lerInfoPacientes.o: lerInfoPacientes.c header.h
	$(CC) $(CFLAGS) -c lerInfoPacientes.c

workPrinc.o: workPrinc.c header.h
	$(CC) $(CFLAGS) -c workPrinc.c

sigusr1.o: sigusr1.c header.h
	$(CC) $(CFLAGS) -c sigusr1.c

mmf.o: mmf.c header.h
	$(CC) $(CFLAGS) -c mmf.c

writeMmf.o: writeMmf.c header.h
	$(CC) $(CFLAGS) -c writeMmf.c

clean:
	rm -rf *.o

mrproper:	clean
		rm -rf header
