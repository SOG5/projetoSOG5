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
    int state;
    double timeToTask;
    int dia, mes, ano, hora, minuto;
    char output[1024];
};

struct Alarm
{
    int taskID[100];
    double timeToTask;
    int ntasks;
};

struct Comands
{
    char cmd[100];
};

//Variáveis globais...
struct Tarefas tarefa[200];
struct Alarm alarms[200];
struct Comands comandos[100];

int countTarefas()
{
    int count = 0, i = 0;
    while (tarefa[i].dia != NULL)
        i++;
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

int getAlarm()
{
}

time_t getSeconds(int *dataValues[3], int *timeValues[2])
{

    struct tm date, tA, tB, *tptr;
    ;
    time_t timeA, timeB;
    double difference;
    struct tm *timeinfo;

    time(&timeA);
    time(&timeB);

    timeinfo = localtime(&timeA);
    // printf("Current local time and date: %s", asctime(timeinfo));

    tptr = localtime(&timeA);
    tA = *tptr;
    tptr = localtime(&timeB);
    tB = *tptr;
    tA.tm_mday = tA.tm_mday;
    tA.tm_mon = tA.tm_mon + 1;
    tA.tm_year = (tA.tm_year + 1900);

    // printf("Tn\n");
    // printf("dia - %d\n", tA.tm_mday);
    // printf("mes - %d\n", tA.tm_mon);
    // printf("ano - %d\n", tA.tm_year);
    // printf("hor - %d\n", tA.tm_hour);
    // printf("min - %d\n", tA.tm_min);

    tB.tm_mday = dataValues[0];
    tB.tm_mon = dataValues[1];
    tB.tm_year = dataValues[2];
    tB.tm_hour = timeValues[0];
    tB.tm_min = timeValues[1];

    // printf("Tn\n");
    // printf("dia - %d\n", tB.tm_mday);
    // printf("mes - %d\n", tB.tm_mon);
    // printf("ano - %d\n", tB.tm_year);
    // printf("hor - %d\n", tB.tm_hour);
    // printf("min - %d\n", tB.tm_min);

    timeA = mktime(&tA);
    timeB = mktime(&tB);

    difference = difftime(timeB, timeA);
    printf("Difference is %.0f seconds\n", difference);
    return difference;
}
int setAlarm()
{
    int *dia, *mes, *ano, *hora, *minuto;
    int *dataAux[3], *timeAux[2], taskAux[10];
    double diffAux = 999999999999999999999999999;
    int eq = 0;
    int n = countTarefas();
    for (int i = 0; i < n; i++)
    {
        if (tarefa[i].state == 0)
        {
            dataAux[0] = tarefa[i].dia;
            dataAux[1] = tarefa[i].mes;
            dataAux[2] = tarefa[i].ano;
            timeAux[0] = tarefa[i].hora;
            timeAux[1] = tarefa[i].minuto;

            double difference = getSeconds(dataAux, timeAux);

            tarefa[i].timeToTask = difference;
            if (difference < diffAux)
            {
                diffAux = difference;
            }
        }
    }

    alarms[0].timeToTask = diffAux;

    int aux = 0;
    for (int i = 0; i < n; i++)
    {
        if (tarefa[i].timeToTask == diffAux)
        {
            alarms[0].taskID[aux] = tarefa[i].id;
            aux++;
        }
    }
    alarms[0].ntasks = aux;
    if (aux == 1)
    {
        printf("Proxima tarefa %d em  %.0f seconds\n", alarms[0].taskID[0], alarms[0].timeToTask);
    }
    else
    {
        printf("\n Numero de tarefas agendadas para o mesmo instante: %d\n", aux);
        printf("Tarefas:\n");
        for (int i = 0; i < aux; i++)
        {
            printf("%d\n", alarms[0].taskID[i]);
        }
        printf("em %.0f segundos", alarms[0].timeToTask);
    }
    alarm(alarms[0].timeToTask);
    return 0;
}

int cancelTask(int taskID)
{
    int n = countTarefas();
    for (int i = 0; i < n; i++)
    {
        if (tarefa[i].id == taskID)
        {
            tarefa[i].state = 2; // 2 para cancelado
            // n = countAlarms();
            // for(i=0; i< n ; i++){
            //     if(alarms[i].taskID == taskID)

            // }
        }
    }
}

int execTask()
{

    char *output[10][1024];
    int nbyte = 256;
    char delim[] = " ";
    char *cmd[256], *token[nbyte];
    printf("\nEXEC TASK\n");

    int *taskID[100];
    int countCmd = alarms[0].ntasks;

    for (int i = 0; i < countCmd; i++)
    {
        taskID[i] = alarms[0].taskID[i];
    }

    int n = countTarefas();

    for (int aux = 0; aux < countCmd; aux++)
    {
        for (int i = 0; i < n; i++)
        {
            if (tarefa[i].id == taskID[aux])
            {
                strcpy(cmd, tarefa[i].cmd);
                strcpy(comandos[aux].cmd, cmd);
            }
        }
    }

    for (int i = 0; i < countCmd; i++)
    {
        printf("comando a executrar: %s\n", comandos[i].cmd);
    }

    // printf("comando a executrar: %s\n", comandos[0].cmd);

    pid_t pid;
    int status;
    int pid_number;
    int pd[10][2];

    // pipe(pd);
   

    for (int i = 0; i < countCmd; i++)
    {
        pipe(pd[i]);
        if ((pid = fork()) < 0)
        {
            perror("fork");
            abort();
        }
        else if (pid == 0)
        {
            close(pd[i][0]);
            close(1);
            dup(pd[i][1]);
            token[0] = strtok(comandos[i].cmd, delim);
            // printf("TOKEN0 !!!!! %s ", token[0]);
            int i = 0;
            while (token[i] != NULL)
            {
                if (i == 0)
                    printf("Executável: %s\n", token[i]);
                else
                    printf("Arg %d : %s\n", i, token[i]);
                i++;
                token[i] = strtok(NULL, delim);
            }
            execvp(token[0], token);
            perror("Erro no execlp: ");
        }

        while (pid_number = wait(&status) > 0)
        {

            close(pd[i][1]); //Fecho o descritor de ficheiro associado ao descritor de escrita do pipe
            dup2(pd[i][0], 0);
            read(0, output[i], sizeof(output[i]));
            printf("\n -------- OUTPUT ----\n");

            printf("%s\n", output[i]);
            printf("\n ---------------\n");
        
            pid_number = wait(&status);
            // printf("Child with PID %d exited with status 0x%x.\n, my father %d", getppid(), WEXITSTATUS(status), getppid());
        }
    }

   for (int aux = 0; aux < countCmd; aux++)
    {
        for (int i = 0; i < n; i++)
        {
            int taksid = taskID[aux];
            if (tarefa[i].id == taksid)
            {
                strcpy(tarefa[taksid].output, output[aux]);
                tarefa[taksid].state = 1;
                printf("OUTPUT GUARDADO!!!! ");
                printf("%s", tarefa[taksid].output);
            }
        }
    }

    return 0;
}

void signalhandler(int sig)
{
    int pid;
    switch (sig)
    {
    case 1:
        printf("Cliente terminou conexão.\n");
        break;
    case 14:
        dprintf(1, "Alarme recebido\n");
        if ((pid = fork()) < 0)
        {
            perror("fork no alarme");
            exit(-1);
        }
        else if (pid == 0)
        {
            execTask();
        }
        else
        {
            wait(&pid);
        }
        break;
    default:
        printf("Sinal %d recebido\n", sig);
    }
}

int main()
{

    int fd1, nbyte = 256;
    char *buf1 = (char *)malloc(nbyte * sizeof(char)), *buf2 = (char *)malloc(nbyte * sizeof(char));
    char delim[] = " ";
    char *agendar = "-a", *listar = "-l", *exec = "-e", *cancelar = "-c";
    int len;
    char *data[10];
    char *horas[10];
    char *comando[nbyte];
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
    signal(SIGALRM, signalhandler);

    char *token[nbyte], *dataToken[nbyte], *timeToken[100], response[256];
    while (1)
    {

        // First open in read only and read to buf1
        fd1 = open(myfifo, O_RDONLY);
        read(fd1, buf1, nbyte);

        printf("BUFFER TEM : %s\n", buf1);

        token[0] = strtok(buf1, delim);
        int i = 0;
        while (token[i] != NULL || i < 4)
        {
            printf("TOKEN %d : %s\n", i, token[i]);
            i++;
            if (i == 3)
                token[i] = strtok(NULL, "\n");
            else
            {
                token[i] = strtok(NULL, delim);
            }
        }
        //agendar
        if (strcmp(token[0], agendar) == 0)
        {

            printf("Pee agendamento recebido\n");
            sscanf(token[1], "%s\n", data);
            sscanf(token[2], "%s\n", horas);
            strcpy(comando, token[3]);
            // sscanf(token[3], "%s\n", comando);
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

            double difference = getSeconds(dataAux, timeAux);
            // printf("Difference is %.0f seconds\n", difference);

            int n = countTarefas();
            printf("\nnumero de tarefas: \t %d\n", n);
            printf("\n");

            tarefa[n].id = n;
            tarefa[n].dia = dia;
            tarefa[n].mes = mes;
            tarefa[n].ano = ano;
            tarefa[n].hora = hora;
            tarefa[n].minuto = minuto;
            tarefa[n].timeToTask = difference;
            tarefa[n].state = 0;
            strcpy(tarefa[n].cmd, comando);

            setAlarm();
            // timeToTask = getSeconds(dataAux, timeAux);

            // storeTask(tarefa[n].id, tarefa[n].data, tarefa[n].hora);
        }
        // listar
        else if (strcmp(token[0], listar) == 0)
        {
            char *printState;
            printf("Pedido de listagem\n");
            num = countTarefas();
            for (int i = 0; i < num; i++)
            {
                if (tarefa[i].state == 0)
                    printState = "Agendada";
                else if (tarefa[i].state == 1)
                    printState = "Efectuada";
                else if (tarefa[i].state == 2)
                    printState = "Cancelada";

                printf("tafera: \t %d\n data : \t %d/%d/%d\n horas: \t %d:%d:00\n estado: %s\n comando: %s\n", tarefa[i].id, tarefa[i].dia, tarefa[i].mes, tarefa[i].ano, tarefa[i].hora, tarefa[i].minuto, printState, tarefa[i].cmd);
            }
        }

        else if ((strcmp(token[0], exec) == 0))
        {
            printf("Pedido para exec\n");
            setAlarm();
        }

        else if ((strcmp(token[0], cancelar) == 0))
        {
            int idTarefa;
            printf("Pedido para cancelar\n");
            sscanf(token[1], "%d\n", &idTarefa);
            cancelTask(idTarefa);
            printf("tarefa cancelada");
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