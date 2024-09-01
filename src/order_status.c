#include <stdio.h>
#include <sqlite3.h>
#include "order_status.h"

sqlite3 *db;

// Function to initialize the SQLite database and create the orders table if it doesn't exist
void initialize_orders() {
    int rc = sqlite3_open("order_statuses.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    } else {
        printf("Opened database successfully\n");
    }

    const char *sql_create_table = 
        "CREATE TABLE IF NOT EXISTS orders ("
        "order_number INTEGER PRIMARY KEY,"
        "status INTEGER NOT NULL);";

    char *err_msg = 0;
    rc = sqlite3_exec(db, sql_create_table, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Table 'orders' is ready.\n");
    }
}

// Function to generate a new unique order number
int generate_new_order_number() {
    const char *sql_max_order = "SELECT MAX(order_number) FROM orders;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_max_order, -1, &stmt, 0);

    int max_order_number = 0;
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            max_order_number = sqlite3_column_int(stmt, 0);
        }
    } else {
        fprintf(stderr, "Failed to retrieve max order number: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return max_order_number + 1;
}

// Function to create a new order
int create_order() {
    int new_order_number = generate_new_order_number();
    printf("Attempting to create order %d.\n", new_order_number);

    const char *sql_insert = "INSERT INTO orders (order_number, status) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, new_order_number);
        sqlite3_bind_int(stmt, 2, DRAWING);
        sqlite3_step(stmt);
        printf("Order %d created with status DRAWING.\n", new_order_number);
    } else {
        fprintf(stderr, "Failed to create order: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return new_order_number;
}

// Function to update the status of an existing order
void update_order_status(int order_number, OrderStatus status) {
    printf("Attempting to update order %d to status %d.\n", order_number, status);

    const char *sql_update = "UPDATE orders SET status = ? WHERE order_number = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_update, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, status);
        sqlite3_bind_int(stmt, 2, order_number);
        sqlite3_step(stmt);
        printf("Order %d updated to status %d.\n", order_number, status);
    } else {
        fprintf(stderr, "Failed to update order: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

// Function to get the status of an existing order
OrderStatus get_order_status(int order_number) {
    printf("Attempting to get status for order %d.\n", order_number);

    const char *sql_select = "SELECT status FROM orders WHERE order_number = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);

    OrderStatus status = -1;
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, order_number);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            status = sqlite3_column_int(stmt, 0);
            printf("Order %d is in status %d.\n", order_number, status);
        } else {
            printf("Order %d not found.\n", order_number);
        }
    } else {
        fprintf(stderr, "Failed to get order status: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return status;
}

// Function to reset all orders (for testing purposes)
void reset_orders() {
    const char *sql_reset = "DELETE FROM orders;";
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql_reset, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("All orders reset.\n");
    }
}

// Main function to initialize orders
int main() {
    initialize_orders();
    return 0;
}
