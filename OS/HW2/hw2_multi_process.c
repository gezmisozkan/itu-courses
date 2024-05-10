#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h>

#define NUM_OF_CUSTOMERS 100
#define NUM_OF_PRODUCTS 1000

typedef struct {
    int id;
    double balance;
    double initial_balance;
    int ordered_items[NUM_OF_PRODUCTS];
    int purchased_items[NUM_OF_PRODUCTS];
} Customer;

typedef struct {
    int id;
    double price;
    int quantity;
} Product;

typedef struct {
    Customer* customer;
    int id;
    int ordered_count;
} OrderedProduct;

Product* products;
Customer* customers;
pthread_mutex_t lock;

void order_product(OrderedProduct* ordered_product){
    if(ordered_product == NULL ||   ordered_product->ordered_count == 0 || ordered_product->customer == NULL){   
        return;
    }
    Customer* customer = ordered_product->customer;
    pthread_mutex_lock(&lock);
    int purchasable_quantity = customer->balance / products[ordered_product->id].price;

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
    pthread_mutex_unlock(&lock);
}

void order_products(OrderedProduct* ordered_products) {
    for(int i=0;i<NUM_OF_PRODUCTS;i++){  // for each ordered product
        if(ordered_products[i].ordered_count > 0)
            order_product(&ordered_products[i]);
    }
}

void randomly_order_products(int arg){
    int customer_id = arg;
    Customer* customer = &customers[customer_id];
    int scenario = rand()%2 + 1;  // randomly choose scenario 1 or 2
    // Scenario 1: customer orders just 1 product
    if(scenario == 1){
        OrderedProduct ordered_product;
        ordered_product.customer = customer;
        ordered_product.id = rand()%NUM_OF_PRODUCTS;
        ordered_product.ordered_count = rand()%5 +1 ; // order at most 5 items
        customer->ordered_items[ordered_product.id] = ordered_product.ordered_count;
        order_product(&ordered_product);
    }
    // Scenario 2: customer orders list of products
    else if(scenario == 2){
        OrderedProduct* ordered_products = malloc(sizeof(OrderedProduct)*NUM_OF_PRODUCTS) ;
        for(int i=0;i<NUM_OF_PRODUCTS;i++){
            int flag = rand()%2; // if flag is 0 then order this product, if 1 don't give an order for this product
            if(flag == 1){
                ordered_products[i].customer = customer;
                ordered_products[i].id = rand()%NUM_OF_PRODUCTS;
                ordered_products[i].ordered_count = rand()%5 + 1; // order at most 5 items
                customer->ordered_items[ordered_products[i].id] = ordered_products[i].ordered_count;
            }
        }
        order_products(ordered_products);
        free(ordered_products);
    }
}
void print_customers(){
    for(int i=0;i<NUM_OF_CUSTOMERS;i++){
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

void print_products(){
    printf("Product ID    Quantity    Price\n");
    for(int i=0;i<NUM_OF_PRODUCTS;i++){
        printf("%d \t      %d \t  %.2f\n",products[i].id, products[i].quantity, products[i].price);
    }
}

int main() {
    pthread_mutex_init(&lock, NULL);
    srand(time(NULL));
    // Define flags for memory protection and mapping
    int prot_flags = PROT_READ | PROT_WRITE;
    int map_flags = MAP_SHARED | MAP_ANONYMOUS;
    customers = (Customer*)mmap(NULL, sizeof(Customer) * NUM_OF_CUSTOMERS, prot_flags, map_flags, -1, 0);
    products = (Product*)mmap(NULL, sizeof(Product) * NUM_OF_PRODUCTS, prot_flags, map_flags, -1, 0);
    if (customers == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<NUM_OF_PRODUCTS;i++){
        products[i].id = i;
        double random_price = ((double)rand()/RAND_MAX)*20;
        products[i].price = 1+ ((double)(int)(random_price*100))/100;  // round to 2 decimal places
        products[i].quantity = rand()%10 + 1;
    }
    for(int i=0;i<NUM_OF_CUSTOMERS;i++){
        customers[i].id = i;
        customers[i].balance = rand()%201;
        customers[i].initial_balance = customers[i].balance;
        for(int j=0;j<NUM_OF_PRODUCTS;j++){
            customers[i].ordered_items[j] = 0;
            customers[i].purchased_items[j] = 0;
        }
    }
    
    printf("Initial Products: \n");
    print_products();

    for(int i=0;i<NUM_OF_CUSTOMERS;i++){
        pid_t pid = fork();
        if(pid == 0){
            randomly_order_products(i);
            exit(0);
        }
    }
    while(wait(NULL)>0);  // Actually, even though there is no child process at this line, to just make sure that all child processes are terminated, we can use wait(NULL) in a loop.

    print_customers();
    printf("Updated Products: \n");
    print_products();
    
    pthread_mutex_destroy(&lock);
    munmap(customers, sizeof(Customer) * NUM_OF_CUSTOMERS);
    return 0;
}