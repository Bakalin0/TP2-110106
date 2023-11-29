#ifndef __ESTRUCTURA_POKEMONES_H_
#define __ESTRUCTURA_POKEMONES_H_

#include "pokemon.h"

#define MAX_NOMBRE 50
#define MAX_ATAQUES 5

struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo;
	struct ataque *ataques[MAX_ATAQUES];
	int cant_ataques;

};

struct info_pokemon {
	pokemon_t** pokemones;
	int cantidad_pokemones;
};

#endif