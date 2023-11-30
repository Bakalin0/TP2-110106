#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>

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

int contar_delimitadores(const char* linea){
	int delimitadores = 0;
	
	for(int i = 0; linea[i] != '\0'; i++){
		if(linea[i] == ';'){
			delimitadores++;
		}
	}
	return delimitadores;
}

enum TIPO asignar_tipo_pokemon(char pokemon_tipo) {
    switch(pokemon_tipo) {
        case 'N':
            return NORMAL;
        case 'F':
            return FUEGO;
        case 'A':
            return AGUA;
        case 'P':
            return PLANTA;
        case 'E':
            return ELECTRICO;
        case 'R':
            return ROCA;
        default:
            return -1;
    }
}

void ordenar_pokemones(informacion_pokemon_t *info_pokemon){
	int cant_pokes = info_pokemon->cantidad_pokemones;
	pokemon_t** pokemones = info_pokemon->pokemones;

	for (int i = 0; i < cant_pokes-1; i++){
		for(int j = 0; j< cant_pokes-i-1; j++){
			if(strcmp(pokemones[j]->nombre, pokemones[j+1]->nombre) > 0){
				pokemon_t* temp = pokemones[j];
				pokemones[j] = pokemones[j+1];
				pokemones[j+1] = temp;
			}
		}
	}
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if(path == NULL){
		return NULL;
	}

	FILE* archivo = fopen(path, "r");
	if(archivo == NULL) {
		return NULL;
	}
	informacion_pokemon_t *info_pokemon = malloc(sizeof(informacion_pokemon_t));
	if (info_pokemon == NULL){
		fclose(archivo);
		return NULL;
	}
	info_pokemon->pokemones = NULL;
	info_pokemon->cantidad_pokemones = 0;


	pokemon_t* pokemon = calloc(1, sizeof(pokemon_t));
	if(pokemon == NULL){
		free(info_pokemon);
		fclose(archivo);
		return NULL;
	}

	char linea[100];
	int hay_pokemon = 0;
	char pokemon_tipo;

	while(fgets(linea, sizeof(linea), archivo) != NULL){
		int delimitadores = contar_delimitadores(linea);

		if(delimitadores == 1 && hay_pokemon == 0){
			if(sscanf(linea, "%[^;];%c", pokemon->nombre, &pokemon_tipo) != 2){
				free(pokemon);
				fclose(archivo);
				return info_pokemon;
			}
			pokemon->tipo = asignar_tipo_pokemon(pokemon_tipo);
			if(pokemon->tipo == -1){
				free(pokemon);
				fclose(archivo);
				return info_pokemon;
			}

			hay_pokemon = 1;
		} else if (delimitadores == 1 && hay_pokemon == 1){
			free(pokemon);
			fclose(archivo);
			return info_pokemon;
		}

		if(delimitadores == 2){	
			pokemon->ataques[pokemon->cant_ataques] = malloc(sizeof(struct ataque));
			
            if (pokemon->ataques[pokemon->cant_ataques] == NULL) {
                free(pokemon);
                fclose(archivo);
                return info_pokemon;
            }

			if(sscanf(linea, "%[^;];%c;%u", pokemon->ataques[pokemon->cant_ataques]->nombre, &pokemon_tipo, &pokemon->ataques[pokemon->cant_ataques]->poder) != 3){
				free(pokemon->ataques[pokemon->cant_ataques]);
				free(pokemon);
				fclose(archivo);
				return info_pokemon;
			}

			pokemon->ataques[pokemon->cant_ataques]->tipo = asignar_tipo_pokemon(pokemon_tipo);
			if(pokemon->ataques[pokemon->cant_ataques]->tipo == -1){
				free(pokemon->ataques[pokemon->cant_ataques]);
				free(pokemon);
				fclose(archivo);
				return info_pokemon;
			}

			pokemon->cant_ataques++;
		}

		if(pokemon->cant_ataques == 3){
			pokemon_t** aux_pokemones = realloc(info_pokemon->pokemones, (long unsigned int)(info_pokemon->cantidad_pokemones + 1) * sizeof(pokemon_t*));
			hay_pokemon = 0;
			
			if(aux_pokemones == NULL){
				free(pokemon->ataques[pokemon->cant_ataques]);
    			free(pokemon);
				fclose(archivo);
				return info_pokemon;
			}

			info_pokemon->pokemones = aux_pokemones;
			info_pokemon->pokemones[info_pokemon->cantidad_pokemones] = pokemon;

			info_pokemon->cantidad_pokemones++;

			pokemon = calloc(1, sizeof(pokemon_t));
			if(pokemon == NULL){
				fclose(archivo);
				return info_pokemon;
			}
			pokemon->cant_ataques = 0;
		}
	}

	fclose(archivo);
	free(pokemon);
	return info_pokemon;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (ip == NULL){
		return 0;
	}

	int señalador = 0;
	int encontrado = 0;

	for(int i = 0; i < ip->cantidad_pokemones; i++){
		if(strcmp(ip->pokemones[i]->nombre, nombre) == 0){
			señalador = i;
			encontrado = 1;
		}
	}

	if(encontrado == 1){
		return ip->pokemones[señalador];
	}

	return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL){
		return 0;
	}

	return ip->cantidad_pokemones;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if(pokemon == NULL){
		return 0;
	}

	return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if(pokemon == NULL){
		return 0;
	}

	return pokemon->tipo;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon, const char *nombre)
{
	if(!pokemon || !nombre){
		return NULL;
	}

	for(int i = 0; i < pokemon->cant_ataques; i++){
		if(strcmp(pokemon->ataques[i]->nombre, nombre) == 0){
			return pokemon->ataques[i];
		}
	}

	return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *), void *aux)
{
	if(ip == NULL || f == NULL){
		return 0;
	}

	ordenar_pokemones(ip);

	int cantidad_pokemon = ip->cantidad_pokemones;
	pokemon_t** pokemones = ip->pokemones;

	for(int i = 0; i < cantidad_pokemon; i++){
		f(pokemones[i], aux);
	}

	return cantidad_pokemon;
}

int con_cada_ataque(pokemon_t *pokemon, void (*f)(const struct ataque *, void *), void *aux)
{
	if(pokemon == NULL || f == NULL){
		return 0;
	}

	int cant_ataques = pokemon->cant_ataques;

	for(int i = 0; i < cant_ataques; i++){
		f(pokemon->ataques[i], aux);
	}

	return cant_ataques;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip == NULL){
		return;
	}

	int cant_pokemones = ip->cantidad_pokemones;
	
	for (int i = 0; i < cant_pokemones; i++){
		pokemon_t* pokemon = ip->pokemones[i];
		int cant_ataques = pokemon->cant_ataques;

		for (int j = 0; j < cant_ataques; j++){
			free(pokemon->ataques[j]);
		}

		free(pokemon);
	}

	free(ip->pokemones);
	free(ip);
}