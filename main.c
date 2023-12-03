#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include <stdio.h>
#include <string.h>
#include "src/menu.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "src/tipo.h"

struct estado_juego{
	juego_t* juego;
	bool continuar;
	menu_t* menu;
};


/*
int main(int argc, char *argv[])
{
	juego_t *juego = juego_crear();

	//Pide al usuario un nombre de archivo de pokemones
	char *archivo = pedir_archivo();

	//Carga los pokemon
	juego_cargar_pokemon(juego, archivo);

	//Crea un adversario que será utilizado como jugador 2
	adversario_t *adversario =
		adversario_crear(juego_listar_pokemon(juego));

	//Mostrar el listado de pokemones por consola para que el usuario sepa las opciones que tiene
	mostrar_pokemon_disponibles(juego);

	//Pedirle al jugador por consola que ingrese los 3 nombres de pokemon que quiere utilizar
	char *eleccionJugador1, *eleccionJugador2, *eleccionJugador3;
	jugador_seleccionar_pokemon(juego, &eleccion_adversario1,
				    &eleccion_adversario2,
				    &eleccion_adversario3);

	//pedirle al adversario que indique los 3 pokemon que quiere usar
	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
				       &eleccionAdversario2,
				       &eleccionAdversario3);

	//Seleccionar los pokemon de los jugadores
	juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1,
				  eleccionJugador2, eleccionJugador3);
	juego_seleccionar_pokemon(juego, JUGADOR2, eleccionAdversario1,
				  eleccionAdversario2, eleccionAdversario3);

	//informarle al adversario cuales son los pokemon del jugador
	adversario_pokemon_seleccionado(adversario, eleccionJugador1,
					eleccionJugador2, eleccionJugador3);

	while (!juego_finalizado(juego)) {
		resultado_jugada_t resultado_ronda;

		//Pide al jugador que ingrese por consola el pokemon y ataque para la siguiente ronda
		jugada_t jugada_jugador = jugador_pedir_nombre_y_ataque();

		//Pide al adversario que informe el pokemon y ataque para la siguiente ronda
		jugada_t jugada_adversario =
			adversario_proxima_jugada(adversario);

		//jugar la ronda y después comprobar que esté todo ok, si no, volver a pedir la jugada del jugador
		resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
						    jugada_adversario);

		//Si se pudo jugar el turno, le informo al adversario la jugada realizada por el jugador
		adversario_informar_jugada(adversario, jugada_jugador);
	}

	juego_destruir(juego);
}
*/

/*
//Esta es para la lista
bool mostrar_pokemon(void* pokemon, void* contexto){
	printf("Nombre: %s\n", pokemon_nombre(pokemon));

	return true;
}


//Estas son para el menú
bool cargar_archivo(void* e){
	//struct estado_juego* estado = e;
	return false;
}

bool listar_pokemones(void* e){
	struct estado_juego* estado = e;

	lista_con_cada_elemento(juego_listar_pokemon(estado->juego), mostrar_pokemon, NULL);
	return true;
}

bool salir(void* e){
	struct estado_juego* estado = e;
	estado->continuar = false;
	return true;
}

bool mostrar_ayuda(void* e){
	struct estado_juego* estado = e;

	menu_con_cada_comando(estado->menu, mostrar_comando, NULL);

	return true;
}

int comparador(void *_elemento1, void *_elemento2)
{
	if (!_elemento1 && !_elemento2) {
		return 0;
	}
	int *elemento1 = _elemento1;
	int *elemento2 = _elemento2;
	return *elemento1 - *elemento2;
}*/

	/*menu_t* menu = menu_crear();

	menu_agregar_comando(menu, "c", "Cargar un archivo", pokemon_cargar_archivo);
	menu_agregar_comando(menu, "l", "Listar Pokemones", juego_listar_pokemon);
	menu_agregar_comando(menu, "ayuda", "Muestra esta ayuda", mostrar_ayuda);
	menu_agregar_comando(menu, "q", "Sale del juego", salir);

	struct estado_juego estado = {.juego = juego, .continuar = true, .menu = menu};

	printf("Hola\n\nIngrese comandos a continuación o escriba 'ayuda' para obtener ayuda");

	while(!juego_finalizado(juego) && estado.continuar){
		printf("\nTP2>> ");

		char linea[200];
		char* leido;
		leido = fgets(linea, 200, stdin);
		if(leido == NULL){
			continue;
		}
		
		linea[strlen(linea)-1] = 0;
		MENU_RESULTADO resultado = menu_ejecutar_comando(menu, linea, comparador);

		if(resultado == MENU_INEXISTENTE){
			printf("Ese comando no existe. Podés intentar con 'ayuda'\n");
		}
		else if(resultado == MENU_ERROR){
			printf("Hubo un problema al ejecutar el comando\n");
		}
	}



	menu_destruir(menu);*/

void pedir_input(char *buffer, int longitud) {
	printf("\nTP2>");
    fgets(buffer, longitud, stdin);

    int largo = (int)strlen(buffer);
    if (largo > 0 && buffer[largo - 1] == '\n') {
        buffer[largo - 1] = '\0';
    }
}

void juego_dar_instrucciones(){
	printf("Cómo jugar:\n\n");
		sleep(1);
		printf("Cuando comience el juego, deberás elegir 3 pokemones, llamándolos por su nombre, uno a uno, para pelear en batalla.\n");
		printf("Los primeros dos pokemones serán tuyos y el tercero será de la IA, así que tené cuidado con tus elecciones!\n");
		sleep(1);
		printf("Por su parte, la IA elegirá otros 3 pokemones y el tercero que elija, será tuyo para usar en combate.\n\n");
		sleep(2);
		printf("Cómo ganar:\n\n");
		sleep(1);
		printf("Cada Pokemon tiene 3 ataques. Cada ataque tiene un tipo de ataque y una cantidad de poder.\n");
		printf("La idea del juego es que elijas un Pokemon y luego uno de los ataques que posea, que previamente NO HAYAS USADO.\n");
		printf("Al final, ganará el jugador que más puntos tenga.\n\n");
		sleep(2);
		printf("Un consejo: los ataques efectivos suman a tu puntaje 3 veces la cantidad de poder del ataque. Pero cuidado! Los ataques inefectivos suman solo la mitad.\n");
		printf("Los ataques de efectividad normal suman la misma cantidad de poder a tu puntaje.\n\n");
		sleep(3);
		printf("La relación de ataques es la siguiente:\n");
		printf("- Los ataques NORMAL no son efectivos ni inefectivos contra nada.\n");
  		printf("- Los ataques FUEGO son efectivos contra PLANTA e inefectivos contra AGUA.\n");
  		printf("- Los ataques PLANTA son efectivos contra ROCA e inefectivos contra FUEGO.\n");
  		printf("- Los ataques ROCA son efectivos contra ELECTRICO e inefectivos contra PLANTA.\n");
  		printf("- Los ataques ELECTRICO son efectivos contra AGUA e inefectivos contra ROCA.\n");
  		printf("- Los ataques AGUA son efectivos contra FUEGO e inefectivos contra ELECTRICO.\n\n");

		sleep(5);
}

void parsear_ataque(enum TIPO tipo, char* t){
	switch (tipo)
	{
	case NORMAL:
		strcpy(t, "Normal");
		break;
	case FUEGO:
		strcpy(t, "Fuego");
		break;
	case AGUA:
		strcpy(t, "Agua");
		break;
	case PLANTA:
		strcpy(t, "Planta");
		break;
	case ELECTRICO:
		strcpy(t, "Eléctrico");
		break;
	case ROCA:
		strcpy(t, "Roca");	
		break;
	default:
		break;
	}
}

void mostrar_ataques(const struct ataque* ataque, void* contexto){
	char* tipo_ataque[10];
	parsear_ataque(ataque->tipo, (char*)tipo_ataque);
	printf("Nombre: %s;  Tipo: %s;  Poder: %d\n", ataque->nombre, (char*)tipo_ataque, ataque->poder);
}

bool mostrar_pokemon(void* pokemon, void* contexto){
	pokemon_t* p = (pokemon_t*)pokemon;
	const char* nombre_poke = pokemon_nombre(p);
	enum TIPO tipo_poke = pokemon_tipo(p);
	
	char* tipo[10];
	parsear_ataque(tipo_poke, (char*)tipo);

	printf("Nombre: %s; ", nombre_poke);
	printf("Tipo: %s\n", (char*)tipo);
	printf("Ataques:\n");
	con_cada_ataque(pokemon, mostrar_ataques, NULL);
	printf("\n");

	return true;
}

bool comparar_nombres(void* pokemon, void* nombre1){
	pokemon_t* p = (pokemon_t*)pokemon;
	const char* n = (const char*)nombre1;

	const char* nombre_poke = pokemon_nombre(p);

	if(strcmp(n, nombre_poke) == 0){
		return false;
	}

	return true;
}


int main(){
	srand((unsigned)time(NULL));
	juego_t* juego = juego_crear();

	juego_cargar_pokemon(juego, "ejemplos/correcto.txt");

	lista_t* lista_pokemones;
	lista_pokemones = juego_listar_pokemon(juego);

	printf("Bienvenido al juego!! Ya sabés cómo jugar?\n");
	char respuesta[200];

	pedir_input(respuesta, sizeof(respuesta));

	while((strcmp(respuesta, "si") != 0) && (strcmp(respuesta, "no"))){
		printf("contesta si/no\n");
		pedir_input(respuesta, sizeof(respuesta));
	}

	if(strcmp(respuesta, "no") == 0){
		juego_dar_instrucciones();
	}

	printf("\nPresiona enter o envía cualquier cosa para continuar.\n");
	respuesta[0] = '\0';

	pedir_input(respuesta, sizeof(respuesta));

	printf("\nLa lista de pokemones se ha cargado con éxito. Por favor, elegí 3(tres) pokemones distintos, escribiendo su nombre, uno a uno.\n");
	printf("Lista de pokemones:\n\n");

	lista_con_cada_elemento(lista_pokemones, mostrar_pokemon, NULL);

	char nombre1[200];
	char nombre3[200];
	char nombre2[200];

	printf("Insertá el nombre, tal cual como aparece, del primer pokemon que querés elegir.\n");
	pedir_input((char*)nombre1, sizeof(nombre1));

	printf("Insertá el nombre, tal cual como aparece, del segundo pokemon que querés elegir.\n");
	pedir_input((char*)nombre2, sizeof(nombre2));

	printf("Insertá el nombre, tal cual como aparece, del tercer pokemon del adversario.\n");
	pedir_input((char*)nombre3, sizeof(nombre3));

	JUGADOR jugador1 = JUGADOR1;
	//JUGADOR adversario = JUGADOR2;

	JUEGO_ESTADO estado = juego_seleccionar_pokemon(juego, jugador1, (const char*)nombre1, (const char*)nombre2, (const char*)nombre3);

	while(estado == POKEMON_INEXISTENTE || estado == POKEMON_REPETIDO){
		printf("\nAlguno de los pokemones que elegiste no está en la lista o elegiste 2 veces el mismo. Estás seguro que los escribiste bien? Volvé a insertarlos\n");
		
		printf("Insertá el nombre, tal cual como aparece, del primer pokemon que querés elegir.\n");
		pedir_input((char*)nombre1, sizeof(nombre1));

		printf("Insertá el nombre, tal cual como aparece, del segundo pokemon que querés elegir.\n");
		pedir_input((char*)nombre2, sizeof(nombre2));

		printf("Insertá el nombre, tal cual como aparece, del tercer pokemon del adversario.\n");
		pedir_input((char*)nombre3, sizeof(nombre3));
		
		
		estado = juego_seleccionar_pokemon(juego, jugador1, (const char*)nombre1, (const char*)nombre2, (const char*)nombre3);
	}


	//char nombre1_adversario[200];
	//char nombre2_adversario[200];
	//char nombre3_adversario[200];

	adversario_t* adversario = adversario_crear(lista_pokemones);
	adversario_destruir(adversario);
	juego_destruir(juego);

	return 0;
}