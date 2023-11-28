#ifndef MENU_H_
#define MENU_H_
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

typedef struct menu menu_t;

struct informacion_comando;

typedef enum {MENU_OK, MENU_INEXISTENTE, MENU_ERROR} MENU_RESULTADO;

menu_t* menu_crear();

void menu_agregar_comando(menu_t* menu, char* comando, char* descripcion, bool(*f)(void*));

MENU_RESULTADO menu_ejecutar_comando(menu_t* menu, void* comando, int (*comparador)(void*, void*));

void mostrar_comando(struct informacion_comando* info);

size_t menu_con_cada_comando(menu_t* menu, bool(*f)(void*, void*), void* contexto);

void menu_destruir(menu_t* menu);


#endif // MENU_H_