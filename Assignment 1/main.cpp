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


#include <algorithm> // for sorting the largest colonies

/* END YOUR CODE HERE */

using namespace std;
using namespace std::chrono;

/* USE HERE IF YOU NEED TO DEFINE EXTRA FUNCTIONS */
/* START YOUR CODE HERE */

#define VISITED_ELEMENT_RESOURCE -1000 // value to mark the visited elements

class Node{  // class to represent the elements of the map
    public:
    int resource;
    pair<int,int> index;
    Node(int resource, pair<int,int> index){
        this->resource = resource;
        this->index = index;
    }
};
// function to push the neighbors of the current element to the stack and makes added nodes visited
// if element is VISITED_ELEMENT_RESOURCE it means it is added to the stack
void push_neigbors_to_stack(int row, int col, int lenght, int height, int resource, vector<vector<int>>& map, stack <Node>& dfsStack){   
        if(row-1 >= 0 && map[row-1][col] == resource && map[row-1][col]!= VISITED_ELEMENT_RESOURCE){ // top
            dfsStack.push(Node(map[row-1][col],pair<int,int>(row-1,col))); // push the element to the top of the current element to the stack
            map[row-1][col] = VISITED_ELEMENT_RESOURCE;
        }
        else if(row-1 < 0 && map[lenght-1][col] == resource && map[lenght-1][col]!= VISITED_ELEMENT_RESOURCE){
            dfsStack.push(Node(map[lenght-1][col],pair<int,int>(lenght-1,col))); // if the current element is at the top edge of the map, push the element at the bottom edge to the stack
            map[lenght-1][col] = VISITED_ELEMENT_RESOURCE;
        }
        if(col-1 >= 0 && map[row][col-1] == resource && map[row][col-1]!= VISITED_ELEMENT_RESOURCE){ // left
            dfsStack.push(Node(map[row][col-1],pair<int,int>(row,col-1))); // push the element to the left of the current element to the stack
            map[row][col-1] = VISITED_ELEMENT_RESOURCE;
        }
        else if(col-1 < 0 && map[row][height-1] == resource && map[row][height-1]!= VISITED_ELEMENT_RESOURCE){
            dfsStack.push(Node(map[row][height-1],pair<int,int>(row,height-1))); // if the current element is at the left edge of the map, push the element at the right edge to the stack
            map[row][height-1] = VISITED_ELEMENT_RESOURCE;
        }
        if(row+1 < lenght && map[row+1][col] == resource && map[row+1][col]!= VISITED_ELEMENT_RESOURCE){ // bottom
            dfsStack.push(Node(map[row+1][col],pair<int,int>(row+1,col))); // push the element to the bottom of the current element to the stack
            map[row+1][col] = VISITED_ELEMENT_RESOURCE;
        }
        else if(row+1 >= lenght && map[0][col] == resource && map[0][col]!= VISITED_ELEMENT_RESOURCE){
            dfsStack.push(Node(map[0][col],pair<int,int>(0,col))); // if the current element is at the bottom edge of the map, push the element at the top edge to the stack
            map[0][col] = VISITED_ELEMENT_RESOURCE;
        }
        if(col+1 < height && map[row][col+1] == resource && map[row][col+1]!= VISITED_ELEMENT_RESOURCE){ // right
            dfsStack.push(Node(map[row][col+1],pair<int,int>(row,col+1))); // push the element to the right of the current element to the stack
            map[row][col+1] = VISITED_ELEMENT_RESOURCE;
        }
        else if(col+1 >= height && map[row][0] == resource && map[row][0]!= VISITED_ELEMENT_RESOURCE){
            dfsStack.push(Node(map[row][0],pair<int,int>(row,0))); // if the current element is at the right edge of the map, push the element at the left edge to the stack
            map[row][0] = VISITED_ELEMENT_RESOURCE;
        }
}

// function to push the neighbors of the current element to the queue and makes added nodes visited
// if element is VISITED_ELEMENT_RESOURCE it means it is added to the queue
void push_neigbors_to_queue(int row, int col, int lenght, int height, int resource, vector<vector<int>>& map, queue <Node>& bfsQueue){   
        if(row-1 >= 0 && map[row-1][col] == resource && map[row-1][col]!= VISITED_ELEMENT_RESOURCE){ // top
            bfsQueue.push(Node(map[row-1][col],pair<int,int>(row-1,col))); // push the element to the top of the current element to the queue
            map[row-1][col] = VISITED_ELEMENT_RESOURCE;
        }
        else if(row-1 < 0 && map[lenght-1][col] == resource && map[lenght-1][col]!= VISITED_ELEMENT_RESOURCE){
            bfsQueue.push(Node(map[lenght-1][col],pair<int,int>(lenght-1,col))); // if the current element is at the top edge of the map, push the element at the bottom edge to the queue
            map[lenght-1][col] = VISITED_ELEMENT_RESOURCE;
        }
        if(col-1 >= 0 && map[row][col-1] == resource && map[row][col-1]!= VISITED_ELEMENT_RESOURCE){ // left
            bfsQueue.push(Node(map[row][col-1],pair<int,int>(row,col-1))); // push the element to the left of the current element to the queue
            map[row][col-1] = VISITED_ELEMENT_RESOURCE;
        }
        else if(col-1 < 0 && map[row][height-1] == resource && map[row][height-1]!= VISITED_ELEMENT_RESOURCE){
            bfsQueue.push(Node(map[row][height-1],pair<int,int>(row,height-1))); // if the current element is at the left edge of the map, push the element at the right edge to the queue
            map[row][height-1] = VISITED_ELEMENT_RESOURCE;
        }
        if(row+1 < lenght && map[row+1][col] == resource && map[row+1][col]!= VISITED_ELEMENT_RESOURCE){ // bottom
            bfsQueue.push(Node(map[row+1][col],pair<int,int>(row+1,col))); // push the element to the bottom of the current element to the queue
            map[row+1][col] = VISITED_ELEMENT_RESOURCE;
        }
        else if(row+1 >= lenght && map[0][col] == resource && map[0][col]!= VISITED_ELEMENT_RESOURCE){
            bfsQueue.push(Node(map[0][col],pair<int,int>(0,col))); // if the current element is at the bottom edge of the map, push the element at the top edge to the queue
            map[0][col] = VISITED_ELEMENT_RESOURCE;
        }
        if(col+1 < height && map[row][col+1] == resource && map[row][col+1]!= VISITED_ELEMENT_RESOURCE){ // right
            bfsQueue.push(Node(map[row][col+1],pair<int,int>(row,col+1))); // push the element to the right of the current element to the queue
            map[row][col+1] = VISITED_ELEMENT_RESOURCE;
        }
        else if(col+1 >= height && map[row][0] == resource && map[row][0]!= VISITED_ELEMENT_RESOURCE){
            bfsQueue.push(Node(map[row][0],pair<int,int>(row,0))); // if the current element is at the right edge of the map, push the element at the left edge to the queue
            map[row][0] = VISITED_ELEMENT_RESOURCE;
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
    if(map[row][col] != resource) // if the first element is not the same as the resource, return 0
        return 0;
    
    int component_size = 0; // initialize the size of the colony to 0
    stack <Node> dfsStack; // create a stack to keep track of the elements to be visited, each element is a pair of the resource value and the pair of the row and column index
    int height = map[0].size(); // length of the row
    int length = map.size(); // length of the column

    dfsStack.push(Node(map[row][col],pair<int,int>(row,col))); // push the first element to the queue
    map[row][col] = VISITED_ELEMENT_RESOURCE; // change the value of the current element to VISITED_ELEMENT_RESOURCE to make it visited
    while(!dfsStack.empty()){ // while the queue is not empty
        auto currentElement = dfsStack.top(); // get the first element of the stack
        dfsStack.pop(); // remove the first element from the queue
        component_size++; // increment the size of the colony
        row = currentElement.index.first; // get the row index of the current element
        col = currentElement.index.second; // get the column index of the current element
        push_neigbors_to_stack(row, col, length, height, resource, map, dfsStack); // push the neighbors of the current element to the queue
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

    if(map[row][col] != resource) // if the first element is not the same as the resource, return 0
        return 0;

    int component_size = 0; // initialize the size of the colony to 0
    queue <Node> bfsQueue; // create a queue to keep track of the elements to be visited, each element is a pair of the resource value and the pair of the row and column index
    int height = map[0].size(); // length of the row
    int length = map.size(); // length of the column

    bfsQueue.push(Node(map[row][col],pair<int,int>(row,col))); // push the first element to the queue
    map[row][col] = VISITED_ELEMENT_RESOURCE; // mark the first element as visited

    while(!bfsQueue.empty()){ // while the queue is not empty
        auto currentElement = bfsQueue.front(); // get the first element of the queue
        bfsQueue.pop(); // remove the first element from the queue
        component_size++; // increment the size of the colony
        row = currentElement.index.first; // get the row index of the current element
        col = currentElement.index.second; // get the column index of the current element
        push_neigbors_to_queue(row, col, length, height, resource, map, bfsQueue); // push the neighbors of the current element to the queue
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
    if(k == 0) // if k is 0, return an empty vector
        return vector<pair<int,int>>(0);
    
    vector<pair<int,int>> largest_colonies(k); // initialize the size of the largest colony pair, first index is the size of the colony, second index is the resource type
    for(unsigned i = 0; i < k; i++){
        largest_colonies[i].first = 0; // initialize the size of the largest colony to 0
        largest_colonies[i].second = 0; // initialize the resource type of the largest colony to 0
    }

    // iterate through the map and find the largest colonies
    // in the bfs and dfs functions, I changed the value of the visited nodes to VISITED_ELEMENT_RESOURCE to find the largest colony in the map, so that for connected components I don't have to deal with the same connected component multiple times
    for(int row = 0; row < (int)map.size(); row++){  // iterate through the rows
        for(int col = 0; col < (int)map[row].size(); col++){  // iterate through the columns
            // I assumed that the resource types are between 1 and 5, k is not the number of resource type and I need that information
            int resource_type = map[row][col]; // get the resource type of the current element
            if(resource_type == VISITED_ELEMENT_RESOURCE)  // if the current element is visited, continue to the next element
                continue;  
            else{  // if the current element is not visited
                int size=0;
                if(useDFS == true)
                    size = dfs(map, row, col, resource_type);
                else 
                    size = bfs(map, row, col, resource_type);
                sort(largest_colonies.begin(), largest_colonies.end(),greater<pair<int, int>>()); // sort the largest colonies in decreasing order according to their size
                if(size > largest_colonies.back().first){ // if the size of the current colony is larger than the size of the smallest colony in the vector, update the element
                    largest_colonies.pop_back(); // remove the smallest colony from the vector
                    largest_colonies.push_back(pair<int,int>(size,resource_type)); // add the current colony to the vector
                }
            }
        }
    }

    /* END YOUR CODE HERE */

    auto stop = high_resolution_clock::now();                       // Stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start);       // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    /* START YOUR CODE HERE */

    // Sort the largest colonies in decreasing order according to conditions written in the assignment
    for(unsigned i=0; i<k; i++){
        for(unsigned j=i+1; j<k; j++){
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