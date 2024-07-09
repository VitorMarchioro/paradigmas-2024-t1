#include <stdlib.h>
#include <string.h>
#include "lista.h"

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

struct Lista {
    Node* head;
    Node* current;
    int data_size;
    int size;
    void (*free_data)(void*);
};

Lista* Lista_new(int data_size, void (*free_data)(void*)) {
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    lista->head = NULL;
    lista->current = NULL;
    lista->data_size = data_size;
    lista->size = 0;
    lista->free_data = free_data;
    return lista;
}

void Lista_delete(Lista* lista) {
    Node* node = lista->head;
    while (node != NULL) {
        Node* next = node->next;
        if (lista->free_data) {
            lista->free_data(node->data);
        }
        free(node->data);
        free(node);
        node = next;
    }
    free(lista);
}

int Lista_isEmpty(Lista* lista) {
    return lista->head == NULL;
}

int Lista_size(Lista* lista) {
    return lista->size;
}

void Lista_pushFront(Lista* lista, void* valor) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = malloc(lista->data_size);
    memcpy(node->data, valor, lista->data_size);
    node->next = lista->head;
    lista->head = node;
    lista->size++;
}

void Lista_pushBack(Lista* lista, void* valor) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = malloc(lista->data_size);
    memcpy(node->data, valor, lista->data_size);
    node->next = NULL;
    if (Lista_isEmpty(lista)) {
        lista->head = node;
    } else {
        Node* temp = lista->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = node;
    }
    lista->size++;
}

int Lista_search(Lista* lista, void* chave, void* dest, int (*cmp)(void*, void*)) {
    Node* node = lista->head;
    while (node != NULL) {
        if (cmp(node->data, chave) == 0) {
            memcpy(dest, node->data, lista->data_size);
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void* Lista_first(Lista* lista) {
    if (Lista_isEmpty(lista)) return NULL;
    lista->current = lista->head;
    return lista->current->data;
}

void* Lista_last(Lista* lista) {
    if (Lista_isEmpty(lista)) return NULL;
    Node* node = lista->head;
    while (node->next != NULL) {
        node = node->next;
    }
    lista->current = node;
    return node->data;
}

int Lista_next(Lista* lista) {
    if (lista->current == NULL || lista->current->next == NULL) return 0;
    lista->current = lista->current->next;
    return 1;
}

void* Lista_current(Lista* lista) {
    if (lista->current == NULL) return NULL;
    return lista->current->data;
}

int Lista_remove(Lista* lista, void* chave, int (*cmp)(void*, void*)) {
    Node* node = lista->head;
    Node* prev = NULL;
    while (node != NULL) {
        if (cmp(node->data, chave) == 0) {
            if (prev == NULL) {
                lista->head = node->next;
            } else {
                prev->next = node->next;
            }
            if (lista->free_data) {
                lista->free_data(node->data);
            }
            free(node->data);
            free(node);
            lista->size--;
            return 1;
        }
        prev = node;
        node = node->next;
    }
    return 0;
}

void Lista_insertAfter(Lista* lista, void* dado) {
    if (lista->current == NULL) return;
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = malloc(lista->data_size);
    memcpy(node->data, dado, lista->data_size);
    node->next = lista->current->next;
    lista->current->next = node;
    lista->size++;
}
