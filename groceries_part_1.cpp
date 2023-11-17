#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "split.h" // Include the split.h file

using namespace std;

// Define the Customer struct
struct Customer {
    int id;
    string name;
    string street;
    string city;
    string state;
    string zip;
    string phone;
    string email;
};

// Define the Item struct
struct Item {
    int id;
    string description;
    double price;
};

// Global vectors to store customers and items
vector<Customer> customers;
vector<Item> items;

// Function to read customer data from a file
void read_customers(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening customers file." << endl;
        exit(EXIT_FAILURE);
    }

    Customer customer;
    string line;
    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (fields.size() == 8) {
            customer.id = stoi(fields[0]);
            customer.name = fields[1];
            customer.street = fields[2];
            customer.city = fields[3];
            customer.state = fields[4];
            customer.zip = fields[5];
            customer.phone = fields[6];
            customer.email = fields[7];
            customers.push_back(customer);
        }
    }
    file.close();
}

// Function to read item data from a file
void read_items(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening items file." << endl;
        exit(EXIT_FAILURE);
    }

    Item item;
    string line;
    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (fields.size() == 3) {
            item.id = stoi(fields[0]);
            item.description = fields[1];
            item.price = stod(fields[2]);
            items.push_back(item);
        }
    }
    file.close();
}


// Function to perform one customer's order
void one_customer_order() {
    cout << "Number of customers: " << customers.size() << endl;
    cout << "Number of items: " << items.size() << endl;

    int customerNumber;
    cout << "Enter customer number: ";
    cin >> customerNumber;

    // Search for the customer
    Customer customer;
    bool customerFound = false;
    for (const auto& c : customers) {
        if (c.id == customerNumber) {
            customer = c;
            customerFound = true;
            break;
        }
    }

    if (!customerFound) {
        cout << "Customer not found. Exiting program." << endl;
        exit(EXIT_SUCCESS);
    }

    double totalCost = 0.0;
    int itemCount = 0;

    int itemNumber;
    do {
        cout << "Enter item number to purchase (0 to finish): ";
        cin >> itemNumber;

        if (itemNumber == 0) {
            break; // Exit loop if the user enters 0
        }

        // Search for the item
        Item item;
        bool itemFound = false;
        for (const auto& i : items) {
            if (i.id == itemNumber) {
                item = i;
                itemFound = true;
                break;
            }
        }

        if (itemFound) {
            cout << "Item: " << item.description << ", Price: $" << fixed << setprecision(2) << item.price << endl;
            totalCost += item.price;
            itemCount++;
        } else {
            cout << "Item not found. Please enter a valid item number." << endl;
        }

    } while (itemNumber != 0);

    // Display order summary
    cout << "Number of items purchased: " << itemCount << endl;
    cout << "Total cost: $" << fixed << setprecision(2) << totalCost << endl;
}

int main() {
    read_customers("customers.txt"); // Step 1 above
    read_items("items.txt");         // Step 2 above
    one_customer_order();            // The rest of the steps
    return 0;
}
