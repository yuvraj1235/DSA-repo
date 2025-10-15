#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_DEGREE 3  // Minimum degree (minimum number of keys is t-1)

// B-tree node structure
typedef struct BTreeNode {
    int *keys;                      // Array of keys
    int t;                          // Minimum degree
    struct BTreeNode **children;    // Array of child pointers
    int n;                          // Current number of keys
    bool leaf;                      // Is true if node is leaf
} BTreeNode;

// Function prototypes
BTreeNode* createNode(int t, bool leaf);
void traverse(BTreeNode *root);
void printTree(BTreeNode *root, int level);
void displayTreeStructure(BTreeNode *root);
BTreeNode* search(BTreeNode *root, int key);
void insert(BTreeNode **root, int key, int t);
void insertNonFull(BTreeNode *node, int key);
void splitChild(BTreeNode *parent, int i, BTreeNode *fullChild);
void delete(BTreeNode **root, int key);
void deleteFromNode(BTreeNode *node, int key);
int getPredecessor(BTreeNode *node, int idx);
int getSuccessor(BTreeNode *node, int idx);
void fill(BTreeNode *node, int idx);
void borrowFromPrev(BTreeNode *node, int idx);
void borrowFromNext(BTreeNode *node, int idx);
void merge(BTreeNode *node, int idx);
void freeTree(BTreeNode *root);

// Create a new B-tree node
BTreeNode* createNode(int t, bool leaf) {
    BTreeNode *node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (int*)malloc(sizeof(int) * (2 * t - 1));
    node->children = (BTreeNode**)malloc(sizeof(BTreeNode*) * (2 * t));
    node->n = 0;
    return node;
}

// Traverse the tree (in-order traversal)
void traverse(BTreeNode *root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->n; i++) {
            if (!root->leaf)
                traverse(root->children[i]);
            printf("%d ", root->keys[i]);
        }
        if (!root->leaf)
            traverse(root->children[i]);
    }
}

// Print tree in hierarchical structure
void printTree(BTreeNode *root, int level) {
    if (root != NULL) {
        int i;
        
        // Print current node with indentation
        for (int j = 0; j < level; j++)
            printf("    ");
        
        printf("Level %d: [", level);
        for (i = 0; i < root->n; i++) {
            printf("%d", root->keys[i]);
            if (i < root->n - 1)
                printf(", ");
        }
        printf("]");
        
        if (root->leaf)
            printf(" (Leaf)");
        printf("\n");
        
        // Recursively print children
        if (!root->leaf) {
            for (i = 0; i <= root->n; i++) {
                printTree(root->children[i], level + 1);
            }
        }
    }
}

// Display tree structure wrapper function
void displayTreeStructure(BTreeNode *root) {
    if (root == NULL) {
        printf("\nTree is empty!\n");
        return;
    }
    printf("\n========== B-TREE STRUCTURE ==========\n");
    printTree(root, 0);
    printf("======================================\n");
}

// Search for a key in the tree
BTreeNode* search(BTreeNode *root, int key) {
    if (root == NULL)
        return NULL;
    
    int i = 0;
    while (i < root->n && key > root->keys[i])
        i++;
    
    if (i < root->n && key == root->keys[i])
        return root;
    
    if (root->leaf)
        return NULL;
    
    return search(root->children[i], key);
}

// Insert a key into the B-tree
void insert(BTreeNode **root, int key, int t) {
    if (*root == NULL) {
        *root = createNode(t, true);
        (*root)->keys[0] = key;
        (*root)->n = 1;
    } else {
        if ((*root)->n == 2 * t - 1) {
            BTreeNode *newRoot = createNode(t, false);
            newRoot->children[0] = *root;
            splitChild(newRoot, 0, *root);
            
            int i = 0;
            if (newRoot->keys[0] < key)
                i++;
            insertNonFull(newRoot->children[i], key);
            
            *root = newRoot;
        } else {
            insertNonFull(*root, key);
        }
    }
}

// Insert into a node that is not full
void insertNonFull(BTreeNode *node, int key) {
    int i = node->n - 1;
    
    if (node->leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        while (i >= 0 && node->keys[i] > key)
            i--;
        i++;
        
        if (node->children[i]->n == 2 * node->t - 1) {
            splitChild(node, i, node->children[i]);
            if (node->keys[i] < key)
                i++;
        }
        insertNonFull(node->children[i], key);
    }
}

// Split a full child of a node
void splitChild(BTreeNode *parent, int i, BTreeNode *fullChild) {
    int t = fullChild->t;
    BTreeNode *newChild = createNode(t, fullChild->leaf);
    newChild->n = t - 1;
    
    for (int j = 0; j < t - 1; j++)
        newChild->keys[j] = fullChild->keys[j + t];
    
    if (!fullChild->leaf) {
        for (int j = 0; j < t; j++)
            newChild->children[j] = fullChild->children[j + t];
    }
    
    fullChild->n = t - 1;
    
    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];
    
    parent->children[i + 1] = newChild;
    
    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];
    
    parent->keys[i] = fullChild->keys[t - 1];
    parent->n++;
}

// Delete a key from the B-tree
void delete(BTreeNode **root, int key) {
    if (*root == NULL) {
        printf("Tree is empty\n");
        return;
    }
    
    deleteFromNode(*root, key);
    
    if ((*root)->n == 0) {
        BTreeNode *tmp = *root;
        if ((*root)->leaf)
            *root = NULL;
        else
            *root = (*root)->children[0];
        free(tmp->keys);
        free(tmp->children);
        free(tmp);
    }
}

// Delete from a node
void deleteFromNode(BTreeNode *node, int key) {
    int idx = 0;
    while (idx < node->n && node->keys[idx] < key)
        idx++;
    
    if (idx < node->n && node->keys[idx] == key) {
        if (node->leaf) {
            for (int i = idx + 1; i < node->n; i++)
                node->keys[i - 1] = node->keys[i];
            node->n--;
        } else {
            if (node->children[idx]->n >= node->t) {
                int pred = getPredecessor(node, idx);
                node->keys[idx] = pred;
                deleteFromNode(node->children[idx], pred);
            } else if (node->children[idx + 1]->n >= node->t) {
                int succ = getSuccessor(node, idx);
                node->keys[idx] = succ;
                deleteFromNode(node->children[idx + 1], succ);
            } else {
                merge(node, idx);
                deleteFromNode(node->children[idx], key);
            }
        }
    } else {
        if (node->leaf) {
            printf("Key %d not found in tree\n", key);
            return;
        }
        
        bool flag = (idx == node->n);
        
        if (node->children[idx]->n < node->t)
            fill(node, idx);
        
        if (flag && idx > node->n)
            deleteFromNode(node->children[idx - 1], key);
        else
            deleteFromNode(node->children[idx], key);
    }
}

// Get predecessor key
int getPredecessor(BTreeNode *node, int idx) {
    BTreeNode *curr = node->children[idx];
    while (!curr->leaf)
        curr = curr->children[curr->n];
    return curr->keys[curr->n - 1];
}

// Get successor key
int getSuccessor(BTreeNode *node, int idx) {
    BTreeNode *curr = node->children[idx + 1];
    while (!curr->leaf)
        curr = curr->children[0];
    return curr->keys[0];
}

// Fill child node
void fill(BTreeNode *node, int idx) {
    if (idx != 0 && node->children[idx - 1]->n >= node->t)
        borrowFromPrev(node, idx);
    else if (idx != node->n && node->children[idx + 1]->n >= node->t)
        borrowFromNext(node, idx);
    else {
        if (idx != node->n)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

// Borrow from previous sibling
void borrowFromPrev(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx - 1];
    
    for (int i = child->n - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];
    
    if (!child->leaf) {
        for (int i = child->n; i >= 0; i--)
            child->children[i + 1] = child->children[i];
    }
    
    child->keys[0] = node->keys[idx - 1];
    
    if (!child->leaf)
        child->children[0] = sibling->children[sibling->n];
    
    node->keys[idx - 1] = sibling->keys[sibling->n - 1];
    
    child->n++;
    sibling->n--;
}

// Borrow from next sibling
void borrowFromNext(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];
    
    child->keys[child->n] = node->keys[idx];
    
    if (!child->leaf)
        child->children[child->n + 1] = sibling->children[0];
    
    node->keys[idx] = sibling->keys[0];
    
    for (int i = 1; i < sibling->n; i++)
        sibling->keys[i - 1] = sibling->keys[i];
    
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; i++)
            sibling->children[i - 1] = sibling->children[i];
    }
    
    child->n++;
    sibling->n--;
}

// Merge a child with its sibling
void merge(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];
    
    child->keys[node->t - 1] = node->keys[idx];
    
    for (int i = 0; i < sibling->n; i++)
        child->keys[i + node->t] = sibling->keys[i];
    
    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; i++)
            child->children[i + node->t] = sibling->children[i];
    }
    
    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];
    
    for (int i = idx + 2; i <= node->n; i++)
        node->children[i - 1] = node->children[i];
    
    child->n += sibling->n + 1;
    node->n--;
    
    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

// Free the entire tree
void freeTree(BTreeNode *root) {
    if (root != NULL) {
        if (!root->leaf) {
            for (int i = 0; i <= root->n; i++)
                freeTree(root->children[i]);
        }
        free(root->keys);
        free(root->children);
        free(root);
    }
}

// Display menu
void displayMenu() {
    printf("\n========== B-TREE MENU ==========\n");
    printf("1. Insert a key\n");
    printf("2. Delete a key\n");
    printf("3. Search for a key\n");
    printf("4. Display tree (In-order Traversal)\n");
    printf("5. Display tree structure (Hierarchical)\n");
    printf("6. Exit\n");
    printf("=================================\n");
    printf("Enter your choice: ");
}

// Main function with menu-driven interface
int main() {
    BTreeNode *root = NULL;
    int t = MIN_DEGREE;
    int choice, key;
    BTreeNode *result;
    
    printf("\n*** B-TREE IMPLEMENTATION ***\n");
    printf("Minimum Degree (t) = %d\n", t);
    printf("Each node can have %d to %d keys\n", t-1, 2*t-1);
    
    while (1) {
        displayMenu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("\nEnter key to insert: ");
                if (scanf("%d", &key) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n');
                    break;
                }
                insert(&root, key, t);
                printf("Key %d inserted successfully!\n", key);
                break;
                
            case 2:
                if (root == NULL) {
                    printf("\nTree is empty! Nothing to delete.\n");
                    break;
                }
                printf("\nEnter key to delete: ");
                if (scanf("%d", &key) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n');
                    break;
                }
                delete(&root, key);
                printf("Deletion operation completed.\n");
                break;
                
            case 3:
                if (root == NULL) {
                    printf("\nTree is empty!\n");
                    break;
                }
                printf("\nEnter key to search: ");
                if (scanf("%d", &key) != 1) {
                    printf("Invalid input!\n");
                    while (getchar() != '\n');
                    break;
                }
                result = search(root, key);
                if (result != NULL)
                    printf("Key %d FOUND in the tree!\n", key);
                else
                    printf("Key %d NOT FOUND in the tree!\n", key);
                break;
                
            case 4:
                if (root == NULL) {
                    printf("\nTree is empty!\n");
                    break;
                }
                printf("\nIn-order Traversal: ");
                traverse(root);
                printf("\n");
                break;
                
            case 5:
                displayTreeStructure(root);
                break;
                
            case 6:
                printf("\nFreeing memory and exiting...\n");
                freeTree(root);
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("\nInvalid choice! Please enter a number between 1 and 6.\n");
        }
    }
    
    return 0;
}