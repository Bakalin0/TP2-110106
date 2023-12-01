#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

struct vector_posiciones {
	void **vector;
	size_t tope;
	size_t posicion;
};

nodo_abb_t *nodo_crear(void *elemento)
{
	nodo_abb_t *nuevo_nodo = calloc(1, sizeof(nodo_abb_t));
	if (!nuevo_nodo) {
		return NULL;
	}

	nuevo_nodo->elemento = elemento;

	return nuevo_nodo;
}

nodo_abb_t *insertar_rec(nodo_abb_t *nodo_actual, nodo_abb_t *nodo_nuevo,
			 abb_comparador comparador)
{
	if (!nodo_nuevo) {
		return NULL;
	}

	if (!nodo_actual) {
		return nodo_nuevo;
	}

	if (comparador(nodo_nuevo->elemento, nodo_actual->elemento) <= 0) {
		nodo_actual->izquierda = insertar_rec(nodo_actual->izquierda,
						      nodo_nuevo, comparador);
	}

	else {
		nodo_actual->derecha = insertar_rec(nodo_actual->derecha,
						    nodo_nuevo, comparador);
	}

	return nodo_actual;
}

nodo_abb_t *extraer_elemento_mas_derecho(nodo_abb_t *nodo_actual,
					 void **elemento_extraido)
{
	if (nodo_actual->derecha == NULL) {
		*elemento_extraido = nodo_actual->elemento;
		nodo_abb_t *izquierdo = nodo_actual->izquierda;
		free(nodo_actual);
		return izquierdo;
	}

	nodo_actual->derecha = extraer_elemento_mas_derecho(
		nodo_actual->derecha, elemento_extraido);
	return nodo_actual;
}

void *quitar_rec(abb_t *arbol, nodo_abb_t *nodo_actual, void *elemento,
		 abb_comparador comparador, void **extraido)
{
	if (!nodo_actual) {
		return NULL;
	}

	int comparacion = comparador(elemento, nodo_actual->elemento);

	if (comparacion == 0) {
		nodo_abb_t *izquierdo = nodo_actual->izquierda;
		nodo_abb_t *derecho = nodo_actual->derecha;
		*extraido = nodo_actual->elemento;
		arbol->tamanio--;

		if (izquierdo != NULL && derecho != NULL) {
			void *elemento_derecho = NULL;
			nodo_actual->izquierda = extraer_elemento_mas_derecho(
				nodo_actual->izquierda, &elemento_derecho);

			nodo_actual->elemento = elemento_derecho;
			return nodo_actual;
		} else {
			free(nodo_actual);
			if (izquierdo == NULL) {
				return derecho;
			}
			return izquierdo;
		}
	}

	else if (comparacion < 0) {
		nodo_actual->izquierda =
			quitar_rec(arbol, nodo_actual->izquierda, elemento,
				   comparador, extraido);
	} else {
		nodo_actual->derecha = quitar_rec(arbol, nodo_actual->derecha,
						  elemento, comparador,
						  extraido);
	}

	return nodo_actual;
}

bool con_cada_elemento_inorden(nodo_abb_t *nodo_actual,
			       bool (*funcion)(void *, void *), void *aux,
			       size_t *contador)
{
	if (!nodo_actual) {
		return true;
	}

	bool continuar = con_cada_elemento_inorden(nodo_actual->izquierda,
						   funcion, aux, contador);

	if (!continuar) {
		return false;
	}

	(*contador)++;
	if (!funcion(nodo_actual->elemento, aux)) {
		return false;
	}

	return con_cada_elemento_inorden(nodo_actual->derecha, funcion, aux,
					 contador);
}

bool con_cada_elemento_preorden(nodo_abb_t *nodo_actual,
				bool (*funcion)(void *, void *), void *aux,
				size_t *contador)
{
	if (!nodo_actual) {
		return true;
	}

	(*contador)++;
	if (!funcion(nodo_actual->elemento, aux)) {
		return false;
	}

	bool continuar = con_cada_elemento_preorden(nodo_actual->izquierda,
						    funcion, aux, contador);

	if (!continuar) {
		return false;
	}

	return con_cada_elemento_preorden(nodo_actual->derecha, funcion, aux,
					  contador);
}

bool con_cada_elemento_postorden(nodo_abb_t *nodo_actual,
				 bool (*funcion)(void *, void *), void *aux,
				 size_t *contador)
{
	if (!nodo_actual) {
		return true;
	}

	bool continuar = con_cada_elemento_postorden(nodo_actual->izquierda,
						     funcion, aux, contador);

	if (!continuar) {
		return false;
	}

	con_cada_elemento_postorden(nodo_actual->derecha, funcion, aux,
				    contador);

	(*contador)++;
	return funcion(nodo_actual->elemento, aux);
}

bool rellenar_vector(void *elemento, void *vec)
{
	struct vector_posiciones *vp = vec;
	if (vp->posicion >= vp->tope) {
		return false;
	}

	vp->vector[vp->posicion] = elemento;
	vp->posicion++;

	return true;
}

void destruir_todo_rec(abb_t *arbol, void (*destructor)(void *),
		       nodo_abb_t *nodo_actual)
{
	if (!arbol) {
		return;
	}

	if (arbol->tamanio == 0) {
		free(arbol);
		return;
	}

	if (!nodo_actual) {
		return;
	}

	destruir_todo_rec(arbol, destructor, nodo_actual->izquierda);
	destruir_todo_rec(arbol, destructor, nodo_actual->derecha);

	if (destructor) {
		destructor(nodo_actual->elemento);
	}

	free(nodo_actual);
	arbol->tamanio--;

	if (arbol->tamanio == 0) {
		free(arbol);
	}

	return;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador) {
		return NULL;
	}

	abb_t *abb = calloc(1, sizeof(abb_t));
	if (!abb) {
		return NULL;
	}

	abb->comparador = comparador;

	return abb;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	arbol->nodo_raiz = insertar_rec(arbol->nodo_raiz, nodo_crear(elemento),
					arbol->comparador);

	arbol->tamanio++;
	return arbol;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || arbol->tamanio == 0) {
		return NULL;
	}

	void *extraido = NULL;

	arbol->nodo_raiz = quitar_rec(arbol, arbol->nodo_raiz, elemento,
				      arbol->comparador, &extraido);
	return extraido;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol || !elemento || !arbol->nodo_raiz) {
		return NULL;
	}

	nodo_abb_t *nodo_actual = arbol->nodo_raiz;

	while (nodo_actual) {
		int comparacion =
			arbol->comparador(elemento, nodo_actual->elemento);

		if (comparacion == 0) {
			return nodo_actual->elemento;
		}

		else if (comparacion < 0) {
			nodo_actual = nodo_actual->izquierda;
		}

		else {
			nodo_actual = nodo_actual->derecha;
		}
	}

	return NULL;
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol) {
		return true;
	}

	if (arbol->tamanio != 0) {
		return false;
	}
	return true;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol) {
		return 0;
	}
	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol) {
		return;
	}

	destruir_todo_rec(arbol, NULL, arbol->nodo_raiz);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol) {
		return;
	}

	destruir_todo_rec(arbol, destructor, arbol->nodo_raiz);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion) {
		return 0;
	}

	size_t contador = 0;

	if (recorrido == INORDEN) {
		con_cada_elemento_inorden(arbol->nodo_raiz, funcion, aux,
					  &contador);
	} else if (recorrido == PREORDEN) {
		con_cada_elemento_preorden(arbol->nodo_raiz, funcion, aux,
					   &contador);
	}

	else if (recorrido == POSTORDEN) {
		con_cada_elemento_postorden(arbol->nodo_raiz, funcion, aux,
					    &contador);
	}

	else {
		return 0;
	}

	return contador;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol || !array) {
		return 0;
	}

	struct vector_posiciones vector_a_llenar;
	vector_a_llenar.vector = array;
	vector_a_llenar.posicion = 0;
	vector_a_llenar.tope = tamanio_array;

	abb_con_cada_elemento(arbol, recorrido, rellenar_vector,
			      &vector_a_llenar);

	return vector_a_llenar.posicion;
}
