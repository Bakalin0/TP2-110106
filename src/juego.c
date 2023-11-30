#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MAX_TURNOS 9


typedef struct jugador{
	lista_t* lista_pokemones;
	int puntaje;
}jugador_t;

typedef struct turno{
	jugada_t* jugada_1;
	jugada_t* jugada_2;
}turno_t;

struct juego{
	jugador_t jugador1;
	jugador_t jugador2;

	informacion_pokemon_t *info;
	lista_t* pokemones_totales;

	turno_t* turno[MAX_TURNOS];
	int numero_turno;
};

JUEGO_ESTADO agregar_pokemones(juego_t *juego, const char *nombre1, const char *nombre2, const char* nombre3, jugador_t jugador_seleccionado, jugador_t otro_jugador){
	int cantidad = pokemon_cantidad(juego->info);
	int contador = 0;
	pokemon_t* pokemon1;
	pokemon_t* pokemon2;
	pokemon_t* pokemon3;

	for(int i = 0; i < cantidad; i++){
		pokemon_t *pokemon = lista_elemento_en_posicion(juego->pokemones_totales, (size_t)i);
		const char *nombre_pokemon = pokemon_nombre(pokemon);

		if(nombre1 == nombre_pokemon){
			contador++;
			pokemon1 = pokemon;
		}

		if(nombre2 == nombre_pokemon){
			contador++;
			pokemon2 = pokemon;
		}

		if(nombre3 == nombre_pokemon){
			contador++;
			pokemon3 = pokemon;
		}
	}

	if(contador < 3){
		return POKEMON_INEXISTENTE;
	}

	lista_t* lista_jug_s = jugador_seleccionado.lista_pokemones;
	lista_t* lista_jug_o = otro_jugador.lista_pokemones;
	
	lista_insertar(lista_jug_s, pokemon1);
	lista_insertar(lista_jug_s, pokemon2);
	lista_insertar(lista_jug_o, pokemon3);

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

	else if(tipo_ataque == NORMAL){
		return NORMAL;
	}

	return ATAQUE_ERROR;
}

int asignar_poder_de_ataque(struct ataque* ataque_seleccionado, pokemon_t* pokemon_rival){
	if(!ataque_seleccionado || !pokemon_rival){
		return 0;
	}
	
	RESULTADO_ATAQUE efectividad = calcular_efectiviad(ataque_seleccionado->tipo, pokemon_tipo(pokemon_rival));
	if(efectividad == ATAQUE_ERROR){
		return 0;
	}

	if(efectividad == ATAQUE_EFECTIVO){
		return ((int)ataque_seleccionado->poder * 3);
	}

	else if(efectividad == ATAQUE_INEFECTIVO){
		return ((int)ataque_seleccionado->poder / 2);
	}

	return (int)ataque_seleccionado->poder;

}

/*JUEGO_ESTADO usar_ataque(pokemon_t* pokemon_elegido, struct ataque* ataque_seleccionado){
	if(!pokemon_elegido || !ataque_seleccionado){
		return ERROR_GENERAL;
	}
	bool sacado = false;
	for(int i = 0; i < pokemon_elegido->cantidad_ataques; i++){
		if(strcmp(pokemon_elegido->ataques[i].nombre, ataque_seleccionado->nombre) == 0){
			free(&pokemon_elegido->ataques[i]);
			pokemon_elegido->ataques[i] = NULL;
			sacado = true;
		}
	}

	if(!sacado){
		return ERROR_GENERAL;
	}

	return TODO_OK;
}*/

juego_t *juego_crear()
{
	juego_t* juego = calloc(1, sizeof(juego_t));
	if(!juego){
		return NULL;
	}

	juego->pokemones_totales = lista_crear();
	if(!juego->pokemones_totales){
		free(juego);
		return NULL;
	}

	return juego;
}

void agregar_a_lista(pokemon_t* p, void* lista)
{
	lista_insertar(lista, p);
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	juego->info = pokemon_cargar_archivo(archivo);
	if (juego->info == NULL){
		return ERROR_GENERAL;
	}
	
	if(pokemon_cantidad(juego->info) < 6){
		pokemon_destruir_todo(juego->info);
		return POKEMON_INSUFICIENTES;
	}

	con_cada_pokemon(juego->info, agregar_a_lista, juego->pokemones_totales);	

	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if(!juego)
		return NULL;
	return juego->pokemones_totales;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if(!juego || !nombre1 || !nombre2 || !nombre3 || !jugador){
		return ERROR_GENERAL;
	}

	if(strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 || strcmp(nombre2, nombre3)){
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

int comparador(void *_elemento1, void *_elemento2)
{
	if (!_elemento1 && !_elemento2) {
		return 0;
	}
	int *elemento1 = _elemento1;
	int *elemento2 = _elemento2;
	return *elemento1 - *elemento2;
}

pokemon_t* hallar_pokemon(char* nombre_poke, jugador_t* jugador){
	if(!nombre_poke || !jugador){
		return NULL;
	}

	return lista_buscar_elemento(jugador->lista_pokemones, comparador, nombre_poke);
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;

	pokemon_t* poke_jug_1 = pokemon_buscar(juego->info, jugada_jugador1.pokemon);
	struct ataque* ataque_jug_1 = (struct ataque*)pokemon_buscar_ataque(poke_jug_1, jugada_jugador1.ataque);

	pokemon_t* poke_jug_2 = pokemon_buscar(juego->info, jugada_jugador2.pokemon);
	struct ataque* ataque_jug_2 = (struct ataque*)pokemon_buscar_ataque(poke_jug_2, jugada_jugador2.ataque);
	
	if(!poke_jug_1 || !ataque_jug_1){
		resultado.jugador1 = ATAQUE_ERROR;
	}
	else{
		resultado.jugador1 = calcular_efectiviad(ataque_jug_1->tipo, pokemon_tipo(poke_jug_2));
		juego->jugador1.puntaje += asignar_poder_de_ataque(ataque_jug_1, poke_jug_2);
	}

	if(!ataque_jug_2 || !poke_jug_2){
		resultado.jugador2 = ATAQUE_ERROR;
	}
	else{
		resultado.jugador2 = calcular_efectiviad(ataque_jug_2->tipo, pokemon_tipo(poke_jug_1));
		juego->jugador1.puntaje += asignar_poder_de_ataque(ataque_jug_2, poke_jug_1); //Esto está bien? o va en obtener puntaje? Funciona igual pero depende de las pruebas. Si devuelve resultado tendría sentido
	}
	
	return resultado;//AGREGAR CONTADOR TURNOS
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if(!juego){
		return 0;
	}

	if(jugador == JUGADOR1){
		return juego->jugador1.puntaje;
	}

	else if(jugador == JUGADOR2){
		return juego->jugador2.puntaje;
	}

	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	if(juego->numero_turno == 9){
		return true;
	}

	return false;
}

void juego_destruir(juego_t *juego)
{
	lista_destruir(juego->jugador1.lista_pokemones);
	free(&juego->jugador1);
	lista_destruir(juego->jugador2.lista_pokemones);
	free(&juego->jugador2);

	lista_destruir(juego->pokemones_totales);

	pokemon_destruir_todo(juego->info);

	free(juego);
}
