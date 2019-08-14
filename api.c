#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>

#define MAX_TYPES_OF_RELATIONS 6
//#define MAX_RELATION_KEY_BUFFER_LENGTH 100

typedef struct rel_node {
    char color;
    char *key;
    struct rel_node *p;
    struct rel_node *left;
    struct rel_node *right;
} rel_node;

typedef struct relation {
    char *name;
    rel_node *tree;
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

void input_function_picker(char *input_array[]);
void in_order_walk(rel_node *T_root);
void ent_in_order_walk(ent_node *T_root);
int search(rel_node *T_root, char *k);
int ent_search(ent_node *T_root, char *k);
rel_node *node_search(rel_node *T_root, char *k);
ent_node *ent_node_search(ent_node *T_root, char *k);
void left_rotate(rel_node **T_root, rel_node *x);
void ent_left_rotate(ent_node **T_root, ent_node *x);
void right_rotate(rel_node **T_root, rel_node *x);
void ent_right_rotate(ent_node **T_root, ent_node *x);
void RB_insert(rel_node **T_root, rel_node *z);
void ent_RB_insert(ent_node **T_root, ent_node *z);
void RB_insert_fixup(rel_node **T_root, rel_node *z);
void ent_RB_insert_fixup(ent_node **T_root, ent_node *z);
void RB_transplant(rel_node **T_root, rel_node *u, rel_node *v);
void ent_RB_transplant(ent_node **T_root, ent_node *u, ent_node *v);
rel_node *RB_minimum(rel_node *T_root);
ent_node *ent_RB_minimum(ent_node *T_root);
void RB_delete(rel_node **T_root, rel_node *z);
void ent_RB_delete(ent_node **T_root, ent_node *z);
void RB_delete_fixup(rel_node **T_root, rel_node *x);
void ent_RB_delete_fixup(ent_node **T_root, ent_node *x);
void empty_tree(rel_node **T_root);
void empty_relation(relation *rel);
void empty_ent_tree(ent_node **T_root);
void post_order_deletion(ent_node *T_root, char *entity);
void insertionSort(relation *arr[], int relation_index);
void addent(char *entity);
void addrel(char *sender, char *receiver, char *relation_name);
void delrel(char *sender, char *receiver, char *relation_name);
void delent(char *entity);

rel_node T_nil;
ent_node *ent_root;
ent_node T_nil_ent;
//relation *relations[MAX_TYPES_OF_RELATIONS];
//static int relations_index = -1;

int main(void) {
    //clock_t begin = clock();
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

    char *input_line= NULL;
    size_t size = 0;
    while (getline(&input_line, &size, stdin) != -1) {
        char *delimiter;
        input_line[strcspn(input_line, "\r\n")] = 0;
        //printf("%s", input_line);
        if (strcmp(input_line, "report") == 0) {
            //printf("REPORT\n");
        }
        else if (strcmp(input_line,"end") == 0) {
            printf("END\n");
            //free(input_line);
            //return 0;
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
    free(input_line);

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

    //clock_t end = clock();
    //double time_spent = 0.0;
    //time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("Time elpased is %f seconds", time_spent);
    return 0;
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

void in_order_walk(rel_node *T_root) {
    //printf("called in_order walk\n");
    if (T_root != &T_nil) {
        in_order_walk(T_root->left);
        printf("%s\n",T_root->key);
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

int search(rel_node *T_root, char *k) {
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

rel_node *node_search(rel_node *T_root, char *k) {
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

void left_rotate(rel_node **T_root, rel_node *x) {
    //printf("called left rotate\n");
    if ((**T_root).p == &T_nil && x->right != &T_nil) {
        rel_node *y = x->right;
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

void right_rotate(rel_node **T_root, rel_node *x) {
    //printf("called right rotate\n");
    if ((**T_root).p == &T_nil && x->left != &T_nil) {
        rel_node *y = x->left;
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

void RB_insert(rel_node **T_root, rel_node *z) {
    //printf("called rb_insert\n");
    //Z'S KEY HAS TO HAVE ALREADY BEEN INITIALIZED HERE
    rel_node *y = &T_nil;
    rel_node *x = *T_root;
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

void RB_insert_fixup(rel_node **T_root, rel_node *z) {
    //printf("called insert fixup\n");
    while (z->p->color == 'r') {
        if (z->p == z->p->p->left) {
            //printf("called insert fixup left\n");
            rel_node *y = z->p->p->right;
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
            rel_node *y = z->p->p->left;
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

void RB_transplant(rel_node **T_root, rel_node *u, rel_node *v) {
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

rel_node *RB_minimum(rel_node *T_root) {
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

void RB_delete(rel_node **T_root, rel_node *z) {
    //printf("called RB_delete\n");
    if (*T_root != &T_nil) {
        rel_node *x;
        rel_node *y = z;
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

void RB_delete_fixup(rel_node **T_root, rel_node *x) {
    //printf("called RB_delete_fixup\n");
    while (x != *T_root && x->color == 'b') {
        if (x == x->p->left) {
            rel_node *w = x->p->right;
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
            rel_node *w = x->p->left;
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

void empty_tree(rel_node **T_root) {
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

void insertionSort(relation *arr[], int relation_index) {
    relation *key;
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

void post_order_deletion(ent_node *T_root, char *entity) {
    if (T_root != &T_nil_ent) {
        post_order_deletion(T_root->left, entity);
        post_order_deletion(T_root->right, entity);
        for(int i=0; i<=T_root->relations_index;i++) {
            rel_node *sender_node = node_search(T_root->relations[i]->tree, entity);
            if (sender_node != &T_nil) {
                RB_delete(&T_root->relations[i]->tree, sender_node);
            }
        }
    }
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
    if (receiving_node != &T_nil_ent && ent_search(ent_root, sender)) {
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
            rel_node *new_relation_tree = &T_nil;
            new_relation_type->tree = new_relation_tree;
            receiving_node->relations[receiving_node->relations_index] = new_relation_type;

            rel_node *rel = malloc(sizeof(rel_node));
            rel->key = strdup(sender);
            RB_insert(&receiving_node->relations[receiving_node->relations_index]->tree, rel);
            receiving_node->relations[receiving_node->relations_index]->number++;
            insertionSort(receiving_node->relations, receiving_node->relations_index+1);
        }
        else {
            rel_node *sender_node = node_search(receiving_node->relations[this_relation_index]->tree, sender);
            if (sender_node == &T_nil) {
                //printf("search relation %d\n", search(relations[index]->tree, relation_key) == 0);
                receiving_node->relations[this_relation_index]->number++;
                rel_node *rel = malloc(sizeof(rel_node));
                rel->key = strdup(sender);
                RB_insert(&receiving_node->relations[this_relation_index]->tree, rel);
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
            rel_node *sender_node = node_search(receiving_node->relations[this_relation_index]->tree,sender);
            if (sender_node != &T_nil) {
                RB_delete(&receiving_node->relations[this_relation_index]->tree, sender_node);
                receiving_node->relations[this_relation_index]->number--;
            }
        }
    }
}

void delent(char *entity) {
    //printf("called delent %s\n", entity);
    ent_node *entity_node = ent_node_search(ent_root, entity);
    if (entity_node != &T_nil_ent) {
        for(int i=0; i<=entity_node->relations_index;i++) {
            empty_relation(entity_node->relations[i]);
        }
        ent_RB_delete(&ent_root, entity_node);
        //FAI INORDER WALK DELLE ENTITY, POI PER OGNI NODO, PER OGNI RELAZIONE, CERCA entity, SE LA TROVI CANCELLALA
        post_order_deletion(ent_root, entity);

    }
//    else {
//        printf("ENTITY %s NOT PRESENT\n", entity);
//    }
}
