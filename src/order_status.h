#ifndef ORDER_STATUS_H
#define ORDER_STATUS_H

#define MAX_ORDERS 100

typedef enum {
    DRAWING,
    COLLECTING_MATERIALS,
    CUTTING,
    CLEANING_ARRANGING,
    COUNTING_PREPARATION,
    SHIPPING_FINISHED,
    FINISHED
} OrderStatus;

typedef struct {
    int order_number;
    OrderStatus status;
} Order;

void initialize_orders();
Order* find_order(int order_number);
void update_order_status(int order_number, OrderStatus status);
OrderStatus get_order_status(int order_number);
int create_order();

#endif
