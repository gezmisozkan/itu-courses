// Özkan Gezmiş 150200033

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define NUM_OF_CUSTOMERS 3  // number of customers
#define NUM_OF_PRODUCTS 5 // number of products

typedef struct {  // Customer struct
    int id;
    double balance;
    double initial_balance;
    int ordered_items[NUM_OF_PRODUCTS];
    int purchased_items[NUM_OF_PRODUCTS];
} Customer;

typedef struct {  // Product struct
    int id;
    double price;
    int quantity;
} Product;

typedef struct {  // OrderedProduct struct
    Customer* customer;
    int id;
    int ordered_count;
} OrderedProduct;

Product* products;  // global variables
Customer* customers;
pthread_mutex_t lock;

void order_product(OrderedProduct* ordered_product){  // order a product
    if(ordered_product == NULL ||   ordered_product->ordered_count == 0 || ordered_product->customer == NULL){  // if ordered_product is NULL or ordered_count is 0 or customer is NULL
        return;
    }
    Customer* customer = ordered_product->customer;  // get customer
    if(customer == NULL) // if customer is NULL return
        return;  
    pthread_mutex_lock(&lock);  // lock the critical section
    int purchasable_quantity = customer->balance / products[ordered_product->id].price;  // calculate purchasable quantity

    if(ordered_product->ordered_count > purchasable_quantity){  // if customer has insufficient funds
        printf("fail customer %d tried to buy %d piece(s) for item %d but customer %d has insufficient funds.\n",customer->id, ordered_product->ordered_count, ordered_product->id, customer->id);
    }
    else if(products[ordered_product->id].quantity >= ordered_product->ordered_count){  // if there is enough stock
        products[ordered_product->id].quantity -= ordered_product->ordered_count;
        customer->purchased_items[ordered_product->id] = ordered_product->ordered_count;
        customer->balance -= products[ordered_product->id].price * ordered_product->ordered_count;
        printf("success! customer %d paid %.2f for %d piece(s) for item %d.\n",customer->id ,products[ordered_product->id].price * ordered_product->ordered_count, ordered_product->ordered_count, ordered_product->id);
    }
    else if(ordered_product->ordered_count > products[ordered_product->id].quantity){  // if there is not enough stock
        printf("fail customer %d tried to buy %d piece(s) for item %d but only %d left in stock.\n",customer->id, ordered_product->ordered_count, ordered_product->id, products[ordered_product->id].quantity);
    }
    pthread_mutex_unlock(&lock);  // unlock the critical section
}

void order_products(OrderedProduct* ordered_products) {  // order list of products
    for(int i=0;i<NUM_OF_PRODUCTS;i++){  // for each ordered product
        if(ordered_products[i].ordered_count > 0)  
            order_product(&ordered_products[i]);  // order the product
    }
}

void* randomly_order_products(void* arg){  // randomly order products, it is a thread function
    int customer_id = *(int*)arg;  // get customer id
    free(arg); // Free the allocated memory
    Customer* customer = &customers[customer_id];
    int scenario = rand()%2 + 1;  // randomly choose scenario 1 or 2
    // Scenario 1: customer orders just 1 product
    if(scenario == 1){
        OrderedProduct ordered_product;
        ordered_product.customer = customer;
        ordered_product.id = rand()%NUM_OF_PRODUCTS;  // randomly choose a product
        ordered_product.ordered_count = rand()%5 +1 ; // order at most 5 items
        customer->ordered_items[ordered_product.id] = ordered_product.ordered_count;  // update ordered items
        order_product(&ordered_product);  // order the product
    }
    // Scenario 2: customer orders list of products
    else if(scenario == 2){
        OrderedProduct* ordered_products = malloc(sizeof(OrderedProduct)*NUM_OF_PRODUCTS) ;  // allocate memory for ordered products
        for(int i=0;i<NUM_OF_PRODUCTS;i++){  // for each product
            int flag = rand()%2; // if flag is 0 then order this product, if 1 don't give an order for this product
            if(flag == 1){
                ordered_products[i].customer = customer;
                ordered_products[i].id = rand()%NUM_OF_PRODUCTS;  // randomly choose a product
                ordered_products[i].ordered_count = rand()%5 + 1; // order at most 5 items
                customer->ordered_items[ordered_products[i].id] = ordered_products[i].ordered_count;  // update ordered items
            }
        }
        order_products(ordered_products);  // order the list of products
        free(ordered_products);  // free the memory
    }
    pthread_exit(NULL);  // exit the thread
}
void print_customers(){  // print customers
    for(int i=0;i<NUM_OF_CUSTOMERS;i++){  // for each customer
        printf("Customer %d-----------\n",customers[i].id);
        printf("initial balance %.2f \n",customers[i].initial_balance);
        printf("updated balance %.2f \n",customers[i].balance);
        printf("Ordered products: \n");
        printf("id \t quantity\n");
        for(int j=0;j<NUM_OF_PRODUCTS;j++){
            if(customers[i].ordered_items[j] > 0)
                printf("%d \t %d\n",j, customers[i].ordered_items[j]);
        }
        printf("\n");
        printf("Purchased items: \n");
        printf("id \t quantity\n");
        int count=0;
        for(int j=0;j<NUM_OF_PRODUCTS;j++){
            if(customers[i].purchased_items[j] > 0){
                printf("%d \t %d\n",j, customers[i].purchased_items[j]);
                count++;
            }
        }
        if(count == 0){
            printf("- \t -\n");
        }
        printf("\n...\n");
    }
}

void print_products(){  // print products
    printf("Product ID    Quantity    Price\n");
    for(int i=0;i<NUM_OF_PRODUCTS;i++){
        printf("%d \t      %d \t  %.2f\n",products[i].id, products[i].quantity, products[i].price);
    }
}

int main() {
    pthread_t* threads = malloc(sizeof(pthread_t)*NUM_OF_CUSTOMERS);  // allocate memory for threads
    customers = malloc(sizeof(Customer)*NUM_OF_CUSTOMERS);  // allocate memory for customers
    products = malloc(sizeof(Product)*NUM_OF_PRODUCTS);  // allocate memory for products
    pthread_mutex_init(&lock, NULL);  // initialize mutex
    srand(time(NULL));  // seed for random number generator

    for(int i=0;i<NUM_OF_PRODUCTS;i++){  // for each product assign id, price and quantity
        products[i].id = i;  
        double random_price = ((double)rand()/RAND_MAX)*200;  // generate random price between 0 and 200
        products[i].price = 1+ ((double)(int)(random_price*100))/100;  // round to 2 decimal places
        products[i].quantity = rand()%10 + 1;  // generate random quantity between 1 and 10
    }
    for(int i=0;i<NUM_OF_CUSTOMERS;i++){  // for each customer assign id, balance, initial_balance, and initialize ordered_items and purchased_items
        customers[i].id = i;
        customers[i].balance = 1 + rand()%200;  // generate random balance between 1 and 200
        customers[i].initial_balance = customers[i].balance;  // set initial balance
        for(int j=0;j<NUM_OF_PRODUCTS;j++){  // for each product initialize ordered_items and purchased_items
            customers[i].ordered_items[j] = 0;
            customers[i].purchased_items[j] = 0;
        }
    }
    printf("Initial Products: \n");  // print initial products
    print_products();

    struct timeval start, end;
    gettimeofday(&start, NULL);
    // clock_t start = clock();  // start the clock

    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {  // create thread
        int *customer_id = malloc(sizeof(int)); // Allocate memory for each customer_id to ensure that each thread has a unique customer_id
        *customer_id = i; // Assign the customer_id
        if(pthread_create(&threads[i], NULL, randomly_order_products, customer_id)){  // pass customer_id to thread function
            fprintf(stderr, "pthread_create failure\n");  // error message
            exit(-1);  // exit
        }
    }
    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {  // join threads
        if (pthread_join(threads[i], NULL)){  // if there is an error
            fprintf(stderr, "pthread_join failure\n");  // error message
            exit(-1);  // exit
        }
    }

    // clock_t end = clock();  // end the clock
    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);

    pthread_mutex_destroy(&lock);  // destroy mutex

    print_customers();  // print customers
    printf("Updated Products: \n");  // print updated products
    print_products();

    printf("Time taken: %f seconds\n", time_taken);

    // printf("Time taken: %f\n", (double)(end-start)/CLOCKS_PER_SEC);  // print the time
    return 0;
}