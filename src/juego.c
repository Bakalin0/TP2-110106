#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "estructura_pokemones.h"


typedef struct jugador{
	lista_t* lista_pokemones;
	int puntaje;
}jugador_t;

struct juego{
	jugador_t* jugador1;
	jugador_t* jugador2;

	informacion_pokemon_t *info;
	lista_t* pokemones_totales;
	int turno;
};

JUEGO_ESTADO agregar_pokemones(juego_t *juego, const char *nombre1, const char *nombre2, const char* nombre3, jugador_t* jugador_seleccionado, jugador_t* otro_jugador){
	int cantidad = juego->info->cantidad_pokemones;
	int contador = 0;
	pokemon_t* pokemon1 = NULL;
	pokemon_t* pokemon2 = NULL;
	pokemon_t* pokemon3 = NULL;

	for(int i = 0; i < cantidad; i++){
		char *nombre_pokemon = juego->info->pokemones[i]->nombre;

		if(nombre1 == nombre_pokemon){
			contador++;
			pokemon1 = juego->info->pokemones[i];
		}

		if(nombre2 == nombre_pokemon){
			contador++;
			pokemon2 = juego->info->pokemones[i];
		}

		if(nombre3 == nombre_pokemon){
			contador++;
			pokemon3 = juego->info->pokemones[i];
		}
	}

	if(contador < 3){
		return POKEMON_INEXISTENTE;
	}
	
	jugador_seleccionado = lista_insertar(jugador_seleccionado->lista_pokemones, pokemon1);
	jugador_seleccionado = lista_insertar(jugador_seleccionado->lista_pokemones, pokemon2);
	otro_jugador = lista_insertar(otro_jugador->lista_pokemones, pokemon3);

	return TODO_OK;
}

bool ataque_caso_efectivo(enum TIPO tipo_ataque, enum TIPO tipo_pokemon){
	if(tipo_ataque == FUEGO && tipo_pokemon == PLANTA){
		return true;
	}
	else if(tipo_ataque == PLANTA && tipo_pokemon == ROCA){
		return true;
	}
	else if(tipo_ataque == ROCA && tipo_pokemon == ELECTRICO){
		return true;
	}
	else if(tipo_ataque == ELECTRICO && tipo_pokemon == AGUA){
		return true;
	}
	else if(tipo_ataque == AGUA && tipo_pokemon == FUEGO){
		return true;
	}
	return false;
}

bool ataque_caso_inefectivo(enum TIPO tipo_ataque, enum TIPO tipo_pokemon){
	if(tipo_ataque == PLANTA && tipo_pokemon == FUEGO){
		return true;
	}
	else if(tipo_ataque == ROCA && tipo_pokemon == PLANTA){
		return true;
	}
	else if(tipo_ataque == ELECTRICO && tipo_pokemon == ROCA){
		return true;
	}
	else if(tipo_ataque == AGUA && tipo_pokemon == ELECTRICO){
		return true;
	}
	else if(tipo_ataque == FUEGO && tipo_pokemon == AGUA){
		return true;
	}
	return false;
}

RESULTADO_ATAQUE calcular_efectiviad(enum TIPO tipo_ataque, enum TIPO tipo_pokemon){
	if(ataque_caso_efectivo(tipo_ataque, tipo_pokemon)){
		return ATAQUE_EFECTIVO;
	}

	else if(ataque_caso_inefectivo(tipo_ataque, tipo_pokemon)){
		return ATAQUE_INEFECTIVO;
	}

	else{
		return NORMAL;
	}
}

int asignar_poder_de_ataque(struct ataque* ataque_seleccionado, pokemon_t* pokemon_rival){
	if(!ataque_seleccionado){
		return 0;
	}
	
	RESULTADO_ATAQUE efectividad = calcular_efectiviad(ataque_seleccionado->tipo, pokemon_rival->tipo);

	if(efectividad == ATAQUE_EFECTIVO){
		return (ataque_seleccionado->poder * 3);
	}

	else if(efectividad == ATAQUE_INEFECTIVO){
		return (ataque_seleccionado->poder / 2);
	}

	return ataque_seleccionado->poder;

}

juego_t *juego_crear()
{
	juego_t* juego = calloc(1, sizeof(juego_t));

	juego->info->cantidad_pokemones = 0;

	return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	juego->info = pokemon_cargar_archivo(archivo);
	if (juego->info == NULL){
		return ERROR_GENERAL;
	}
	
	if(juego->info->cantidad_pokemones < 6){
		return POKEMON_INSUFICIENTES;
	}
	

	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	juego->pokemones_totales = lista_crear();

	printf("%i", juego->info->cantidad_pokemones);

	for(int i = 0; i < juego->info->cantidad_pokemones; i++){
		pokemon_t* pokemon = juego->info->pokemones[i];
		lista_insertar(juego->pokemones_totales, pokemon);
	}

	if(!juego->pokemones_totales){
		return NULL;
	}

	printf("%i\n\n",(int)lista_tamanio(juego->pokemones_totales));
	return juego->pokemones_totales;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if(!juego || !nombre1 || !nombre2 || !nombre3 || !jugador){
		return ERROR_GENERAL;
	}

	if(nombre1 == nombre2 || nombre1 == nombre3 || nombre2 == nombre3){
		return POKEMON_REPETIDO;
	}
	
	if(jugador == JUGADOR1){
		if(agregar_pokemones(juego, nombre1, nombre2, nombre3, juego->jugador1, juego->jugador2) == POKEMON_INEXISTENTE){
			return POKEMON_INEXISTENTE;
		}
	}

	else if(jugador == JUGADOR2){
		if(agregar_pokemones(juego, nombre1, nombre2, nombre3, juego->jugador2, juego->jugador1) == POKEMON_INEXISTENTE){
			return POKEMON_INEXISTENTE;
		}
	}

	return TODO_OK;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;
	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if(!juego || !jugador){
		return 0;
	}

	int puntaje = 0;

	if(jugador == JUGADOR1){
		puntaje = jugador_calcular_puntaje(juego->jugador1);
	}

	else if(jugador == JUGADOR2){
		puntaje = jugador_calcular_puntaje(juego->jugador2);
	}

	//para calcular puntaje puedo actualizar el valor del poder de cada pokemon (usado) y luego los sumo?? 
	//como se si ya esta usado?

}

bool juego_finalizado(juego_t *juego)
{
	if(juego->turno == 9){
		return true;
	}

	return false;
}

void juego_destruir(juego_t *juego)
{
	juego->jugador1->lista_pokemones = lista_destruir()	

	free(juego);
}
