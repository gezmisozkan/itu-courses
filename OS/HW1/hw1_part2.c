// Özkan Gezmiş 
// 150200033

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_OF_ELEMENTS 1e8  // number of elements in the array
int* max_num_array;  // array to store the maximum number of each thread
int array_size;  // size of the array for each thread

typedef struct {  // struct for parameters which consist of array and its id
    int* array;
    int thread_id;
} parameters;

void* find_max(void* ptr){  // function to find the maximum number in the array
    parameters* p = (parameters*) ptr;  // cast the pointer to parameters
    int* arr = p->array;  // array
    int thread_id = p->thread_id;  // thread id
    int max=0;  // maximum number
    for(int i=0; i<array_size; i++){  // find the maximum number
        if( arr[i]> max){ // if number is greater than max, set the maximum number
            max = arr[i];  
        }   
    }
    printf("Thread id: %d, Max number: %d\n", thread_id, max);  // print the maximum number
    max_num_array[thread_id] = max;  // set the maximum number to the array
    pthread_exit(NULL);  // exit the thread
}

int main(int argc, char* argv[]){
    if(argc != 2){  // if there is no argument
        fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);  // error message
        exit(-1);  // exit
    }

    srand(1773); // seed for random number generator
    int num_of_threads = atoi(argv[1]);   // number of threads 
    array_size = NUM_OF_ELEMENTS/num_of_threads; // size of the array for each thread

    int* org_array = malloc(sizeof(int)*NUM_OF_ELEMENTS) ;  // create the original array
    max_num_array = malloc(sizeof(int)*num_of_threads);  // create the max number array

    for(int i = 0; i < NUM_OF_ELEMENTS; i++){ // fill the original array
        int num = rand();
        org_array[i] = num + num % 1000;  // fill the array with random numbers
    }

    pthread_t* threads = malloc(sizeof(pthread_t)*num_of_threads);  //  thread array
    parameters* p = malloc(sizeof(parameters)*num_of_threads);  // parameters array
    for(int i = 0; i < num_of_threads; i++){  // create arrays for each thread
        int* arr = malloc(sizeof(int)*array_size);  // array for each thread
        for(int j = 0; j < array_size; j++){  // fill the array for each thread
            arr[j] = org_array[j + i*array_size];  // divide array into equally sized arrays. For example if thread number is 2, 0-num_of_elements/2 and num_of_elements/2-num_of_elements
        }
        p[i].array = arr;  // set the array
        p[i].thread_id = i;  // set the thread id
    }

    clock_t start = clock();  // start the clock

    for(int i=0; i<num_of_threads; i++){  // create the threads
        if(pthread_create(&threads[i], NULL, find_max, &p[i])){  // send the parameters to the thread
            fprintf(stderr, "pthread_create failure\n");  // error message
            exit(-1);  // exit
        }
    }
    for(int i=0; i<num_of_threads; i++){  // join the threads
        int res = pthread_join(threads[i], NULL);
        if (res){  // if there is an error
            fprintf(stderr, "pthread_join failure\n");  // error message
            exit(-1);  // exit
        }
    }
    int max=0;
    for(int i=0; i<num_of_threads; i++){  // find the global maximum number
        if(max_num_array[i] > max){
            max = max_num_array[i];
        }
    }

    clock_t end = clock();  // end the clock

    printf("Global max number: %d\n", max);  // print the global sum of the array
    printf("Time taken: %f\n", (double)(end-start)/CLOCKS_PER_SEC);  // print the time


    free(org_array);  // free the original array
    free(max_num_array);  // free the max number array
    free(threads);  // free the threads
    free(p);  // free the parameters

    return 0;
}