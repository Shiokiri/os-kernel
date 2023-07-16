#include "vector.h"
#include "types.h"
#include "alloc.h"
#include "stdio.h"

int size(Vector *vec) {
    return vec->list->total;
}

void resize(Vector *vec, int capacity) {
    vec->list->capacity = capacity;
    vec->list->items = realloc(vec->list->items, sizeof(void *) * vec->list->capacity);
}

void add(Vector *vec, void *data) {
    if (vec->list->capacity == vec->list->total) {
        vec->list->capacity *= 2;
        vec->list->items = realloc(vec->list->items, sizeof(void *) * vec->list->capacity);
    }
    vec->list->items[vec->list->total++] = data;
}

void *get(Vector *vec, int index) {
    if (index >= 0 && index < vec->list->total)
        return vec->list->items[index];
    cprintk(ERROR, "Index out of bounds for vector get\n");
    return NULL;
}

void set(Vector *vec, int index, void *data) {
    if (index >= 0 && index < vec->list->total)
        vec->list->items[index] = data;
    else cprintk(ERROR, "Index out of bounds for vector set\n");
}

void delete(Vector *vec, int index) {
    if (index < 0 || index >= vec->list->total)
        cprintk(ERROR, "Index out of bounds for vector delete\n");
    else {
        vec->list->items[index] = NULL;
        int i;
        for (i = index; i < vec->list->total - 1; i++) {
            vec->list->items[i] = vec->list->items[i + 1];
            vec->list->items[i + 1] = NULL;
        }
        vec->list->total--;
        if (vec->list->total > 0 && vec->list->total == vec->list->capacity / 4)
            resize(vec, vec->list->capacity / 2);
    }
}

void *pop(Vector *vec) {
    void *item = vec->get(vec, vec->list->total - 1);
    vec->delete(vec, vec->list->total - 1);
    return item;
}

// TODO 优化
int find(Vector *vec, void *data) {
    int size = vec->size(vec);
    for(int i = 0; i < size; i++) {
        void *temp = vec->get(vec, i);
        if(temp == data) {
            return 1;
        }
    }
    return 0;
}

void free_vector(Vector *vec) {
    free(vec->list->items);
    free(vec->list);
}

void vector_init(Vector *vec) {
    vec->list = (vectorList *)malloc(sizeof(vectorList));
    vec->list->capacity = VECTOR_INIT_CAPACITY;
    vec->list->total = 0;
    vec->list->items = malloc(sizeof(void *) * vec->list->capacity);
    vec->size = size;
    vec->add = add;
    vec->get = get;
    vec->set = set;
    vec->delete = delete;
    vec->free = free_vector;
    vec->pop = pop;
    vec->find = find;
}