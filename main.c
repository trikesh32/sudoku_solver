#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 9
#define HORIZONTAL_LENGTH (4 * N * N)
#define VERTICAL_LENGTH (N * N * N)
#define DEFAULT_STACK_SIZE 10

typedef struct node {
    int index_h, index_v;
    struct node *next_h, *next_v, *prev_h, *prev_v;
} NODE;

typedef struct stack {
    NODE **data;
    int length;
    int capacity;
} STACK;


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


NODE *find_root_horizontal(NODE *node){
    NODE *result = node;
    while(result->prev_h != NULL){
        result = result->prev_h;
    }
    return result;
}


NODE *find_root_vertical(NODE *node){
    NODE *result = node;
    while(result->prev_v != NULL){
        result = result->prev_v;
    }
    return result;
}


void remove_column(NODE *root_column_node){
    NODE* actual = root_column_node;
    while (actual != NULL){
        actual->prev_h->next_h = actual->next_h;
        if (actual->next_h != NULL)
            actual->next_h->prev_h = actual->prev_h;
        actual = actual->next_v;
    }
}
void restore_column(NODE *root_column_node){
    NODE *actual = root_column_node;
    while(actual != NULL){
        actual->prev_h->next_h = actual;
        if (actual->next_h != NULL)
            actual->next_h->prev_h = actual;
        actual = actual->next_v;
    }
}

void remove_row(NODE *root_row_node){
    NODE* actual = root_row_node;
    while (actual != NULL){
        actual->prev_v->next_v = actual->next_v;
        if (actual->next_v != NULL)
            actual->next_v->prev_v = actual->prev_v;
        actual = actual->next_h;
    }
}
void restore_row(NODE *root_row_node){
    NODE *actual = root_row_node;
    while(actual != NULL){
        actual->prev_v->next_v = actual;
        if (actual->next_v != NULL)
            actual->next_v->prev_v = actual;
        actual = actual->next_h;
    }
}


STACK *create_stack(){
    STACK *stack_p = malloc(sizeof(STACK));
    *stack_p = (STACK){malloc(DEFAULT_STACK_SIZE * sizeof(NODE *)), 0, DEFAULT_STACK_SIZE};
    return stack_p;
}


void delete_stack(STACK * stack){
    free(stack->data);
    free(stack);
}

_Bool is_in_stack(STACK * stack, NODE*data){
    for(int i=0;i<stack->length;++i){
        if (stack->data[i] == data) return 1;
    }
    return 0;
}

void push(STACK * stack, NODE * data){
    if (is_in_stack(stack, data)) return;
    if (stack->length == stack->capacity){
        stack->capacity = stack->capacity * 2;
        NODE **new_data_p = realloc(stack->data, 2 * stack->capacity * sizeof(NODE *));
        if (new_data_p == NULL) free(stack->data);
        stack->data = new_data_p;
    }
    stack->data[stack->length] = data;
    stack->length++;
}

NODE * pop(STACK * stack){
    if (stack->length == 0) return NULL;
    return stack->data[--stack->length];
}
int find_x_length(NODE *root_main){
    int counter=0;
    while (root_main->next_h != NULL){
        counter++;
        root_main = root_main->next_h;
    }
    return counter;
}
int find_y_length(NODE *root_main){
    int counter = 0;
    while (root_main->next_v != NULL){
        counter++;
        root_main = root_main->next_v;
    }
    return counter;
}

void print_table(NODE* root){
    int x_length = find_x_length(root), y_length = find_y_length(root);
    NODE *(*p)[x_length] = malloc(x_length * y_length * sizeof(NODE *));
    memset(p, 0, x_length * y_length * sizeof(NODE *));
    NODE *column = root->next_h;
    printf("-1\t");
    int jndex = 0;
    while (column != NULL){
        NODE * node = column->next_v;
        printf("%d\t", column->index_h);
        while (node != NULL){
            int index = y_length - 1 - find_y_length(find_root_horizontal(node));
            p[index][jndex] = node;
            node = node -> next_v;
        }
        column = column->next_h;
        jndex++;
    }
    printf("\n");
    NODE* row = root->next_v;
    for (int i=0; i<y_length; ++i){
        printf("%d\t", row->index_v);
        row = row->next_v;
        for (int j=0; j<x_length;++j){
            if (p[i][j] == NULL) printf("\t");
            else printf("1\t");
        }
        printf("\n");
    }
    for (int i=0;i<100; ++i){
        printf("=");
    }
    printf("\n");
    free(p);
}



NODE *root_main;
NODE *root_nodes_v[VERTICAL_LENGTH];
NODE *root_nodes_h[HORIZONTAL_LENGTH];
NODE *nodes[VERTICAL_LENGTH][HORIZONTAL_LENGTH];
STACK *result;
_Bool is_ready = 0;

void create_new_node(int x, int y) {
    nodes[y][x] = malloc(sizeof(NODE));
    NODE *prev_h = find_last_horizontal(root_nodes_v[y]);
    NODE *prev_v = find_last_vertical(root_nodes_h[x]);
    *nodes[y][x] = (NODE) {x, y, NULL, NULL, prev_h, prev_v};
    prev_v->next_v = nodes[y][x];
    prev_h->next_h = nodes[y][x];
}


NODE *find_minimal(){
    NODE *res;
    int minimal = INT_MAX;
    NODE *header = root_main->next_h;
    while (header != NULL){
        int count = 0;
        NODE* j = header;
        while (j->next_v != NULL){
            count++;
            j = j->next_v;
        }
        if (count < minimal){
            minimal = count;
            res = header;
        }
        header = header->next_h;
    }
    return res;
}


void algorithm_x(){
    if (is_ready) return;
    if (root_main->next_h == NULL){
        is_ready = 1;
        return;
    }
    if (root_main->next_v == NULL) return;
    NODE *interested_column = find_minimal() -> next_v;
    while (interested_column != NULL && !is_ready){
        STACK *death_stack_rows = create_stack();
        STACK *death_stack_columns = create_stack();

        NODE *i = find_root_horizontal(interested_column) ->next_h;
        while (i != NULL){
            push(death_stack_columns, find_root_vertical(i));
            i = i->next_h;
        }
        NODE *interested_row = find_root_horizontal(interested_column);
        push(result, interested_row);
        i = interested_row->next_h;
        while (i != NULL){
            NODE * j  = find_root_vertical(i)->next_v;
            while (j != NULL){
                push(death_stack_rows, find_root_horizontal(j));
                j = j->next_v;
            }
            i = i->next_h;
        }
        for (int index=0;index<death_stack_rows->length;index++){
            remove_row(death_stack_rows->data[index]);
        }
        for (int index=0;index<death_stack_columns->length;index++){
            remove_column(death_stack_columns->data[index]);
        }
        algorithm_x();
        if (!is_ready)
            pop(result);
        for(int index=0; index<death_stack_rows->length; index++){
            restore_row(death_stack_rows->data[index]);
        }
        for (int index=0; index<death_stack_columns->length;index++){
            restore_column(death_stack_columns->data[index]);
        }

        interested_column = interested_column->next_v;
        delete_stack(death_stack_columns);
        delete_stack(death_stack_rows);
    }
}


void print_results(STACK* res){
    int (*table)[N] = malloc(N * N * sizeof(int));
    memset(table, 0, N * N * sizeof(int));
    for (int i=0; i<N * N;++i){
        int index = res->data[i]->index_v;
        int full_coordinates = index % (N * N);
        table[full_coordinates / N][full_coordinates % N] = (index / (N * N)) + 1;
    }
    for (int i=0; i<N;++i){
        for (int j=0;j<N;++j){
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }
    free(table);
}


void delete_num(NODE *num_p){
    STACK *death_stack_rows = create_stack();
    STACK *death_stack_columns = create_stack();

    NODE *i = num_p ->next_h;
    while (i != NULL){
        push(death_stack_columns, find_root_vertical(i));
        i = i->next_h;
    }
    NODE *interested_row = num_p;
    i = interested_row->next_h;
    while (i != NULL){
        NODE * j  = find_root_vertical(i)->next_v;
        while (j != NULL){
            push(death_stack_rows, find_root_horizontal(j));
            j = j->next_v;
        }
        i = i->next_h;
    }
    for (int index=0;index<death_stack_rows->length;index++){
        remove_row(death_stack_rows->data[index]);
    }
    for (int index=0;index<death_stack_columns->length;index++){
        remove_column(death_stack_columns->data[index]);
    }
    delete_stack(death_stack_rows);
    delete_stack(death_stack_columns);
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

    root_main = malloc(sizeof(NODE));
    *root_main = (NODE) {-1, -1, root_nodes_h[0], root_nodes_v[0], NULL, NULL};
    root_nodes_h[0]->prev_h = root_main;
    root_nodes_v[0]->prev_v = root_main;

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
    STACK *already_exist = create_stack();
    for (int i=0; i<N;++i){
        for (int j=0; j<N;++j){
            int num; scanf("%d", &num);
            if (num != 0){
                num--;
                NODE* node = root_nodes_v[num * N * N + i * N + j];
                push(already_exist, node);
                delete_num(node);
            }
        }
    }

    result = create_stack();

    algorithm_x();
    for (int i=0; i<already_exist->length; ++i){
        push(result, already_exist->data[i]);
    }
    print_results(result);

    for (int i = 0; i < HORIZONTAL_LENGTH; ++i) {
        free(root_nodes_h[i]);
    }
    for (int i = 0; i < VERTICAL_LENGTH; ++i) {
        free(root_nodes_v[i]);
    }
    for (int i = 0; i < VERTICAL_LENGTH; ++i)
        for (int j = 0; j < HORIZONTAL_LENGTH; ++j)
            free(nodes[i][j]);
    free(root_main);
    delete_stack(result);
    delete_stack(already_exist);
    return 0;
}
