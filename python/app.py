import ctypes
import os
from flask import Flask, request, render_template, redirect, url_for

app = Flask(__name__)

# Load the C library (DLL on Windows)
dll_path = os.path.join(os.path.dirname(__file__), '../src/order_status.dll')
order_status_lib = ctypes.CDLL(dll_path)

# Define argument and return types for the C functions
order_status_lib.get_order_status.argtypes = [ctypes.c_int]
order_status_lib.get_order_status.restype = ctypes.c_int

order_status_lib.update_order_status.argtypes = [ctypes.c_int, ctypes.c_int]
order_status_lib.update_order_status.restype = None

order_status_lib.create_order.argtypes = [ctypes.c_int]
order_status_lib.create_order.restype = None

class OrderStatus:
    DRAWING = 0
    COLLECTING_MATERIALS = 1
    CUTTING = 2
    CLEANING_ARRANGING = 3
    COUNTING_PREPARATION = 4
    SHIPPING_FINISHED = 5
    FINISHED = 6

status_dict = {
    OrderStatus.DRAWING: "Drawing",
    OrderStatus.COLLECTING_MATERIALS: "Collecting Materials",
    OrderStatus.CUTTING: "Cutting",
    OrderStatus.CLEANING_ARRANGING: "Cleaning & Arranging",
    OrderStatus.COUNTING_PREPARATION: "Counting & Preparation",
    OrderStatus.SHIPPING_FINISHED: "Shipping & Finished",
    OrderStatus.FINISHED: "Finished"
}

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/customer', methods=['GET', 'POST'])
def customer():
    if request.method == 'POST':
        order_number = int(request.form['order_number'])
        order_status_lib.load_order_statuses()  # Reload orders from file
        status_code = order_status_lib.get_order_status(order_number)
        status = status_dict.get(status_code, "Unknown Status")
        return render_template('customer.html', status=status)
    return render_template('customer.html')


@app.route('/employee', methods=['GET', 'POST'])
def employee():
    if request.method == 'POST':
        action = request.form['action']
        order_number = int(request.form['order_number'])

        if action == "Create Order":
            order_status_lib.create_order(order_number)
        elif action == "Update Order":
            status = int(request.form['status'])
            order_status_lib.update_order_status(order_number, status)

        return redirect(url_for('employee'))

    return render_template('employee.html', status_dict=status_dict)

if __name__ == '__main__':
    app.run(debug=True)
