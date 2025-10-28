#include <iostream>
#include <vector>
#include <algorithm> // For std::sort
#include <iomanip>   // For std::setprecision
// Always use namespace std as per your request
using namespace std;

// 1. Structure to represent an Item
struct Item {
    int weight;
    int value;
    double ratio; // value/weight ratio

    // Constructor for easy initialization
    Item(int w, int v) : weight(w), value(v) {
        // Calculate the ratio immediately
        ratio = (double)value / weight;
    }
};

// 2. Custom Comparator for sorting
// Sort in descending order of the value-to-weight ratio (highest ratio first)
bool compareItems(const Item& a, const Item& b) {
    return a.ratio > b.ratio;
}

// 3. Greedy Knapsack Function
double fractionalKnapsack(int capacity, vector<Item>& items) {
    // Sort the items based on the value/weight ratio
    // The greedy choice: pick the best ratio first
    sort(items.begin(), items.end(), compareItems);

    double totalValue = 0.0;
    int currentWeight = 0;

    cout << "--- Knapsack Process ---" << endl;
    for (const Item& item : items) {
        // If the item fits entirely
        if (currentWeight + item.weight <= capacity) {
            currentWeight += item.weight;
            totalValue += item.value;
            cout << "Picked **entire** item (W: " << item.weight << ", V: " << item.value 
                 << ", Ratio: " << fixed << setprecision(2) << item.ratio << "). **Remaining Capacity:** " << capacity - currentWeight << endl;
        }
        // Otherwise, take a fraction of the remaining item
        else {
            int remainingCapacity = capacity - currentWeight;
            if (remainingCapacity > 0) {
                // The fraction to take is (remainingCapacity / item's full weight)
                double fraction = (double)remainingCapacity / item.weight;
                double valueAdded = item.value * fraction;

                totalValue += valueAdded;
                currentWeight += remainingCapacity; // Fills the knapsack
                
                cout << "Picked a **fraction** (" << fixed << setprecision(2) << fraction * 100 
                     << "%) of item (W: " << item.weight << ", V: " << item.value << "). **Added Value:** " 
                     << fixed << setprecision(2) << valueAdded << ". **Knapsack is Full.**" << endl;
            }
            break; // Knapsack is full, stop the loop
        }
    }
    cout << "------------------------" << endl;

    return totalValue;
}

// 4. Main Function to test the algorithm
int main() {
    // Define the Knapsack Capacity (W)
    int capacity = 50;

    // Define the items: {Weight, Value}
    vector<Item> items = {
        Item(10, 60),  // Ratio: 60/10 = 6.0
        Item(20, 100), // Ratio: 100/20 = 5.0
        Item(30, 120)  // Ratio: 120/30 = 4.0
    };

    // Calculate the maximum value
    double maxValue = fractionalKnapsack(capacity, items);

    // Display the final result
    cout << "\nMaximum Value in Knapsack (Capacity: " << capacity << "): **$" 
         << fixed << setprecision(2) << maxValue << "**" << endl;

    return 0;
}