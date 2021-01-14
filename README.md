# Aventura pokemon 

## Descripción del trabajo

## Compilar y ejecutar

### Comilar
Para poder ejecutarlo primero tenemos que compilarlo, esto lo hacemos con la siguiente linea
``` bash
gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -std=c99 -O0 librerias/*.c main.c -o aventura 
```
De esta forma todas las librerias que se usan en el trabajo se vincularan al main.c 

También con la ayuda del makefile se puede compilar con la siguiente linea
``` bash
make build
```

### Ejecutar
Para poder ejecutarlo tenemos que hacer la siguiente linea
``` bash
./aventura
```
De esta forma empezará el juego, pero hay alternativas donde queramos empezar el juego con un personaje principal y unos gimnasios agregados al mapa, para eso podemos usar la linea
``` bash
./aventura default
```
Pero si se quiere empezar el juego con un personaje principal que no sea el default y gimnasios que no sean los default, se puede usar esta linea
``` bash
./aventura archivo_personaje_principal archivo_gimnasio ...
```
Donde el primer elemento es el nombre del archivo del personaje principal y el resto son nombres de los archivos de los gimnasios. Estos archivos tiene que estar guardados en las carpetas /principal y /gimnasio respectivamente para que con solo el nombre el juego pueda interpretar los archivos

Con la ayuda de makefile se puede ejecutar el juego, o ejecturar el juego con los default personaje y gimnasios, con las lineas
``` bash
make exe
```
y 
``` bash
make default
```

## Detalles de implementacion (librerias)

### TDA Heap

### Herramientas

### Menu

### Batallas

## Pruebas

