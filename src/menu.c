#include "menu.h"
#include "lista.h"
#include <stdlib.h>

struct menu{
	lista_t* comandos;
};

struct informacion_comando{
	char* descripcion;
	bool (*funcion)(void*);
	char* comando;
};

menu_t* menu_crear(){
	menu_t* menu = calloc(1, sizeof(menu_t));
	menu->comandos = lista_crear();

	return menu;
}


void menu_agregar_comando(menu_t* menu, char* comando, char* descripcion, bool(*f)(void*)){
	struct informacion_comando* info = malloc(sizeof(struct informacion_comando));

	info->descripcion = descripcion;
	info->funcion = f;
	info->comando = comando;

	lista_insertar(menu->comandos, info);
}


MENU_RESULTADO menu_ejecutar_comando(menu_t* menu, void* contexto, int (*comparador)(void*, void*)){
	struct informacion_comando* info = lista_buscar_elemento(menu->comandos, comparador, contexto);

	if(!info){
		return MENU_INEXISTENTE;
	}

	if(info->funcion(contexto)){
		return MENU_OK;
	}

	return MENU_ERROR;
}


/*void mostrar_comando(struct informacion_comando* info, void* contexto){
	printf("%s: %s\n", info->comando, info->descripcion);
}*/

size_t menu_con_cada_comando(menu_t* menu, bool(*f)(void*, void*), void* contexto){
	return lista_con_cada_elemento((lista_t*)menu, mostrar_comando, contexto);
}

void menu_destruir(menu_t* menu){
	free(menu);
}