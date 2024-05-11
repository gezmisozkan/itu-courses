/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 3
 * <Name & Surname>
 * <Student ID>
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

/* YOU CAN INCLUDE ANY OTHER NECESSARY LIBRARIES */


/* YOU CAN DEFINE EXTRA FUNCTIONS AND MODIFY THE CODE TEMPLATE */

/* YOU HAVE TO CONSIDER THE OUTPUT FORMAT TO TEST YOUR CODE WITH CALICO */


using namespace std;


// struct TimeIntervals {

//     // YOUR CODE HERE

// };

// struct Priority {

//     // YOUR CODE HERE
    
// };

struct Item {
    string name;
    int weight;
    double value;

    Item(string name, int weight, double value){  // Constructor
        this->name = name;
        this->weight = weight;
        this->value = value;
    }
    Item(){}  // Default constructor
};


// /* START: WEIGHTED INTERVAL SCHEDULING (for each floor) */

// struct Schedule
// {

//     // YOUR CODE HERE  

// };

// function to find the best schedule for each floor
// vector<Schedule> weighted_interval_scheduling(vector<Schedule> schedules)
// {
//     // YOUR CODE HERE

//     return optimal_schedules;
// }

// *** END ***


/* START: KNAPSACK (for items) */

// Function to select the most valuable items that can be purchased with a certain budget
vector<Item> knapsack(const vector<Item>& items, int budget) {
    // Get the number of items
    int n = items.size();

    // Initialize the dynamic programming table
    // dp[x][j] will be storing the maximum value that can be achieved with capacity x using the first j items
    // Table size is (budget+1) x (n+1) and all values are initialized to 0
    vector<vector<double>> dp(budget+1, vector<double>(n+1, 0));

    // Build the dp table in bottom-up manner
    for(int x=1; x<=budget; x++){
        for(int j=1; j<=n; j++){
            dp[x][j] = dp[x][j-1];

            // If the item's weight is less than or equal to x, then include the item
            if(items[j-1].weight <= x)
                // Update dp[x][j] by comparing the current value and the value obtained by including the current item
                dp[x][j] = max(dp[x][j], items[j-1].value + dp[x-items[j-1].weight][j-1]);
        }
    }

    // Stores the items selected for the optimal solution
    vector<Item> selected_items;

    // Start from the total weight and move towards zero
    int x = budget;
    for(int j=n; j>0 && x>0; j--){
        // If the solution includes an item j, then the solution for dp[x][j] differs from dp[x][j-1]
        if(dp[x][j] != dp[x][j-1]){
            // This item is included in the optimal solution
            selected_items.push_back(items[j-1]);

            // Subtract the weight of the item from the remaining capacity
            x -= items[j-1].weight;
        }
    }

    // Return the items selected for the optimal solution
   return selected_items;
}

// *** END ***


int main(int argc, char *argv[])
{
    // string case_no = argv[1];
    string case_no = "3";
    string case_name = "case_" + case_no;
    string path = "./inputs/" + case_name;
    string item_path = path + "/items.txt";
    int total_budget = 200000; 

    // Open the file
    ifstream file(item_path);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    // Vector to store the items
    vector<Item> items;
    string line;

    // Skip the first line
    getline(file, line);

    // Read the items from the file
    while (getline(file, line)) {
        stringstream ss(line);
        double weight, value;
        string name;
        ss >> name >> weight >> value;
        items.push_back(Item(name, weight, value));
    }

    // Close the file
    file.close();

    // Call the knapsack function
    vector<Item> selected_items = knapsack(items, total_budget);

    double total_value = 0;  // Total value of the selected items
    for(const auto& item : selected_items) {  
        total_value += item.value;
    }

    // Print the selected items
    cout << "Best Use of Budget" << endl;
    cout << "Total Value --> " << total_value << endl;

    for(const auto& item : selected_items)
        cout << item.name << endl;

     return 0;
}