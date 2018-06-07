/*
*Aplicação principal: 
* Thread para criar a ponte entre o client e o servidor 
*   (Aplicação principal é o sv)
*
*/
#include "link.h"

int received = 0;
int nmrConnections = 0;
int* Connection = NULL;
int flag = -1;
int work = 0;
int threading = 1;

void itoa (int a, char* num);

void handleRequest (int fd);

void handleLogging (int fd, int* archived);

char* readFifo (int FIFO);

void* flags (void* pipe);

int main()
{
    pthread_t thread, fofi;
    int fd, n, archived = 0, aux1, pipe, pepi, log, n1;
    char name[150], aux[50], buff[7], aux2[512];
    char * FIFO = "cachimbo", *FOFI = "pipeta";

    mkfifo(FIFO,0666);
    mkfifo(FOFI,0666);
    pthread_create(&thread, NULL, create_listener, NULL);

    pipe = open (FIFO, O_RDONLY);
	if (pipe < 0)
	{
		printf("Couldn't open fifo\n");
		return -1;
	}
	pepi = open (FOFI, O_WRONLY);
	if (pepi < 0)
	{
		printf("Couldn't open fifo\n");
		return -1;
	}
	pthread_create(&fofi, NULL, flags, &pipe);
    while(1){
    	if (work == 1) {
    		work = 0;
    		if (flag == 1)
    			;
    		else if (flag == 2) {
    			received = 0;
    			archived = 0;
    			//MATAR CLIENTES PUTA QUE OS PARIU
    			work = 1;
    			continue;
    		}
    		else if (flag == 3) {
    			received = 0;
    			archived = 0;
    			//MATAR CLIENTES PUTA QUE OS PARIU
    		}
    		else if (flag == 4){
    			work = 1;
    			continue;
    		} 
    		else if (flag == 5){
    			//NUMERO CLIENTES
    		}
    		else if (flag == 6){
    			//MATAR UM CLIENTE ESPECIFICO
    		}
    		else if (flag == 7){
    			//REPORTS
    		}
    		else if (flag == 8){
    			log = open ("Data.log", O_RDWR);
    			if (log < 0) {
					printf("Error opening Data.log\n");
					return ;
				}
				while ( (n1=read (log, aux2, 512)) > 0 )
					if (write(pepi, aux2, n1) < 0) {
						printf("Error writting fifo\n");
						break;
					}
				close(log);
    		}
    	}
    	printf("%d\n", nmrConnections);
    	printf("WAITING...\n");
    	name[0] = '\0';
        printf("RECEIVED %d\n", received);
        if (received != 0){
        	itoa ((received-1), aux);
        	strcat(name, "Received");
        	strcat(name, aux);
        	strcat(name, ".log");
        	fd = open (name, O_RDONLY);
        	if (fd < 0)
        	{
        		printf("Error Opening File\n");
        		received--;
        		continue;
        	}

        	if ((n = read (fd, buff, 7)) > 0)
        	{
        		if (strncmp(buff,"Request",7) == 0)
        			handleRequest(fd);
        		if (strncmp(buff,"Logging",7) == 0) {
        			aux1 = archived;
        			handleLogging(fd, &archived);
        			if (aux1 != archived)
        			{
        				if (remove(name) < 0)
        				{
        					printf("Error deleting file\n");
        				}
        			}
        		}
        	}
            received--;
            close(fd);
        }
        sleep(5);
    }
    close(pepi);
    close(pipe);
}

void itoa (int a, char* num) {
	int aux = a, i;
	for (i=0; i<50; i++)
		num[i] = '\0';
	i = 0;
	while ((aux % 10)) {
		if (i >= 50)
			return ;
		num[i] = (char) ((aux % 10) + 48);
		aux = aux / 10;
		i++;
	}
	return ;
}

void handleRequest (int fd) {

	return ;
}

void handleLogging (int fd, int *archived) {
	int log, n;
	char buff[256], aux[256];

	/*ABRIR O FICHEIRO ONDE TAO OS LOGS*/
	log = open ("Data.log", O_RDWR);
	if (log < 0) {
		printf("Error opening Data.log\n");
		return ;
	}
	/*COLOCAR NO FIM PARA DEPOIS SE ESCREVER*/
	lseek(log, 0, SEEK_END);
	lseek(fd, 7, SEEK_SET);
	
	while ((n = read(fd, buff, 256)) < 0) {
		if ( write (log, buff, n)) {
			printf("Error writing to file\n");
			return ;
		}
	}
	if ( write (log , "\n", 1) < 0) {
		printf("Error writing to file\n");
			return ;
	}
	(*archived)++;
	close (log);

	return ;
}

void* flags (void* pipe) {
	int pip = * (int*) pipe;
	printf("%d\n", pip);
	char aux[256];
	while (threading) {
		read(pip, aux, 256);
		if (strncmp(aux, "start", 5) == 0) {
			flag = 1;
			work = 1;
		}
		else if (strncmp(aux, "stop", 4) == 0) {
			flag = 2;
			work = 1;
		}
		else if (strncmp(aux, "restart", 7) == 0) {
			flag = 3;
			work = 1;
		}
		else if (strncmp(aux, "pause", 5) == 0) {
			flag = 4;
			work = 1;
		}
		else if (strncmp(aux, "numc", 4) == 0) {
			flag = 5;
			work = 1;
		}
		else if (strncmp(aux, "terc", 4) == 0) {
			flag = 6;
			work = 1;
		}
		else if (strncmp(aux, "report", 6) == 0) {
			flag = 7;
			work = 1;
		}
		else if (strncmp(aux, "access", 6) == 0) {
			flag = 8;
			work = 1;
		}
	}
}