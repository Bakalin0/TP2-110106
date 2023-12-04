<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Sebastián Loe - 110106 - sloe@fi.uba.ar

- Para compilar:

```bash
El trabajo compila y ejecuta con 'make'.
```

- Para ejecutar:

```bash
make / ./pruebas_chanutron (pruebas de la cátedra)
```

- Para ejecutar con valgrind:
```bash
make / make pruebas_chanutron (pruebas de la cátedra)
```
---
##  Funcionamiento
A diferencia de los anteriores TDA entregados, se dificulta mucho (por la longevidad de la tarea), explicar todas y cada una de las funciones de este trabajo. Sin embargo, se explicarán con la mayor profundidad posible las principales funciones del trabajo.

Empezando por juego.c, el archivo cuenta con 3 funciones principales:
- juego_cargar_pokemon: utilizando los archivos pertenecientes al TP1, esta función reutiliza principalmente la función pokemon_cargar_archivo para poder almacenar toda la información del archivo a utilizar por el juego.

- juego_seleccionar_pokemon: con 3 nombres pasados por parámetro, esta función guarda en uno de los jugadores (que también se pasa por parámetro) la información de los pokemones cuyo nombre sea igual al pasado por parámetro. Para verificar esto, se compara el nombre pasado por parámetro con el nombre de cada pokemon que se obtiene utilizando la función lista_elemento_en_posicion. Esto implica que todos los pokemones leídos del archivo se cargan en una lista (esto será explicado en las respuestas a las preguntas teóricas).

- juego_jugar_turno: esta es la función más importante de este archivo. juego_jugar_turno recibe por parámetro las dos jugadas de los jugadores previamente cargados y realiza todo el "movimiento" de batalla de los pokemones. Aquí se calculan los nuevos puntajes, el tipo de ataque, y se verifica que un ataque no haya sido utilizado más de una vez.

Continuando con adversario.c, este archivo cuenta con otras 3 funciones principales:
- adversario_seleccionar_pokemon: en esta implementación, se busca en una lista de pokemones previamente cargada 3 pokemones. Sin embargo, una vez obtenida esta información, se busca el nombre de estos pokemones para que la información pueda volcarse en la función juego_seleccionar_pokemon y así no generar mayores problemas de encapsulamiento. A su vez, se almacenan en listas y árboles la información obtenida.

- adversario_pokemon_seleccionado: simplemente le informa al adversario del tercer pokemon elegido por el usuario. Debido a que este pokemon le pertenece al adversario, se utiliza esta función para que él pueda llevar un trackeo de los pokemones y ataques que posee.

- adversario_proxima_jugada: quizás la función más importante de este archivo, elige un pokemon random de su lista, y luego un ataque random de ese pokemon. Compara la informaciókn obtenida con ataques registrados en un abb. Si esta información no fue previamente utilizada, se encontrará en el árbol, y será elegible para su próxima jugada. 

Por último, main.c:
En este caso, brindaré una explicación del funcionamiento del main, y no de las funciones secundarias creadas para ayudar con la modularización de esta función:
Primeramente se crea el juego y se cargan los pokemones del archivo correcto.txt. Luego, se crea una lista que contiene todos los pokemones de este archivo y se usa esa lista para crear el adversario. Acto seguido se le pregunta al usuario si ya sabe jugar al juego. Si la respuesta es negativa, se muestra por pantalla la idea general del juego y en qué consiste la batalla de pokemones. De lo contrario, se omite este paso y se procede. Se muestra por pantalla el listado de pokemones y se le solicita que ingrese tres nombres de esta lista para almacenar esos pokemones siguiendo la lógica del juego. Siempre y cuando estos nombres elegidos pertenezcan a los nombres de los pokemones, se procede a guardar los pokemones correspondientes y luego se hace lo propio con el adversario: elige 3 nombres y se le asignan sus pokemones. Una vez terminada esta etapa, comienza el juego.
Ahora, mientras la cantidad de turnos sea menor a 9, se repite un bucle while en el que se muestran por pantalla todos los pokemones del juego y se le informa al usuario los pokemones que posee. El usuario ahora deberá elegir un nombre de un pokemon y un ataque perteneciente al mismo. Si el pokemon y el ataque existen, son pertenecientes al usuario, y el ataque no fue utilizado previamente, entonces es el turno del adversario de elegir su pokemon y su ataque. Hecho esto, se utiliza la función juego_jugar_turno para calcular las efectividades de los ataques y asignar los puntos a los jugadores. 
Pasadas las 9 rondas, el juego termina y se calcula el ganador en base al que tenga más puntos. Al término de esta acción, se destruyen todos los elementos creados y la función cierra con un return 0. 


Aclaración: la función adversario_informar_jugada está vacía, y la de adversario_pokemon_seleccionado no cumple con lo que se imaginaba que podía ser la función. Esto es méramente por falta de tiempo. Se suponía que adversario_pokemon_seleccionado informara al adversario de los 3 (no solo uno) pokemones elegidos por el usuario para que la elección de sos pokemones fuese "inteligente", para lograr que el jugador no sume ataques efectivos. De la misma manera, algo similar quería lograr adversario_informar_jugada. El objetivo era hacer a la "máquina" más inteligente de lo que acabó siendo, sin embargo, por falta de tiempo, fue dificultoso de lograr.

De la misma forma, la función "main" de main.c esta poco modularizada. Se entiende que el while habría merecido cierta modularización, de la misma forma que podría haberse modularizado el proceso de elección de los pokemones iniciales. Debido a, valga la redundancia, falta de tiempo y que la modularización tiraba algunos errores (de compilación y luego de valgrind), se decidió que era mejor idea dejarlo como estaba, como también evitar la creación de un TDA Menú.

Dicho esto, me disculpo por el presente estado del trabajo porque, aunque funciona bien, sé que podría haberse hecho aún mejor.

---

## Respuestas a las preguntas teóricas
Incluír acá las respuestas a las preguntas del enunciado (si aplica).

- Explicar las estructuras utilizadas para implementar la lógica del juego y del adversario. Cuando utilice algún TDA de los implementados en la materia explique por qué.

En parte, esta pregunta fue respondida en el funcionamiento del trabajo. Sin embargo, es posible agregar que el objetivo del trabajo es reutilizar la mayor cantidad de funciones (pertenecientes a trabajos anteriores en la materia) para lograr optimizar el código y evitar su repetición. Por esta razón, se utiliza la función pokemon_cargar_archivo en juego_cargar_pokemon, como otras funciones de pokemon.c como con_cada_pokemon (para agregarlos a la lista de pokemones). Asimismo se usan las diversas funciones de lista y abb (lista_insertar, lista_con_cada_elemento, abb_insertar, abb_con_cada_elemento, abb_quitar) para ayudar con esta optimización. 
Así, esta parte da pie para hablar sobre los TDAs utilizados en este trabajo: Lista y ABB. Lista fue esencial para guardar la información de los pokemones. Tanto para el juego, como para ambos jugadores, almacenar los pokemones totales y luego los de cada uno habría sido imposible sin insertarlos en una Lista. Además, la función lista_con_cada_elemento permitió un muy fácil acceso a una iteración de los elementos. Ya sea para buscar los pokemones de los jugadores o para mostrarlos por pantalla.
ABB por su parte tuvo un uso particular: para poder verificar que los ataques elegidos por el usuario y por el adversario no hayan sido elegidos más de una vez, los 3 ataques de los 3 pokemones (9 en total) fueron almacenados en un ABB. De esta manera, cuando se elegía un ataque, se intentaba eliminarlo del ABB. Como la función abb_quitar busca y devuelve el elemento quitado, fue posible dictaminar que, si el ataque ya había sido utilizado, y, por ende, previamente quitado, no se encontraría en el ABB, y habría sido necesario buscar uno nuevo. De lo contrario, si el ataque se encontraba en el ABB es porque aún no había sido utilizado.


- Explique las complejidades de cada función implementada.

- juego_crear: complejidad de O(4). Crear las listas y los abb tienen complejidad de O(1) ya que se componen solo de un calloc. 

- juego_cargar_pokemon: pokemon_cargar_archivo tiene una complejidad de O(n) donde n es la cantidad de líneas del archivo que se pasa. Suponiendo que los archivos tienen el formato correcto, la función 'con_cada_pokemon' tiene una complejidad de n/4 ya que 4 líneas del archivo equivalen a un pokemon; y agregar_a_lista tiene una complejidad de O(1) ya que agregar a lo último de una lista enlazada es inmediato ya que se puede acceder al último elemento sin necesidad de recorrerlos todos. En definitiva, Esta función tiene complejidad O(n).

- juego_listar_pokemon: como se cargó la lista en juego_cargar_pokemon, en esta función solo se devuelve la lista. Invocarla conlleva una complejidad de O(1).

- juego_seleccionar_pokemon: complejidad de O(n). La mayor complejidad de esta función se encuentra en agregar_pokemones, donde se recorren todos los pokemones de la lista. Suponiendo que la lista tiene n elementos, la complejidad es O(n). Luego, los lista_insertar tienen complejidad de O(1) como fue explicado antes y con_cada_ataque recorre 3 ataques por lo que tiene complejidad O(3).

- juego_jugar_turno: complejidad O(n). pokemon_buscar itera entre n pokemones de juego->info, pokemon_buscar_ataque posee una complejidad de O(3) ya que recorre 3 ataques, y los abb_quitar tienen una complejidad de O(3n) ya que cada pokemon tiene 3 ataques. calcular_efectividad y asignar_poder_de_ataque tienen complejidades menores, por lo que la complejidad final es 3n --> O(n).

- juego_obtener_puntaje: O(1), solo busca el puntaje contenido en el struct de jugador. 

- juego_finalizado: O(1), solo busca el número de turno contenido en el struct de juego.

- juego_destruir: O(n), todos los destruir poseen complejidades O(n) porque recorren sus respectivos espacios (lista, abb, vectores dinámicos) para liberar memoria. A razón de 6 'destruir', la complejidad final es 6n --> O(n).

- adversario_crear: O(1), ninguna de las acciones de crear constituyen una complejidad mayor a O(1). Lo que más suma comlejidad es el bucle for, que repite 2 acciones, 3 veces. 

- adversario_seleccionar_pokemon: O(1), ninguna de las acciones tiene la función de iterar, salvo con_cada_ataque, que siempre itera 3 veces únicamente, y luego los dos bucles while que se aseguran que j != i y k != ambos. Como la semilla de rand() está determinada por el tiempo (time.h) y es casi 100% aleatoria, es difícil interpetar si la complejidad puede escalar a un número muy alto. Se supone entonces que, en el peor caso, las probabilidades de que el valor de i y de k sean distintos de las otras variables son 11/12. Por eso, la notación Big O puede resumirse en O(1).

- adversario_pokemon_seleccionado: O(n), dada por el bucle for, que itera por la cantidad de pokemones de la lista, que es n. Luego, los dos con_cada_ataque iteran cada uno, como dije antes, 3 veces.

- adversario_proxima_jugada: O(1), la mayor complejidad de esta función se da en el bucle while. Asumiendo que el máximo tamaño que puede tener el abb es de 9 elementos (9 ataques), las probabilidades de que se elija un ataque aleatorio que esté en el abb descienden de 100% a un 11,11%, que es, salvo en el primer caso (1), menor a 1. Las demás acciones constituyen complejidades O(1). 

- adversario_informar_jugada: O(1), pese a que está vacía, se considera que la complejidad temporal es constante en el tiempo. 

- adversario_destruir: O(1), se utiliza un abb y una lista para almacenar los ataques. Por como está hecho el juego, siempre contendrán, ambos, 9 elementos. Cada una de las tres listas contienen 3 elementos, y el abb contiene 9 elementos. Por eso, se deduce que la complejidad es constante, O(1). Esto podría cambiar si se modifican las reglas del juego, así como también podrían cambiar las complejidades de todas las demás funciones. 

