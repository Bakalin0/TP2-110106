#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include <stdlib.h>
#include <time.h>
#include "abb.h"

struct adversario{
	lista_t* pokemones;
	lista_t* pokemones_seleccionados; // le cargo los pokemones que tiene el adversario?
	abb_t* ataques; // le cargo los ataques de la lista de pokemones seleccionados ^ ?
};

int comparador(void *_elemento1, void *_elemento2)
{
	if (!_elemento1 || !_elemento2) {
		return 0;
	}
	char *elemento1 = _elemento1;
	char *elemento2 = _elemento2;
	return strcmp(elemento1, elemento2);
}

adversario_t *adversario_crear(lista_t *pokemon)
{
	adversario_t *adversario = calloc(1, sizeof(adversario_t));
	if(!adversario){
		return NULL;
	}

	adversario->pokemones = lista_crear();
	adversario->ataques = abb_crear(comparador);
	adversario->pokemones_seleccionados = lista_crear();

	adversario->pokemones = pokemon;

	return adversario;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	if(!adversario, !nombre1, !nombre2, !nombre3){
		return false;
	}

	int cantidad = lista_tamanio(adversario->pokemones);
	int i = rand() % cantidad;
	int j = rand() % cantidad;
	int k = rand() % cantidad;

	while(j == i){
		j = rand() % cantidad;
	}
	while(k == j || k == i){
		k = rand() % cantidad;
	}

	pokemon_t* pokemon1;
	pokemon_t* pokemon2;
	pokemon_t* pokemon3;

	pokemon1 = lista_elemento_en_posicion(adversario->pokemones, (size_t)i);
	if(!pokemon1){
		return false;
	}
	pokemon2 = lista_elemento_en_posicion(adversario->pokemones, (size_t)j);
	if(!pokemon2){
		return false;
	}
	pokemon3 = lista_elemento_en_posicion(adversario->pokemones, (size_t)k);
	if(!pokemon3){
		return false;
	}

	*nombre1 = (char*)pokemon_nombre(pokemon1);
	*nombre2 = (char*)pokemon_nombre(pokemon2);
	*nombre3 = (char*)pokemon_nombre(pokemon3);

	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	
	
	return false;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };
	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
	lista_destruir(adversario->pokemones);

	abb_destruir(adversario->ataques);

	lista_destruir(adversario->pokemones_seleccionados);

	free(adversario);
}
