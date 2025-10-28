#include <iostream>
#include <vector>
#include <algorithm> // For std::sort
#include <numeric>   // For std::accumulate

// Always use namespace std as per your request
using namespace std;

// 1. Structure to represent a Job
struct Job {
    char id;       // Job ID (e.g., 'a', 'b')
    int deadline;  // Maximum time unit by which the job must be finished
    int profit;    // Profit gained if the job is completed
};

// 2. Custom Comparator for sorting
// Sort in descending order of profit (highest profit first)
bool compareJobs(const Job& a, const Job& b) {
    return a.profit > b.profit;
}

// 3. Greedy Job Scheduling Function
int scheduleJobs(vector<Job>& jobs) {
    // Step 1: Sort all jobs by profit in descending order (The Greedy Choice)
    sort(jobs.begin(), jobs.end(), compareJobs);

    // Find the maximum deadline to determine the size of the time slot array
    int maxDeadline = 0;
    for (const auto& job : jobs) {
        if (job.deadline > maxDeadline) {
            maxDeadline = job.deadline;
        }
    }

    // Dynamic array (vector) to keep track of occupied time slots.
    // Index 'i' represents time slot i+1 (slots 1 to maxDeadline).
    // Initialized with 0 (meaning time slot is free).
    // The size is maxDeadline + 1 to use 1-based indexing for slots (easier).
    vector<char> timeSlots(maxDeadline + 1, '\0'); 
    int totalProfit = 0;
    int scheduledCount = 0;
    
    cout << "\n--- Job Scheduling Process ---" << endl;

    // Step 2: Iterate through the sorted jobs and schedule them
    for (const auto& job : jobs) {
        // Try to place the job as late as possible, up to its deadline.
        // We use 'job.deadline' because the last valid slot is at the deadline.
        // The loop goes backward (from deadline down to 1).
        for (int j = job.deadline; j >= 1; --j) {
            // Check if the current time slot 'j' is free
            if (timeSlots[j] == '\0') {
                // Schedule the job in this slot
                timeSlots[j] = job.id;
                totalProfit += job.profit;
                scheduledCount++;
                
                cout << "Scheduled Job " << job.id << " (Profit: " << job.profit 
                     << ", Deadline: " << job.deadline << ") in time slot **" << j << "**." << endl;
                
                // Once a job is scheduled, break the inner loop and go to the next job
                break;
            }
        }
    }

    // 4. Output Results
    cout << "------------------------------" << endl;
    cout << "Total Scheduled Jobs: " << scheduledCount << endl;
    
    cout << "Scheduled Jobs Sequence (by time slot): ";
    for (int i = 1; i <= maxDeadline; ++i) {
        if (timeSlots[i] != '\0') {
            cout << timeSlots[i] << " (Slot " << i << ") | ";
        }
    }
    cout << "\n------------------------------" << endl;

    return totalProfit;
}

// 4. Main Function to test the algorithm
int main() {
    // Define the jobs: {ID, Deadline, Profit}
    // Dynamic memory is used for the vector container.
    vector<Job> jobs = {
        {'a', 2, 100}, // Ratio: 100
        {'b', 1, 19},  // Ratio: 19
        {'c', 2, 27},  // Ratio: 27
        {'d', 1, 25},  // Ratio: 25
        {'e', 3, 15}   // Ratio: 15
    };

    // Calculate the maximum profit
    int maxProfit = scheduleJobs(jobs);

    // Display the final result
    cout << "\nMaximum Total Profit achievable: **" << maxProfit << "**" << endl;

    return 0;
}