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
    }
}

void load_order_statuses() {
    FILE *file = fopen(STATUS_FILE, "r");
    if (file != NULL) {
        int order_number;
        int status;
        while (fscanf(file, "%d %d", &order_number, &status) != EOF) {
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
    }
}


void initialize_orders() {
    for (int i = 0; i < MAX_ORDERS; i++) {
        orders[i].order_number = -1; // Initialize with invalid order number
        orders[i].status = DRAWING;
    }
}

Order* find_order(int order_number) {
    for (int i = 0; i < MAX_ORDERS; i++) {
        if (orders[i].order_number == order_number) {
            return &orders[i];
        }
    }
    return NULL;
}

void update_order_status(int order_number, OrderStatus status) {
    Order* order = find_order(order_number);
    if (order != NULL) {
        order->status = status;
        save_order_statuses();  // Save to file after updating status
    } else {
        printf("Order not found!\n");
    }
}


OrderStatus get_order_status(int order_number) {
    Order* order = find_order(order_number);
    if (order != NULL) {
        return order->status;
    }
    return -1;
}
