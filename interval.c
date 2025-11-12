#include <stdio.h>

struct Interval {
    int start, finish;
};

void sortByInterval(struct Interval arr[], int n);
void sortByFinish(struct Interval arr[], int n);
void schedule(struct Interval arr[], int n);

int main() {
    int n, choice;
    printf("\n--- INTERVAL SCHEDULING PROBLEM ---\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);
    struct Interval arr[n];

    printf("Enter start and finish time of each request:\n");
    for (int i = 0; i < n; i++) {
        printf("Request %d: ", i + 1);
        scanf("%d %d", &arr[i].start, &arr[i].finish);
    }

    do {
        printf("\nMENU:\n");
        printf("1. When requests have smallest interval of time\n");
        printf("2. When requests have minimum finish time\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sortByInterval(arr, n);
                schedule(arr, n);
                break;
            case 2:
                sortByFinish(arr, n);
                schedule(arr, n);
                break;
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);

    return 0;
}

void sortByInterval(struct Interval arr[], int n) {
    struct Interval temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++) {
            int interval_i = arr[i].finish - arr[i].start;
            int interval_j = arr[j].finish - arr[j].start;
            if (interval_i > interval_j) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
}

void sortByFinish(struct Interval arr[], int n) {
    struct Interval temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i].finish > arr[j].finish) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}

void schedule(struct Interval arr[], int n) {
    printf("\nSelected Requests:\n");
    int lastFinish = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i].start >= lastFinish) {
            printf("Request (%d, %d)\n", arr[i].start, arr[i].finish);
            lastFinish = arr[i].finish;
        }
    }
}
