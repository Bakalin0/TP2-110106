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
}

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

int main(){
	srand((unsigned)time(NULL));
	juego_t* juego = juego_crear();

	juego_cargar_pokemon(juego, "ejemplos/correcto.txt");

	printf("");

	printf("La lista de pokemones se ha cargado con éxito. Por favor, elegí 3(tres) pokemones distintos, escribiendo su nombre, uno a uno.\n");
	
	

}