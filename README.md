# Aventura pokemon 

## Descripción del trabajo

Este trabajo se trata te imitar un juego de pokemon, de forma limitada. En esta juego tenemos a nuestro personaje principal que va recorriendo gimnasios, al principio más fáciles y en el progreso del juego la dificultad va aumentando, y en cada gimnasio va a ir luchando contra los entrenadores más principiantes hasta el líder del gimnasio. Cada vez que se gane un gimnasio se podrá tomar prestado un pokemon del líder para ir mejorando tu colección de pokemones. Cuando le ganes a todos los gimnasios del juego te convertís en un maestro pokemon y se termina el juego.

Tanto el personaje principal como los gimnasios se pueden agregar al juego, mediante archivos de texto que veremos luego como están formateados. Y aunque ingreses gimnasios sin un orden, el juego los sabrá ordenar para que siempre empiece con los más fáciles y progresivamente aumentando la dificultad.

### Información importante

Para formatear los archivos, ya sea del gimnasio o del personaje principal, se utilizan los siguientes formatos. Cada tipo de dato, gimnasio, líder, entrenador y pokemon tiene que estar en una linea separado, y la linea tiene que tener.

Primero: Un identificador de que tipo de dato es, para un gimnasio es la G, para el líder es la L, un entrenador la E y para el pokemon la P, seguido por un punto y coma.

Segundo: El nombre, seguido por un punto y coma.

Tercero: Los atributos de ese tipo de dato, el gimnasio tiene la  dificultad del gimnasio, punto y coma, un numero del 1 al 5 indicando un estilo de lucha que se cumplirá para todo el gimnasio. El líder solo tiene el nombre pero tiene que estar ubicado después del gimnasio, ser único, y tienen que tener al menos un pokemon después de su línea ya que estos van a ser los pokemones del líder. Al igual que el líder, los entrenadores tiene solo tienen el nombre pero pueden haber varios, pero para ser un entrenador valido tienen que tener al menos un pokemon en las líneas siguientes a este. Por último, el pokemon, tiene tres atributos, velocidad, ataque y defensa, en ese orden y separados por un punto y coma

```
G;Nombre del gimnasio;dificultad;id del estilo de batalla
L;Nombre del lider
P;Nombre del pokemon;velocidad;ataque;defensa
E;Nombre del entenador
P;Nombre del pokemon;velocidad;ataque;defensa
```

Si la información del gimnasio esta mal no se usará como gimnasio, si no hay líder o el líder no tiene pokemones el gimnasio no se usará, pero si hay un entrenador que tiene errores en el formato o no tiene pokemones, simplemente se saltea ese entrenador. El líder y los entrenadores pueden tener hasta 6 pokemones, si se pasan de esa cantidad simplemente se saltearán esos pokemones.

El personaje principal es un archivo donde solo hay un entrenador

```
E;Nombre del personaje principal
P;Nombre del pokemon;velocidad;ataque;defensa
```

Pero a diferencia de los líderes y entrenadores, el personaje principal puede tener la cantidad que quiera de pokemones.

Ya teniendo un personaje principal o un gimnasio para agregar, se tiene que ubicar en la carpeta principal/ o gimnasio/ respectivamente y cuando se quiera ingresar un archivo en el propio juego solo se tiene que ingresar el nombre del archivo ya que se sabe que están en esas carpetas.

## Compilar y ejecutar

### Compilar
Para poder ejecutarlo primero tenemos que compilarlo, esto lo hacemos con la siguiente linea
``` bash
gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -std=c99 -O0 librerias/*.c main.c -o aventura 
```
De esta forma todas las librerías que se usan en el trabajo se vincularan al main.c 

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

Con la ayuda de makefile se puede ejecutar el juego, o ejecutar el juego con los default personaje y gimnasios, con las lineas
``` bash
make exe
```
y 
``` bash
make default
```

## Detalles de implementación (librerías)

### TDA Heap

### Herramientas

### Menú

### Batallas

## Pruebas

