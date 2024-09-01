import os
from flask import Flask, request, render_template, redirect, url_for
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)

# Set up the SQLite database URI
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///order_statuses.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

# Initialize the database with SQLAlchemy
db = SQLAlchemy(app)

# Define the Order model to map to the orders table in the database
class Order(db.Model):
    order_number = db.Column(db.Integer, primary_key=True)
    status = db.Column(db.Integer, nullable=False)

# Define the status dictionary for converting status codes to human-readable status
status_dict = {
    0: "Drawing",
    1: "Collecting Materials",
    2: "Cutting",
    3: "Cleaning & Arranging",
    4: "Counting & Preparation",
    5: "Shipping & Finished",
    6: "Finished"
}

# Initialize the database and create the orders table if it doesn't exist
@app.before_first_request
def initialize_database():
    db.create_all()

# Route for the main page
@app.route('/')
def index():
    return render_template('index.html')

# Route for customer actions
@app.route('/customer', methods=['GET', 'POST'])
def customer():
    if request.method == 'POST':
        order_number = int(request.form['order_number'])
        print(f"[DEBUG] Customer checking order number: {order_number}")
        order = Order.query.get(order_number)
        if order:
            status = status_dict.get(order.status, "Unknown Status")
        else:
            status = "Unknown Status"
        print(f"[DEBUG] Retrieved status: {status}")
        return render_template('customer.html', status=status)
    return render_template('customer.html')

# Route for employee actions
@app.route('/employee', methods=['GET', 'POST'])
def employee():
    if request.method == 'POST':
        order_number = int(request.form['order_number'])
        status = int(request.form['status'])
        print(f"[DEBUG] Attempting to update order {order_number} to status {status}")
        order = Order.query.get(order_number)
        if order:
            order.status = status
        else:
            order = Order(order_number=order_number, status=status)
            db.session.add(order)
        db.session.commit()
        return redirect(url_for('employee'))

    return render_template('employee.html', status_dict=status_dict)

# Run the Flask application
if __name__ == '__main__':
    app.run(debug=True)
