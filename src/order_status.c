#include <stdio.h>
#include "order_status.h"

#define STATUS_FILE "order_statuses.txt"

Order orders[MAX_ORDERS];

void print_orders() {
    for (int i = 0; i < MAX_ORDERS; i++) {
        printf("Order Slot %d: Order Number = %d, Status = %d\n", i, orders[i].order_number, orders[i].status);
    }
}

void save_order_statuses() {
    FILE *file = fopen(STATUS_FILE, "w");
    if (file != NULL) {
        printf("Saving order statuses to file.\n");
        for (int i = 0; i < MAX_ORDERS; i++) {
            if (orders[i].order_number != -1) {
                fprintf(file, "%d %d\n", orders[i].order_number, orders[i].status);
                printf("Saved order %d with status %d to file.\n", orders[i].order_number, orders[i].status);
            }
        }
        fclose(file);
    } else {
        printf("Failed to open file for saving order statuses.\n");
    }
}

void load_order_statuses() {
    FILE *file = fopen(STATUS_FILE, "r");
    if (file != NULL) {
        int order_number;
        int status;
        printf("Loading order statuses from file.\n");
        while (fscanf(file, "%d %d", &order_number, &status) != EOF) {
            printf("Loaded order %d with status %d from file.\n", order_number, status);
            Order *order = find_order(order_number);
            if (order != NULL) {
                order->order_number = order_number;
                order->status = status;
            } else {
                for (int i = 0; i < MAX_ORDERS; i++) {
                    if (orders[i].order_number == -1) {
                        orders[i].order_number = order_number;
                        orders[i].status = status;
                        break;
                    }
                }
            }
        }
        fclose(file);
    } else {
        printf("File not found or failed to open for loading order statuses.\n");
    }
}

void initialize_orders() {
    for (int i = 0; i < MAX_ORDERS; i++) {
        orders[i].order_number = -1; // Initialize with invalid order number
        orders[i].status = DRAWING;  // Default status
    }
    printf("After initialization:\n");
    print_orders(); // Print the order array after initialization
    load_order_statuses(); // Load existing orders from file
    printf("Orders initialized.\n");
}

Order* find_order(int order_number) {
    for (int i = 0; i < MAX_ORDERS; i++) {
        if (orders[i].order_number == order_number) {
            return &orders[i];
        }
    }
    return NULL;
}

int generate_new_order_number() {
    int max_order_number = 0;
    for (int i = 0; i < MAX_ORDERS; i++) {
        if (orders[i].order_number > max_order_number) {
            max_order_number = orders[i].order_number;
        }
    }
    return max_order_number + 1;
}

int create_order() {
    int new_order_number = generate_new_order_number();
    printf("Attempting to create order %d.\n", new_order_number);
    print_orders();  // Print the current status of all orders
    for (int i = 0; i < MAX_ORDERS; i++) {
        if (orders[i].order_number == -1) {
            orders[i].order_number = new_order_number;
            orders[i].status = DRAWING;  // New orders start with DRAWING status
            save_order_statuses();
            printf("Order %d created with status DRAWING\n", new_order_number);
            print_orders();  // Print the status after the order is created
            return new_order_number;
        }
    }
    printf("Failed to create order. Maximum number of orders reached.\n");
    return -1;
}

void update_order_status(int order_number, OrderStatus status) {
    printf("Attempting to update order %d to status %d.\n", order_number, status);
    Order* order = find_order(order_number);
    if (order != NULL) {
        order->status = status;
        save_order_statuses();  // Persist the updated status
        printf("Order %d updated to status %d\n", order_number, status);
    } else {
        printf("Order not found! Attempted to update order number: %d\n", order_number);
    }
}

OrderStatus get_order_status(int order_number) {
    printf("Attempting to get status for order %d.\n", order_number);
    Order* order = find_order(order_number);
    if (order != NULL) {
        printf("Order %d is in status %d.\n", order_number, order->status);
        return order->status;
    }
    printf("Order %d not found.\n", order_number);
    return -1;
}

void reset_orders() {
    for (int i = 0; i < MAX_ORDERS; i++) {
        orders[i].order_number = -1;
        orders[i].status = DRAWING;
    }
    printf("Orders reset.\n");
    print_orders();
}

int main() {
    initialize_orders(); // Ensure this works correctly
    return 0;
}
