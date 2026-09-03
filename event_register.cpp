#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Structure for Event
struct Event {
    int id;
    string name;
    string date;
};

// Structure for Registration
struct Registration {
    int regId;
    int eventId;
    string userName;
    string userEmail;
};

// File handling functions for Events
vector<Event> loadEvents() {
    vector<Event> events;
    ifstream file("events.txt");
    if (!file) return events;
    Event e;
    while (file >> e.id) {
        file.ignore();
        getline(file, e.name);
        getline(file, e.date);
        events.push_back(e);
    }
    file.close();
    return events;
}

void saveEvents(const vector<Event>& events) {
    ofstream file("events.txt");
    for (const auto& e : events) {
        file << e.id << "\n" << e.name << "\n" << e.date << "\n";
    }
    file.close();
}

// File handling functions for Registrations
vector<Registration> loadRegistrations() {
    vector<Registration> regs;
    ifstream file("registrations.txt");
    if (!file) return regs;
    Registration r;
    while (file >> r.regId >> r.eventId) {
        file.ignore();
        getline(file, r.userName);
        getline(file, r.userEmail);
        regs.push_back(r);
    }
    file.close();
    return regs;
}

void saveRegistrations(const vector<Registration>& regs) {
    ofstream file("registrations.txt");
    for (const auto& r : regs) {
        file << r.regId << "\n" << r.eventId << "\n" << r.userName << "\n" << r.userEmail << "\n";
    }
    file.close();
}

// 1. Create a new event
void createEvent(vector<Event>& events) {
    Event e;
    cout << "\nEnter Event ID: ";
    cin >> e.id;
    cin.ignore();
    cout << "Enter Event Name: ";
    getline(cin, e.name);
    cout << "Enter Event Date (DD-MM-YYYY): ";
    getline(cin, e.date);

    events.push_back(e);
    saveEvents(events);
    cout << "Event created successfully!\n";
}

// 2. View all events
void viewEvents(const vector<Event>& events) {
    if (events.empty()) {
        cout << "\nNo events available.\n";
        return;
    }
    cout << "\n--- Available Events ---\n";
    for (const auto& e : events) {
        cout << "ID: " << e.id << " | Name: " << e.name << " | Date: " << e.date << "\n";
    }
}

// 3. Register a user for an event
void registerForEvent(const vector<Event>& events, vector<Registration>& regs) {
    if (events.empty()) {
        cout << "\nNo events to register for.\n";
        return;
    }

    int eventId;
    cout << "\nEnter Event ID you want to register for: ";
    cin >> eventId;

    bool found = false;
    for (const auto& e : events) {
        if (e.id == eventId) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Event ID not found!\n";
        return;
    }

    Registration r;
    r.regId = regs.size() + 1;
    r.eventId = eventId;
    cin.ignore();
    cout << "Enter Your Name: ";
    getline(cin, r.userName);
    cout << "Enter Your Email: ";
    getline(cin, r.userEmail);

    regs.push_back(r);
    saveRegistrations(regs);
    cout << "Registration successful! Your Registration ID is " << r.regId << "\n";
}

// 4. View all registrations
void viewRegistrations(const vector<Registration>& regs) {
    if (regs.empty()) {
        cout << "\nNo registrations found.\n";
        return;
    }
    cout << "\n--- All Event Registrations ---\n";
    for (const auto& r : regs) {
        cout << "Reg ID: " << r.regId << " | Event ID: " << r.eventId 
             << " | Name: " << r.userName << " | Email: " << r.userEmail << "\n";
    }
}

// 5. Cancel a registration
void cancelRegistration(vector<Registration>& regs) {
    int regId;
    cout << "\nEnter Registration ID to cancel: ";
    cin >> regId;

    for (auto it = regs.begin(); it != regs.end(); ++it) {
        if (it->regId == regId) {
            regs.erase(it);
            saveRegistrations(regs);
            cout << "Registration cancelled successfully!\n";
            return;
        }
    }
    cout << "Registration ID not found.\n";
}

int main() {
    vector<Event> events = loadEvents();
    vector<Registration> regs = loadRegistrations();
    int choice;

    do {
        cout << "\n=================================\n";
        cout << "    EVENT REGISTRATION SYSTEM    \n";
        cout << "=================================\n";
        cout << "1. Create New Event\n";
        cout << "2. View All Events\n";
        cout << "3. Register for an Event\n";
        cout << "4. View All Registrations\n";
        cout << "5. Cancel Registration\n";
        cout << "6. Exit\n";
        cout << "Enter choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1: createEvent(events); break;
            case 2: viewEvents(events); break;
            case 3: registerForEvent(events, regs); break;
            case 4: viewRegistrations(regs); break;
            case 5: cancelRegistration(regs); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}
