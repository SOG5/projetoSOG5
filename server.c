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
#include <errno.h>
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
    int exitValue;
    char output[1024];
    char outputERR[1024];
};

struct Alarm
{
    int taskID[100];
    double timeToTask;
    int ntasks;
    int nMax;
};

struct Comands
{
    char cmd[100];
};

//Variáveis globais...
struct Tarefas tarefa[200];
struct Alarm alarms[1];
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
    double diffAux = 99999999999999999999999999999;
    int eq = 0;
    int n = countTarefas();
    for (int i = 0; i < n; i++)
    {
        if (tarefa[i].state == 0 && tarefa[i].timeToTask > 0)
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
        if (tarefa[i].state == 0 && tarefa[i].timeToTask == diffAux)
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

int setMax(int nmax)
{
    alarms[0].nMax = nmax;
}

int cancelTask(int taskID)
{
    int n = countTarefas();
    for (int i = 0; i < n; i++)
    {
        if (tarefa[i].id == taskID)
        {
            tarefa[i].state = 2;       // 2 para cancelado
            tarefa[i].timeToTask = -1; // -1 para cancelada
        }
    }
    setAlarm();
}

int execTask()
{

    char *output[10][1024];
    char *outputERR[10][1024];
    int nbyte = 256;
    char delim[] = " ";
    int *exitVal;
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

    pid_t pid;
    int status;
    int pid_number;
    int pd[10][2];
    int cmdAux = countCmd;
    int taskMAX = alarms[0].nMax;
    int rounds = 0;

    while (cmdAux > 0)
    {

        if (cmdAux - taskMAX >= 0)
        {
            countCmd = taskMAX;
            cmdAux = cmdAux - taskMAX;
        }
        else if (cmdAux - taskMAX < 0)
        {
            countCmd = cmdAux;
            cmdAux = cmdAux - taskMAX;
        }
        for (int i = 0; i < countCmd; i++)
        {
            pipe(pd[i + rounds]);
            if ((pid = fork()) < 0)
            {
                perror("fork");
                abort();
            }
            else if (pid == 0)
            {
                close(pd[i + rounds][0]);
                close(1);
                dup(pd[i + rounds][1]);

                token[0] = strtok(comandos[i + rounds].cmd, delim);
                int i = 0;
                while (token[i] != NULL)
                {
                    i++;
                    token[i] = strtok(NULL, delim);
                }
                execvp(token[0], token);
                perror("erro no exec");
                exit(errno);
            }

            while (pid_number = wait(&status) > 0)
            {
                close(pd[i + rounds][1]); 
                dup2(pd[i + rounds][0], 0);
                read(0, output[i + rounds], sizeof(output[i + rounds]));
                printf("\n -------- OUTPUT ----\n");

                printf("%s\n", output[i + rounds]);


                printf("\n ---------------\n");

                pid_number = wait(&status);
                if (WIFEXITED(status))
                {

                    snprintf(outputERR[i + rounds], sizeof(output[i + rounds]), "%s", strerror(WEXITSTATUS(status)));
                    exitVal = WEXITSTATUS(status);
            
                }

                // printf("Child with PID %d exited with status 0x%x.\n, my father %d", getppid(), WEXITSTATUS(status), getppid());
            }
        }
        rounds = rounds + countCmd;
        setAlarm();
    }

    for (int aux = 0; aux < alarms[0].ntasks; aux++)
    {
        for (int i = 0; i < n; i++)
        {
            int taksid = taskID[aux];
            if (tarefa[i].id == taksid)
            {
                strcpy(tarefa[taksid].output, output[aux]);
                strcpy(tarefa[taksid].outputERR, outputERR[aux]);
                tarefa[taksid].timeToTask = -1;
                tarefa[taksid].state = 1;
                tarefa[taksid].exitValue = exitVal;
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
    char *buf1 = (char *)malloc(nbyte * sizeof(char)), *buf2 = (char *)malloc(10048 * sizeof(char));
    char delim[] = " ";
    char *agendar = "-a", *listar = "-l", *cancelar = "-c", *nmax = "-n", *email = "-e", *result = "-r";
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

    alarms[0].nMax = 10;
    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    signal(SIGALRM, signalhandler);

    char *token[nbyte], *dataToken[nbyte], *timeToken[100], response[256];
    while (1)
    {

        memset(buf1, 0, strlen(buf1));
        memset(buf2, 0, strlen(buf2));

        fd1 = open(myfifo, O_RDONLY);
        read(fd1, buf1, nbyte);
        close(fd1);
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
            if (difference < 0)
                break;
            // printf("Difference is %.0f seconds\n", difference);

            int n = countTarefas();
            printf("\nnumero de tarefas: \t %d\n", n);
            printf("\n");

            char *empty = "<empty>";
            tarefa[n].id = n;
            tarefa[n].dia = dia;
            tarefa[n].mes = mes;
            tarefa[n].ano = ano;
            tarefa[n].hora = hora;
            tarefa[n].minuto = minuto;
            tarefa[n].timeToTask = difference;
            tarefa[n].state = 0;
            tarefa[n].exitValue = 0;

            strcpy(tarefa[n].cmd, comando);
            strcpy(tarefa[n].output, empty);
            strcpy(tarefa[n].outputERR, empty);

            setAlarm();

            fd1 = open(myfifo, O_WRONLY);
            write(fd1, "ok", nbyte);
            close(fd1);
        }
        // listar
        else if (strcmp(token[0], listar) == 0)
        {

            int buffsize = 10048;
            char *listBuffer = (char *)malloc(buffsize);
            printf("Pedido de listagem\n");
            num = countTarefas();
            char *listBufferAux1[100][1024];
            char *listBufferAux2[100][1024];
            char *listBufferAux3[100][1024];
            int countL[3] = {0, 0, 0};

            for (int i = 0; i < num; i++)
            {

                if (tarefa[i].state == 0)
                {
                    printf("andenda");
                    int aux1 = snprintf(NULL, 0, "%d %d-%d-%d %d:%d:00 %s\n", tarefa[i].id, tarefa[i].ano, tarefa[i].mes, tarefa[i].dia, tarefa[i].hora, tarefa[i].minuto, tarefa[i].cmd);
                    snprintf(listBufferAux1[countL[0]], aux1 + 1, "%d %d-%d-%d %d:%d:00 %s\n", tarefa[i].id, tarefa[i].ano, tarefa[i].mes, tarefa[i].dia, tarefa[i].hora, tarefa[i].minuto, tarefa[i].cmd);
                    printf("%s", listBufferAux1[countL[0]]);
                    countL[0]++;
                }

                else if (tarefa[i].state == 1)
                {
                    printf("exec");

                    int aux2 = snprintf(NULL, 0, "%d %d-%d-%d %d:%d:00 %s\n", tarefa[i].id, tarefa[i].ano, tarefa[i].mes, tarefa[i].dia, tarefa[i].hora, tarefa[i].minuto, tarefa[i].cmd);

                    snprintf(listBufferAux2[countL[1]], aux2 + 1, "%d %d-%d-%d %d:%d:00 %s\n", tarefa[i].id, tarefa[i].ano, tarefa[i].mes, tarefa[i].dia, tarefa[i].hora, tarefa[i].minuto, tarefa[i].cmd);
                    printf("%s", listBufferAux2[countL[1]]);
                    countL[1]++;
                }
                else if (tarefa[i].state == 2)
                {
                    printf("cancelada");

                    int aux3 = snprintf(NULL, 0, "%d %d-%d-%d %d:%d:00 %s\n", tarefa[i].id, tarefa[i].ano, tarefa[i].mes, tarefa[i].dia, tarefa[i].hora, tarefa[i].minuto, tarefa[i].cmd);
                    snprintf(listBufferAux3[countL[2]], aux3 + 1, "%d %d-%d-%d %d:%d:00 %s\n", tarefa[i].id, tarefa[i].ano, tarefa[i].mes, tarefa[i].dia, tarefa[i].hora, tarefa[i].minuto, tarefa[i].cmd);
                    printf("%s", listBufferAux3[countL[2]]);
                    countL[2]++;
                }
            }

            if (countL[0] != 0)
            {

                strcat(listBuffer, "angendadas:\n");

                for (int i = 0; i < countL[0]; i++)
                {
                    strcat(listBuffer, listBufferAux1[i]);
                    printf("\nAgenda Buffer\n");
                    printf("%s\n", listBuffer);
                }
            }

            if (countL[1] != 0)
            {
                strcat(listBuffer, "\nexecutadas:\n");
                for (int i = 0; i < countL[1]; i++)
                {
                    strcat(listBuffer, listBufferAux2[i]);
                    printf("\nExec Buffer\n");
                    printf("%s\n", listBuffer);
                }
            }

            if (countL[2] != 0)
            {
                strcat(listBuffer, "canceladas:\n");
                for (int i = 0; i < countL[2]; i++)
                {
                    strcat(listBuffer, listBufferAux3[i]);
                    printf("\n Cnela Buffer\n");
                    printf("%s\n", listBuffer);
                }
            }

            strcpy(buf2, listBuffer);

            fd1 = open(myfifo, O_WRONLY);
            write(fd1, buf2, buffsize);
            close(fd1);
            memset(buf2, 0, buffsize);
            memset(listBuffer, 0, buffsize);
        }

        else if ((strcmp(token[0], result) == 0))
        {

            printf("estou no if!!!!!!!!!");
            int buffsize = 10048;
            char *resultBuffer = (char *)malloc(buffsize);
            memset(resultBuffer, 0, buffsize);
            char *resultBufferAux[1024];
            int *taksid;
            int n = countTarefas();
            printf("\n\nN %d\n", n);
            sscanf(token[1], "%d\n", &taksid);
            printf("taskID %d\n", taksid);
            for (int i = 0; i < n; i++)
            {
                printf("estou no For!!!!!!!!!");
                if (tarefa[i].id == taksid)
                {
                    printf("estou no if dentro do for!!!!!!!!!");
                    if (tarefa[i].state == 1)
                    { //efetuada

                        int aux = snprintf(NULL, 0, "\nId:\n%d\n\nData:\n%d-%d-%d %d:%d:00\n\nLinha de comando:\n%s\n\nValor de saída:\n%d\n\nStandard output:\n%s\n\nStandard error:\n%s\n\n", tarefa[i].id, tarefa[i].ano, tarefa[i].mes, tarefa[i].dia, tarefa[i].hora, tarefa[i].minuto, tarefa[i].cmd, tarefa[i].exitValue, tarefa[i].output, tarefa[i].outputERR);
                        printf("output Size: %d", aux);
                        snprintf(resultBufferAux, aux + 1, "\nId:\n%d\n\nData:\n%d-%d-%d %d:%d:00\n\nLinha de comando:\n%s\n\nValor de saída:\n%d\n\nStandard output:\n%s\n\nStandard error:\n%s\n\n", tarefa[i].id, tarefa[i].ano, tarefa[i].mes, tarefa[i].dia, tarefa[i].hora, tarefa[i].minuto, tarefa[i].cmd, tarefa[i].exitValue, tarefa[i].output, tarefa[i].outputERR);
                    }
                    else
                    {
                        printf("\nTArefa ainda não efetuada\n");
                    }

                    i = n;
                }
            }
            strcpy(resultBuffer, resultBufferAux);

            strcat(buf2, resultBuffer);

            fd1 = open(myfifo, O_WRONLY);
            write(fd1, buf2, buffsize);
            close(fd1);
            memset(buf2, 0, buffsize);
            memset(resultBuffer, 0, buffsize);

            printf("\n RESULT BUFFER \n");
            printf("%s\n", resultBuffer);
        }
        else if ((strcmp(token[0], cancelar) == 0))
        {

            int idTarefa;
            printf("Pedido para cancelar\n");
            sscanf(token[1], "%d\n", &idTarefa);
            cancelTask(idTarefa);
            printf("tarefa cancelada");

            fd1 = open(myfifo, O_WRONLY);
            write(fd1, "ok", nbyte);
            close(fd1);
        }
        else if ((strcmp(token[0], nmax) == 0))
        {

            int nmax;

            printf("Pedido definir N MAX\n");
            sscanf(token[1], "%d\n", &nmax);

            setMax(nmax);
            printf("Novo numero maximo de tarefas definido");
            fd1 = open(myfifo, O_WRONLY);
            write(fd1, "ok", nbyte);
            close(fd1);
        }
        else
        {
            fd1 = open(myfifo, O_WRONLY);

            write(fd1, "comando inválido", nbyte);
            close(fd1);
        }
    }
    return 0;
}