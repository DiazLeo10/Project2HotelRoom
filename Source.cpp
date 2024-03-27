#include <iostream>
#include <vector>

using namespace std;

// Abstract Handler
class RoomHandler {
public:
    // Pure virtual function to handle bid requests
    virtual void handleRequest(int bidPrice, int& remainingSuite, int& remainingStandard, int& remainingDeluxe) = 0;

    // Function to set the next handler in the chain
    void setNextHandler(RoomHandler* next) {
        nextHandler = next;
    }

protected:
    RoomHandler* nextHandler; // Pointer to the next handler in the chain
};

// Concrete Handlers
class SuiteHandler : public RoomHandler {
public:
    void handleRequest(int bidPrice, int& remainingSuite, int& remainingStandard, int&remainingDeluxe) override {
        if (bidPrice >= 280 && remainingSuite > 0) {
            // Accept the bid if conditions are met
            cout << "Bid price $" << bidPrice << ", accepted for the Suite room." << endl;
            remainingSuite--;
        }
        else if (nextHandler != nullptr) {
            // Pass the request to the next handler if conditions are not met
            nextHandler->handleRequest(bidPrice, remainingSuite, remainingStandard, remainingDeluxe);
        }
        else {
            // No handler can handle the request, bid rejected
            cout << "Sorry, no rooms available for your bid." << endl;
        }
    }
};

class DeluxeHandler : public RoomHandler {
public:
    void handleRequest(int bidPrice, int& remainingSuite, int& remainingStandard, int& remainingDeluxe) override {
        if ((bidPrice >= 150 && bidPrice < 280 && remainingDeluxe > 0) || (bidPrice >= 280 && remainingSuite == 0)) {
            // Accept the bid if conditions are met
            cout << "Bid price $" << bidPrice << ", accepted for the Deluxe room." << endl;
            remainingDeluxe--;
        }
        else if (nextHandler != nullptr) {
            // Pass the request to the next handler if conditions are not met
            nextHandler->handleRequest(bidPrice, remainingSuite, remainingStandard, remainingDeluxe);
        }
        else {
            // No handler can handle the request, bid rejected
            cout << "Sorry, no rooms available for your bid." << endl;
        }
    }
};

class StandardHandler : public RoomHandler {
public:
    void handleRequest(int bidPrice, int& remainingSuite, int& remainingStandard, int& remainingDeluxe) override {
        if ((bidPrice >= 80 && bidPrice < 150) || (bidPrice >= 150 && remainingDeluxe == 0 && remainingSuite == 0)) {
            // Accept the bid if conditions are met
            cout << "Bid price $" << bidPrice << ", accepted for the Standard room." << endl;
            remainingStandard--;
        }
        else if (nextHandler != nullptr) {
            // Pass the request to the next handler if conditions are not met
            nextHandler->handleRequest(bidPrice, remainingSuite, remainingStandard, remainingDeluxe);
        }
        else {
            // No handler can handle the request, bid rejected
            cout << "Sorry, no rooms available for your bid." << endl;
        }
    }
};

// Client class
class HotelRoomBiddingSystem {
public:
    HotelRoomBiddingSystem() {
        // Initialize handlers
        suiteHandler = new SuiteHandler();
        deluxeHandler = new DeluxeHandler();
        standardHandler = new StandardHandler();

        // Construct chain of responsibility
        suiteHandler->setNextHandler(deluxeHandler);
        deluxeHandler->setNextHandler(standardHandler);
    }

    void processBid(int bidPrice, int& remainingStandard, int& remainingDeluxe, int& remainingSuite) {
        suiteHandler->handleRequest(bidPrice, remainingSuite, remainingStandard, remainingDeluxe);
    }

private:
    RoomHandler* suiteHandler;
    RoomHandler* deluxeHandler;
    RoomHandler* standardHandler;
};

int main() {
    int remainingStandard = 45;
    int remainingDeluxe = 1;
    int remainingSuite = 0;
    int bidInput;
    bool loop = true;
    int counter = 0;

    HotelRoomBiddingSystem biddingSystem;

    // Example bids
    cout << "Welcome to Leo's Hotel \nAvailable Standard rooms: " <<
        remainingStandard << "\nAvailable Deluxe Rooms: " << remainingDeluxe << "\nAvailable Suites: " << remainingSuite << endl;
    while (loop) {
        if (remainingStandard > 0 || remainingDeluxe > 0 || remainingSuite > 0) {
            cout << "\nInput a bidding price: ";
            cin >> bidInput;
            counter++;
            if (bidInput < 80) {
                biddingSystem.processBid(bidInput, remainingStandard, remainingDeluxe, remainingSuite);
                cout << "Please enter a bid price greater than or equal to $80." << endl;
            }
            else {
                biddingSystem.processBid(bidInput, remainingStandard, remainingDeluxe, remainingSuite);
                cout << "Available Standard rooms: " <<
                    remainingStandard << "\nAvailable Deluxe Rooms: " << remainingDeluxe << "\nAvailable Suites: " << remainingSuite << endl;
                if (counter == 2)
                {
                    loop = false;
                }
            }
        }
        else {
            cout << "ALL ROOMS ARE SOLD OUT!" << endl;
            loop = false; // Exit loop as all rooms are sold out
        }
    }

    return 0;
}
