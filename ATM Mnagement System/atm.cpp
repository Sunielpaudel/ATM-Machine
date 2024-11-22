#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include "atm.hpp"

namespace atmProject {

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    void ATM::loadDataFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Failed to open file for reading.\n";
            return;
        }

        int accNum;
        std::string holder, pass, typeStr;
        double balance;
        AccountType type;

        while (file >> accNum) {
            file.ignore();  
            std::getline(file, holder);
            holder = trim(holder);
            if (holder.empty()) continue;
            std::getline(file, pass);
            pass = trim(pass);
            if (pass.empty()) continue;

            file >> balance;
            file.ignore(); 

            std::getline(file, typeStr);
            typeStr = trim(typeStr);
            while (typeStr.empty() && std::getline(file, typeStr)) {
                typeStr = trim(typeStr);
            }

            if (typeStr == "Checking") {
                type = AccountType::Checking;
            }
            else if (typeStr == "Savings" || typeStr == "Saving") {  
                type = AccountType::Savings;
            }
            else {
                std::cout << "Error: Unknown account type: '" << typeStr << "'\n";
                continue;  
            }

            accounts.push_back(std::make_unique<Account>(accNum, holder, pass, balance, type));

        }

        file.close();
    }




    // Function to save account data to a file

    void ATM::saveDataToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::trunc);
        if (!file.is_open()) {
            std::cout << "Failed to open file for writing.\n";
            return;
        }

        for (const auto& acc : accounts) {
            file << acc->accountNumber << " "
                << acc->accountHolderName << " "
                << acc->password << " "
                << acc->balance << " "
                << (acc->type == AccountType::Checking ? "Checking" : "Savings") << "\n";
        }

        file.close();
    }

    // Function to display the ATM menu
    void ATM::displayMenu() {
        std::cout << "\nATM Menu:\n";
        std::cout << "1. Check Balance\n";
        std::cout << "2. Deposit\n";
        std::cout << "3. Withdraw\n";
        std::cout << "4. Transfer\n";
        std::cout << "5. View Transaction History\n";
        std::cout << "6. Sort Accounts by Balance\n";
        std::cout << "7. Search Account by Name\n";
        std::cout << "8. Exit\n";
    }

    // Function to log in to an account

    bool ATM::login(int accountNumber, const std::string& password) {

        // Trim the input password to avoid spaces issues

        std::string trimmedPassword = trim(password);

        std::cout << "Trying to log in with account number: " << accountNumber << " and password: " << trimmedPassword << "\n";

        for (size_t i = 0; i < accounts.size(); ++i) {
            std::string accountPassword = trim(accounts[i]->password);  // Trim password from file

            std::cout << "Checking account " << accounts[i]->accountNumber << " with password: " << accountPassword << "\n";
            if (accounts[i]->accountNumber == accountNumber && accountPassword == trimmedPassword) {
                loggedInAccountIndex = i;
                std::cout << "Login successful.\n";
                return true;
            }
        }

        std::cout << "Invalid account number or password.\n";
        return false;
    }

    // Function to check the balance of the logged-in account

    void ATM::checkBalance() {
        if (loggedInAccountIndex != -1) {
            std::cout << "Current balance: $" << accounts[loggedInAccountIndex]->balance << "\n";
        }
    }

    // Function to deposit money into the account

    void ATM::deposit() {
        if (loggedInAccountIndex != -1) {
            double amount;
            std::cout << "Enter amount to deposit: ";
            while (!(std::cin >> amount) || amount <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount. Please enter a positive number: ";
            }
            accounts[loggedInAccountIndex]->balance += amount;
            accounts[loggedInAccountIndex]->transactionHistory.push_back("Deposited: $" + std::to_string(amount));
            std::cout << "Deposited $" << amount << "\n";
        }
    }

    // Function to withdraw money from the account

    void ATM::withdraw() {
        if (loggedInAccountIndex != -1) {
            double amount;
            std::cout << "Enter amount to withdraw: ";
            while (!(std::cin >> amount) || amount <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount. Please enter a positive number: ";
            }

            if (accounts[loggedInAccountIndex]->balance >= amount) {
                accounts[loggedInAccountIndex]->balance -= amount;
                accounts[loggedInAccountIndex]->transactionHistory.push_back("Withdrew: $" + std::to_string(amount));
                std::cout << "Withdrew $" << amount << "\n";
            }
            else {
                std::cout << "Insufficient balance.\n";
            }
        }
    }

    // Function to transfer money between accounts
    void ATM::transfer() {
        if (loggedInAccountIndex != -1) {
            int targetAccountNumber;
            double amount;
            std::cout << "Enter target account number: ";
            std::cin >> targetAccountNumber;
            std::cout << "Enter amount to transfer: ";
            while (!(std::cin >> amount) || amount <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount. Please enter a positive number: ";
            }

            if (accounts[loggedInAccountIndex]->balance >= amount) {
                for (auto& account : accounts) {
                    if (account->accountNumber == targetAccountNumber) {
                        accounts[loggedInAccountIndex]->balance -= amount;
                        account->balance += amount;
                        accounts[loggedInAccountIndex]->transactionHistory.push_back("Transferred: $" + std::to_string(amount) + " to Account " + std::to_string(targetAccountNumber));
                        account->transactionHistory.push_back("Received: $" + std::to_string(amount) + " from Account " + std::to_string(accounts[loggedInAccountIndex]->accountNumber));
                        std::cout << "Transferred $" << amount << " to account " << targetAccountNumber << "\n";
                        return;
                    }
                }
                std::cout << "Target account not found.\n";
            }
            else {
                std::cout << "Insufficient balance.\n";
            }
        }
    }

    // Function to view the transaction history
    void ATM::viewTransactionHistory() {
        if (loggedInAccountIndex != -1) {
            std::cout << "Transaction History:\n";
            for (const auto& record : accounts[loggedInAccountIndex]->transactionHistory) {
                std::cout << record << "\n";
            }
        }
    }

    // Custom function to sort accounts by balance
    void ATM::sortAccountsByBalance() {

        for (size_t i = 0; i < accounts.size() - 1; ++i) {
            for (size_t j = 0; j < accounts.size() - i - 1; ++j) {
                if (accounts[j]->balance < accounts[j + 1]->balance) {

                    auto temp = std::move(accounts[j]);
                    accounts[j] = std::move(accounts[j + 1]);
                    accounts[j + 1] = std::move(temp);
                }
            }
        }
        std::cout << "Accounts sorted by balance.\n";
    }

    // Function to search accounts by name
    void ATM::searchAccountByName() {
        std::string name;
        std::cout << "Enter name to search: ";
        std::cin.ignore();  // This is needed to discard the leftover newline from previous inputs
        std::getline(std::cin, name);

        bool found = false;
        for (const auto& account : accounts) {
            if (account->accountHolderName.find(name) != std::string::npos) {  // Check if name is a substring
                std::cout << "Found account for: " << account->accountHolderName << "\n";
                std::cout << "Account Number: " << account->accountNumber << ", Balance: $" << account->balance << "\n";
                found = true;
            }
        }

        if (!found) {
            std::cout << "No account found with the name: " << name << "\n";
        }
    }

}