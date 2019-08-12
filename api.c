#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TYPES_OF_RELATIONS 6
#define MAX_RELATION_KEY_BUFFER_LENGTH 100
#define RELATION_KEY_SEPARATOR "&"

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
    node *max_tree;
    int max;
} relation;

void input_function_picker(char *input_array[]);
void in_order_walk(node *T_root);
int search(node *T_root, char *k);
node *node_search(node *T_root, char *k);
void left_rotate(node **T_root, node *x);
void right_rotate(node **T_root, node *x);
void RB_insert(node **T_root, node *z);
void RB_insert_fixup(node **T_root, node *z);
void RB_transplant(node **T_root, node *u, node *v);
node *RB_minimum(node *T_root);
void RB_delete(node **T_root, node *z);
void RB_delete_fixup(node **T_root, node *x);
void addent(char *entity);
void addrel(char *sender, char *receiver, char *relation_name);

node T_nil;
node *ent_root;
relation *relations[MAX_TYPES_OF_RELATIONS];
static int relations_index = -1;

int main(void) {
    T_nil.color = 'b';
    T_nil.key = NULL;
    T_nil.p = &T_nil;
    T_nil.right = &T_nil;
    T_nil.left = &T_nil;

    ent_root = &T_nil;

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
    printf("STARTING ENTITIES INORDER WALK\n");
    in_order_walk(ent_root);
//    printf("STARTING REL 0 RECEIVER INORDER WALK\n");
//    printf("%s\n", relations[0]->name);
//    in_order_walk(relations[0]->tree);
//    printf("STARTING REL 1 SENDER INORDER WALK\n");
//    printf("%s\n", relations[1]->name);
//    in_order_walk(relations[1]->tree);
    node *my_node = node_search(ent_root,"\"Hannis_Gruer\"");
    RB_delete(&ent_root, my_node);
    printf("\nSTARTING ENTITIES INORDER WALK\n");
    in_order_walk(ent_root);
    free(my_node->key);
    free(my_node);
    return 0;
}

void input_function_picker(char *input_array[]) {
    if (strcmp(input_array[0], "addent") == 0) {
        //printf("ADDENT\n");
        addent(input_array[1]);
    }
    else if (strcmp(input_array[0], "addrel") == 0) {
        //printf("ADDREL\n");
        addrel(input_array[1], input_array[2], input_array[3]);
    }
    else if (strcmp(input_array[0], "delent") == 0) {
        printf("DELENT\n");
    }
    else if (strcmp(input_array[0], "delrel") == 0) {
        printf("DELREL\n");
    }
    else {
        printf("INPUT NOT RECOGNIZED\n");
    }
}

void in_order_walk(node *T_root) {
    //printf("called in_order walk\n");
    if (T_root != &T_nil) {
        in_order_walk(T_root->left);
        printf("%s\n",T_root->key);
        in_order_walk(T_root->right);
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

void RB_insert_fixup(node **T_root, node *z) {
    //printf("called insert fixup\n");
    while (z->p->color == 'r') {
        if (z->p == z->p->p->left) {
            //printf("called insert fixup left\n");
            node y = *z->p->p->right;
            if (y.color == 'r') {
                z->p->color = 'b';
                y.color = 'b';
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
            node y = *z->p->p->left;
            if (y.color == 'r') {
                z->p->color = 'b';
                y.color = 'b';
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

node *RB_minimum(node *T_root) {
    //printf("called RB_minimum\n");
    while(T_root->left != &T_nil) {
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
            printf("called HERE\n");
            y = RB_minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->p == z) {
                x->p = y;
            } else {
                RB_transplant(T_root, y, y->right);
            }
            RB_transplant(T_root, z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (y_original_color == 'b') {
            RB_delete_fixup(T_root, x);
        }
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

void addent(char *entity) {
    if (search(ent_root, entity) == 0) {
        node *ent = malloc(sizeof(node));
        ent->key = strdup(entity);
        RB_insert(&ent_root, ent);
    }
    else {
        //printf("%s already present in tree\n", ent->key);
    }
}

void addrel(char *sender, char *receiver, char *relation_name) {
    //printf("called addrel %s %s %s\n", sender, receiver, relation_name);
    char result[MAX_RELATION_KEY_BUFFER_LENGTH];
    strcpy(result,receiver);
    strcat(result, RELATION_KEY_SEPARATOR);
    strcat(result,sender);
    char *relation_key = result;

    int relation_found_flag = 0;
    int index = 0;
    for (int i=0; i<=relations_index; i++) {
        if (strcmp(relation_name,relations[i]->name) == 0) {
            relation_found_flag = 1;
            index = i;
        }
    }
    //printf("relation found %d\n", relation_found_flag);
    if (search(ent_root, receiver) == 1 && search(ent_root, sender) == 1) {
        if (relation_found_flag == 1 && search(relations[index]->tree, relation_key) == 0) {
            //printf("search relation %d\n", search(relations[index]->tree, relation_key) == 0);
            node *rel = malloc(sizeof(node));
            rel->key = strdup(relation_key);
            RB_insert(&relations[index]->tree, rel);
        } else if (relation_found_flag == 0) {
            relations_index++;
            relation *new_relation_type = malloc(sizeof(relation));
            new_relation_type->name = strdup(relation_name);
            node *new_relation_tree = &T_nil;
            new_relation_type->tree = new_relation_tree;
            relations[relations_index] = new_relation_type;

            node *rel = malloc(sizeof(node));
            rel->key = strdup(relation_key);
            RB_insert(&relations[relations_index]->tree, rel);
        }
//        else if ( search(relations[index]->tree, relation_key) == 1) {
//            printf("RELATION ALREADY PRESENT\n");
//        }
    }
//    else {
//        printf("ENTITY NOT PRESENT\n");
//    }
}
