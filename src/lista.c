#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_final;
	int cantidad;
};

struct lista_iterador {
	nodo_t *nodo_actual;
	lista_t *lista;
};

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista) {
		return NULL;
	}

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) {
		lista_destruir(lista);
		return NULL;
	}
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = NULL;

	if (lista->cantidad == 0) {
		lista->nodo_inicio = nuevo_nodo;
		lista->nodo_final = nuevo_nodo;
	} else {
		lista->nodo_final->siguiente = nuevo_nodo;
		lista->nodo_final = nuevo_nodo;
	}

	lista->cantidad++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return NULL;
	}

	if (posicion >= lista->cantidad) {
		return lista_insertar(lista, elemento);
	}

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) {
		lista_destruir(lista);
		return NULL;
	}
	nuevo_nodo->elemento = elemento;

	if (posicion == 0) {
		nuevo_nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo_nodo;

		if (!lista->nodo_inicio->siguiente) {
			lista->nodo_final = nuevo_nodo;
		}
	}

	else if (posicion < lista->cantidad) {
		nodo_t *nodo_actual = lista->nodo_inicio;
		nodo_t *nodo_anterior = NULL;

		for (size_t i = 0; i < posicion; i++) {
			nodo_anterior = nodo_actual;
			nodo_actual = nodo_actual->siguiente;
		}

		nuevo_nodo->siguiente = nodo_actual;
		nodo_anterior->siguiente = nuevo_nodo;
	}

	lista->cantidad++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista->cantidad == 0) {
		return NULL;
	}

	if (lista->cantidad == 1) {
		void *elemento = lista->nodo_inicio->elemento;

		free(lista->nodo_final);

		lista->nodo_final = NULL;
		lista->nodo_inicio = NULL;

		lista->cantidad--;

		return elemento;
	}
	nodo_t *nodo_actual = lista->nodo_inicio;

	for (int i = 0; i < lista->cantidad - 2; i++) {
		nodo_actual = nodo_actual->siguiente;
	}

	void *elemento = lista->nodo_final->elemento;
	free(lista->nodo_final);

	lista->nodo_final = nodo_actual;
	lista->nodo_final->siguiente = NULL;
	lista->cantidad--;
	return elemento;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista->cantidad == 0) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *nodo_anterior = NULL;
	void *elemento = nodo_actual->elemento;

	if (posicion == 0) {
		if (lista->nodo_inicio->siguiente) {
			nodo_t *nodo_siguiente = lista->nodo_inicio->siguiente;
			free(nodo_actual);
			lista->nodo_inicio = nodo_siguiente;

		}

		else if (!lista->nodo_inicio->siguiente) {
			return lista_quitar(lista);
		}
	}

	else if (posicion < lista->cantidad) {
		for (size_t i = 0; i < posicion; i++) {
			nodo_anterior = nodo_actual;
			nodo_actual = nodo_actual->siguiente;
			elemento = nodo_actual->elemento;
		}

		nodo_anterior->siguiente = nodo_actual->siguiente;
		free(nodo_actual);

	}

	else if (posicion >= lista->cantidad) {
		return lista_quitar(lista);
	}

	lista->cantidad--;
	return elemento;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || posicion >= lista->cantidad) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	for (size_t i = 0; i < posicion; i++) {
		nodo_actual = nodo_actual->siguiente;
	}

	return nodo_actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || !comparador) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	while (nodo_actual) {
		if (comparador(nodo_actual->elemento, contexto) == 0) {
			return nodo_actual->elemento;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista->cantidad == 0) {
		return NULL;
	}

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista->cantidad == 0) {
		return NULL;
	}

	return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || lista->cantidad == 0) {
		return true;
	}
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista) {
		return 0;
	}
	return (size_t)lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (!lista) {
		return;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	for (int i = 0; i < lista->cantidad; i++) {
		nodo_t *nodo_siguiente = nodo_actual->siguiente;
		free(nodo_actual);
		nodo_actual = nodo_siguiente;
	}

	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista) {
		return;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	for (int i = 0; i < lista->cantidad; i++) {
		if (funcion) {
			funcion(nodo_actual->elemento);
		}
		nodo_actual = nodo_actual->siguiente;
	}

	lista_destruir(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador) {
		return NULL;
	}

	iterador->nodo_actual = lista->nodo_inicio;
	iterador->lista = lista;
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || !(iterador->nodo_actual)) {
		return false;
	}
	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || !(iterador->nodo_actual)) {
		return false;
	}
	iterador->nodo_actual = iterador->nodo_actual->siguiente;
	return iterador->nodo_actual != NULL;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !(iterador->nodo_actual)) {
		return NULL;
	}
	return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion) {
		return 0;
	}

	size_t elementos_recorridos = 0;
	nodo_t *nodo_actual = lista->nodo_inicio;
	while (nodo_actual) {
		elementos_recorridos++;

		if (!funcion(nodo_actual->elemento, contexto)) {
			return elementos_recorridos;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	return elementos_recorridos;
}