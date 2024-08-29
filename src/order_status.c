#include <stdio.h>
#include "order_status.h"

#define STATUS_FILE "order_statuses.txt"

Order orders[MAX_ORDERS];

void save_order_statuses() {
    FILE *file = fopen(STATUS_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < MAX_ORDERS; i++) {
            if (orders[i].order_number != -1) {
                fprintf(file, "%d %d\n", orders[i].order_number, orders[i].status);
            }
        }
        fclose(file);
        printf("Saved order statuses to file.\n");
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
        printf("Failed to open file for loading order statuses.\n");
    }
}

void initialize_orders() {
    for (int i = 0; i < MAX_ORDERS; i++) {
        orders[i].order_number = -1; // Initialize with invalid order number
        orders[i].status = DRAWING;  // Default status
    }
    load_order_statuses(); // Load existing orders from file
}

Order* find_order(int order_number) {
    for (int i = 0; i < MAX_ORDERS; i++) {
        if (orders[i].order_number == order_number) {
            return &orders[i];
        }
    }
    return NULL;
}

void create_order(int order_number) {
    printf("Attempting to create order %d.\n", order_number);
    Order* order = find_order(order_number);
    if (order != NULL) {
        printf("Order %d already exists!\n", order_number);
    } else {
        for (int i = 0; i < MAX_ORDERS; i++) {
            if (orders[i].order_number == -1) {
                orders[i].order_number = order_number;
                orders[i].status = DRAWING;  // New orders start with DRAWING status
                save_order_statuses();
                printf("Order %d created with status DRAWING\n", order_number);
                return;
            }
        }
        printf("Failed to create order. Maximum number of orders reached.\n");
    }
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
