#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include <stdlib.h>
#include <time.h>
#include "abb.h"
#include <string.h>
#include "ataque.h"

typedef struct estructura_pokemones{
	pokemon_t* pokemon;
	lista_t* ataques;
}estructura_pokemones_t;


struct adversario{
	lista_t* pokemones;
	estructura_pokemones_t* pokemones_seleccionados[3]; 
	abb_t* ataques_totales; 
};

int comparador1(void *_elemento1, void *_elemento2)
{
	if (!_elemento1 || !_elemento2) {
		return 0;
	}
	char *elemento1 = _elemento1;
	char *elemento2 = _elemento2;
	return strcmp(elemento1, elemento2);
}

void agregar_a_abb1(const struct ataque* ataque, void* arbol)
{
	abb_insertar(arbol, (void*)ataque);
}

void agregar_a_lista1(const struct ataque* a, void* lista)
{
	lista_insertar(lista, (struct ataque*)a);
}

adversario_t *adversario_crear(lista_t *pokemon)
{
	adversario_t *adversario = calloc(1, sizeof(adversario_t));
	if(!adversario){
		return NULL;
	}

	free(adversario->ataques_totales);
	adversario->ataques_totales = abb_crear(comparador1);
	if(!adversario->ataques_totales){
		free(adversario);
		return NULL;
	}
	
	for(int i = 0; i < 3; i++){
		adversario->pokemones_seleccionados[i] = calloc(1, sizeof(adversario->pokemones_seleccionados));
		adversario->pokemones_seleccionados[i]->ataques = lista_crear();
	}

	adversario->pokemones = pokemon;

	return adversario;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	if(!adversario || !nombre1 || !nombre2 || !nombre3){
		return false;
	}

	int cantidad = (int)lista_tamanio(adversario->pokemones);
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

	adversario->pokemones_seleccionados[0]->pokemon = pokemon1;
	adversario->pokemones_seleccionados[1]->pokemon = pokemon2;

	con_cada_ataque(pokemon1, agregar_a_lista1, adversario->pokemones_seleccionados[0]->ataques);
	con_cada_ataque(pokemon1, agregar_a_abb1, adversario->ataques_totales);

	con_cada_ataque(pokemon2, agregar_a_lista1, adversario->pokemones_seleccionados[1]->ataques);
	con_cada_ataque(pokemon2, agregar_a_abb1, adversario->ataques_totales);

	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{	
	if(!adversario || !nombre1 || !nombre2 || !nombre3){
		return false;
	}

	int cantidad = (int)lista_tamanio(adversario->pokemones);
	int contador = 0;
	pokemon_t* pokemon3;

	for(int i = 0; i < cantidad; i++){
		pokemon_t *pokemon = lista_elemento_en_posicion(adversario->pokemones, (size_t)i);
		const char *nombre_pokemon = pokemon_nombre(pokemon);
		
		if(strcmp(nombre3, nombre_pokemon) == 0){
			contador++;
			pokemon3 = pokemon;
		}
	}

	if(contador < 1){
		return false;
	}
	

	adversario->pokemones_seleccionados[2]->pokemon = pokemon3;
	con_cada_ataque(pokemon3, agregar_a_abb1, adversario->ataques_totales);
	con_cada_ataque(pokemon3, agregar_a_lista1, adversario->pokemones_seleccionados[2]->ataques);

	return true;
}


jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };

	int i, k = 0;

	i = rand() % 3;
	k = rand() % 3;

	pokemon_t* pokemon_elegido = adversario->pokemones_seleccionados[i]->pokemon;

	struct ataque* ataque_elegido = lista_elemento_en_posicion(adversario->pokemones_seleccionados[i]->ataques, (size_t)k);

	while(!abb_quitar(adversario->ataques_totales, ataque_elegido)){
		i = rand() % 3;
		k = rand() % 3;

		pokemon_elegido = adversario->pokemones_seleccionados[i]->pokemon;
		ataque_elegido = lista_elemento_en_posicion(adversario->pokemones_seleccionados[i]->ataques, (size_t)k);
	}

	const char* nombre_poke_elegido = pokemon_nombre(pokemon_elegido);

	strcpy(j.ataque, ataque_elegido->nombre);
	strcpy(j.pokemon, nombre_poke_elegido);

	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
	abb_destruir(adversario->ataques_totales);

	for(int i = 0; i < 3; i++){
		lista_destruir(adversario->pokemones_seleccionados[i]->ataques);
		free(adversario->pokemones_seleccionados[i]);
	}

	free(adversario);
}
