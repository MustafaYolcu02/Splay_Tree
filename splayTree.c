#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int cost_original = 0;
int cost_mod = 0;

struct node {
    int key;
    int count;
    struct node *left, *right, *parent;
};

struct node* newNode(int key) {
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->key = key;
    node->count = 0;
    node->left = node->right = node->parent = NULL;
    return node;
}

void right_rotate(struct node **root, struct node *x) {
    struct node *y = x->left;
    x->left = y->right;
    if (y->right != NULL)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void left_rotate(struct node **root, struct node *x) {
    struct node *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void splay_original(struct node **root, struct node *x) {
    while (x->parent != NULL) {
        if (x->parent->parent == NULL) {
            if (x == x->parent->left) {
                right_rotate(root, x->parent);
                cost_original++;
            } else {
                left_rotate(root, x->parent);
                cost_original++;
            }
        } else if (x == x->parent->left && x->parent == x->parent->parent->left) {
            right_rotate(root, x->parent->parent);
            right_rotate(root, x->parent);
            cost_original += 2;
        } else if (x == x->parent->right && x->parent == x->parent->parent->right) {
            left_rotate(root, x->parent->parent);
            left_rotate(root, x->parent);
            cost_original += 2;
        } else if (x == x->parent->right && x->parent == x->parent->parent->left) {
            left_rotate(root, x->parent);
            right_rotate(root, x->parent);
            cost_original += 2;
        } else {
            right_rotate(root, x->parent);
            left_rotate(root, x->parent);
            cost_original += 2;
        }
    }
}

void splay_mod(struct node **root, struct node *x) {
    while (x->parent != NULL) {
        if (x->parent->parent == NULL) {
            if (x == x->parent->left) {
                right_rotate(root, x->parent);
                cost_mod++;
            } else {
                left_rotate(root, x->parent);
                cost_mod++;
            }
        } else if (x == x->parent->left && x->parent == x->parent->parent->left) {
            right_rotate(root, x->parent->parent);
            right_rotate(root, x->parent);
            cost_mod += 2;
        } else if (x == x->parent->right && x->parent == x->parent->parent->right) {
            left_rotate(root, x->parent->parent);
            left_rotate(root, x->parent);
            cost_mod += 2;
        } else if (x == x->parent->right && x->parent == x->parent->parent->left) {
            left_rotate(root, x->parent);
            right_rotate(root, x->parent);
            cost_mod += 2;
        } else {
            right_rotate(root, x->parent);
            left_rotate(root, x->parent);
            cost_mod += 2;
        }
    }
}

struct node* insert_original(struct node *root, int k) {
    struct node *z = root;
    struct node *p = NULL;

    while (z != NULL) {
        p = z;
        if (k == z->key) {
            z->count++;
            splay_original(&root, z);
            cost_original++;
            return root;
        } else if (k < z->key) {
            z = z->left;
        } else {
            z = z->right;
        }
        cost_original++;
    }

    z = newNode(k);
    z->parent = p;

    if (p == NULL) {
        root = z;
    } else if (k < p->key) {
        p->left = z;
    } else {
        p->right = z;
    }

    splay_original(&root, z);

    return root;
}

struct node* insert_mod(struct node *root, int k) {
    struct node *z = root;
    struct node *p = NULL;

    while (z != NULL) {
        p = z;
        if (k == z->key) {
            z->count++;
            if (root == NULL || z->count > root->count) {
                splay_mod(&root, z);
            }
            cost_mod++;
            return root;
        } else if (k < z->key) {
            z = z->left;
        } else {
            z = z->right;
        }
        cost_mod++;
    }

    z = newNode(k);
    z->parent = p;

    if (p == NULL) {
        root = z;
    } else if (k < p->key) {
        p->left = z;
    } else {
        p->right = z;
    }

    if (root == NULL || z->count > root->count) {
        splay_mod(&root, z);
    }

    return root;
}

void preOrder_original(struct node *root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preOrder_original(root->left);
        preOrder_original(root->right);
    }
}

void preOrder_mod(struct node *root) {
    if (root != NULL) {
        printf("(%d,%d) ", root->key, root->count);
        preOrder_mod(root->left);
        preOrder_mod(root->right);
    }
}

void readInputFile(const char *filename, int *keys, int *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    char *token = strtok(buffer, ",");
    while (token != NULL) {
        keys[*n] = atoi(token);
        (*n)++;
        token = strtok(NULL, ",");
    }
}

int main() {
    struct node *root_original = NULL;
    struct node *root_mod = NULL;

    int keys[100];
    int n = 0;
    readInputFile("input.txt", keys, &n);

    for (int i = 0; i < n; i++) {
        root_original = insert_original(root_original, keys[i]);
        root_mod = insert_mod(root_mod, keys[i]);
    }

    printf("Splay Tree\n");
    printf("Tree (pre-order traversal): ");
    preOrder_original(root_original);
    printf("\nCost: %d\n\n", cost_original);

    printf("Mod-Splay Tree\n");
    printf("Tree (pre-order traversal): ");
    preOrder_mod(root_mod);
    printf("\nCost: %d\n", cost_mod);

    return 0;
}