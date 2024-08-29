from flask import Flask, request, render_template
import ctypes

app = Flask(__name__)

# Load the C library
order_status_lib = ctypes.CDLL('./order_status.so')

class OrderStatus(ctypes.Enum):
    DRAWING = 0
    COLLECTING_MATERIALS = 1
    CUTTING = 2
    CLEANING_ARRANGING = 3
    COUNTING_PREPARATION = 4
    SHIPPING_FINISHED = 5
    FINISHED = 6

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/customer', methods=['GET', 'POST'])
def customer():
    if request.method == 'POST':
        order_number = int(request.form['order_number'])
        status = order_status_lib.get_order_status(order_number)
        return render_template('customer.html', status=status)
    return render_template('customer.html')

@app.route('/employee', methods=['GET', 'POST'])
def employee():
    if request.method == 'POST':
        order_number = int(request.form['order_number'])
        status = int(request.form['status'])
        order_status_lib.update_order_status(order_number, status)
        return "Status updated"
    return render_template('employee.html')

if __name__ == '__main__':
    app.run(debug=True)
