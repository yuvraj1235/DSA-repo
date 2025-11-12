#include <stdio.h>
#include <stdlib.h>

// Structure for a singly linked list node
struct Node {
    int data;
    struct Node* next;
};

// --- Utility Functions ---

// Function to create a new node
struct Node* newNode(int data) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    temp->data = data;
    temp->next = NULL;
    return temp;
}

// Function to print the list
void printList(struct Node* node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

// Function to free the memory used by the list
void deleteList(struct Node* head) {
    struct Node* current = head;
    struct Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

// --- Merge Sort Core Functions ---

// 1. Merges two sorted lists into one sorted list (Recursive)
struct Node* SortedMerge(struct Node* a, struct Node* b) {
    // Base cases
    if (a == NULL) return b;
    if (b == NULL) return a;

    struct Node* result = NULL;

    // Pick either a or b, and recurse
    if (a->data <= b->data) {
        result = a;
        result->next = SortedMerge(a->next, b);
    } else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return result;
}

// 2. Splits the list into two halves (front and back) using fast/slow pointers
void FrontBackSplit(struct Node* source, struct Node** frontRef, struct Node** backRef) {
    struct Node* fast;
    struct Node* slow;

    // Handle the case of 0 or 1 node
    if (source == NULL || source->next == NULL) {
        *frontRef = source;
        *backRef = NULL;
        return;
    }

    slow = source;
    fast = source->next; 

    // Advance fast two nodes, slow one node
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    // 'slow' is at the end of the front list
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL; // Terminate the first half
}

// 3. Main recursive function to sort the list (Divide, Conquer, and Combine)
void MergeSort(struct Node** headRef) {
    struct Node* head = *headRef;
    struct Node* a; // Front half
    struct Node* b; // Back half

    // Base case: 0 or 1 element is already sorted
    if (head == NULL || head->next == NULL) {
        return;
    }

    // Divide: Split head into 'a' and 'b' sublists
    FrontBackSplit(head, &a, &b);

    // Conquer: Recursively sort the sublists
    MergeSort(&a);
    MergeSort(&b);

    // Combine: Merge the two sorted lists
    *headRef = SortedMerge(a, b);
}

// --- Main Program ---
int main() {
    // Create the unsorted linked list: 4 -> 2 -> 1 -> 5 -> 3
    struct Node* head = newNode(4);
    head->next = newNode(2);
    head->next->next = newNode(1);
    head->next->next->next = newNode(5);
    head->next->next->next->next = newNode(3);
    head->next->next->next->next->next = newNode(9);
    head->next->next->next->next->next->next = newNode(7);

    printf("Original List: ");
    printList(head);

    // Perform Merge Sort by passing the address of the head pointer
    MergeSort(&head);

    printf("Sorted List:   ");
    printList(head); // Output: 1 2 3 4 5 7 9 
    
    // Clean up memory
    deleteList(head);

    return 0;
}