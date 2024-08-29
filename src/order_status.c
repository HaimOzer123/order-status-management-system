#include <stdio.h>
#include "order_status.h"

Order orders[MAX_ORDERS];

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
