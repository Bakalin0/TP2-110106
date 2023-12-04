#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "src/tipo.h"
#include <stdlib.h>


struct estado_juego{
	juego_t* juego;
	bool continuar;
};



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

	sleep(1);
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

jugada_t jugador_pedir_jugada(jugada_t jugada){
	printf("Insertá el nombre de un pokemon (que hayas elegido) que quieras usar para atacar.\n");
	pedir_input(jugada.pokemon, sizeof(jugada.pokemon));

	printf("Ahora insertá el nombre de un ataque perteneciente a ese Pokemon. Acordate que debe ser uno que no hayas usado previamente!!\n");
	pedir_input(jugada.ataque, sizeof(jugada.ataque));

	return jugada;
}

char* traducir_jugada(RESULTADO_ATAQUE resultado, char* r_traducido){
	if(resultado == ATAQUE_EFECTIVO){
		r_traducido = "Ataque Efectivo";
	} 
	else if(resultado == ATAQUE_INEFECTIVO){
		r_traducido = "Ataque Inefectivo";
	}
	else{
		r_traducido = "Ataque Regular";
	}

	return r_traducido;
}

char* obtener_ganador(int puntaje_jugador, int puntaje_adversario){
	char* ganador;

	if(puntaje_jugador > puntaje_adversario){
		ganador = "Jugador";
	}
	else if(puntaje_jugador < puntaje_adversario){
		ganador = "Adversario";
	}

	return ganador;
}

int puntaje_ganador(int puntaje_jugador1, int puntaje_adversario){
	if(puntaje_jugador1 > puntaje_adversario){
		return puntaje_jugador1;
	}
	else{
		return puntaje_adversario;
	}
}

int main(){
	srand((unsigned)time(NULL));
	juego_t* juego = juego_crear();

	juego_cargar_pokemon(juego, "ejemplos/correcto.txt");

	lista_t* lista_pokemones;
	lista_pokemones = juego_listar_pokemon(juego);

	adversario_t* adversario = adversario_crear(lista_pokemones);

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

	printf("\nLa lista de pokemones se ha cargado con éxito. Por favor, elegí 3(tres) pokemones distintos de la siguiente lista, escribiendo su nombre, uno a uno.\n");
	sleep(2);
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

	JUEGO_ESTADO estado = juego_seleccionar_pokemon(juego, JUGADOR1, (const char*)nombre1, (const char*)nombre2, (const char*)nombre3);

	while(estado == POKEMON_INEXISTENTE || estado == POKEMON_REPETIDO){
		printf("\nAlguno de los pokemones que elegiste no está en la lista o elegiste 2 veces el mismo. Estás seguro que los escribiste bien? Volvé a insertarlos\n");
		
		printf("Insertá el nombre, tal cual como aparece, del primer pokemon que querés elegir.\n");
		pedir_input((char*)nombre1, sizeof(nombre1));

		printf("Insertá el nombre, tal cual como aparece, del segundo pokemon que querés elegir.\n");
		pedir_input((char*)nombre2, sizeof(nombre2));

		printf("Insertá el nombre, tal cual como aparece, del tercer pokemon del adversario.\n");
		pedir_input((char*)nombre3, sizeof(nombre3));
		
		
		estado = juego_seleccionar_pokemon(juego, JUGADOR1, (const char*)nombre1, (const char*)nombre2, (const char*)nombre3);
	}

	adversario_pokemon_seleccionado(adversario, (char*)nombre1, (char*)nombre2, (char*)nombre3);

	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
	
	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1, &eleccionAdversario2, &eleccionAdversario3);

	juego_seleccionar_pokemon(juego, JUGADOR2, eleccionAdversario1, eleccionAdversario2, eleccionAdversario3);
	
	printf("Pokemones elegidos!\n");
	sleep(2);

	printf("\nEl adversario ha elegido los pokemones: %s, %s y %s.\n", eleccionAdversario1, eleccionAdversario2, eleccionAdversario3);
	sleep(2);
	printf("Estás listo? El juego está por comenzar!\n\n");
	sleep(2);
	printf("3\n");
	sleep(1);
	printf("2\n");
	sleep(1);
	printf("1\n");
	sleep(2);

	printf("El juego está comenzando. A continuación, verás la lista de pokemones para saber cuáles pokemones hay en el juego.\n");
	sleep(3);

	int puntaje_jug1 = 0;
	int puntaje_jug2 = 0;
	int turno = 1;

	while (!juego_finalizado(juego)) {
		printf("Comenzando ronda %i/9...\n\n", turno);
		sleep(1);
		printf("Lista de pokemones:\n\n");

		lista_con_cada_elemento(lista_pokemones, mostrar_pokemon, NULL);

		resultado_jugada_t resultado_ronda;

		printf("Tus pokemones:\n%s, %s, %s\n\n", nombre1, nombre2, eleccionAdversario3);
		printf("Pokemones del adversario:\n%s, %s, %s\n\n", eleccionAdversario1, eleccionAdversario2, nombre3);
		jugada_t jugada_jugador = jugador_pedir_jugada(jugada_jugador);

		jugada_t jugada_adversario = adversario_proxima_jugada(adversario);
		
		resultado_ronda = juego_jugar_turno(juego, jugada_jugador, jugada_adversario);

		while(resultado_ronda.jugador1 == ATAQUE_ERROR || resultado_ronda.jugador2 == ATAQUE_ERROR){
			printf("Error! Insertaste un Pokemon inexistente, un ataque inexistente, o un ataque repetido. Volvelos a insertar!\n");

			jugada_jugador = jugador_pedir_jugada(jugada_jugador);

			resultado_ronda = juego_jugar_turno(juego, jugada_jugador, jugada_adversario);
		}

		char* resultado_ataque_jugador = traducir_jugada(resultado_ronda.jugador1, resultado_ataque_jugador);
		char* resultado_ataque_adversario = traducir_jugada(resultado_ronda.jugador2, resultado_ataque_adversario);

		printf("Has elegido a %s con su ataque %s!!\n", jugada_jugador.pokemon, jugada_jugador.ataque);
		sleep(1);
		printf("El adversario ha elegido a %s con su ataque %s!! Ahora se baten en batalla.\n", jugada_adversario.pokemon, jugada_adversario.ataque);
		

		sleep(1);
		printf("\nHas hecho un %s mientras que el adversario ha hecho un %s!!!\n\n", resultado_ataque_jugador, resultado_ataque_adversario);
		sleep(2);

		puntaje_jug1 = juego_obtener_puntaje(juego, JUGADOR1);
		puntaje_jug2 = juego_obtener_puntaje(juego, JUGADOR2);

		printf("Puntajes:\n");
		printf("Jugador: %i;  Adversario: %i\n", puntaje_jug1, puntaje_jug2);
		sleep(1);
		printf("Fin de la ronda %i/9.\n", turno);
		turno++;
		sleep(2);
		printf("Envía un input para continuar.\n");
		pedir_input(respuesta, sizeof(respuesta));
	}
	sleep(1);
	char* ganador = obtener_ganador(puntaje_jug1, puntaje_jug2);

	if(puntaje_jug1 == puntaje_jug2){
		printf("Increible!! Terminó el juego y el resultado fue un EMPATE técnico. Ambos jugadores empataron con %i puntos!!\n", puntaje_jug1);
	}
	else{
		printf("El juego ha terminado! El ganador fue %s con un puntaje de %i. Felicitaciones al ganador y gracias por jugar!!\n", ganador, puntaje_ganador(puntaje_jug1, puntaje_jug2));
	}
	sleep(2);
	adversario_destruir(adversario);
	juego_destruir(juego);

	return 0;
}