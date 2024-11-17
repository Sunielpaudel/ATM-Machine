#include "ATM.hpp"
using namespace atmProject;

int main() {
    ATM atm;
    atm.loadDataFromFile("accounts.dat");

    int accountNumber;
    std::string password;

    std::cout << "Welcome to the ATM\n";
    std::cout << "Enter account number: ";
    std::cin >> accountNumber;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (!atm.login(accountNumber, password)) {
        return 1;
    }

    int choice;
    do {
        atm.displayMenu();
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            atm.checkBalance();
            break;
        case 2:
            atm.deposit();
            break;
        case 3:
            atm.withdraw();
            break;
        case 4:
            atm.transfer();
            break;
        case 5:
            atm.viewTransactionHistory();
            break;
        case 6:
            atm.sortAccountsByBalance();
            break;
        case 7:
            atm.searchAccountByName();
            break;
        case 8:
            std::cout << "Goodbye!\n";
            break;
        default:
            std::cout << "Invalid option.\n";
        }
    } while (choice != 8);

    atm.saveDataToFile("accounts.dat");
    return 0;
}