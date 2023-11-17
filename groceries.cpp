#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include "split.h"

using namespace std;

// Define the Customer class
class Customer {
public:
    int id;
    string name;
    string street;
    string city;
    string state;
    string zip;
    string phone;
    string email;

    // Default constructor for Customer
    Customer() : id(0), name(""), street(""), city(""), state(""), zip(""), phone(""), email("") {}

    // Parameterized constructor
    Customer(int id, const string& name, const string& street, const string& city, const string& state,
             const string& zip, const string& phone, const string& email)
        : id(id), name(name), street(street), city(city), state(state), zip(zip), phone(phone), email(email) {}

    // Function to print customer details
    void print_detail() const {
        cout << "Customer ID: " << id << ", Name: " << name << ", Address: " << street << ", " << city << ", " << state << ", " << zip
             << ", Phone: " << phone << ", Email: " << email << endl;
    }
};

// Define the Item class
class Item {
public:
    int id;
    string description;
    double price;

    // Default constructor for Item
    Item() : id(0), description(""), price(0.0) {}

    // Parameterized constructor
    Item(int id, const string& description, double price)
        : id(id), description(description), price(price) {}

    // Function to print item details
    void print_detail() const {
        cout << "Item ID: " << id << ", Description: " << description << ", Price: $" << fixed << setprecision(2) << price << endl;
    }
};

// Define the LineItem class
class LineItem {
private:
    int item_id;
    int qty;

public:
    // Constructor
    LineItem(int item_id, int qty) : item_id(item_id), qty(qty) {}

    // Accessor functions to get item_id and qty
    int get_item_id() const {
        return item_id;
    }

    int get_qty() const {
        return qty;
    }

    // Function to calculate the subtotal for the line item
    double sub_total(const Item& item) const {
        // Assuming items is a global vector of Item objects
        return qty * item.price;
    }
};

// Abstract Payment class
class Payment {
public:
    virtual ~Payment() = default;
    virtual string print_detail() const = 0;
    virtual void set_amount(double amount) = 0;
    virtual void set_payment_details(int code, const string& info) = 0; // Add this line
};

// Derived CreditCardPayment class
class CreditCardPayment : public Payment {
private:
    string card_number;
    string expiration_date;
    double amount;

public:
    // Constructor
    CreditCardPayment(const string& card_number, const string& expiration_date)
        : card_number(card_number), expiration_date(expiration_date), amount(0.0) {}

    // Override print_detail
    string print_detail() const override {
        return "Credit Card: Card Number - " + card_number + ", Expiration Date - " + expiration_date;
    }

    // Implement set_amount
    void set_amount(double amount) override {
        this->amount = amount;
    }

    // Implement set_payment_details
    void set_payment_details(int code, const string& info) override {
        // Implement as needed
    }
};

// Derived PayPalPayment class
class PayPalPayment : public Payment {
private:
    string paypal_id;
    double amount;

public:
    // Constructor
    PayPalPayment(const string& paypal_id) : paypal_id(paypal_id), amount(0.0) {}

    // Override print_detail
    string print_detail() const override {
        return "PayPal: PayPal ID - " + paypal_id;
    }

    // Implement set_amount
    void set_amount(double amount) override {
        this->amount = amount;
    }

    // Implement set_payment_details
    void set_payment_details(int code, const string& info) override {
        // Implement as needed
    }
};

// Derived WireTransferPayment class
class WireTransferPayment : public Payment {
private:
    string bank_id;
    string account_id;
    double amount;

public:
    // Constructor
    WireTransferPayment(const string& bank_id, const string& account_id)
        : bank_id(bank_id), account_id(account_id), amount(0.0) {}

    // Override print_detail
    string print_detail() const override {
        return "Wire Transfer: Bank ID - " + bank_id + ", Account ID - " + account_id;
    }

    // Implement set_amount
    void set_amount(double amount) override {
        this->amount = amount;
    }

    // Implement set_payment_details
    void set_payment_details(int code, const string& info) override {
        // Implement as needed
    }
};

// Define the Order class
class Order {
public:
    int customer_id;
    int order_number;
    string order_date;
    list<LineItem> line_items;
    unique_ptr<Payment> payment;
    int payment_code;
    string payment_info;

    // Default constructor for Order
    Order() : customer_id(0), order_number(0), order_date(""), payment(nullptr), payment_code(0), payment_info("") {}

    // Parameterized constructor
    Order(int customer_id, int order_number, const string& order_date, const list<LineItem>& line_items,
          unique_ptr<Payment>&& payment, int payment_code, const string& payment_info)
        : customer_id(customer_id), order_number(order_number), order_date(order_date),
          line_items(line_items), payment(std::move(payment)), payment_code(payment_code), payment_info(payment_info) {
    }

    // Function to calculate total cost of the order
    double total(const vector<Item>& items) const {
        double total_cost = 0.0;
        for (const auto& line_item : line_items) {
            // Assuming items is a global vector of Item objects
            const Item& item = items[line_item.get_item_id() - 1];  // Adjust for 1-based index
            total_cost += line_item.sub_total(item);
        }
        return total_cost;
    }

    // Function to print an order and return a formatted string
        string print_order(const Customer& customer, const vector<Item>& items) const {
        ostringstream oss;
        oss << "===========================\n";
        oss << "Order #" << order_number << ", Date: " << order_date << "\n";
        oss << "Amount: $" << fixed << setprecision(2) << total(items) << ", " << "Paid by " << payment->print_detail() << "\n";
        oss << "Customer ID #" << customer.id << ":\n";
        customer.print_detail();
        oss << "Order Detail:\n";

        // Sort line items by item_id
        vector<LineItem> sorted_line_items(line_items.begin(), line_items.end());
        sort(sorted_line_items.begin(), sorted_line_items.end(),
             [](const LineItem& a, const LineItem& b) { return a.get_item_id() < b.get_item_id(); });

        for (const auto& line_item : sorted_line_items) {
            int item_id = line_item.get_item_id();
            int quantity = line_item.get_qty();
            // Assuming items is a global vector of Item objects
            const Item& item = items[item_id - 1]; // Adjust for 1-based index
            oss << "Item " << item_id << ": \"" << item.description << "\", " << quantity << " @ $" << fixed << setprecision(2) << item.price << "\n";
        }

        oss << "===========================\n";
        return oss.str();
    }
};

// Global vectors to store customers and items
vector<Customer> customers;
vector<Item> items;

// Global list to store orders
list<Order> orders;

// Function to read customer data from a file
void read_customers(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening customers file." << endl;
        exit(EXIT_FAILURE);
    }

    Customer customer;
    while (file >> customer.id >> ws && getline(file, customer.name, ',') >> ws &&
           getline(file, customer.street, ',') >> ws &&
           getline(file, customer.city, ',') >> ws &&
           getline(file, customer.state, ',') >> ws &&
           getline(file, customer.zip, ',') >> ws &&
           getline(file, customer.phone, ',') >> ws &&
           getline(file, customer.email)) {
        customers.push_back(customer);
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
    while (file >> item.id >> ws && getline(file, item.description, ',') >> ws &&
           file >> item.price) {
        items.push_back(item);
    }
    file.close();
}

// Function to read order data from a file
void read_orders(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening orders file." << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Order order;

        // Read customer_id, order_number, order_date
        iss >> order.customer_id >> order.order_number >> order.order_date;

        // Read item_id-quantity pairs
        string item_qty_pair;
        while (iss >> item_qty_pair) {
            vector<string> pair_parts = split(item_qty_pair, '-');
            int item_id = stoi(pair_parts[0]);
            int quantity = stoi(pair_parts[1]);
            order.line_items.emplace_back(item_id, quantity);
        }

        // Read payment code and payment info
        iss >> order.payment_code >> ws;
        getline(iss, order.payment_info);

        // Create Payment object based on payment code
        switch (order.payment_code) {
            case 1: {
                string card_number, expiration_date;
                iss >> card_number >> expiration_date;
                order.payment = make_unique<CreditCardPayment>(card_number, expiration_date);
                break;
            }
            case 2: {
                string paypal_id;
                iss >> paypal_id;
                order.payment = make_unique<PayPalPayment>(paypal_id);
                break;
            }
            case 3: {
                string bank_id, account_id;
                iss >> bank_id >> account_id;
                order.payment = make_unique<WireTransferPayment>(bank_id, account_id);
                break;
            }
            default:
                cerr << "Invalid payment code." << endl;
                exit(EXIT_FAILURE);
        }

        // Add the order to the global list
        orders.push_back(order);
    }

    file.close();
}

int main() {
    read_customers("customers.txt");
    read_items("items.txt");
    read_orders("orders.txt");

    ofstream ofs("order_report.txt");
    if (!ofs) {
        cerr << "Error opening order_report.txt for writing." << endl;
        return EXIT_FAILURE;
    }

    // Test printing orders
    for (const auto& order : orders) {
        const Customer& customer = customers[order.customer_id - 1]; // Assuming customer_id is 1-based
        ofs << order.print_order(customer) << endl;
    }

    return 0;
}
