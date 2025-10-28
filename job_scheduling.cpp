#include <iostream>
#include <vector>
#include <algorithm> // For std::sort
#include <iomanip>   // For display

using namespace std;

struct Job {
    char id;       // Job ID (e.g., 'A', 'B')
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

    // Dynamic Memory Allocation using std::vector for time slots.
    // Index 'i' represents time slot i (slots 1 to maxDeadline).
    // Initialized with '\0' (meaning time slot is free).
    // This vector is dynamically sized based on the user-provided deadlines.
    vector<char> timeSlots(maxDeadline + 1, '\0'); 
    int totalProfit = 0;
    
    cout << "\n--- Scheduling Process (Max Profit First) ---" << endl;

    // Step 2: Iterate through the sorted jobs and schedule them
    for (const auto& job : jobs) {
        // Try to place the job as late as possible, up to its deadline.
        // Loop backward from the job's deadline to time slot 1.
        for (int j = job.deadline; j >= 1; --j) {
            // Check if the current time slot 'j' is free
            if (timeSlots[j] == '\0') {
                // Schedule the job in this slot
                timeSlots[j] = job.id;
                totalProfit += job.profit;
                
                cout << "Scheduled Job " << job.id << " (Profit: " << job.profit 
                     << ") in time slot **" << j << "**." << endl;
                
                // Once scheduled, move to the next job
                break;
            }
        }
    }

    // 4. Output Sequence
    cout << "------------------------------------------" << endl;
    cout << "Final Scheduled Sequence (by time slot): ";
    for (int i = 1; i <= maxDeadline; ++i) {
        if (timeSlots[i] != '\0') {
            cout << timeSlots[i] << " (Slot " << i << ") | ";
        }
    }
    cout << "\n------------------------------------------" << endl;

    return totalProfit;
}

// 4. Main Function for User Input and Execution
int main() {
    int numJobs;

    // --- User Input for Number of Jobs ---
    cout << "Enter the number of jobs (N): ";
    if (!(cin >> numJobs) || numJobs <= 0) {
        cerr << "Invalid number of jobs entered." << endl;
        return 1;
    }

    // Dynamic Memory Allocation using std::vector
    // This vector dynamically holds the Job structs.
    vector<Job> jobs;
    
    // --- User Input for Job Details ---
    cout << "\nEnter the Job ID (single letter), Deadline, and Profit for each job:" << endl;
    for (int i = 0; i < numJobs; ++i) {
        char id;
        int deadline, profit;
        
        cout << "Job " << i + 1 << " (ID, Deadline, Profit): ";
        // Clear newline/whitespace before reading char
        cin >> id >> deadline >> profit; 

        if (deadline <= 0 || profit < 0) {
            cerr << "Invalid Deadline or Profit for Job " << i + 1 << endl;
            return 1;
        }
        
        // Add job to the dynamically allocated vector
        jobs.push_back({id, deadline, profit});
    }

    // --- Run Algorithm and Display Result ---
    int maxProfit = scheduleJobs(jobs);

    cout << "\nMaximum Total Profit achievable: **" << maxProfit << "**" << endl;

    return 0;
}