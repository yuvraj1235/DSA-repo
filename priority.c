#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int heap[MAX];
int heapSize = 0;

// Swap function
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Insert into the priority queue (max-heap)
void insert(int value) {
    if (heapSize >= MAX) {
        printf("Priority Queue is full!\n");
        return;
    }

    heap[heapSize] = value; // Insert at the end
    int i = heapSize;
    heapSize++;

    // Move up (heapify)
    while (i != 0 && heap[(i - 1) / 2] < heap[i]) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    printf("Inserted %d\n", value);
}

// Delete the highest priority element
int deleteMax() {
    if (heapSize <= 0) {
        printf("Priority Queue is empty!\n");
        return -1;
    }

    int max = heap[0];
    heap[0] = heap[heapSize - 1];
    heapSize--;

    // Heapify down
    int i = 0;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < heapSize && heap[left] > heap[largest])
            largest = left;

        if (right < heapSize && heap[right] > heap[largest])
            largest = right;

        if (largest != i) {
            swap(&heap[i], &heap[largest]);
            i = largest;
        } else
            break;
    }

    return max;
}

// Display the heap (priority queue)
void display() {
    if (heapSize == 0) {
        printf("Priority Queue is empty!\n");
        return;
    }

    printf("Priority Queue elements: ");
    for (int i = 0; i < heapSize; i++)
        printf("%d ", heap[i]);
    printf("\n");
}

// Main function
int main() {
    int choice, value;

    while (1) {
        printf("\n--- Priority Queue Menu ---\n");
        printf("1. Insert\n");
        printf("2. Delete (highest priority)\n");
        printf("3. Display\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                insert(value);
                break;
            case 2:
                value = deleteMax();
                if (value != -1)
                    printf("Deleted highest priority element: %d\n", value);
                break;
            case 3:
                display();
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
