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

struct Tarefas tarefa[200];

//Variáveis globais...
static int alarme[200];
struct Tarefas tarefa[200];

int countTarefas()
{
    int count = 0, i = 0;
    while (tarefa[i].data != 0)
        i++;
    printf("%d", i);
    return i;
};

// int agendarTarefa(char **token)

// {
//     char delim[] = " ";
//     char buffer[] = "cenas";
//     int sizeTarefas = countTarefas();
//     printf("token 0: %s", token[0]);
//     printf("token size: %ld", sizeof(token));
//     printf("FISRST %s\n", token[0]);
//     int i = 0;
//     while (token[i] != NULL)
//     {
//         printf("TOKEN %d : %s\n", i, token[i]);
//         i++;
//         token[i] = strtok(NULL, delim);
//     }
//     printf("AQUI");
//     tarefa[sizeTarefas].id = sizeTarefas;
//     tarefa[sizeTarefas].data << *token[1];

//     int n = 0;
//     for (i = 2; i <= sizeof(token); i++)
//     {
//         tarefa[sizeTarefas].cmd[n] << *token[i];
//         n++;
//     }

//     while (token[i] != NULL)
//     {
//         printf("TOKEN %d : %s\n", i, token[i]);
//         i++;
//         token[i] = strtok(NULL, " ");
//     }

//     // tarefa[sizeTarefas].data = token[2];
//     printf("Pee agendamento recebido");
// }
int manageCommand(char *command)
{

    // char str = *command.Substring(0,5);
    // int nbyte =256;
    // if(prefixo("-a",command) == true){
    //     printf("true");
    // }

    return 0;
};

int main()
{

    int fd1, nbyte = 256;
    char *buf1 = (char *)malloc(nbyte * sizeof(char));
    char *buf2 = (char *)malloc(nbyte * sizeof(char));
    char delim[] = " ";
    char *agendar = "-a";
    char *listar = "-l";
    int len;
         int data;
        int horas;
        char *comando[100];
    int init_size = strlen(buf1);

    size_t num;
    char *tarefaRealizar;
    int index;
    char *aux2;
    // FIFO file path
    char *myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    char *token[nbyte], response[256];
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

        if (strcmp(token[0], agendar) == 0)
        {

            printf("Pee agendamento recebido");
            sscanf(token[1], "%d\n", &data);
            sscanf(token[2], "%d\n", &horas);
            sscanf(token[3], "%s\n", comando);
            printf("data: %d\n horas: %d \n comando: %s", data, horas, &comando);
            // char *aux = strstr(buf1, &comando);
            // printf("\nAUX :%c", aux);


            int n = countTarefas();

            tarefa[n].id = n;
            tarefa[n].data = data;
            tarefa[n].hora = horas;
            strcpy(tarefa[n].cmd, comando);

            // strcpy(tarefa[n].cmd, aux);
            printf("Pee agendamento a");
        }
        else if (strcmp(token[0], listar) == 0)
        {
            printf("Pedido de listagem\n");
            num = countTarefas();
            for (int i = 0; i < num; i++)
            {
                printf("tafera: %d \n data : %d\n horas: %d\n comando: %s", tarefa[i].id, tarefa[i].data, tarefa[i].hora, tarefa[i].cmd);
                // for (int n = 0; n <= 2; n++)
                //     printf("comandos: %d", tarefa[i].cmd[n]);
            }
        }

        // printf("FISRST %s\n", token[0]);
        // int i = 0;
        // while (token[i] != NULL)
        // {
        //     printf("TOKEN %d : %s\n", i, token[i]);
        //     i++;
        //     token[i] = strtok(NULL, delim);
        // }

        // for (i = 0; i < sizeof(token); i++)
        // {
        //     printf("TOKEN %c", token[i]);
        // }

        // Print the read string and close
        // printf("User1: %s\n", buf);
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