#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include <stdlib.h>
#include <stddef.h>

struct juego{
	lista_t* jugador1;
	lista_t* jugador2;
};

juego_t *juego_crear()
{
	juego_t* juego;

	juego->jugador1 = calloc(1, sizeof(lista_t*));
	juego->jugador2 = calloc(1, sizeof(lista_t*));

	return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	return ERROR_GENERAL;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	return NULL;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
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
}
