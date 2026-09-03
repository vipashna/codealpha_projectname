#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Structure for Menu Item
struct MenuItem {
    int id;
    string name;
    double price;
};

// Structure for Table Reservation
struct Reservation {
    int tableNumber;
    string customerName;
    int partySize;
};

// File handling for Menu
vector<MenuItem> loadMenu() {
    vector<MenuItem> menu;
    ifstream file("menu.txt");
    if (!file) return menu;
    MenuItem item;
    while (file >> item.id) {
        file.ignore();
        getline(file, item.name);
        file >> item.price;
        menu.push_back(item);
    }
    file.close();
    return menu;
}

void saveMenu(const vector<MenuItem>& menu) {
    ofstream file("menu.txt");
    for (const auto& item : menu) {
        file << item.id << "\n" << item.name << "\n" << item.price << "\n";
    }
    file.close();
}

// File handling for Reservations
vector<Reservation> loadReservations() {
    vector<Reservation> resList;
    ifstream file("reservations.txt");
    if (!file) return resList;
    Reservation r;
    while (file >> r.tableNumber) {
        file.ignore();
        getline(file, r.customerName);
        file >> r.partySize;
        resList.push_back(r);
    }
    file.close();
    return resList;
}

void saveReservations(const vector<Reservation>& resList) {
    ofstream file("reservations.txt");
    for (const auto& r : resList) {
        file << r.tableNumber << "\n" << r.customerName << "\n" << r.partySize << "\n";
    }
    file.close();
}

// 1. Add item to menu
void addMenuItem(vector<MenuItem>& menu) {
    MenuItem item;
    cout << "\nEnter Item ID: ";
    cin >> item.id;
    cin.ignore();
    cout << "Enter Item Name: ";
    getline(cin, item.name);
    cout << "Enter Price: $";
    cin >> item.price;

    menu.push_back(item);
    saveMenu(menu);
    cout << "Menu item added successfully!\n";
}

// 2. View Menu
void viewMenu(const vector<MenuItem>& menu) {
    if (menu.empty()) {
        cout << "\nMenu is currently empty.\n";
        return;
    }
    cout << "\n--- RESTAURANT MENU ---\n";
    for (const auto& item : menu) {
        cout << "ID: " << item.id << " | Name: " << item.name << " | Price: $" << item.price << "\n";
    }
}

// 3. Place Order
void placeOrder(const vector<MenuItem>& menu) {
    if (menu.empty()) {
        cout << "\nNo menu items available to order.\n";
        return;
    }

    viewMenu(menu);
    int itemId, quantity;
    double totalBill = 0.0;
    char addMore;

    do {
        cout << "\nEnter Item ID to order: ";
        cin >> itemId;
        cout << "Enter Quantity: ";
        cin >> quantity;

        bool found = false;
        for (const auto& item : menu) {
            if (item.id == itemId) {
                double cost = item.price * quantity;
                totalBill += cost;
                cout << "Added " << quantity << " x " << item.name << " ($" << cost << ")\n";
                found = true;
                break;
            }
        }

        if (!found) cout << "Invalid Item ID!\n";

        cout << "Add another item? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');

    cout << "\n===============================\n";
    cout << "TOTAL BILL AMOUNT: $" << totalBill << "\n";
    cout << "===============================\n";
}

// 4. Reserve a Table
void reserveTable(vector<Reservation>& resList) {
    Reservation r;
    cout << "\nEnter Table Number (1-10): ";
    cin >> r.tableNumber;

    for (const auto& existing : resList) {
        if (existing.tableNumber == r.tableNumber) {
            cout << "Table " << r.tableNumber << " is already reserved!\n";
            return;
        }
    }

    cin.ignore();
    cout << "Enter Customer Name: ";
    getline(cin, r.customerName);
    cout << "Enter Party Size (Number of guests): ";
    cin >> r.partySize;

    resList.push_back(r);
    saveReservations(resList);
    cout << "Table " << r.tableNumber << " reserved successfully for " << r.customerName << "!\n";
}

// 5. View Reservations
void viewReservations(const vector<Reservation>& resList) {
    if (resList.empty()) {
        cout << "\nNo active table reservations.\n";
        return;
    }
    cout << "\n--- CURRENT TABLE RESERVATIONS ---\n";
    for (const auto& r : resList) {
        cout << "Table No: " << r.tableNumber 
             << " | Reserved For: " << r.customerName 
             << " | Guests: " << r.partySize << "\n";
    }
}

int main() {
    vector<MenuItem> menu = loadMenu();
    vector<Reservation> resList = loadReservations();
    int choice;

    do {
        cout << "\n=================================\n";
        cout << "  RESTAURANT MANAGEMENT SYSTEM   \n";
        cout << "=================================\n";
        cout << "1. Add Menu Item\n";
        cout << "2. View Menu\n";
        cout << "3. Place Order & Calculate Bill\n";
        cout << "4. Reserve a Table\n";
        cout << "5. View All Reservations\n";
        cout << "6. Exit\n";
        cout << "Enter choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1: addMenuItem(menu); break;
            case 2: viewMenu(menu); break;
            case 3: placeOrder(menu); break;
            case 4: reserveTable(resList); break;
            case 5: viewReservations(resList); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice! Select 1-6.\n";
        }
    } while (choice != 6);

    return 0;
}
