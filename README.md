# Haim Ozer
 Order Status Management System
---------------------------------------------------------

## Description
This project implements a computerized order status management system, providing both customer and employee portals. The backend logic is built using C, ensuring high performance and efficient handling of order statuses, while the web interface is developed using Python and Flask. Customers can check the status of their orders, and employees can update the status at each stage of the process, from drawing to shipping.
-----------------------------------------------------

## run the script
    step 1: compile the C code 
        Navigate to the src/ directory and compile the C code into a shared library:

        gcc -shared -o order_status.dll -fPIC order_status.c

    step 2: Set Up Python Environment
         Install Flask in your virtual environment:

         pip install flask

    Step 3: Run the Flask App
        Ensure you are in the python/ directory and run the Flask app:

        python app.py

    Step 4: Access the Web Application
        you can access the application using a web browser:

        Customer Portal: http://127.0.0.1:5000/customer
        Employee Portal: http://127.0.0.1:5000/employee
----------------------------------------------------------------

## Project Screenshots
Here are some screenshots of the application:

- [Screenshot 1](https://drive.google.com/file/d/1S-4Hm0_N4YQVRjPVfZNqKjg2703AWLvq/view?usp=sharing)
- [Screenshot 2](https://drive.google.com/file/d/1NcwJhst7dhArs0Pg2oVplvkdRkvm6eiL/view?usp=sharing)


### Key Points:
- **GCC Commands:** The README includes the specific GCC command to compile the C code on Windows.
- **Python Setup:** Instructions for setting up the Python environment and installing Flask are provided.
- **Running the App:** Clear steps on how to run the Flask app and access it via a web browser are included.

