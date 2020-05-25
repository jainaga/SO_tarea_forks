#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>



void inicializacion_matriz_tiempos(int arreglo[5][5]){
    int i, j;
    for (i=0;i<5;i++){  
            for(j=0;j<5;j++){
                arreglo[i][j] = 0;  //se configura la matriz con cada celda o topo con tiempo cero, por lo que no hay ningun topo levantado
            }
        }
}

void lectura_matriz_tiempos(int arreglo[5][5]){
    int i, j;
    int valores[5];
    for (i=0;i<5;i++){  
            for(j=0;j<5;j++){
                valores[j] = arreglo[i][j];
            }
        printf("[%d], [%d], [%d], [%d], [%d]\n", valores[0], valores[1], valores[2], valores[3], valores[4]);
        }
}

int main(void){
    int p_h1[2], h1_h2[2], h2_h3[2], h3_p[2];       //pipes
    char buf[100];
    int num;
    pid_t pid_p, pid_h1, pid_h2, pid_h3, pid;            //pids

    pipe(p_h1);
    pipe(h1_h2);
    pipe(h2_h3);
    pipe(h3_p);

    switch (pid = fork())       //creacion de hijos 1, 2 y 3
    {
    case 0:         //hijo 1
        close(h1_h2[0]);       //cerramos lectura del hijo 1 a hijo 2
        close(p_h1[1]);         //cerramos escritura de hijo 1 a padre
        pid_h1 = getpid();      //almaceno pid hijo 1
        break;
    
    case -1:
        /*no funciona*/
    
    default:     //padre
        //printf("%d, y el del padre %d\n", pid, getpid());
        close(h3_p[1]);     //cerramos escritura de padre a hijo 3
        close(p_h1[0]);     //cerramos lectura de padre a hijo 1
        pid_p = getpid();   //almaceno pid padre
        switch (pid = fork())
        {
        case 0:         //hijo 2
            close(h2_h3[0]);       //cerramos lectura del hijo 2 a hijo 3
            close(h1_h2[1]);         //cerramos escritura de hijo 2 a hijo 1
            pid_h2 = getpid();      //almaceno pid hijo 2
            break;
        
        case -1:
            /*no funciona*/
        
        default:     //padre
            switch (pid = fork())
            {
            case 0:     //hijo 3
                pid_h3 = getpid();
                close(h3_p[0]);     //cerramos lectura de hijo 3 a padre
                close(h2_h3[1]);    //cerramos escritura de hijo 3 a hijo 2
                break;
            case -1:
                //no funciona 
                break;
            
            default:        //padre
                sleep(1);
                break;
            }
            break;
        }
        
        break;
    }

    //creacion de matriz de tiempo de los topos solo para el padre
    //int i, j;
    if (getpid() == pid_p){
        int arreglo_tiempos[5][5];      //arreglo de 5x5 que almacena el estado de cada celda del tablero con tiempos de cada topo 
        inicializacion_matriz_tiempos(arreglo_tiempos);
        lectura_matriz_tiempos(arreglo_tiempos);
    }
    printf("buena soy el proceso %d\n", getpid());

    //aqui empieza el juego 

    if (getpid() == pid_p){
        while(1){
            int opcion;
            printf("Bienvenido a Golpéa el Topo\n\n\n");
            printf("1.- A jugar\n");
            printf("2.- Salir\n");
            printf("Elige una opción del menú y enter para continuar\n");
            scanf("%d", &opcion);
            switch (opcion)
            {
            case 1:     //jugar al juego
                while (1)
                {
                    printf("Elige un modo de juego\n");
                    printf("1.- Dame el maso, los acabare\n");  //se le pide al jugador la posicion donde golpeara
                    printf("2.- Modo rápido\n");                //se golpea de manera aleatoria en el tablero
                    printf("3.- Volver\n");                     //se desea volver al menu principal
                    printf("Elige un modo de juego y enter para continuar\n");
                    scanf("%d", &opcion);
                    if(opcion==1 || opcion==2){
                        //juega, y se toma encuenta el modo de juego


                        
                    }
                    else if(opcion == 3){   //vuelve al menu principal
                        break;
                    }
                    else{
                        printf("Ingresa un número válido\n\n\n");   //ingreso numero invalido, se mostrara nuevamente el menu de modo de juego
                        sleep(2);
                    }
                }
                
                
                break;
            case 2:     //salir del juego 
                exit(1);
            
            default:    //numero ingresado invalido, se vuelve a mostrar el menú
                printf("Ingresa un número válido\n\n\n");
                sleep(2);
                break;
            }

        }
    }

    return 0;
}
