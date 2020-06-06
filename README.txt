Francisco Reyes Jainaga

Rol: 201773529-5

Instrucciones para el uso de la tarea:
1.- Instalar gcc
2.- Abrir en terminal la ubicación de la carpeta con los archivos de la tarea.
    En la carpeta se encontraran los siguientes archivos:

	TAREA2_Francisco_Reyes.c: Codigo donde se encuentra la funcion inicializacion_matriz_tiempos, lectura_matriz_tiempos, mostrar_topos, restar_vida_topos, agregar_topo, golpear_topo y main.
	makefile: Archivo con reglas de compilación.
	README.txt: Archivo con instrucciones de uso de la Tarea 2.

3.- En la terminal realizar los siguientes comandos para compilar el archivo:

	En terminal escribir comando:

	$ make

	Este comando crea el ejecutable de la Tarea 2 llamado tarea.
	Para borrar el archivo creado y reiniciar la compilación, escribir en terminal:

	$ make clean

Consideraciones:

-Al utilizar el menú de navegación en terminal, ingresar los números que aparecen en las opciones en pantalla y luego enter.

-Leer las instrucciones que entrega en menú de la terminal, ya que estas indican como navegar.

-Como mensaje entre los pipes se usa una estructura que tienen todos los procesos la cual contiene cantidad de topos, tiempo de topos y coordenadas de topos.

-La cantidad de topos a generar es aleatoria entre 0 y 3 incluyendo ambos valores.

-La matriz de topos se mostrará por terminal con sus filas y columnas enumeradas para que el usuario sepa las coordenadas en donde quiera golpear.

-Al momento de tener las coordenadas del topo a golpear, se verifica si el lugar está ocupado por un topo, en ese caso no se hace nada y así sigue existiendo el topo que estaba.

-El tiempo de vida del topo a crear es aleatorio entre 0 y 3 tiempos incluyendo a estos valores.

