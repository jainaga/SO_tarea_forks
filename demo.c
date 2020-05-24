#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    int fd[2];
    char buf[100];
    int num;
    pid_t pid;

    pipe(fd);

    switch (pid = fork())
    {
    case 0:         //hijo
        close(fd[0]);
        write(fd[1], "aloha!", 6);
        write(fd[1], " adios", 6);
        close(fd[1]);
        exit(0);
        break;
    
    case -1:
        /*no funciona*/
    
    default:     //padre
        printf("%d, y el del padre %d\n", pid, getpid());
        close(fd[1]);
        while(0 < (num = read(fd[0], buf , 6))){
            buf[num] = 0;
            printf("El padre recibe %d bytes, %s \n", num, buf);
        }
        printf("El padre va a terminar ya ...\n");
        close(fd[0]);
        break;
    }
    return 0;
}
