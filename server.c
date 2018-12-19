// C program to implement one side of FIFO
// This side reads first, then writes
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define tamanhoTarefa 200
#define tamanhoEstado 15

struct Tarefas
{
    int id;
    char cmd[100];
    char estado;
    int data;
    int hora;
    char output[1024];
};

struct Alarm
{
    int alarmID;
    int timeToTask;
};

//Variáveis globais...
struct Tarefas tarefa[200];
struct Alarm alarms[200];

int countTarefas()
{
    int count = 0, i = 0;
    while (tarefa[i].data != 0)
        i++;
    printf("%d", i);
    return i;
};

int countAlarms()
{
    int count = 0, i = 0;
    while (alarms[i].timeToTask != 0)
        i++;
    printf("%d", i);
    return i;
};

int manageCommand(char *command)
{

    // char str = *command.Substring(0,5);
    // int nbyte =256;
    // if(prefixo("-a",command) == true){
    //     printf("true");
    // }

    return 0;
}

int getAlarm()
{
}

time_t getSeconds(int *dataValues[3], int *timeValues[2])
{
    
    struct tm date;
    time_t timeA, timeB;
    struct tm tA, tB, *tptr;
    double difference;
    struct tm *timeinfo;

    time(&timeA);
    time(&timeB);

    timeinfo = localtime(&timeA);
    printf("Current local time and date: %s", asctime(timeinfo));

    tptr = localtime(&timeA);
    tA = *tptr;
    tptr = localtime(&timeB);
    tB = *tptr;
    tA.tm_mday = tA.tm_mday;
    tA.tm_mon = tA.tm_mon + 1;
    tA.tm_year = (tA.tm_year + 1900);

    printf("Tn\n");
    printf("dia - %d\n", tA.tm_mday);
    printf("mes - %d\n", tA.tm_mon);
    printf("ano - %d\n", tA.tm_year);
    printf("hor - %d\n", tA.tm_hour);
    printf("min - %d\n", tA.tm_min);

    tB.tm_mday = dataValues[0];
    tB.tm_mon = dataValues[1];
    tB.tm_year = dataValues[2];
    tB.tm_hour = timeValues[0];
    tB.tm_min = timeValues[1];

    printf("Tn\n");
    printf("dia - %d\n", tB.tm_mday);
    printf("mes - %d\n", tB.tm_mon);
    printf("ano - %d\n", tB.tm_year );
    printf("hor - %d\n", tB.tm_hour);
    printf("min - %d\n", tB.tm_min);


    timeA = mktime(&tA);
    timeB = mktime(&tB);

    difference = difftime(timeB, timeA);
    printf ("Difference is %.0f seconds\n", difference);
    return 0;
}
int setAlarm(timeToTask)
{
    int timeToNext = getAlarm();
    if (timeToNext > timeToTask)
    {

        //CRIAR ALARME
    }
}

int main()
{

    int fd1, nbyte = 256;
    char *buf1 = (char *)malloc(nbyte * sizeof(char));
    char *buf2 = (char *)malloc(nbyte * sizeof(char));
    char delim[] = " ";
    char *agendar = "-a";
    char *listar = "-l";
    int len;
    char *data[100];
    char *horas[10];
    char *comando[100];
    int init_size = strlen(buf1);
    time_t timeToTask;
    size_t num;
    char *tarefaRealizar;
    int index, dateToConvert, timeToConvert, *dataAux[3], *timeAux[2], *dia, *mes, *ano, *hora, *minuto;
    int *newTime, *newDate;
    char *aux2[100];
    char *aux3[100];
    memset(aux3, 0, sizeof aux3);
    memset(aux2, 0, sizeof aux2);
    // FIFO file path
    char *myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    char *token[nbyte], *dataToken[nbyte], *timeToken[100], response[256];
    while (1)
    {

        // First open in read only and read to buf1
        fd1 = open(myfifo, O_RDONLY);
        read(fd1, buf1, nbyte);

        printf("BUFFER TEM : %s\n", buf1);
     
        token[0] = strtok(buf1, delim);
        int i = 0;
        while (token[i] != NULL)
        {
            printf("TOKEN %d : %s\n", i, token[i]);
            i++;
            token[i] = strtok(NULL, delim);
        }

        //agendar
        if (strcmp(token[0], agendar) == 0)
        {

            printf("Pee agendamento recebido\n");
            sscanf(token[1], "%s\n", data);
            sscanf(token[2], "%s\n", horas);
            sscanf(token[3], "%s\n", comando);
            printf("data: %s\nhoras: %s \ncomando: %s\n", &data, &horas, &comando);

        
            dataToken[0] = strtok(&data, "/");
            int i = 0;
            while (dataToken[i] != NULL)
            {
                i++;
                dataToken[i] = strtok(NULL, "/");
            }
            sscanf(dataToken[0], "%d\n", &dia);
            sscanf(dataToken[1], "%d\n", &mes);
            sscanf(dataToken[2], "%d\n", &ano);

            printf("dia: %d\nmes: %d \nano: %d\n", dia, mes, ano);
            dataAux[0] = dia;
            dataAux[1] = mes;
            dataAux[2] = ano;

            timeToken[0] = strtok(&horas, ":");
            int c = 0;
            while (timeToken[c] != NULL)
            {
                c++;
                timeToken[c] = strtok(NULL, ":");
            }
            sscanf(timeToken[0], "%d\n", &hora);
            sscanf(timeToken[1], "%d\n", &minuto);
            printf("hora: %d\nminuto: %d\n", hora, minuto);
            
            timeAux[0] = hora;
            timeAux[1] = minuto;


            getSeconds(dataAux, timeAux);

            printf("numero de tarefas: \t");



            int n = countTarefas();
            printf("\n");

            tarefa[n].id = n;
            tarefa[n].data = data;
            tarefa[n].hora = horas;
            strcpy(tarefa[n].cmd, comando);

            // timeToTask = getSeconds(dataAux, timeAux);
            printf("tempo que falta: %d", timeToTask);

            // storeTask(tarefa[n].id, tarefa[n].data, tarefa[n].hora);
        }
        // listar
        else if (strcmp(token[0], listar) == 0)
        {
            printf("Pedido de listagem\n");
            num = countTarefas();
            for (int i = 0; i < num; i++)
            {
                printf("tafera: %d \t data : %d\t horas: %d\t comando: %s", tarefa[i].id, tarefa[i].data, tarefa[i].hora, tarefa[i].cmd);
            }
        }

        memset(buf1, 0, strlen(buf1));

        close(fd1);

        // Now open in write mode and write
        // string taken from user.
        fd1 = open(myfifo, O_WRONLY);
        // read(0, buf2, nbyte);
        write(fd1, "ok", nbyte);
        close(fd1);
        memset(buf2, 0, strlen(buf2));
    }
    return 0;
}