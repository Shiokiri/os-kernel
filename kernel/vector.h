#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_INIT_CAPACITY 8

typedef struct {
    void **items;
    int capacity;
    int total;
} vectorList;

struct vector {
    vectorList *list;
    int (*size)(struct vector *);
    void (*add)(struct vector *, void *);
    void *(*get)(struct vector *, int);
    void (*set)(struct vector *, int, void *);
    void (*delete)(struct vector *, int);
    void (*free)(struct vector *);
    int (*find)(struct vector *, void *);
    void *(*pop)(struct vector *);
};

typedef struct vector Vector;

void vector_init(Vector *vec);

#endif /* VECTOR_H */