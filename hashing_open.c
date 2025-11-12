#include <stdio.h>
#include <stdlib.h>

// --- Configuration ---
#define TABLE_SIZE 10 // The divisor 'm' for the hash function
#define EMPTY 0       // Value to indicate an empty slot (for simplicity, assuming keys are positive)

// --- Structure for the Linked List (Chaining) ---
struct Node {
    int key;
    struct Node* next;
};

// Global Hash Table Array (Array of linked list pointers/heads)
struct Node* hashTable[TABLE_SIZE];

// Global collision counter
int collisionCount = 0;

// --- 1. Division Hash Function ---
/**
 * Calculates the hash index using the Division Method: h(k) = k mod m
 */
int divisionHash(int key) {
    return key % TABLE_SIZE;
}

// --- 2. Insertion using Open Hashing (Chaining) ---
/**
 * Inserts a key into the hash table, resolving collisions via chaining.
 */
void insert(int key) {
    if (key <= 0) {
        printf("Error: Key must be positive.\n");
        return;
    }

    // Calculate the index
    int index = divisionHash(key);

    // Create a new node for the key
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->key = key;
    newNode->next = NULL;

    printf("\nInserting Key %d:\n", key);
    printf("  1. Hash Index calculated: %d\n", index);

    // Check if the slot is empty
    if (hashTable[index] == NULL) {
        // No collision, insert as the head
        hashTable[index] = newNode;
        printf("  2. Index was EMPTY. Key %d placed as the head of the chain.\n", key);
    } else {
        // Collision detected: use chaining
        printf("  2. **COLLISION** detected at index %d (Current head: %d).\n", index, hashTable[index]->key);
        
        // Step-by-step resolution: Traverse to the end of the list and append
        struct Node* current = hashTable[index];
        struct Node* previous = NULL;
        int chainLength = 0;
        
        // Traverse to the end of the chain
        while (current != NULL) {
            if (current->key == key) {
                printf("  Key %d already exists. Skipping insertion.\n", key);
                free(newNode);
                return;
            }
            previous = current;
            current = current->next;
            chainLength++;
        }
        
        // Append the new node to the end of the chain
        if (previous != NULL) {
             previous->next = newNode;
             collisionCount++; // A collision occurred because we had to chain
             printf("  3. Collision resolved by **CHAINING**. Key %d linked to the end of the chain (length %d).\n", key, chainLength + 1);
        }
    }
}

// --- Display Function ---
void displayTable() {
    printf("\n--- Final Hash Table (Chaining) ---\n");
    for (int i = 0; i < TABLE_SIZE; ++i) {
        printf("Bucket %2d: ", i);
        struct Node* current = hashTable[i];
        if (current == NULL) {
            printf("EMPTY\n");
        } else {
            while (current != NULL) {
                printf("%d", current->key);
                if (current->next != NULL) {
                    printf(" -> ");
                }
                current = current->next;
            }
            printf("\n");
        }
    }
    printf("--------------------------------------\n");
}

// --- Main Function ---
int main() {
    // Initialize the hash table pointers to NULL
    for (int i = 0; i < TABLE_SIZE; ++i) {
        hashTable[i] = NULL;
    }

    // Sample keys for demonstration
    int keys[] = {5, 15, 25, 30, 8, 18, 4};
    int numKeys = sizeof(keys) / sizeof(keys[0]);

    printf("--- Hashing Demonstration: Division Method (m=%d) & Chaining ---\n", TABLE_SIZE);

    // Insert all keys
    for (int i = 0; i < numKeys; ++i) {
        insert(keys[i]);
    }

    displayTable();

    printf("\n--- Summary ---\n");
    printf("Total Number of Collisions Resolved via Chaining: **%d**\n", collisionCount);
    
    // Free allocated memory (Good practice)
    for (int i = 0; i < TABLE_SIZE; ++i) {
        struct Node* current = hashTable[i];
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    return 0;
}