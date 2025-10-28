#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

// User information: Always use namespace std.
using namespace std;

// Structure to hold process details
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
};

// Custom comparison function for sorting processes based on Burst Time
bool compareBurstTime(const Process& a, const Process& b) {
    return a.burstTime < b.burstTime;
}

// Function to calculate waiting and turnaround times
void findAvgTime(Process* processes, int n) {
    // 1. Convert the dynamic array (pointer) to a vector for easy sorting
    //    We need a vector to easily sort the 'ready queue' by burst time later.
    vector<Process> allProcesses(processes, processes + n);
    
    // Sort all processes by Arrival Time initially
    sort(allProcesses.begin(), allProcesses.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    int completed = 0;
    vector<Process> readyQueue;
    int processIndex = 0; 
    float total_wt = 0;
    float total_tat = 0;

    // Main loop: continues until all processes are completed
    while (completed < n) {
        
        // Add processes that have arrived to the ready queue
        while (processIndex < n && allProcesses[processIndex].arrivalTime <= currentTime) {
            readyQueue.push_back(allProcesses[processIndex]);
            processIndex++;
        }
        
        if (!readyQueue.empty()) {
            // **Greedy Choice**: Sort the ready queue by shortest burst time
            sort(readyQueue.begin(), readyQueue.end(), compareBurstTime);
            
            // Pick the shortest job
            Process p = readyQueue.front(); 
            
            // Execute the chosen process
            currentTime += p.burstTime;
            
            // Calculate metrics for the completed process
            int completion = currentTime;
            int turnAround = completion - p.arrivalTime;
            int waiting = turnAround - p.burstTime;

            // Update the metrics in the original dynamic array for final display
            for (int i = 0; i < n; i++) {
                if (processes[i].id == p.id) {
                    processes[i].completionTime = completion;
                    processes[i].turnAroundTime = turnAround;
                    processes[i].waitingTime = waiting;
                    total_wt += waiting;
                    total_tat += turnAround;
                    break;
                }
            }
            
            // Remove the completed process from the ready queue
            readyQueue.erase(readyQueue.begin());
            completed++;
            
        } else {
            // CPU is idle. Advance time to the arrival of the next process
            if (processIndex < n) {
                currentTime = allProcesses[processIndex].arrivalTime;
            } else {
                break;
            }
        }
    }

    // Display results (The processes array now holds the calculated results)
    cout << "\n--- **SJF (Shortest Job First) Results** ---" << endl;
    cout << left << setw(8) << "PID" 
         << setw(10) << "Arrival" 
         << setw(8) << "Burst" 
         << setw(12) << "Completion" 
         << setw(12) << "TurnAround" 
         << setw(10) << "Waiting" << endl;
    
    cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;

    for (int i = 0; i < n; i++) {
        cout << left << setw(8) << processes[i].id 
             << setw(10) << processes[i].arrivalTime 
             << setw(8) << processes[i].burstTime 
             << setw(12) << processes[i].completionTime 
             << setw(12) << processes[i].turnAroundTime 
             << setw(10) << processes[i].waitingTime << endl;
    }
    
    cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;

    // Output average times
    cout << fixed << setprecision(2);
    cout << "\n**Average Waiting Time** = " << (total_wt / n) << endl;
    cout << "**Average Turn Around Time** = " << (total_tat / n) << endl;
}

int main() {
    int n; // Variable to store the number of processes
    
    // **1. User Input for Size**
    cout << "Enter the number of processes: ";
    if (!(cin >> n) || n <= 0) {
        cerr << "Invalid number of processes. Exiting." << endl;
        return 1;
    }

    // **2. Dynamic Memory Allocation**
    // Allocate memory on the heap for 'n' Process structures using the 'new' operator.
    Process* processes = new Process[n];
    
    // **3. User Input for Process Details**
    cout << "\nEnter Arrival Time and Burst Time for each process:" << endl;
    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1; // Assign ID
        processes[i].completionTime = 0; // Initialize metrics
        processes[i].turnAroundTime = 0;
        processes[i].waitingTime = 0;
        
        cout << "Process " << processes[i].id << " (PID " << processes[i].id << "):" << endl;
        cout << "  Arrival Time: ";
        cin >> processes[i].arrivalTime;
        cout << "  Burst Time (Job Length): ";
        cin >> processes[i].burstTime;
    }
    
    // **4. Run SJF Algorithm**
    findAvgTime(processes, n);

    // **5. Dynamic Memory Deallocation**
    // Release the memory allocated on the heap using 'delete[]'. This is crucial.
    delete[] processes;
    processes = nullptr; // Set the pointer to nullptr after deleting

    return 0;
}