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

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/customer', methods=['GET', 'POST'])
def customer():
    if request.method == 'POST':
        order_number = int(request.form['order_number'])
        print(f"[DEBUG] Customer checking order number: {order_number}")
        status_code = order_status_lib.get_order_status(order_number)
        status_dict = {
            0: "Drawing",
            1: "Collecting Materials",
            2: "Cutting",
            3: "Cleaning & Arranging",
            4: "Counting & Preparation",
            5: "Shipping & Finished",
            6: "Finished",
            -1: "Unknown Status"
        }
        status = status_dict.get(status_code, "Unknown Status")
        print(f"[DEBUG] Retrieved status: {status}")
        return render_template('customer.html', status=status)
    return render_template('customer.html')

@app.route('/employee', methods=['GET', 'POST'])
def employee():
    if request.method == 'POST':
        order_number = int(request.form['order_number'])
        status = int(request.form['status'])
        print(f"[DEBUG] Attempting to update order {order_number} to status {status}")
        order_status_lib.update_order_status(order_number, status)
        return redirect(url_for('employee'))

    status_dict = {
        0: "Drawing",
        1: "Collecting Materials",
        2: "Cutting",
        3: "Cleaning & Arranging",
        4: "Counting & Preparation",
        5: "Shipping & Finished",
        6: "Finished"
    }
    return render_template('employee.html', status_dict=status_dict)

if __name__ == '__main__':
    app.run(debug=True)
