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


struct juego{
	lista_t* jugador1;
	lista_t* jugador2;

	informacion_pokemon_t *info;
};

JUEGO_ESTADO agregar_pokemones(juego_t *juego, const char *nombre1, const char *nombre2, const char* nombre3, lista_t* jugador_seleccionado, lista_t* otro_jugador){
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
	
	jugador_seleccionado = lista_insertar(jugador_seleccionado, pokemon1);
	jugador_seleccionado = lista_insertar(jugador_seleccionado, pokemon2);
	otro_jugador = lista_insertar(otro_jugador, pokemon3);

	return TODO_OK;
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
	/*lista_t* lista = lista_crear();

	printf("%i", juego->info->cantidad_pokemones);

	for(int i = 0; i < juego->info->cantidad_pokemones; i++){
		pokemon_t* pokemon = juego->info->pokemones[i];
		lista_insertar(lista, pokemon);
	}

	if(!lista){
		return NULL;
	}

	printf("%i\n\n",(int)lista_tamanio(lista));
	return lista;*/

	return NULL;
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
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	return true;
}

void juego_destruir(juego_t *juego)
{
	free(juego);
}
