#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 9
#define HORIZONTAL_LENGTH (4 * N * N)
#define VERTICAL_LENGTH (N * N * N)

typedef struct node {
    int index_h, index_v;
    struct node *next_h, *next_v, *prev_h, *prev_v;
} NODE;

NODE root_main;
NODE *root_nodes_v[VERTICAL_LENGTH];
NODE *root_nodes_h[HORIZONTAL_LENGTH];
NODE *nodes[VERTICAL_LENGTH][HORIZONTAL_LENGTH];

NODE *find_last_vertical(NODE *root_h) {
    NODE *result = root_h;
    while (result->next_v != NULL) {
        result = result->next_v;
    }
    return result;
}

NODE *find_last_horizontal(NODE *root_v) {
    NODE *result = root_v;
    while (result->next_h != NULL) {
        result = result->next_h;
    }
    return result;
}

void create_new_node(int x, int y) {
    nodes[y][x] = malloc(sizeof(NODE));
    NODE *prev_h = find_last_horizontal(root_nodes_v[y]);
    NODE *prev_v = find_last_vertical(root_nodes_h[x]);
    *nodes[y][x] = (NODE) {x, y, NULL, NULL, prev_h, prev_v};
    prev_v->next_v = nodes[y][x];
    prev_h->next_h = nodes[y][x];
}

int main(void) {

    root_nodes_v[0] = (NODE *) malloc(sizeof(NODE));
    *root_nodes_v[0] = (NODE) {-1, 0, NULL, NULL, NULL, NULL};
    for (int v = 1; v < VERTICAL_LENGTH; ++v) {
        root_nodes_v[v] = (NODE *) malloc(sizeof(NODE));
        *root_nodes_v[v] = (NODE) {-1, v, NULL, NULL, NULL, root_nodes_v[v - 1]};
        root_nodes_v[v - 1]->next_v = root_nodes_v[v];

    } // заполняем корневые узлы по вертикале

    root_nodes_h[0] = (NODE *) malloc(sizeof(NODE));
    *root_nodes_h[0] = (NODE) {0, -1, NULL, NULL, NULL, NULL};
    for (int h = 1; h < HORIZONTAL_LENGTH; ++h) {
        root_nodes_h[h] = (NODE *) malloc(sizeof(NODE));
        *root_nodes_h[h] = (NODE) {h, -1, NULL, NULL, root_nodes_h[h - 1], NULL};
        root_nodes_h[h - 1]->next_h = root_nodes_h[h];
    } // заполняем корневые узлы по горизонтале

    root_main = (NODE) {-1, -1, root_nodes_h[0], root_nodes_v[0], NULL, NULL};
    root_nodes_h[0]->prev_h = &root_main;
    root_nodes_v[0]->prev_v = &root_main;

    int v_counter = 0;
    for (int num = 0; num < N; ++num) {
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                create_new_node(y * N + x, v_counter);
                create_new_node((N * N) + y * N + num, v_counter);
                create_new_node((2 * N * N) + x * N + num, v_counter);

                int n_sqrt = (int)(sqrt(N));
                int n_box = x / n_sqrt + n_sqrt * (y / n_sqrt);
                create_new_node((3 * N * N) + n_box * N + num, v_counter);
                v_counter++;

            }
        }
    }


    for (int i = 0; i < HORIZONTAL_LENGTH; ++i) {
        free(root_nodes_h[i]);
    }
    for (int i = 0; i < VERTICAL_LENGTH; ++i) {
        free(root_nodes_v[i]);
    }
    for (int i = 0; i < VERTICAL_LENGTH; ++i)
        for (int j = 0; j < HORIZONTAL_LENGTH; ++j)
            free(nodes[i][j]);
    return 0;
}
