#include <stdio.h>
#include <stdlib.h>

#define N 9
#define HORIZONTAL_LENGTH (4 * N * N)
#define VERTICAL_LENGTH (N * N * N)

typedef struct node {
    int index_h, index_v;
    struct node *next_h, *next_v, *prev_h, *prev_v;
} NODE;

NODE *root_nodes_v[VERTICAL_LENGTH];
NODE *root_nodes_h[HORIZONTAL_LENGTH];
NODE *nodes[VERTICAL_LENGTH][HORIZONTAL_LENGTH];

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
    for (int num=0; num<N; ++num){
        for (int y=0; y<N;++y){
            for (int x=0; x<N; ++x){

            }
        }
    }




    for (int i=0;i<HORIZONTAL_LENGTH; ++i){
        free(root_nodes_h[i]);
    }
    for (int i=0;i<VERTICAL_LENGTH; ++i){
        free(root_nodes_v[i]);
    }
    for (int i=0; i<VERTICAL_LENGTH; ++i)
        for (int j=0; j<HORIZONTAL_LENGTH; ++j)
            free(nodes[i][j]);
    return 0;
}
