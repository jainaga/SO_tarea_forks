#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 


typedef struct informacion_topos {      //estrucura que almacena la informacion de los nuevos topos
    int cant_topos;                     //este será el mensaje que se enviarán los hijos y el tercero al padre
    int tiempo_topos[3];
    int coordenadas_topos[6];
}informacion_topos;


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

void mostrar_topos(int arreglo[5][5]){      //imprime la matriz de los topos 
    int i, j;
    char valores[5];
    int cont_y = 0;
    for (i=0;i<5;i++){  
        for(j=0;j<5;j++){
            if (arreglo[i][j]==0){
                valores[j] = '_';    //no topo
            }
            else{
                valores[j] = '^';      //topo
            }
        }
    printf("%d [%c], [%c], [%c], [%c], [%c]\n", cont_y, valores[0], valores[1], valores[2], valores[3], valores[4]);       //se imprime la fila de la matriz de los topos
    cont_y += 1;
    }
    printf("   0    1    2    3    4 \n");
}

void restar_vida_topos(int arreglo[5][5]){      //se resta uno de vida a cada topo
    int i, j;
    for (i=0;i<5;i++){  
        for(j=0;j<5;j++){
            if(arreglo[i][j]>0){
                arreglo[i][j] = arreglo[i][j] - 1;
            }
        }
    }
}

void agregar_topo(int arreglo[5][5], int x, int y, int tiempo){     //se agrega tiempo de vida del topo en la ubicación asignada de la matriz de tiempo
    arreglo[y][x] = tiempo;
}

void golpear_topo(int arreglo[5][5], int x, int y){             //se iguala a cero el lugar de la matriz de tiempo en donde estaba el topo golpeado
    arreglo[y][x] = 0;
}

int main(void){
    int p_h1[2], h1_h2[2], h2_h3[2], h3_p[2];       //pipes
    int num;
    pid_t pid_p, pid;            //pids

    informacion_topos* mensaje;
    mensaje = (informacion_topos*)malloc(sizeof(informacion_topos));

    pipe(p_h1);
    pipe(h1_h2);
    pipe(h2_h3);
    pipe(h3_p);

    srand(time(0));

    switch (pid = fork())       //creacion de hijos 1, 2 y 3
    {
    case 0:         //hijo 1
        close(h1_h2[0]);       //cerramos lectura del hijo 1 a hijo 2
        close(p_h1[1]);         //cerramos escritura de hijo 1 a padre
        close(h2_h3[0]);        //cerramos las pipes que no usara este proceso 
        close(h2_h3[1]);
        close(h3_p[0]);
        close(h3_p[1]);
        while (0 < (num = read(p_h1[0], mensaje , sizeof(informacion_topos))))
        {
            
            if (mensaje->cant_topos == -1){
                //mandar mensaje a hijo 2 para hacer break, se acabó el juego
                close(p_h1[0]);         //cerramos lectura de hijo 1 con padre
                mensaje->cant_topos = -1;
                write(h1_h2[1], mensaje, sizeof(informacion_topos));        //mandamos mensaje a hijo 2 para que termine 
                close(h1_h2[1]);        //cerramos escritura hijo 1 a hijo 2
                break;
            }
            else if(mensaje->cant_topos > -1){
                //calcular cantidad de topos a salir de 0-3
                //mandar mensaje a hijo 2 de hacer su pega, con el struct de mensaje
                mensaje->cant_topos = (rand() % 4);     //se genera el numero de topos a salir con random de 0 a 3 
                write(h1_h2[1], mensaje, sizeof(informacion_topos));
            }
        }
        
        break;
    
    case -1:
        /*no funciona el fork*/
        exit(1);
    
    default:     //padre
        //printf("%d, y el del padre %d\n", pid, getpid());
        close(p_h1[0]);     //cerramos lectura de padre a hijo 1
        pid_p = getpid();   //almaceno pid padre
        switch (pid = fork())
        {
        case 0:         //hijo 2
            close(h2_h3[0]);       //cerramos lectura del hijo 2 a hijo 3
            close(h1_h2[1]);         //cerramos escritura de hijo 2 a hijo 1
            close(p_h1[1]);
            close(h3_p[0]);
            close(h3_p[1]);

            int z;                  //contador de para el for 
            while(0 < (num = read(h1_h2[0], mensaje, sizeof(informacion_topos)))){
                

                if (mensaje->cant_topos == -1){
                //mandar mensaje a hijo 3 para hacer break, se acabó el juego
                close(h1_h2[0]);         //cerramos lectura de hijo 2 con hijo 1
                mensaje->cant_topos = -1;
                write(h2_h3[1], mensaje, sizeof(informacion_topos));        //mandamos mensaje a hijo 3 para que termine 
                close(h2_h3[1]);        //cerramos escritura hijo 2 a hijo 3
                break;
                }
                else if(mensaje->cant_topos > -1){
                    //calcular timepos de los topos a salir de 0-3
                    //mandar mensaje a hijo 3 de hacer su pega, con el struct de mensaje
                    for (z=0; z<mensaje->cant_topos; z++){
                        mensaje->tiempo_topos[z] = (rand() % 4);
                    }
                    sleep(0.5);
                    write(h2_h3[1], mensaje, sizeof(informacion_topos));        //se manda mensaje con los tiempos de vida de los topos
                }
            }
            
            break;
        
        case -1:
            /*no funciona el fork*/
            exit(1);
        
        default:     //padre
            switch (pid = fork())
            {
            case 0:     //hijo 3
                close(h3_p[0]);     //cerramos lectura de hijo 3 a padre
                close(h2_h3[1]);    //cerramos escritura de hijo 3 a hijo 2
                close(p_h1[1]);
                close(h1_h2[0]);
                close(h1_h2[1]);
                int z;

                while(0 < (num = read(h2_h3[0], mensaje, sizeof(informacion_topos)))){
                    

                    if (mensaje->cant_topos == -1){
                    
                        close(h2_h3[0]);         //cerramos lectura de hijo 3 con hijo 2
                        close(h3_p[1]);        //cerramos escritura hijo 3 a padre
                        break;
                    }
                    else if(mensaje->cant_topos > -1){
                        //calcular coordenadas de los topos a salir dentro del tablero
                        //mandar mensaje a padre de hacer su pega, con el struct de mensaje
                        for(z=0; z<mensaje->cant_topos; z++){
                            mensaje->coordenadas_topos[z*2] = (rand() % 5);     //coordenadas x
                            mensaje->coordenadas_topos[(z*2)+1] = (rand() % 5); //coordenadas y
                        }
                        
                        write(h3_p[1], mensaje, sizeof(informacion_topos)); //mensaje al padre con las informacion de los topos
                    }
                }
                break;
            case -1:
                //no funciona el fork
                exit(1);
            
            default:        //padre
                close(h3_p[1]);     //cerramos escritura de padre a hijo 3
                sleep(0.5);
                break;
            }
            break;
        }
        
        break;
    }

    

    //aqui empieza el juego 

    if (getpid() == pid_p){         //se revisa el pid del proceso para que solo el padre entre al if, ya que este maneja el juego
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
                        int arreglo_tiempos[5][5];      //arreglo de 5x5 que almacena el estado de cada celda del tablero con tiempos de cada topo 
                        inicializacion_matriz_tiempos(arreglo_tiempos);
                        int m;      //contador de for
                        int coor_x, coor_y;
                        int salida;

                        
                        while (1)
                        {
                            mensaje->cant_topos = 0;        //mensaje para que hijo 1 empiece su trabajo
                            write(p_h1[1], mensaje, sizeof(informacion_topos));
                            read(h3_p[0], mensaje, sizeof(informacion_topos));

                            
                            if (mensaje->cant_topos>0){
                                //se hace la pega, de otra manera no se modifica la matriz
                                for(m=0; m<mensaje->cant_topos; m++){
                                    if(arreglo_tiempos[mensaje->coordenadas_topos[(2*m)+1]][mensaje->coordenadas_topos[2*m]] == 0){     //se revisa si el topo puede salir es la posicion asignada
                                        agregar_topo(arreglo_tiempos, mensaje->coordenadas_topos[2*m], mensaje->coordenadas_topos[(2*m)+1], mensaje->tiempo_topos[m]);
                                    }
                                    
                                }
                            }
                            //se muestra la matriz con los topos
                            mostrar_topos(arreglo_tiempos);

                            //se analiza opcion de juego para ver la accion
                            if(opcion==1){
                                //se pide por pantalla golpe
                                printf("Ingresa coordena de x o -1 para salir\n");
                                while (1)
                                {
                                    scanf("%d", &coor_x);
                                    if((coor_x>-2) && (coor_x<5)){
                                        break;
                                    }
                                    else{
                                        printf("Ingrese opción válida\n");
                                    }
                                }
                                
                                if(coor_x == -1){
                                    break;
                                }
                                printf("Ingrese coordenada de y\n");
                                while (1)
                                {
                                    scanf("%d", &coor_y);
                                    if((coor_y>-2) && (coor_y<5)){
                                        break;
                                    }
                                    else{
                                        printf("Ingrese opción válida\n");
                                    }
                                }
                                
                                golpear_topo(arreglo_tiempos, coor_x, coor_y);
                            }else if(opcion==2){
                                //el golpe es aleatorio
                                coor_x = (rand() % 5);
                                coor_y = (rand() % 5);
                                golpear_topo(arreglo_tiempos, coor_x, coor_y);
                                printf("Se golpeó en las coordenadas %d, %d\n", coor_x, coor_y);
                                printf("Ingresa un número para continuar o ingrese 0 para salir\n");
                                scanf("%d", &salida);
                                if (salida == 0){
                                    break;
                                }
                                sleep(0.5);
                            }
                            //restar vida a topos 
                            restar_vida_topos(arreglo_tiempos);
                        }
                        

                    }
                    else if(opcion == 3){   //vuelve al menu principal
                        break;
                    }
                    else{
                        printf("Ingresa un número válido\n\n\n");   //ingreso numero invalido, se mostrara nuevamente el menu de modo de juego
                        sleep(0.5);
                    }
                }
                
                
                break;
            case 2:     //salir del juego 
                mensaje->cant_topos = -1;
                write(p_h1[1], mensaje,sizeof(informacion_topos));
                sleep(2);
                close(p_h1[1]);     //cerramos escritura de padre a hijo 1
                close(h3_p[0]);
                close(h1_h2[0]);
                close(h1_h2[1]);
                close(h2_h3[1]);
                close(h2_h3[0]);
                free(mensaje);
                printf("Hasta otra\n");
                exit(1);
            
            default:    //numero ingresado invalido, se vuelve a mostrar el menú
                printf("Ingresa un número válido\n\n\n");
                sleep(1);
                break;
            }

        }
    }
    free(mensaje);

    return 0;
}
