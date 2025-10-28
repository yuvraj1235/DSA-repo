#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

// User information: Always use namespace std.
using namespace std;

// Structure to represent an activity/interval
struct Activity {
    int start;
    int finish;
    int id;
};

// Custom comparison function for the greedy choice:
// Sorts activities based on their **Finish Time** in ascending order.
bool compareActivities(const Activity& a, const Activity& b) {
    return a.finish < b.finish;
}

/**
 * @brief Implements the Greedy Interval Scheduling algorithm.
 * @param activities The list of available activities (dynamically sized).
 */
void scheduleActivities(vector<Activity>& activities) {
    int n = activities.size();
    
    if (n == 0) {
        cout << "No activities to schedule." << endl;
        return;
    }

    // --- 1. THE GREEDY STEP: Sort by Finish Time ---
    sort(activities.begin(), activities.end(), compareActivities);

    cout << "\n--- **Greedy Activity Schedule (Sorted by Finish Time)** ---" << endl;
    cout << left << setw(8) << "ID" 
         << setw(10) << "Start" 
         << setw(10) << "Finish" << endl;
    cout << setfill('-') << setw(28) << "-" << setfill(' ') << endl;
    
    // Display sorted list
    for(const auto& act : activities) {
        cout << left << setw(8) << act.id 
             << setw(10) << act.start 
             << setw(10) << act.finish << endl;
    }
    
    cout << setfill('-') << setw(28) << "-" << setfill(' ') << endl;

    // --- 2. Select compatible activities ---
    vector<Activity> selectedActivities;
    
    // The first activity in the sorted list is always selected
    selectedActivities.push_back(activities[0]);
    int lastFinishTime = activities[0].finish;

    for (int i = 1; i < n; i++) {
        // If the current activity's start time is AFTER the last selected activity's finish time
        if (activities[i].start >= lastFinishTime) {
            selectedActivities.push_back(activities[i]);
            lastFinishTime = activities[i].finish; 
        }
    }

    // --- 3. Final Result ---
    cout << "\n✅ **Maximum Activities Selected (" << selectedActivities.size() << " Total)**:" << endl;
    cout << setfill('-') << setw(50) << "-" << setfill(' ') << endl;
    for (const auto& act : selectedActivities) {
        cout << "ID " << act.id << " (Starts: " << act.start << ", Ends: " << act.finish << ")" << endl;
    }
    cout << setfill('-') << setw(50) << "-" << setfill(' ') << endl;
}

// Driver code
int main() {
    int n;
    
    // **1. User Input for Size**
    cout << "Enter the number of activities/intervals: ";
    if (!(cin >> n) || n <= 0) {
        cerr << "Invalid number of activities. Exiting." << endl;
        return 1;
    }

    // **2. Dynamic Storage using std::vector**
    // std::vector handles dynamic memory allocation and deallocation automatically.
    vector<Activity> activities;
    
    // **3. User Input for Activity Details**
    cout << "\nEnter Start Time and Finish Time for each activity:" << endl;
    for (int i = 0; i < n; ++i) {
        Activity temp;
        temp.id = i + 1; // Assign ID
        
        cout << "Activity " << temp.id << ":" << endl;
        cout << "  Start Time: ";
        cin >> temp.start;
        cout << "  Finish Time: ";
        cin >> temp.finish;
        
        // Add the activity to the dynamic vector
        activities.push_back(temp);
    }
    
    // **4. Run Interval Scheduling Algorithm**
    scheduleActivities(activities);

    // *Note: Since we used std::vector, we do not need to call 'delete' manually.*

    return 0;
}