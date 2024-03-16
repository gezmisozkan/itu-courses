/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 1
 * Özkan Gezmiş
 * 150200033
*/

/* * * * * * * * * * * * * * * * * *
 * DO NOT MODIFY UNLESS REQUESTED  *
* * * * * * * * * * * * * * * * * */

#include <iostream>         
#include <vector>
#include <fstream>
#include <sstream>       
#include <stack>
#include <queue>
#include <chrono>       // For time measurement

/* INCLUDE ANY OTHER NECESSARY LIBRARIES HERE */
/* START YOUR CODE HERE */
#include <algorithm> // for sort function

/* END YOUR CODE HERE */

using namespace std;
using namespace std::chrono;

/* USE HERE IF YOU NEED TO DEFINE EXTRA FUNCTIONS */
/* START YOUR CODE HERE */

class Node{  // class to represent the elements of the map
    public:
    int resource;
    bool is_visited = false;
    pair<int,int> index;
    Node(int resource, pair<int,int> index){
        this->resource = resource;
        this->index = index;
    }
};
// function to push the neighbors of the current element to the stack
void push_neigbors_to_stack(int row, int col, int lenght, int height, int resource, vector<vector<int>>& map, stack <Node>& dfsStack, bool** visited_nodes){   
        if(row-1 >= 0 && map[row-1][col] == resource && !visited_nodes[row-1][col]){ // top
            dfsStack.push(Node(map[row-1][col],pair(row-1,col))); // push the element to the top of the current element to the stack
            visited_nodes[row-1][col] = true;
        }
        else if(row-1 < 0 && map[lenght-1][col] == resource && !visited_nodes[lenght-1][col]){
            dfsStack.push(Node(map[lenght-1][col],pair(lenght-1,col))); // if the current element is at the top edge of the map, push the element at the bottom edge to the stack
            visited_nodes[lenght-1][col] = true;
        }
        if(col-1 >= 0 && map[row][col-1] == resource && !visited_nodes[row][col-1]){ // left
            dfsStack.push(Node(map[row][col-1],pair(row,col-1))); // push the element to the left of the current element to the stack
            visited_nodes[row][col-1] = true;
        }
        else if(col-1 < 0 && map[row][height-1] == resource && !visited_nodes[row][height-1]){
            dfsStack.push(Node(map[row][height-1],pair(row,height-1))); // if the current element is at the left edge of the map, push the element at the right edge to the stack
            visited_nodes[row][height-1] = true;
        }
        if(row+1 < lenght && map[row+1][col] == resource && !visited_nodes[row+1][col]){ // bottom
            dfsStack.push(Node(map[row+1][col],pair(row+1,col))); // push the element to the bottom of the current element to the stack
            visited_nodes[row+1][col] = true;
        }
        else if(row+1 >= lenght && map[0][col] == resource && !visited_nodes[0][col]){
            dfsStack.push(Node(map[0][col],pair(0,col))); // if the current element is at the bottom edge of the map, push the element at the top edge to the stack
            visited_nodes[0][col] = true;
        }
        if(col+1 < height && map[row][col+1] == resource && !visited_nodes[row][col+1]){ // right
            dfsStack.push(Node(map[row][col+1],pair(row,col+1))); // push the element to the right of the current element to the stack
            visited_nodes[row][col+1] = true;
        }
        else if(col+1 >= height && map[row][0] == resource && !visited_nodes[row][0]){
            dfsStack.push(Node(map[row][0],pair(row,0))); // if the current element is at the right edge of the map, push the element at the left edge to the stack
            visited_nodes[row][0] = true;
        }
}

// function to push the neighbors of the current element to the queue
void push_neigbors_to_queue(int row, int col, int lenght, int height, int resource, vector<vector<int>>& map, queue <Node>& bfsQueue,bool** visited_nodes){   
        if(row-1 >= 0 && map[row-1][col] == resource && !visited_nodes[row-1][col]){ // top
            bfsQueue.push(Node(map[row-1][col],pair(row-1,col))); // push the element to the top of the current element to the queue
            visited_nodes[row-1][col] = true;
        }
        else if(row-1 < 0 && map[lenght-1][col] == resource && !visited_nodes[lenght-1][col]){
            bfsQueue.push(Node(map[lenght-1][col],pair(lenght-1,col))); // if the current element is at the top edge of the map, push the element at the bottom edge to the queue
            visited_nodes[lenght-1][col] = true;
        }
        if(col-1 >= 0 && map[row][col-1] == resource && !visited_nodes[row][col-1]){ // left
            bfsQueue.push(Node(map[row][col-1],pair(row,col-1))); // push the element to the left of the current element to the queue
            visited_nodes[row][col-1] = true;
        }
        else if(col-1 < 0 && map[row][height-1] == resource && !visited_nodes[row][height-1]){
            bfsQueue.push(Node(map[row][height-1],pair(row,height-1))); // if the current element is at the left edge of the map, push the element at the right edge to the queue
            visited_nodes[row][height-1] = true;
        }
        if(row+1 < lenght && map[row+1][col] == resource && !visited_nodes[row+1][col]){ // bottom
            bfsQueue.push(Node(map[row+1][col],pair(row+1,col))); // push the element to the bottom of the current element to the queue
            visited_nodes[row+1][col] = true;
        }
        else if(row+1 >= lenght && map[0][col] == resource && !visited_nodes[0][col]){
            bfsQueue.push(Node(map[0][col],pair(0,col))); // if the current element is at the bottom edge of the map, push the element at the top edge to the queue
            visited_nodes[0][col] = true;
        }
        if(col+1 < height && map[row][col+1] == resource && !visited_nodes[row][col+1]){ // right
            bfsQueue.push(Node(map[row][col+1],pair(row,col+1))); // push the element to the right of the current element to the queue
            visited_nodes[row][col+1] = true;
        }
        else if(col+1 >= height && map[row][0] == resource && !visited_nodes[row][0]){
            bfsQueue.push(Node(map[row][0],pair(row,0))); // if the current element is at the right edge of the map, push the element at the left edge to the queue
            visited_nodes[row][0] = true;
        }
}


/* END YOUR CODE HERE */

/**
 * Perform Depth-First Search (DFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the DFS traversal.
 * @param col The starting column index for the DFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the DFS traversal.
 */
int dfs(vector<vector<int>>& map, int row, int col, int resource) {

    /* START YOUR CODE HERE */
    int component_size = 0; // initialize the size of the colony to 0
    stack <Node> dfsStack; // create a stack to keep track of the elements to be visited, each element is a pair of the resource value and the pair of the row and column index
    int height = map[0].size(); // length of the row
    int length = map.size(); // length of the column
    bool** visited_nodes = new bool*[length]; // create a 2D array to keep track of visited nodes
    // Declare memory block of size length
	for (int i = 0; i < length; i++) {
		// Declare a memory block
		// of size height
		visited_nodes[i] = new bool[height];
	}
    for(int i = 0; i < length; i++){ // initialize the visited nodes to false
        for(int j = 0; j < height; j++){
            visited_nodes[i][j] = false;
        }
    }

    if(map[row][col] != resource) // if the first element is not the same as the resource, return 0
        return 0;
    else
        dfsStack.push(Node(map[row][col],pair(row,col))); // push the first element to the queue
    while(!dfsStack.empty()){ // while the queue is not empty
        auto currentElement = dfsStack.top(); // get the first element of the stack
        currentElement.is_visited = true; // mark the current element as visited
        visited_nodes[currentElement.index.first][currentElement.index.second] = true; // mark the current element as visited
        dfsStack.pop(); // remove the first element from the queue
        component_size++; // increment the size of the colony
        row = currentElement.index.first; // get the row index of the current element
        col = currentElement.index.second; // get the column index of the current element
        push_neigbors_to_stack(row, col, length, height, resource, map, dfsStack, visited_nodes); // push the neighbors of the current element to the queue
    }
    return component_size; // return the size of the colony

    /* END YOUR CODE HERE */

}


/**
 * Perform Breadth-First Search (BFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the BFS traversal.
 * @param col The starting column index for the BFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the BFS traversal.
 */
int bfs(vector<vector<int>>& map, int row, int col, int resource) {
    
    /* START YOUR CODE HERE */
    int component_size = 0; // initialize the size of the colony to 0
    queue <Node> bfsQueue; // create a queue to keep track of the elements to be visited, each element is a pair of the resource value and the pair of the row and column index
    int height = map[0].size(); // length of the row
    int length = map.size(); // length of the column
    bool** visited_nodes = new bool*[length]; // create a 2D array to keep track of visited nodes
    // Declare memory block of size length
	for (int i = 0; i < length; i++) {
		// Declare a memory block
		// of size height
		visited_nodes[i] = new bool[height];
	}
    for(int i = 0; i < length; i++){ // initialize the visited nodes to false
        for(int j = 0; j < height; j++){
            visited_nodes[i][j] = false;
        }
    }

    if(map[row][col] != resource) // if the first element is not the same as the resource, return 0
        return 0;
    else
        bfsQueue.push(Node(map[row][col],pair(row,col))); // push the first element to the queue
    while(!bfsQueue.empty()){ // while the queue is not empty
        auto currentElement = bfsQueue.front(); // get the first element of the queue
        currentElement.is_visited = true; // mark the current element as visited
        visited_nodes[currentElement.index.first][currentElement.index.second] = true; // mark the current element as visited
        bfsQueue.pop(); // remove the first element from the queue
        component_size++; // increment the size of the colony
        row = currentElement.index.first; // get the row index of the current element
        col = currentElement.index.second; // get the column index of the current element
        push_neigbors_to_queue(row, col, length, height, resource, map, bfsQueue, visited_nodes); // push the neighbors of the current element to the queue
    }
    return component_size; // return the size of the colony
    /* END YOUR CODE HERE */

}

/**
 * Finds the top-k largest colonies in a given map.s
 *
 * @param map The map represented as a 2D vector of integers.
 * @param useDFS A boolean flag indicating whether to use Depth-First Search (DFS) or Breadth-First Search (BFS) algorithm.
 * @param k The number of top colonies to find.
 * @return A vector of pairs representing the size and resource type of the top-k largest colonies.
 */
vector<pair<int, int>> top_k_largest_colonies(vector<vector<int>>& map, bool useDFS, unsigned int k) {

    auto start = high_resolution_clock::now();      // Start measuring time
    
    /* START YOUR CODE HERE */
    vector<pair<int,int>> largest_colonies(5); // initialize the size of the largest colony pair, first index is the size of the colony, second index is the resource type
    for(int i = 0; i < 5; i++){
        largest_colonies[i].first = 0; // initialize the size of the largest colony to 0
        largest_colonies[i].second = i+1; // initialize the resource type of the largest colony
    }

    for(int i = 0; i < (int)map.size(); i++){
        for(int j = 0; j < (int)map[i].size(); j++){
            for(int k=1; k<=5; k++){
                int size=0;
                if(useDFS == true)    
                    size = dfs(map, i, j, k);
                else
                    size = bfs(map, i, j, k);
                if(size > largest_colonies[k-1].first){ // if the size of the current colony is larger than the size of the largest colony, update the size of the largest colony
                    largest_colonies[k-1].first = size;
                }
            }
        }
    }
    /* END YOUR CODE HERE */

    auto stop = high_resolution_clock::now();                       // Stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start);       // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    /* START YOUR CODE HERE */

    // Sort the largest colonies in decreasing order
    for(int i=0; i<5; i++){
        for(int j=i+1; j<5; j++){
            if(largest_colonies[i].first < largest_colonies[j].first){ // sort the largest colonies in decreasing order according to their size
                auto temp = largest_colonies[i]; // swap the largest colonies
                largest_colonies[i] = largest_colonies[j];
                largest_colonies[j] = temp;
            }
            else if(largest_colonies[i].first == largest_colonies[j].first && largest_colonies[i].second > largest_colonies[j].second){ // if the size of the largest colonies are the same, sort them in increasing order according to their resource type
                auto temp = largest_colonies[i]; // swap the largest colonies
                largest_colonies[i] = largest_colonies[j];
                largest_colonies[j] = temp;
            }
        }
    }
    while(largest_colonies[largest_colonies.size()-1].first == 0){
        largest_colonies.pop_back(); // remove the colonies with size 0
    }

    return largest_colonies; // return the largest colonies
    /* END YOUR CODE HERE */

}

// Main function
int main(int argc, char* argv[]) {
    // Check the command line arguments
    if (argc != 4) {

        // It should be: <1 or 0> <k> <filename.txt>
        cerr << "Usage: " << argv[0] << " <1 or 0> <k> <filename.txt>" << endl;
        return 1;
    }

    // Get the arguments
    bool useDFS = (atoi(argv[1]) == 1);
    int k = atoi(argv[2]);

    // Read the map from the text file
    string filename = argv[3];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<vector<int>> sector_map;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<int> row;
        int num;
        while (iss >> num) {
            row.push_back(num);
        }
        sector_map.push_back(row);
    }
    file.close();   

    // Find the largest colony
    vector<pair<int, int>> result = top_k_largest_colonies(sector_map, useDFS, k);

    // Print the result
    cout << "Algorithm: " << (useDFS ? "DFS" : "BFS") << endl;
    cout << "Map: " << filename << endl;

    // If no colonies are found
    if (result.empty()) {
        cout << "No colonies found." << endl;
        return 0;
    }

    // Print the top-k colonies
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "Colony " << i + 1 << ": Size = " << result[i].first << ", Resource Type = " << result[i].second << endl;
    }

    return 0;
}