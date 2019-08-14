#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TYPES_OF_RELATIONS 17

/*OPTIMIZATION IDEAS
 * 1) mettere un puntatore in ogni relations, che punti all max corrispondente per non starlo a cercare tutte le volte
 * 2) invece di mettere delle copie dei nodi per rappresentare i massimi, mettere dei puntatori ai nodi
 * */

typedef struct node {
    char color;
    char *key;
    struct node *p;
    struct node *left;
    struct node *right;
} node;

typedef struct relation {
    char *name;
    node *tree;
    int number;
} relation;

typedef struct ent_node {
    char color;
    char *key;
    struct ent_node *p;
    struct ent_node *left;
    struct ent_node *right;
    int relations_index;
    relation *relations[MAX_TYPES_OF_RELATIONS];
} ent_node;

typedef struct maximum {
    char *name;
    int number;
    struct node *tree;
} maximum;

void input_handler();
void input_function_picker(char *input_array[]);
void in_order_walk(node *T_root);
void ent_in_order_walk(ent_node *T_root);
int search(node *T_root, char *k);
int ent_search(ent_node *T_root, char *k);
node *node_search(node *T_root, char *k);
ent_node *ent_node_search(ent_node *T_root, char *k);
void left_rotate(node **T_root, node *x);
void ent_left_rotate(ent_node **T_root, ent_node *x);
void right_rotate(node **T_root, node *x);
void ent_right_rotate(ent_node **T_root, ent_node *x);
void RB_insert(node **T_root, node *z);
void ent_RB_insert(ent_node **T_root, ent_node *z);
void RB_insert_fixup(node **T_root, node *z);
void ent_RB_insert_fixup(ent_node **T_root, ent_node *z);
void RB_transplant(node **T_root, node *u, node *v);
void ent_RB_transplant(ent_node **T_root, ent_node *u, ent_node *v);
node *RB_minimum(node *T_root);
ent_node *ent_RB_minimum(ent_node *T_root);
void RB_delete(node **T_root, node *z);
void ent_RB_delete(ent_node **T_root, ent_node *z);
void RB_delete_fixup(node **T_root, node *x);
void ent_RB_delete_fixup(ent_node **T_root, ent_node *x);
void empty_tree(node **T_root);
void empty_relation(relation *rel);
void empty_ent_tree(ent_node **T_root);
void pre_order_deletion(ent_node *T_root, char *entity);
void max_insertionSort(maximum *arr[], int relation_index);
void recalculate_maximum_number(ent_node *T_root, maximum *max_tree_root, int *max_number);
void populate_maximum_tree(ent_node *T_root, maximum *max_tree_root, int max_number);
void recalculate_maximum(maximum *max_tree_root);
void addent(char *entity);
void addrel(char *sender, char *receiver, char *relation_name);
void delrel(char *sender, char *receiver, char *relation_name);
void delent(char *entity);
void report();

ent_node *ent_root;
ent_node T_nil_ent;
node T_nil;
maximum *max_array[MAX_TYPES_OF_RELATIONS];
int max_index = -1;

int main(void) {
    T_nil.color = 'b';
    T_nil.key = NULL;
    T_nil.p = &T_nil;
    T_nil.right = &T_nil;
    T_nil.left = &T_nil;

    T_nil_ent.color = 'b';
    T_nil_ent.key = NULL;
    T_nil_ent.p = &T_nil_ent;
    T_nil_ent.right = &T_nil_ent;
    T_nil_ent.left = &T_nil_ent;

    ent_root = &T_nil_ent;

    input_handler();

    /*STARTING DEBUG AREA MFS*/
//    printf("STARTING ENTITIES INORDER WALK\n");
//    ent_in_order_walk(ent_root);
//    empty_tree(&ent_root);

//    for (int i = 0; i<=relations_index; i++) {
//        printf("STARTING REL %s RECEIVER INORDER WALK %d\n", relations[i]->name, i);
//        in_order_walk(relations[i]->tree);
//        empty_tree(&(relations[i]->tree));
//        free(relations[i]->name);
//        free(relations[i]);
//    }
    return 0;
}

void input_handler() {
    char *input_line= NULL;
    size_t size = 0;
    while (getline(&input_line, &size, stdin) != -1) {
        char *delimiter;
        input_line[strcspn(input_line, "\r\n")] = 0;
        //printf("%s", input_line);
        if (strcmp(input_line, "report") == 0) {
            //printf("REPORT\n");
            report();
        }
        else if (strcmp(input_line,"end") == 0) {
            //printf("END\n");
            free(input_line);
            return;
        }
        else {
            delimiter = " ";
            char *token;
            token = strtok(input_line, delimiter);
            if (token != NULL) {
                int input_line_word_length = 1;
                if (strcmp(token, "addent") == 0 || strcmp(token, "delent") == 0) {
                    input_line_word_length = 2;
                } else if (strcmp(token, "addrel") == 0 || strcmp(token, "delrel") == 0) {
                    input_line_word_length = 4;
                }
                char *input_divider[input_line_word_length];
                //printf("%s", token);
                input_divider[0] = token;

                for (int i = 1; i < input_line_word_length; i++) {
                    token = strtok(NULL, delimiter);
                    if (token != NULL) {
                        input_divider[i] = token;
                    }
                }
                input_function_picker(input_divider);
            }
        }
    }
    //free(input_line);
}

void input_function_picker(char *input_array[]) {
    if (strcmp(input_array[0], "addent") == 0) {
        addent(input_array[1]);
    }
    else if (strcmp(input_array[0], "addrel") == 0) {
        addrel(input_array[1], input_array[2], input_array[3]);
    }
    else if (strcmp(input_array[0], "delent") == 0) {
        delent(input_array[1]);
    }
    else if (strcmp(input_array[0], "delrel") == 0) {
        delrel(input_array[1], input_array[2], input_array[3]);
    }
    else {
        printf("INPUT NOT RECOGNIZED\n");
    }
}

void in_order_walk(node *T_root) {
    //printf("called in_order walk\n");
    if (T_root != &T_nil) {
        in_order_walk(T_root->left);
        fputs(T_root->key, stdout);
        fputs(" ", stdout);
        in_order_walk(T_root->right);
    }
}

void ent_in_order_walk(ent_node *T_root) {
    //printf("called in_order walk\n");
    if (T_root != &T_nil_ent) {
        ent_in_order_walk(T_root->left);
        printf("%s\n",T_root->key);
        ent_in_order_walk(T_root->right);
    }
}

int search(node *T_root, char *k) {
    //printf("called search\n");
    while (T_root != &T_nil && strcmp(k, T_root->key) != 0) {
        if (strcmp(k, T_root->key) < 0) {
            T_root = T_root->left;
        } else {
            T_root = T_root->right;
        }
    }
    if (T_root == &T_nil) {
        return 0;
    } else {
        return 1;
    }
}

int ent_search(ent_node *T_root, char *k) {
    //printf("called search\n");
    while (T_root != &T_nil_ent && strcmp(k, T_root->key) != 0) {
        if (strcmp(k, T_root->key) < 0) {
            T_root = T_root->left;
        } else {
            T_root = T_root->right;
        }
    }
    if (T_root == &T_nil_ent) {
        return 0;
    } else {
        return 1;
    }
}

node *node_search(node *T_root, char *k) {
    //printf("called search\n");
    while (T_root != &T_nil && strcmp(k, T_root->key) != 0) {
        if (strcmp(k, T_root->key) < 0) {
            T_root = T_root->left;
        } else {
            T_root = T_root->right;
        }
    }
    if (T_root == &T_nil) {
        return &T_nil;
    } else {
        return T_root;
    }
}

ent_node *ent_node_search(ent_node *T_root, char *k) {
    //printf("called search\n");
    while (T_root != &T_nil_ent && strcmp(k, T_root->key) != 0) {
        if (strcmp(k, T_root->key) < 0) {
            T_root = T_root->left;
        } else {
            T_root = T_root->right;
        }
    }
    if (T_root == &T_nil_ent) {
        return &T_nil_ent;
    } else {
        return T_root;
    }
}

void left_rotate(node **T_root, node *x) {
    //printf("called left rotate\n");
    if ((**T_root).p == &T_nil && x->right != &T_nil) {
        node *y = x->right;
        x->right = y->left;
        if (y->left != &T_nil ) {
            y->left->p = x;
        }
        y->p = x->p;
        if (x->p == &T_nil) {
            *T_root = y;
        }
        else if (x == x->p->left) {
            x->p->left = y;
        }
        else {
            x->p->right = y;
        }
        y->left = x;
        x->p = y;
    }
}

void ent_left_rotate(ent_node **T_root, ent_node *x) {
    //printf("called left rotate\n");
    if ((**T_root).p == &T_nil_ent && x->right != &T_nil_ent) {
        ent_node *y = x->right;
        x->right = y->left;
        if (y->left != &T_nil_ent) {
            y->left->p = x;
        }
        y->p = x->p;
        if (x->p == &T_nil_ent) {
            *T_root = y;
        }
        else if (x == x->p->left) {
            x->p->left = y;
        }
        else {
            x->p->right = y;
        }
        y->left = x;
        x->p = y;
    }
}

void right_rotate(node **T_root, node *x) {
    //printf("called right rotate\n");
    if ((**T_root).p == &T_nil && x->left != &T_nil) {
        node *y = x->left;
        x->left = y->right;
        if (y->right != &T_nil ) {
            y->right->p = x;
        }
        y->p = x->p;
        if (x->p == &T_nil ) {
            *T_root = y;
        }
        else if (x == x->p->right) {
            x->p->right = y;
        }
        else {
            x->p->left = y;
        }
        y->right = x;
        x->p = y;
    }
}

void ent_right_rotate(ent_node **T_root, ent_node *x) {
    //printf("called right rotate\n");
    if ((**T_root).p == &T_nil_ent && x->left != &T_nil_ent) {
        ent_node *y = x->left;
        x->left = y->right;
        if (y->right != &T_nil_ent ) {
            y->right->p = x;
        }
        y->p = x->p;
        if (x->p == &T_nil_ent ) {
            *T_root = y;
        }
        else if (x == x->p->right) {
            x->p->right = y;
        }
        else {
            x->p->left = y;
        }
        y->right = x;
        x->p = y;
    }
}

void RB_insert(node **T_root, node *z) {
    //printf("called rb_insert\n");
    //Z'S KEY HAS TO HAVE ALREADY BEEN INITIALIZED HERE
    node *y = &T_nil;
    node *x = *T_root;
    while (x != &T_nil) {
        //printf("SWITCH\n");
        y = x;
        if (strcmp(z->key , x->key) < 0) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->p = y;
    if (y == &T_nil) {
        //printf("CASO BASE\n");
        *T_root = z;
    }
    else if (strcmp(z->key,y->key) < 0) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = &T_nil;
    z->right = &T_nil;
    z->color = 'r';
    RB_insert_fixup(T_root, z);
}

void ent_RB_insert(ent_node **T_root, ent_node *z) {
    //printf("called rb_insert\n");
    //Z'S KEY HAS TO HAVE ALREADY BEEN INITIALIZED HERE
    ent_node *y = &T_nil_ent;
    ent_node *x = *T_root;
    while (x != &T_nil_ent) {
        //printf("SWITCH\n");
        y = x;
        if (strcmp(z->key , x->key) < 0) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->p = y;
    if (y == &T_nil_ent) {
        //printf("CASO BASE\n");
        *T_root = z;
    }
    else if (strcmp(z->key,y->key) < 0) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = &T_nil_ent;
    z->right = &T_nil_ent;
    z->color = 'r';
    ent_RB_insert_fixup(T_root, z);
}

void RB_insert_fixup(node **T_root, node *z) {
    //printf("called insert fixup\n");
    while (z->p->color == 'r') {
        if (z->p == z->p->p->left) {
            //printf("called insert fixup left\n");
            node *y = z->p->p->right;
            if (y->color == 'r') {
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;
            }
            else {
                if (z == z->p->right) {
                    z = z->p;
                    left_rotate(T_root, z);
                }
                z->p->color = 'b';
                z->p->p->color = 'r';
                right_rotate(T_root, z);
            }
        }
        else {
            //printf("called insert fixup right\n");
            node *y = z->p->p->left;
            if (y->color == 'r') {
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;
            }
            else {
                if (z == z->p->left) {
                    z = z->p;
                    right_rotate(T_root, z);
                }
                z->p->color = 'b';
                z->p->p->color = 'r';
                left_rotate(T_root, z);
            }
        }
    }
    (*T_root)->color = 'b';
}

void ent_RB_insert_fixup(ent_node **T_root, ent_node *z) {
    //printf("called insert fixup\n");
    while (z->p->color == 'r') {
        if (z->p == z->p->p->left) {
            //printf("called insert fixup left\n");
            ent_node *y = z->p->p->right;
            if (y->color == 'r') {
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;
            }
            else {
                if (z == z->p->right) {
                    z = z->p;
                    ent_left_rotate(T_root, z);
                }
                z->p->color = 'b';
                z->p->p->color = 'r';
                ent_right_rotate(T_root, z);
            }
        }
        else {
            //printf("called insert fixup right\n");
            ent_node *y = z->p->p->left;
            if (y->color == 'r') {
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;
            }
            else {
                if (z == z->p->left) {
                    z = z->p;
                    ent_right_rotate(T_root, z);
                }
                z->p->color = 'b';
                z->p->p->color = 'r';
                ent_left_rotate(T_root, z);
            }
        }
    }
    (*T_root)->color = 'b';
}

void RB_transplant(node **T_root, node *u, node *v) {
    //printf("RB_transplant\n");
    if (u->p == &T_nil) {
        *T_root = v;
    }
    else if (u == u->p->left) {
        u->p->left = v;
    }
    else {
        u->p->right = v;
    }
    v->p = u->p;
}

void ent_RB_transplant(ent_node **T_root, ent_node *u, ent_node *v) {
    //printf("RB_transplant\n");
    if (u->p == &T_nil_ent) {
        *T_root = v;
    }
    else if (u == u->p->left) {
        u->p->left = v;
    }
    else {
        u->p->right = v;
    }
    v->p = u->p;
}

node *RB_minimum(node *T_root) {
    //printf("called RB_minimum\n");
    while(T_root->left != &T_nil) {
        T_root = T_root->left;
    }
    return T_root;
}

ent_node *ent_RB_minimum(ent_node *T_root) {
    //printf("called RB_minimum\n");
    while(T_root->left != &T_nil_ent) {
        T_root = T_root->left;
    }
    return T_root;
}

void RB_delete(node **T_root, node *z) {
    //printf("called RB_delete\n");
    if (*T_root != &T_nil) {
        node *x;
        node *y = z;
        char y_original_color = y->color;
        if (z->left == &T_nil) {
            x = z->right;
            RB_transplant(T_root, z, z->right);
        } else if (z->right == &T_nil) {
            x = z->left;
            RB_transplant(T_root, z, z->left);
        } else {
            y = RB_minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->p == z) {
                x->p = y;
            } else {
                RB_transplant(T_root, y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            RB_transplant(T_root, z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (y_original_color == 'b') {
            RB_delete_fixup(T_root, x);
        }
        if (y != z) {
            y = z;
        }
        free(y->key);
        free(y);
    }
}

void ent_RB_delete(ent_node **T_root, ent_node *z) {
    //printf("called RB_delete\n");
    if (*T_root != &T_nil_ent) {
        ent_node *x;
        ent_node *y = z;
        char y_original_color = y->color;
        if (z->left == &T_nil_ent) {
            x = z->right;
            ent_RB_transplant(T_root, z, z->right);
        } else if (z->right == &T_nil_ent) {
            x = z->left;
            ent_RB_transplant(T_root, z, z->left);
        } else {
            y = ent_RB_minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->p == z) {
                x->p = y;
            } else {
                ent_RB_transplant(T_root, y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            ent_RB_transplant(T_root, z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (y_original_color == 'b') {
            ent_RB_delete_fixup(T_root, x);
        }
        if (y != z) {
            y = z;
        }
        free(y->key);
        free(y);
    }
}

void RB_delete_fixup(node **T_root, node *x) {
    //printf("called RB_delete_fixup\n");
    while (x != *T_root && x->color == 'b') {
        if (x == x->p->left) {
            node *w = x->p->right;
            if (w->color == 'r') {
                w->color = 'b';
                x->p->color = 'r';
                left_rotate(T_root, x->p);
                w = x->p->right;
            }
            if (w->left->color == 'b' && w->right->color == 'b') {
                w->color = 'r';
                x = x->p;
            }
            else {
                if (w->right->color == 'b') {
                    w->left->color = 'b';
                    w->color = 'r';
                    right_rotate(T_root, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = 'b';
                w->right->color = 'b';
                left_rotate(T_root, x->p);
                x = *T_root;
            }
        }
        else {
            node *w = x->p->left;
            if (w->color == 'r') {
                w->color = 'b';
                x->p->color = 'r';
                right_rotate(T_root, x->p);
                w = x->p->left;
            }
            if (w->right->color == 'b' && w->left->color == 'b') {
                w->color = 'r';
                x = x->p;
            }
            else {
                if (w->left->color == 'b') {
                    w->right->color = 'b';
                    w->color = 'r';
                    left_rotate(T_root, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = 'b';
                w->left->color = 'b';
                right_rotate(T_root, x->p);
                x = *T_root;
            }
        }
    }
    x->color = 'b';
}

void ent_RB_delete_fixup(ent_node **T_root, ent_node *x) {
    //printf("called RB_delete_fixup\n");
    while (x != *T_root && x->color == 'b') {
        if (x == x->p->left) {
            ent_node *w = x->p->right;
            if (w->color == 'r') {
                w->color = 'b';
                x->p->color = 'r';
                ent_left_rotate(T_root, x->p);
                w = x->p->right;
            }
            if (w->left->color == 'b' && w->right->color == 'b') {
                w->color = 'r';
                x = x->p;
            }
            else {
                if (w->right->color == 'b') {
                    w->left->color = 'b';
                    w->color = 'r';
                    ent_right_rotate(T_root, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = 'b';
                w->right->color = 'b';
                ent_left_rotate(T_root, x->p);
                x = *T_root;
            }
        }
        else {
            ent_node *w = x->p->left;
            if (w->color == 'r') {
                w->color = 'b';
                x->p->color = 'r';
                ent_right_rotate(T_root, x->p);
                w = x->p->left;
            }
            if (w->right->color == 'b' && w->left->color == 'b') {
                w->color = 'r';
                x = x->p;
            }
            else {
                if (w->left->color == 'b') {
                    w->right->color = 'b';
                    w->color = 'r';
                    ent_left_rotate(T_root, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = 'b';
                w->left->color = 'b';
                ent_right_rotate(T_root, x->p);
                x = *T_root;
            }
        }
    }
    x->color = 'b';
}

void empty_tree(node **T_root) {
    while (*T_root != &T_nil) {
        RB_delete(T_root, *T_root);
    }
}

void empty_relation(relation *rel) {
    empty_tree(&rel->tree);
    free(rel->name);
    free(rel);
}

void empty_ent_tree(ent_node **T_root) {
    while (*T_root != &T_nil_ent) {
        ent_RB_delete(T_root, *T_root);
    }
}

void max_insertionSort(maximum *arr[], int relation_index) {
    maximum *key;
    int j;
    for (int i = 1; i < relation_index; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && strcmp(arr[j]->name, key->name) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void pre_order_deletion(ent_node *T_root, char *entity) {
    if (T_root != &T_nil_ent) {
        for(int i=0; i<=T_root->relations_index;i++) {
            node *sender_node = node_search(T_root->relations[i]->tree, entity);
            if (sender_node != &T_nil) {
                RB_delete(&T_root->relations[i]->tree, sender_node);
                T_root->relations[i]->number--;
                for (int j=0; j<=max_index; j++) {
                    if (strcmp(T_root->relations[i]->name, max_array[j]->name) == 0) {
                        max_array[j]->number = 0;
                        empty_tree(&max_array[j]->tree);
                    }
                }
            }
        }
        pre_order_deletion(T_root->left, entity);
        pre_order_deletion(T_root->right, entity);
    }
}

void recalculate_maximum_number(ent_node *T_root, maximum *max_tree_root, int *max_number) {
    if (T_root != &T_nil_ent) {
        for (int i = 0; i<=T_root->relations_index; i++) {
            if (strcmp(T_root->relations[i]->name, max_tree_root->name) == 0) {
                if (T_root->relations[i]->number > *max_number) {
                    *max_number = T_root->relations[i]->number;
                }
            }
        }
        recalculate_maximum_number(T_root->left, max_tree_root, max_number);
        recalculate_maximum_number(T_root->right, max_tree_root, max_number);
    }
}

void populate_maximum_tree(ent_node *T_root, maximum *max_tree_root, int max_number) {
    if (T_root != &T_nil_ent) {
        for (int i = 0; i<=T_root->relations_index; i++) {
            if (strcmp(T_root->relations[i]->name, max_tree_root->name) == 0) {
                if (T_root->relations[i]->number == max_number) {
                    node *search = node_search(max_tree_root->tree, T_root->key);
                    if (search == &T_nil) {
                        node *max = malloc(sizeof(node));
                        max->key = strdup(T_root->key);
                        RB_insert(&max_tree_root->tree, max);
                    }
                }
            }
        }
        populate_maximum_tree(T_root->left, max_tree_root, max_number);
        populate_maximum_tree(T_root->right, max_tree_root, max_number);
    }
}

void recalculate_maximum(maximum *max_tree_root) {
    int a = 0;
    int *b = &a;
    recalculate_maximum_number(ent_root, max_tree_root, b);
    int max_num = *b;
    max_tree_root->number = max_num;
    populate_maximum_tree(ent_root, max_tree_root, max_num);
}

void addent(char *entity) {
    //printf("called addent %s\n", entity);
    if (ent_search(ent_root, entity) == 0) {
        ent_node *ent = malloc(sizeof(ent_node));
        ent->relations_index = -1;
        ent->key = strdup(entity);
        ent_RB_insert(&ent_root, ent);
    }
//    else {
//        printf("%s is already present in tree\n", entity);
//    }
}

void addrel(char *sender, char *receiver, char *relation_name) {
    //printf("called addrel %s %s %s\n", sender, receiver, relation_name);
    ent_node *receiving_node = ent_node_search(ent_root, receiver);
    if (receiving_node != &T_nil_ent && ent_search(ent_root, sender) == 1) {
        int this_relation_index = -1;
        for (int i=0; i<=receiving_node->relations_index; i++) {
            if (strcmp(relation_name,receiving_node->relations[i]->name) == 0) {
                this_relation_index = i;
            }
        }
        if (this_relation_index == -1) {
            receiving_node->relations_index++;
            relation *new_relation_type = malloc(sizeof(relation));
            new_relation_type->name = strdup(relation_name);
            node *new_relation_tree = &T_nil;
            new_relation_type->tree = new_relation_tree;
            receiving_node->relations[receiving_node->relations_index] = new_relation_type;

            node *rel = malloc(sizeof(node));
            rel->key = strdup(sender);
            RB_insert(&receiving_node->relations[receiving_node->relations_index]->tree, rel);
            receiving_node->relations[receiving_node->relations_index]->number = 1;

            int first_occurrence_of_relation_flag = 1;
            for (int k=0; k<=max_index; k++) {
                if (strcmp(relation_name,max_array[k]->name) == 0) {
                    first_occurrence_of_relation_flag = 0;
                }
            }
            if (first_occurrence_of_relation_flag == 1) {
                max_index++;
                maximum *new_maximum_type = malloc(sizeof(maximum));
                new_maximum_type->number = 1;
                new_maximum_type->name = strdup(relation_name);
                node *new_maximum_root = &T_nil;
                new_maximum_type->tree = new_maximum_root;
                max_array[max_index] = new_maximum_type;
                //printf("%d\n", max_index);
                node *max = malloc(sizeof(node));
                max->key = strdup(receiver);
                RB_insert(&max_array[max_index]->tree, max);
                max_insertionSort(max_array, max_index + 1);
            }
            else {
                for (int k=0; k<=max_index; k++) {
                    if (strcmp(receiving_node->relations[receiving_node->relations_index]->name,max_array[k]->name) == 0) {
                        if (receiving_node->relations[receiving_node->relations_index]->number == max_array[k]->number) {
                            //printf("called addrel %s %s %s %s\n", sender, receiver, relation_name, max_array[k]->name);
                            if(search(max_array[k]->tree, receiver) == 0) {
                                node *max = malloc(sizeof(node));
                                max->key = strdup(receiver);
                                RB_insert(&max_array[k]->tree, max);
                            }
                        }
                        else if (receiving_node->relations[receiving_node->relations_index]->number > max_array[k]->number) {
                            empty_tree(&max_array[k]->tree);
                            max_array[k]->number = receiving_node->relations[receiving_node->relations_index]->number;
                            node *max = malloc(sizeof(node));
                            max->key = strdup(receiver);
                            RB_insert(&max_array[k]->tree, max);
                        }
                    }
                }
            }
        }
        else {
            node *sender_node = node_search(receiving_node->relations[this_relation_index]->tree, sender);
            if (sender_node == &T_nil) {
                receiving_node->relations[this_relation_index]->number++;
                node *rel = malloc(sizeof(node));
                rel->key = strdup(sender);
                RB_insert(&receiving_node->relations[this_relation_index]->tree, rel);

                for (int k=0; k<=max_index; k++) {
                    if (strcmp(receiving_node->relations[this_relation_index]->name,max_array[k]->name) == 0) {
                        if (receiving_node->relations[this_relation_index]->number == max_array[k]->number) {
                            if(search(max_array[k]->tree, receiver) == 0) {
                                node *max = malloc(sizeof(node));
                                max->key = strdup(receiver);
                                RB_insert(&max_array[k]->tree, max);
                            }
                        }
                        else if (receiving_node->relations[this_relation_index]->number > max_array[k]->number) {
                            empty_tree(&max_array[k]->tree);
                            max_array[k]->number = receiving_node->relations[this_relation_index]->number;
                            node *max = malloc(sizeof(node));
                            max->key = strdup(receiving_node->key);
                            RB_insert(&max_array[k]->tree, max);
                        }
                    }
                }
            }
//            else {
//                printf("RELATION ALREADY PRESENT\n");
//            }
        }
    }
//    else {
//        printf("AT LEAST ONE ENTITY NOT PRESENT\n");
//    }
}

void delrel(char *sender, char *receiver, char *relation_name) {
    //printf("called delrel %s %s %s\n", sender, receiver, relation_name);
    ent_node *receiving_node = ent_node_search(ent_root, receiver);
    if (receiving_node != &T_nil_ent) {
        int this_relation_index = -1;
        for (int i=0; i<=receiving_node->relations_index; i++) {
            if (strcmp(relation_name,receiving_node->relations[i]->name) == 0) {
                this_relation_index = i;
            }
        }
        if (this_relation_index >= 0) {
            node *sender_node = node_search(receiving_node->relations[this_relation_index]->tree,sender);
            if (sender_node != &T_nil) {
                RB_delete(&receiving_node->relations[this_relation_index]->tree, sender_node);
                receiving_node->relations[this_relation_index]->number--;

                for (int j=0; j<=max_index; j++) {
                    if (strcmp(receiving_node->relations[this_relation_index]->name,max_array[j]->name) == 0) {
                        node *max_node = node_search(max_array[j]->tree, receiving_node->key);
                        if (max_node != &T_nil) {
                            RB_delete(&max_array[j]->tree, max_node);
                            if (max_array[j]->tree == &T_nil) {
                                max_array[j]->number = 0;
                                recalculate_maximum(max_array[j]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void delent(char *entity) {
    //printf("called delent %s\n", entity);
    ent_node *entity_node = ent_node_search(ent_root, entity);
    if (entity_node != &T_nil_ent) {
        pre_order_deletion(ent_root, entity);

        for(int i=0; i<=entity_node->relations_index;i++) {
            for (int k=0; k<=max_index; k++) {
                if(strcmp(entity_node->relations[i]->name, max_array[k]->name)==0) {
                    node *del = node_search(max_array[k]->tree, entity);
                    if (del != &T_nil) {
                        RB_delete(&max_array[k]->tree, del);
                        if (max_array[k]->tree == &T_nil) {
                            max_array[k]->number = 0;
                            empty_tree(&max_array[k]->tree);
                        }
                    }
                }
            }
        }
        for(int ii=0; ii<=entity_node->relations_index;ii++) {
            empty_relation(entity_node->relations[ii]);
        }
        ent_RB_delete(&ent_root, entity_node);

        for(int k=0; k<=max_index;k++) {
            if (max_array[k]->number == 0) {
                recalculate_maximum(max_array[k]);
            }
        }
    }
//    else {
//        printf("ENTITY %s NOT PRESENT\n", entity);
//    }
}

void report() {
    int all_zeros = 1;
    for(int j=0;j<=max_index;j++) {
        if (max_array[j]->number != 0) {
            all_zeros = 0;
        }
    }
    if (max_index == -1 || all_zeros == 1) {
        fputs("none\n", stdout);
    }
    else {
        for (int i=0; i<=max_index; i++) {
            if (max_array[i]->number > 0) {
                fputs(max_array[i]->name, stdout);
                fputs(" ", stdout);
                in_order_walk(max_array[i]->tree);
                printf("%d", max_array[i]->number);
                if (i == max_index) {
                    fputs(";", stdout);
                } else {
                    fputs("; ", stdout);
                }
            }
        }
        fputs("\n", stdout);
    }
}
