#include <iostream>
#include "atm.hpp"

using namespace atmProject;

int main() {
    ATM atm;
    atm.loadDataFromFile("accounts.dat");

    int userType;
    std::cout << "Welcome to ATM Virtual Machine \n";
    std::cout << "Are you a Customer or Administrator?\n";
    std::cout << "1. Customer\n2. Administrator\n";
    std::cin >> userType;

    if (userType == 1) {
        int accountNumber;
        std::string password;

        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;
        std::cout << "Enter Password: ";
        std::cin >> password;

        if (atm.loginCustomer(accountNumber, password)) {
            bool exit = false;
            while (!exit) {
                atm.displayCustomerMenu();
                int choice;
                std::cin >> choice;
                switch (choice) {
                case 1: atm.checkBalance(); break;
                case 2: atm.deposit(); break;
                case 3: atm.withdraw(); break;
                case 4: atm.transfer(); break;
                case 5: atm.viewTransactionHistory(); break;
                case 6: exit = true; break;
                default: std::cout << "Invalid choice.\n"; break;
                }
            }
        }
    }
    else if (userType == 2) {
        bool exit = false;
        while (!exit) {
            atm.displayAdminMenu();
            int choice;
            std::cin >> choice;
            switch (choice) {
            case 1: atm.sortAccountsByBalance(); 
                atm.saveDataToFile("accounts.dat");
                std::cout << "Accounts sorted and saved to file.\n"; 
                break;
            case 2: atm.searchAccountByName(); break;
            case 3: atm.searchAccountByBalance(); break;
            case 4: exit = true; break;
            default: std::cout << "Invalid choice.\n"; break;
            }
        }
    }
    else {
        std::cout << "Invalid user type.\n";
    }

    atm.saveDataToFile("accounts.dat");
    return 0;
}
  