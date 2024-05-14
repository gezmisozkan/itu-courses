/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 3
 * Özkan GEZMİŞ
 * 150200033
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

/* YOU CAN INCLUDE ANY OTHER NECESSARY LIBRARIES */
#include <map>
#include <cmath>
/* YOU CAN DEFINE EXTRA FUNCTIONS AND MODIFY THE CODE TEMPLATE */

// int binary_search(const std::vector<Schedule>& schedules, int i)

/* YOU HAVE TO CONSIDER THE OUTPUT FORMAT TO TEST YOUR CODE WITH CALICO */


using namespace std;


struct TimeIntervals {
    string floor, room;  // Floor and room numbers
    string str_start,str_finish;  // Keep the original string values
    int start, finish;  // Start and finish times in minutes

    TimeIntervals(string floor, string room, string start_time, string finish_time){  // Constructor
        this->str_start = start_time;  // Keep original string values
        this->str_finish = finish_time;  // Keep original string values
        this->start = stoi(start_time.substr(0, 2)) * 60 + stoi(start_time.substr(3, 2));  // Convert the start time to minutes
        this->finish = stoi(finish_time.substr(0, 2)) * 60 + stoi(finish_time.substr(3, 2));  // Convert the finish time to minutes
        this->floor = floor;
        this->room = room;
    }
    TimeIntervals(){}  // Default constructor
};

// struct Priority {
// I did not use this struct because I used a map to store the priorities
// };

struct Item {
    string name;  
    int weight;  // Price of the item
    double value;

    Item(string name, int weight, double value){  // Constructor
        this->name = name;
        this->weight = weight;
        this->value = value;
    }
};


// /* START: WEIGHTED INTERVAL SCHEDULING (for each floor) */

struct Schedule {  // Struct to store the schedule which includes the time interval and the priority
    TimeIntervals interval;  
    int priority;
    Schedule(TimeIntervals interval, int priority){
        this->interval = interval;
        this->priority = priority;
    }
};

// Function to find the last non-conflicting schedule for index i using binary search
int non_conflicting_bs(const vector<Schedule>& schedules, int i){
    int low = 0, high = i - 1;  // Initialize low and high
    while (low <= high) {  // Binary search
        int mid = (high + low) / 2;  // Calculate the middle index
        if (schedules[mid].interval.finish <= schedules[i].interval.start) {  
            if (schedules[mid + 1].interval.finish <= schedules[i].interval.start) {  // Check if the next schedule is also non-conflicting
                low = mid + 1;  // Move to the right
            } else {  // If the next schedule is conflicting, return the index of the last non-conflicting schedule
                return mid;
            }
        } else {  // If the current schedule is conflicting, move to the left
            high = mid - 1;
        }
    }
    return -1;  // If there is no non-conflicting schedule, return -1
}

int compare(const Schedule& a, const Schedule& b) {  // Compare function to sort the schedules in ascending order based on finish time
    return a.interval.finish < b.interval.finish;
}

// function to find the best schedule for each floor
vector<Schedule> weighted_interval_scheduling(vector<Schedule> schedules){
    sort(schedules.begin(), schedules.end(), compare);  // Sort using the compare function

    int n = schedules.size();  // Get the number of schedules
    vector<double> dp(n, 0);  // Initialize the dp table
    dp[0] = schedules[0].priority;  // Set the first value of the dp table to the priority of the first schedule
    
    // Create an array to store the index of the last non-conflicting job for each job
    vector<int> non_conflicting(n, -1);  
    for (int i = 1; i < n; i++) {  // Find the last non-conflicting schedule for each schedule
        non_conflicting[i] = non_conflicting_bs(schedules, i);
    }

    // Build the dp table in bottom-up manner
    for (int i = 1; i < n; i++) {  
        double select = schedules[i].priority;  // Select the current schedule
        int p = non_conflicting[i];  // Index of the last non-conflicting schedule
        if (p != -1) {  // If there is a non-conflicting schedule
            select += dp[p];  // Add the priority of the non-conflicting schedule
        }
        dp[i] = max(select, dp[i - 1]);  // Update the dp table by comparing including the current schedule and excluding the current schedule
    }

    vector<Schedule> optimal_schedules;  
    // Build the optimal schedule
    for (int i = n - 1; i >= 0; ) {
        int check = 0;  // Check if the current schedule is selected, for the first schedule if dp[0] != 0 then it is selected
        if(i > 0) {  // If schedule i is not the first schedule 
            check = dp[i - 1];  // Check value is the previous value of the dp table
        }
        if (dp[i] != check) {  // If current value is not equal to the previous value then the current schedule is selected
            optimal_schedules.push_back(schedules[i]);  // Add the current schedule to the optimal schedules
            i = non_conflicting[i];  // Move to the last non-conflicting schedule
        } else {  // If not selected, move to the previous schedule
            i--;
        }
    }
    reverse(optimal_schedules.begin(), optimal_schedules.end());  // Reverse the optimal schedules, because we started from the last schedule
    
    return optimal_schedules;  // Return the optimal schedules
}
// *** END ***


/* START: KNAPSACK (for items) */

// Function to select the most valuable items that can be purchased with a certain budget
vector<Item> knapsack(const vector<Item>& items, int budget) {
    int n = items.size();  // Get the number of items

    // Initialize the dynamic programming table
    // dp[x][j] will be storing the maximum value that can be achieved with capacity x using the first j items
    // Table size is (budget+1) x (n+1) and all values are initialized to 0
    vector<vector<double>> dp(budget+1, vector<double>(n+1, 0));

    // Build the dp table in bottom-up manner
    for(int x=1; x<=budget; x++){  // Iterate over the capacities
        for(int j=1; j<=n; j++){  // Iterate over the items
            dp[x][j] = dp[x][j-1];  // Initialize the value by excluding the current item
            if(items[j-1].weight <= x)  // If the item's weight is less than or equal to x, then include the item
                // Update dp[x][j] by comparing the current value and the value obtained by including the current item
                dp[x][j] = max(dp[x][j], items[j-1].value + dp[x-items[j-1].weight][j-1]);
        }
    }

    vector<Item> selected_items;  // Store the items selected for the optimal solution

    // Find the selected items by backtracking the dp table
    for(int j=n, x = budget; j>0 && x>0; j--){  // Start from the last item and the last capacity
        if(dp[x][j] != dp[x][j-1]){  // If the solution includes an item j, then the solution for dp[x][j] differs from dp[x][j-1]
            selected_items.push_back(items[j-1]);  // Add the item to the selected items
            x -= items[j-1].weight;  // Subtract the weight of the item from the remaining capacity
        }
    }

    // Return the items selected for the optimal solution
    return selected_items;
}

// *** END ***


int main(int argc, char *argv[])
{
    string case_no = argv[1];  // Get the case number from the command line argument
    string case_name = "case_" + case_no;  // Create the case name
    string path = "./inputs/" + case_name;  // Create the path to the input files
    string item_path = path + "/items.txt";  // Path to the items file
    string interval_path = path + "/room_time_intervals.txt";  // Path to the room_time_intervals file
    string priority_path = path + "/priority.txt";  // Path to the priority file

    int total_budget = 200000;  // Total budget

    ifstream item_file(item_path);  // Open the item file
    if (!item_file.is_open()) {  // Check if the file was opened successfully
        cerr << "Error: Unable to open items.txt file." << endl;  // Print an error message
        return 1;
    }

    string line;  // String to store the lines of the file
    getline(item_file, line);  // Skip the first line  since it is the header

    vector<Item> items;  // Vector to store the items
    // Read the items from the file
    while (getline(item_file, line)) {
        stringstream ss(line);  // Create a stringstream to read the values
        double weight, value;  // Variables to store the weight and value of the item
        string name;  // Variable to store the name of the item
        ss >> name >> weight >> value;  // Read the values
        items.push_back(Item(name, weight, value));  // Add the item to the vector
    }
    item_file.close();  // Close the file


    ifstream priority_file(priority_path);  // Open the priority.txt file
    if (!priority_file.is_open()) {  // Check if the file was opened successfully
        cerr << "Error: Unable to open priority.txt file." << endl;  // Print an error message
        return 1;
    }

    getline(priority_file, line);  // Skip the first line since it is the header

    map<pair<string, string>, int> priority_map;  // Map to store the priorities, I did not use the Priority struct
    // Read the priorities from the file
    while (getline(priority_file, line)) {  
        stringstream ss(line);
        string floor, room;
        int priority;
        ss >> floor >> room >> priority;
        priority_map[{floor, room}] = priority;  // Add the priority to the map, key is the pair of floor and room
    }
    priority_file.close();  // Close the file


    ifstream interval_file(interval_path);  // Open the room_time_intervals.txt file
    if (!interval_file.is_open()) {  // Check if the file was opened successfully
        cerr << "Error: Unable to open room_time_intervals.txt file." << endl;  // Print an error message
        return 1;
    }
    
    getline(interval_file, line);  // Skip the first line  since it is the header

    vector<TimeIntervals> intervals;  // Vector to store the time intervals
    // Read the time intervals from the file
    while (getline(interval_file, line)) {
        stringstream ss(line);
        string floor, room, start, finish;
        ss >> floor >> room >> start >> finish;
        intervals.push_back(TimeIntervals(floor, room, start, finish));  // Add the time interval to the vector
    }
    interval_file.close();  // Close the file

    map<int,vector<Schedule>> schedules;  // Map to store the schedules for each floor, I used map since floor number is unknown at

    // Store the schedules for each floor
    for(const auto& interval : intervals) {
        string floor = interval.floor;  // Get the floor number as a string
        int floor_number = stoi(interval.floor.substr(6, 1));  // Get the floor number as an integer by extracting the last character
        int priority = priority_map[{interval.floor, interval.room}];  // Get the priority of the room from the map
        Schedule schedule(interval, priority);  // Create a schedule
        schedules[floor_number].push_back(schedule);  // Add the schedule to the map using the floor number as the key
    }

    map<int,vector<Schedule>> optimal_schedules;  // Map to store the optimal schedules for each floor
    map<int,int> priority_gain;  // Map to store the priority gain for each floor
    
    // Find the optimal schedule for each floor and calculate the priority gain
    for(auto& schedule : schedules){  // Iterate over the schedules
        optimal_schedules[schedule.first] = weighted_interval_scheduling(schedule.second);  // Find the optimal schedule for each floor
        for(auto& optimal_schedule : optimal_schedules[schedule.first]) {  // Calculate the priority gain for each floor
            priority_gain[schedule.first] += optimal_schedule.priority;  // Add the priority of the schedule to the priority gain
        }
    }

    // Call the knapsack function to find the selected items
    vector<Item> selected_items = knapsack(items, total_budget);

    double total_value = 0;  // Total value of the selected items
    for(const auto& item : selected_items) {  // Calculate the total value of the selected items
        total_value += item.value;
    }

    // Print optimal schedules
    cout << "Best Schedule for Each Floor" << endl << endl;
    for(auto& optimal_schedule : optimal_schedules) {  // Print the optimal schedule for each floor
        cout << optimal_schedule.second[0].interval.floor <<" --> Priority Gain: "<< priority_gain[optimal_schedule.first] << endl;
        for(auto& schedule : optimal_schedule.second) {
            cout << schedule.interval.floor << "\t" << schedule.interval.room << "\t" << schedule.interval.str_start << "\t" << schedule.interval.str_finish << endl;
        }
        cout<<endl;
    }

    // Print the selected items
    cout << "Best Use of Budget" << endl;
    printf("Total Value --> %.1lf\n", (round(total_value*10))/10);  // Round the total value to one decimal place

    for(const auto& item : selected_items)  // Print the selected item names
        cout << item.name << endl;

     return 0;
}