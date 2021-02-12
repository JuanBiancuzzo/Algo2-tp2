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
El tipo de dato abstracto Heap, y en este caso mínimal, tiene una estructura donde almacena el vector de elementos, la cantidad de elementos, un comparador y un destructor. Las funciones con las que opera son dos administrativas (crear y destruir) e insertar, elemento en la raiz y eliminar la raiz

##### Aclaracion
El heap es un árbol casi completo, es decir que se completa de izquierda a derecha y por lo tanto se puede ir numerando cada elemento de izquierda a derecha, y de arriba hacia abajo, de forma secuencial, por lo que podemos hablar de un vector de elementos a la vez que estamos hablando de este árbol.

#### Crear y destruir
Para crear un heap, se reserva espacio en el heap y se agregan a la estructura del heap la función comparador, que es una función dado dos elementos te devuelve el 1 si el primero es más chico y -1 si el segundo es el más chico, y la función liberar elemento, donde cada vez que se tenga que eliminar se utiliza esta función por si se necesita hacer un proceso antes de elimiar dicho elemento.

En la destrucción del heap, se elimina cada elemento del vector, usando la función liberar elemento para cada elemento, y luego se libera la memoria que estaba reservada para el heap.

#### Insertar
Para insertar agregamos el elemento al final del vector, y después comprobamos que cumpla los requisitos de un heap minimal, en el caso que los cumpla terminamos de insertar; pero si no los cumple tenemos que intercambiar con el padre y recalcular tanto el padre como si cumple los requisitos, y repetir hasta que se cumpla los requisitos o llegar a la posición 0 que es la raiz del heap.

Para calcular la posición del padre de la posición actual, se le resta uno, y se lo divide por 2. Como el heap lo podemos pensar como un vector y un árbol, podemos ver como el padre tiene dos hijos, entonces por cada nivel del árbol se duplica la cantidad de nodos del árbol.

Como ejemplo si tenemo un heap de 3 nodos, empezando por el 1, la raíz tiene posición 1 y sus hijos 2 y 3, que si dividimos en dos nos da 1 y 1,5 que redondeado hacia abajo es 1 y 1, la posición de su padre.

Pero como los vectores empiezan en la posición 0, entonces, siguiendo el ejemplo, si estamos en la raíz le sumamos uno para empezar en 1 y después multiplicamos por dos para tener al hijo derecho, si hacemos el proceso contrario, restamos uno y después dividimos tenemos al padre.

#### Elemento en la raiz
Como el heap es un array, el primer elemento del heap es la raiz del heap, entonces se devuelve el primer elemento en el caso que exista el heap.

#### Eliminar la raiz
Para eliminar la raiz del heap, es utilizando el destructor (en el caso que lo tenga) para destruir los que se guardo en ese elemento y después se remplaza por el último elemento del array.

De forma similar al insertar, se comprueba si cumple los requisitos de un heap minimal, se busca el hijo que sean menor, y se comprueba se cumple los requisitos.

Veamos un ejemplo porque buscamos el hijo menor, tenemos un heap invalido de 3 elementos, donde la raiz es 8, el hijo izquierdo es 2 y el hijo derecho es 6. Ahora tenemos que modificar la raiz para si elegimos el hijo más grande (6) nos queda como raíz el 6, con 2 a la izquierda y 8 a la derecha, todavía no es un heap valido. Entonces elegimos siempre el menor porque así sabemos que si cambiamos el padre por el hijo menor, esos 3 nodos van a quedar en un heap valido.

En el caso de que si se cumplan se termina la eliminación; pero si no se cumple, se intercambian y se recalcula el hijo y la comprobación, y repetir hasta que se cumpla los requisitos o hasta llegar a los últimos hijos del heap.

Para calcular los hijos, tanto el hijo izquierdo como el derecho lo primero es multiplicar por dos, por el motivo que explique cuando estabamos insertando, y luego para el hijo izquierdo le sumamos uno y para el derecho le sumamos 2.

### Herramientas
Primero expliquemos las estructuras que creamos al principio del herramientas.h
#### Estructuras
Empecemos por lo mas simple, tenemos una estructura de un pokemon, este tiene su nombre y 3 atributos, velocidad, defensa y ataque.

Despues tenemos la estructura de un entrenado, este tiene nombre, una indicacion si es lider, y una lista de pokemones.

El siguiente paso son la estructura del gimnasio, que tiene su nombre, la dificultad, un id para el estilo de pelea entre pokemones, y una pila de entrenadores.
Por ultimo tenemos la estructura del mapa, que principalmente tiene el rol de tener el heap de los gimnasios, y para saber cuantos gimnasios se tiene, se puede acceder atraves del heap a la cantidad de elementos que tiene

#### Crear
En este apartado tenemos crear un entrenador, crear un gimnasio y crear un mapa. En los tres casos se repite el mismo formato. 

Primero creamos una estructura en el stack, donde inicializamos esa estructura, en este proceso tendremos que crear un heap para el mapa, una pila para el gimansio o una lista para los entrenadores. En el caso de que salga mal, devolvemos NULL. En caso de que salga bien, creamos la estructura en el heap y en el caso de que el puntero sea valido, pasamos todos los datos de la estructura que esta en el stack a esta nueva estructura y devolvemos su puntero.

La idea de primero crear una estructura en el stack es para retrasar lo maximo posible reservar memoria en el heap, porque cuando uno reserva memoria, si sucede algo como que no pudiste crear una lista, tenes que liberar esa memoria. Entonces lo mejor es retrasarlo asi tenes menos momentos para manejar memoria y por lo tanto posibles errores

#### Destruir
Por como esta estructurado, cada estructura (pokemon, entrenador, gimnasio, mapa) esta claramente dividida en niveles, donde tenemos el pokemon en el nivel mas bajo, y el mapa en el nivel mas alto. 

Esto no ayuda a la forma de destruir porque lo podemos pensar *casi* de una forma recursiva, la funcion base es destruir un pokemon que es liberar el puntero a ese pokemon. Y despues destruir entrenador, gimnasio, y mapa es destruir la estructura que usan, el entrenador destruye la lista de pokemones, el gimnasio destruye la pila de entrenadores, y el mapa el heap de gimnasios. Entonces cada estructura tiene un destructor que usa la funcion de un nivel mas abajo, de esa forma ahorrandonos tener que hacerlo nosotros

#### De un archivo a una estructura
Vamos a ver cada caso en particular, porque son suficientemente diferentes para necesitar que se expliquen por separado 

##### De un archivo al personaje principal
Primero intentamos abril el archivo, si no se puede entonces devolvemos NULL, pero si se puede empezamos el proceso de leer el archivo.

Usando al funcion archivo_2_entrenador, que necesita el archivo, un entrenador mas, determinar si es el personaje principal y/o lider. 

Usamos un entrenador mas para solucionar el problema leer las lineas de un archivo sin poder volver para atras; donde para saber si uno tiene que dejar de leer, tiene que leer la proxima linea y por lo tanto perder la informacion si no esta preparado para guardarla. Entonces usamos un entrenador mas por si anteriormente se guardo informacion en este y asi pasarsela al entrenador actual, y para guardar la informacion para la siguiente lectura

Entonces lo primero que se hace es crear un entrenador, y despues se comprueba si habiamos leido algo antes, pero como es el personaje principal no tiene informacion, por lo que tenemos que buscar el nombre, y va intentar conseguir un nombre ignorando todas las lineas que esten en el medio, si llega al final del archivo sin encontrar un nombre devuelve NULL para indicar que no encontro nada ningun entrenador

Ahora vamos a guardar los pokemones del entrenador, usando la funcion guardar_pokemones, que va leyendo cada linea y guardando los pokemones, si es un entrenador cualquiera este solo puede guardar 6 pokemones entonces cuando llegue a ese numero de pokemones dejara de leer, pero si es el personaje principal este seguira leyendo hasta que termine el archivo o encontrar un entrenador. 

Para leer las lineas y en caso de encontrar un entrenador guardarlo, se usa la funcion dato_esperado donde esta preparado para leer un gimnasio, entrenador o pokemon. Entonces cuando estamos leyendo, pasamos a la funcion el entrenador de mas y un pokemon, si lee un pokemon lo guardamos, si lee un entrenador lo guarda, y de esa forma en la funcion de guardar pokemones sabemos cuando frenar.

Y despues de hacer todo esto, ya tenemos el personaje principal, con todos sus pokemones. En caso de que este personaje principal no tenga pokemones se destruye el puntero en el heap, y se devuelve NULL

##### De un archivo al gimnasio
Primer leemos el archivo, y creamos un gimnasio. En este caso no podemos evitar crear el gimnasio al principio porque lo necesitamos para guardar los entrenadores. 

Para guardar los entrenadores usamos la funcion (nombrada de forma muy original) como guardar_entrenadores. Esta primero lee usando la funcion que habiamos mencionado para leer entrenadores (archivo_2_entrenador) el lider del gimnasio, en caso de que no este sale, pero si hay entonces vamos a empezar a leer entrenadores y agregandolos a la pila de entrenadores que tiene el gimnasio

### Menús

### Batallas

### Lista
El TDA Lista ya tiene su propia documentacion por lo que no se explicara aca como es su funcionamiento

## Pruebas
Estas pruebas sobre el funcionamiento del heap, y las herramientas usadas en el trabajo, estan implementados como TDA independientes, por lo que estas pruebas muestran su correcto funcionamiento de ambas.

### Compilar y ejecutar
Para compilar las pruebas usamos la linea

``` bash
gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -std=c99 -O0 librerias/*.c pruebas/pruebas.c -o prueba 
```
y con ayuda del makefile podemos usar la linea

``` bash
make vbuild
```
para compilar, para ejecutar las pruebas, usamos valgrind con la linea

``` bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./prueba
```
y con el makefile es 

``` bash
make valgrind
```
