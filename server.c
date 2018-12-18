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

int *converterData(int data)
{
    //Entra -> 20181229
    //Fica no array -> 9 2 2 1 8 1 0 2
    int newData[8];
    static int valoresData[3];

    for (int i = 0; i < 8; i++)
    {
        newData[i] = data % 10;
        data = data / 10;
    }

    //Dia
    valoresData[0] = newData[1] * 10 + newData[0];
    //Mês
    valoresData[1] = newData[3] * 10 + newData[2];
    //Ano
    valoresData[2] = newData[7] * 1000 + newData[6] * 100 + newData[5] * 10 + newData[4];

    return valoresData;
}

int *converterHoras(int horas)
{
    //Entra -> 1634
    //Fica no array -> 4 3 6 1
    int newHoras[4];
    static int valoresHoras[2];

    for (int i = 0; i < 4; i++)
    {
        newHoras[i] = horas % 10;
        horas = horas / 10;
    }

    //Minutos
    valoresHoras[0] = newHoras[1] * 10 + newHoras[0];
    //Hora
    valoresHoras[1] = newHoras[3] * 10 + newHoras[2];

    return valoresHoras;
}

time_t getSeconds(int *valoresData, int *valoresHoras)
{
    struct tm date;

    if ((*(valoresData + 2) - 1900) < 117 || *(valoresData + 1) - 1 > 11 || *(valoresData) > 31 || *(valoresHoras + 1) > 23 || *(valoresHoras) > 60)
    {
        return -1;
    }

    date.tm_year = *(valoresData + 2) - 1900;
    date.tm_mon = *(valoresData + 1) - 1;
    date.tm_mday = *(valoresData);
    date.tm_hour = *(valoresHoras + 1);
    date.tm_min = *(valoresHoras);
    date.tm_sec = 0;
    date.tm_isdst = -1;

    time_t agendamento = mktime(&date);
    time_t now = time(NULL);
    time_t diferenca = agendamento - now;

    if (diferenca < 0)
    {
        return 0;
    }
    else
    {
        return diferenca;
    }
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
    char *dia[3], *mes[3], *ano[5], *hora[3], *minuto[3];
    char *comando[100];
    int init_size = strlen(buf1);
    time_t timeToTask;
    size_t num;
    char *tarefaRealizar;
    int index, dateToConvert, timeToConvert, *dataAux, *timeAux;
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

            //recebe string com data no formato DD/MM/AAAA e divide em token[0] = dia, token[1] = mes, token[2]=ano
            dataToken[0] = strtok(&data, "/");
            int i = 0;
            while (dataToken[i] != NULL)
            {
                i++;
                dataToken[i] = strtok(NULL, "/");
            }
            sscanf(dataToken[0], "%s\n", dia);
            sscanf(dataToken[1], "%s\n", mes);
            sscanf(dataToken[2], "%s\n", ano);

            strcat(aux2, ano);
            strcat(aux2, mes);
            strcat(aux2, dia);
            printf("dia: %s\nmes: %s \nano: %s\n", dia, mes, ano);

            printf("AUX2: %s\n", &aux2);
            sscanf(aux2, "%d\n", &dateToConvert);
            printf("date to convert: %d\n", dateToConvert);

            memset(aux2, 0, sizeof aux2);

            timeToken[0] = strtok(&horas, ":");
            int c = 0;
            while (timeToken[c] != NULL)
            {
                c++;
                timeToken[c] = strtok(NULL, ":");
            }
            sscanf(timeToken[0], "%s\n", hora);
            sscanf(timeToken[1], "%s\n", minuto);
            printf("hora: %s\nminuto: %s\n", hora, minuto);

            strcat(aux3, hora);
            strcat(aux3, minuto);
            sscanf(aux3, "%d\n", &timeToConvert);
            // sscanf(aux2,"%d\n",dateToConvert);
            printf("time to convert: %d\n", timeToConvert);
            printf("numero de tarefas: \t");

            memset(aux3, 0, sizeof aux3);

            int n = countTarefas();
            printf("\n");

            dataAux = converterData(dateToConvert);
            timeAux = converterHoras(timeToConvert);
            printf("---------------------------\n");
            printf("newDate : %d %d %d\n", dataAux[0],data[1],data[2]);
            printf("newTime : %d %d \n", timeAux[0], timeAux[1]);
            printf("-----------------------------\n");

            tarefa[n].id = n;
            tarefa[n].data = data;
            tarefa[n].hora = horas;
            strcpy(tarefa[n].cmd, comando);

           
            timeToTask = getSeconds(dataAux, timeAux);
            printf ( "tempo que falta: %d",timeToTask );


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