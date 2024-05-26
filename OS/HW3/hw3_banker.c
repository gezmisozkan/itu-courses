#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 1  // Initial capacity for the matrix
// Like vector in C++, we need to reallocate memory when the capacity is not enough
// We will double the capacity when the number of rows or columns is equal to the capacity

// Define a structure to hold the matrix, number of rows, and number of columns
typedef struct {
    int** matrix;  // 2D array to store the matrix
    int rows;
    int cols;
} Matrix;

// Function to read a matrix from a file
Matrix readMatrixFromFile(char* filename) {
    // Open the file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {  // If the file cannot be opened, print an error message and return an empty matrix
        printf("Could not open file %s\n", filename);
        Matrix empty = {NULL, 0, 0};
        return empty;
    }

    // Initialize row and column capacities and allocate memory for the matrix
    int rowCapacity = INITIAL_CAPACITY;
    int colCapacity = INITIAL_CAPACITY;
    int** matrix = (int**)malloc(rowCapacity * sizeof(int*));

    // Initialize row and column counters
    int rows = 0;
    int cols = 0;

    int value;
    // Read integers from the file
    while (fscanf(file, "%d", &value) == 1) {
        // Allocate memory for a new row if needed
        if (rows == rowCapacity) {  // If the number of rows is equal to the row capacity, then reallocate memory for the matrix
            rowCapacity *= 2;  // Double the row capacity
            matrix = (int**)realloc(matrix, rowCapacity * sizeof(int*));  
        }

        // Allocate memory for a new column if needed
        if (cols == 0) {  // If the number of columns is 0, then allocate memory for the first column
            matrix[rows] = (int*)malloc(colCapacity * sizeof(int));
        } else if (cols == colCapacity) {  // If the number of columns is equal to the column capacity, then reallocate memory for the row
            colCapacity *= 2;
            matrix[rows] = (int*)realloc(matrix[rows], colCapacity * sizeof(int));
        }

        // Store the value in the matrix
        matrix[rows][cols] = value;
        cols++;

        // Check for spaces and newlines. If a newline is found, move to the next row and reset the column counter. Incremention of the column is done in the previous if statement
        char c;
        fscanf(file, "%c", &c);
        if (c == '\n') {
            // Move to the next row
            rows++;
            cols = 0;
        }
    }
    rows++;  // Increment the rows by 1 because the last row is not counted
    // Resize the rows to the exact number of rows read
    matrix = (int**)realloc(matrix, rows * sizeof(int*));

    // Free unused memory
    for (int i = 0; i < rows; i++) {  // Since we increased the column number by 1 cols will be the number of columns
        matrix[i] = (int*)realloc(matrix[i], cols * sizeof(int));
    }

    fclose(file);

    Matrix result = {matrix, rows, cols};  // Create a matrix structure to store the matrix, number of rows, and number of columns
    return result;  // Return the matrix
}

// Banker's algorithm
void bankersAlgorithm(Matrix allocations, Matrix requests, int* available) {
    int process_count = allocations.rows; // Number of processes
    int resource_count = allocations.cols; // Number of resources

    bool finish[process_count];  // Array to store the finish status of each process

    // Initialize the finish array
    for (int i = 0; i < process_count; i++) {  // Initially all processes are not finished
        finish[i] = false;
    }
    
    int order[process_count];  // Array to store the order of the processes
    for(int i=0; i<process_count; i++){
        order[i] = -1;  // Initialize the order array with -1
    }
    int index = 0; // Index for the order array
    
    // Banker's algorithm
    for (int i = 0; i < process_count; i++) {  // Iterate over all processes
        for (int j = 0; j < process_count; j++) {  // Double loop to iterate over all processes, since after releasing resources, the available resources may be enough for a process that was not able to run before
            if (finish[j] == false) {  // If the process is not finished
                bool flag = true;
                for (int k = 0; k < resource_count; k++) {  // Iterate over all resources
                    if (requests.matrix[j][k] > available[k]) {  // If the process needs more resources than available
                        flag = false;  // Set the flag to false and break the loop
                        break;
                    }
                }

                if (flag == true) {  // If the process can run
                    finish[j] = true;  // Set the process as finished
                    order[index++] = j; // Add the process to the order array
                    for (int k = 0; k < resource_count; k++) {  // Iterate over all resources
                        available[k] += allocations.matrix[j][k];  // Release the resources
                        allocations.matrix[j][k] = 0;  // Set the allocated resources to 0
                    }
                }
            }
        }
    }

    // Print the order of the processes
    printf("Running order for processes: ");
    for (int i = 0; i < allocations.rows && order[i] != -1; i++) {  // If the order[i] is -1, then there is a deadlock
        printf("P%d ", order[i] + 1);  // order[i] + 1 because the process number starts from 1
    }
    printf("\n");

    // Check if all processes have finished, if not then there is a deadlock
    bool deadlock = false;  // Flag to check if there is a deadlock
    for (int i = 0; i < process_count; i++) {  // Iterate over all processes
        if (finish[i] == false) {  // If the process is not finished
            if (!deadlock) {  // This is the print statement for the first process that is not finished
                printf("There is a deadlock. Processes that are the cause of the deadlock: ");
                deadlock = true;  // Set the deadlock flag to true
            }
            printf("P%d ", i+1);  // Print the process number
        }
    }
    if (deadlock) {  // If there is a deadlock, print a new line
        printf("\n");
    }
    else{  // If there is no deadlock
        printf("There is no deadlock.\n");  // If there is no deadlock
    }
}
int main(int argc, char* argv[]){
    // Check if the number of arguments is correct
    if (argc != 4) {
        printf("Usage: %s <allocation_file> <resource_file> <request_file>\n", argv[0]);
        return 1;
    }
    
    // Define the file names for the allocation, resource, and request matrices
    char* allocation_file = argv[1];
    char* resource_file = argv[2];
    char* request_file = argv[3];  

    Matrix allocations = readMatrixFromFile(allocation_file);  // Read the allocations matrix from the file, it stores with malloc so we need to free it
    Matrix resources = readMatrixFromFile(resource_file);  // Read the resources matrix from the file, it stores with malloc so we need to free it
    Matrix requests = readMatrixFromFile(request_file);  // Read the requests matrix from the file, it stores with malloc so we need to free it
    // The requests matrix is the need matrix, it shows the resources that each process needs

    int available[resources.cols];  // Initialize an array to store the available resources

    // Calculate the available resources
    for(int i=0; i<allocations.cols; i++){  // Since the number of columns in allocations is the number of resources
        int sum = 0;
        for(int j=0; j<allocations.rows; j++){  // Since the number of rows in allocations is the number of processes
            sum += allocations.matrix[j][i];  // Sum the allocated resources for each process
        }
        available[i] = resources.matrix[0][i] - sum;
    }

    // Print information of processes
    for (int i=0; i<allocations.rows; i++){  // Since the number of rows in allocations is the number of processes
        printf("Information for process P%d:\n", i+1);  // i+1 because the process number starts from 1
        printf("Allocated resources: ");
        for (int j=0; j<allocations.cols; j++){  // Since the number of columns in allocations is the number of resources
            printf("R%d:%d ", j+1, allocations.matrix[i][j]);
        }
        printf("\n");
        printf("Resource request:    ");
        for(int j=0; j<resources.cols; j++){  // Since the number of columns in resources is the number of resources
            printf("R%d:%d ", j+1, requests.matrix[i][j]);
        }
        printf("\n\n");
    }

    // Run the Banker's algorithm
    bankersAlgorithm(allocations, requests, available);

    // Free the memory allocated for the aloocations, resources, and requests matrices
    for (int i = 0; i < allocations.rows; i++) {
        free(allocations.matrix[i]);
    }
    free(allocations.matrix);  

    for (int i = 0; i < resources.rows; i++) {
        free(resources.matrix[i]);
    }
    free(resources.matrix);

    for (int i = 0; i < requests.rows; i++) {
        free(requests.matrix[i]);
    }
    free(requests.matrix);

    return 0;
}