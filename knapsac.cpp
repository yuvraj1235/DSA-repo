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
    int id;       // To track the original item number

    // Constructor for easy initialization
    Item(int w, int v, int i) : weight(w), value(v), id(i) {
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
    sort(items.begin(), items.end(), compareItems);

    double totalValue = 0.0;
    int currentWeight = 0;

    cout << "\n--- Knapsack Process (Greedy Choice: Best Ratio First) ---" << endl;
    for (const Item& item : items) {
        // If the item fits entirely
        if (currentWeight + item.weight <= capacity) {
            currentWeight += item.weight;
            totalValue += item.value;
            cout << "Item " << item.id << ": Picked **entirely** (W: " << item.weight 
                 << ", V: " << item.value << ", Ratio: " << fixed << setprecision(2) << item.ratio 
                 << "). **Remaining Capacity:** " << capacity - currentWeight << endl;
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
                
                cout << "Item " << item.id << ": Picked a **fraction** (" << fixed << setprecision(2) 
                     << fraction * 100 << "%) of item. **Added Value:** " 
                     << fixed << setprecision(2) << valueAdded << ". **Knapsack is Full.**" << endl;
            }
            break; // Knapsack is full, stop the loop
        }
    }
    cout << "--------------------------------------------------------" << endl;

    return totalValue;
}

// 4. Main Function for User Input and Execution
int main() {
    int capacity;
    int numItems;

    // --- User Input for Capacity ---
    cout << "Enter the maximum capacity of the knapsack (W): ";
    if (!(cin >> capacity) || capacity <= 0) {
        cerr << "Invalid capacity entered." << endl;
        return 1;
    }

    // --- User Input for Number of Items ---
    cout << "Enter the number of items (N): ";
    if (!(cin >> numItems) || numItems <= 0) {
        cerr << "Invalid number of items entered." << endl;
        return 1;
    }

    // Dynamic Memory Allocation using std::vector
    // This vector dynamically grows to hold 'numItems' and is managed automatically.
    vector<Item> items;
    
    // --- User Input for Item Details ---
    cout << "\nEnter the weight and value for each item:" << endl;
    for (int i = 0; i < numItems; ++i) {
        int w, v;
        cout << "Item " << i + 1 << " (Weight, Value): ";
        if (!(cin >> w >> v) || w <= 0 || v < 0) {
            cerr << "Invalid weight or value for Item " << i + 1 << endl;
            return 1;
        }
        // Add item to the dynamically allocated vector
        items.emplace_back(w, v, i + 1);
    }

    // --- Run Algorithm and Display Result ---
    double maxValue = fractionalKnapsack(capacity, items);

    cout << "\nMaximum Value achievable in the Knapsack: **$" 
         << fixed << setprecision(2) << maxValue << "**" << endl;

    return 0;
}