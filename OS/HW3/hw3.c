#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 1

// Define a structure to hold the matrix, number of rows, and number of columns
typedef struct {
    int** matrix;
    int rows;
    int cols;
} Matrix;

// Function to read a matrix from a file
Matrix readMatrixFromFile(char* filename) {
    // Open the file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
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
        if (rows == rowCapacity) {
            rowCapacity *= 2;
            matrix = (int**)realloc(matrix, rowCapacity * sizeof(int*));
        }

        // Allocate memory for a new column if needed
        if (cols == 0) {
            matrix[rows] = (int*)malloc(colCapacity * sizeof(int));
        } else if (cols == colCapacity) {
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

    Matrix result = {matrix, rows, cols};
    return result;
}

int main(){
    char* allocation_file = "allocations.txt";
    char* resource_file = "resources.txt";
    char* request_file = "requests.txt";

    Matrix allocations = readMatrixFromFile(allocation_file);
    Matrix resources = readMatrixFromFile(resource_file);
    Matrix requests = readMatrixFromFile(request_file);

    // print the allocations matrix
    printf("Allocations matrix:\n");
    for (int i = 0; i < allocations.rows; i++) {
        for (int j = 0; j < allocations.cols; j++) {
            printf("%d ", allocations.matrix[i][j]);
        }
        printf("\n");
    }
    //print resources matrix
    printf("Resources matrix:\n");
    for (int i = 0; i < resources.rows; i++) {
        for (int j = 0; j < resources.cols; j++) {
            printf("%d ", resources.matrix[i][j]);
        }
        printf("\n");
    }
    //print requests matrix
    printf("Requests matrix:\n");
    for (int i = 0; i < requests.rows; i++) {
        for (int j = 0; j < requests.cols; j++) {
            printf("%d ", requests.matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}