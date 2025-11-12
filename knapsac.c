#include <stdio.h>

struct Item {
    int profit, weight;
    float ratio;
};

void maxProfitMethod(struct Item items[], int n, int W);
void minWeightMethod(struct Item items[], int n, int W);
void ratioMethod(struct Item items[], int n, int W);

int main() {
    int n, W, choice;
    printf("\n--- FRACTIONAL KNAPSACK PROBLEM ---\n");
    printf("Enter number of items: ");
    scanf("%d", &n);
    struct Item items[n];
    
    printf("Enter profit and weight of each item:\n");
    for (int i = 0; i < n; i++) {
        printf("Item %d: ", i + 1);
        scanf("%d %d", &items[i].profit, &items[i].weight);
        items[i].ratio = (float)items[i].profit / items[i].weight;
    }

    printf("Enter knapsack capacity: ");
    scanf("%d", &W);

    do {
        printf("\nMENU:\n");
        printf("1. Select item based on Maximum Profit\n");
        printf("2. Select item based on Minimum Weight\n");
        printf("3. Select item based on Profit/Weight Ratio\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: maxProfitMethod(items, n, W); break;
            case 2: minWeightMethod(items, n, W); break;
            case 3: ratioMethod(items, n, W); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 4);

    return 0;
}

void maxProfitMethod(struct Item items[], int n, int W) {
    struct Item temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (items[i].profit < items[j].profit) {
                temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }

    float totalProfit = 0, remain = W;
    for (int i = 0; i < n && remain > 0; i++) {
        if (items[i].weight <= remain) {
            remain -= items[i].weight;
            totalProfit += items[i].profit;
        } else {
            totalProfit += items[i].profit * (remain / items[i].weight);
            remain = 0;
        }
    }
    printf("Maximum Profit (by Max Profit method): %.2f\n", totalProfit);
}

void minWeightMethod(struct Item items[], int n, int W) {
    struct Item temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (items[i].weight > items[j].weight) {
                temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }

    float totalProfit = 0, remain = W;
    for (int i = 0; i < n && remain > 0; i++) {
        if (items[i].weight <= remain) {
            remain -= items[i].weight;
            totalProfit += items[i].profit;
        } else {
            totalProfit += items[i].profit * (remain / items[i].weight);
            remain = 0;
        }
    }
    printf("Maximum Profit (by Min Weight method): %.2f\n", totalProfit);
}

void ratioMethod(struct Item items[], int n, int W) {
    struct Item temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (items[i].ratio < items[j].ratio) {
                temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }

    float totalProfit = 0, remain = W;
    for (int i = 0; i < n && remain > 0; i++) {
        if (items[i].weight <= remain) {
            remain -= items[i].weight;
            totalProfit += items[i].profit;
        } else {
            totalProfit += items[i].profit * (remain / items[i].weight);
            remain = 0;
        }
    }
    printf("Maximum Profit (by Ratio method): %.2f\n", totalProfit);
}
