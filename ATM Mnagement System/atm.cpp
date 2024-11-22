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
                type = Checking;
            }
            else if (typeStr == "Savings") {
                type = Savings;
            }
            else {
                std::cout << "Error: Unknown account type: '" << typeStr << "'\n";
                continue;
            }

            accounts.push_back(std::make_unique<Account>(accNum, holder, pass, balance, type));
        }

        file.close();
    }

    void ATM::saveDataToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::trunc);
        if (!file.is_open()) {
            std::cout << "Failed to open file for writing.\n";
            return;
        }

        for (const auto& acc : accounts) {
            file << acc->accountNumber << "\n"
                << acc->accountHolderName << "\n"
                << acc->password << "\n"
                << acc->balance << "\n"
                << (acc->type == Checking ? "Checking" : "Savings") << "\n";
        }

        file.close();
    }

    void ATM::displayCustomerMenu() {
        std::cout << "\nCustomer Menu:\n";
        std::cout << "1. Check Balance\n";
        std::cout << "2. Deposit\n";
        std::cout << "3. Withdraw\n";
        std::cout << "4. Transfer\n";
        std::cout << "5. View Transaction History\n";
        std::cout << "6. Exit\n";
    }

    void ATM::displayAdminMenu() {
        std::cout << "\nAdministrator Menu:\n";
        std::cout << "1. Sort Accounts by Balance\n";
        std::cout << "2. Search Account by Name\n";
        std::cout << "3. Search Account by Balance\n";
        std::cout << "4. Exit\n";
    }

    bool ATM::loginCustomer(int accountNumber, const std::string& password) {
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i]->accountNumber == accountNumber && accounts[i]->password == password) {
                loggedInAccountIndex = i;
                std::cout << "Login successful.\n";
                return true;
            }
        }
        std::cout << "Invalid account number or password.\n";
        return false;
    }

    void ATM::checkBalance() {
        if (loggedInAccountIndex != -1) {
            std::cout << "Current balance: $" << accounts[loggedInAccountIndex]->balance << "\n";
        }
    }

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

    void ATM::viewTransactionHistory() {
        if (loggedInAccountIndex != -1) {
            std::cout << "Transaction History:\n";
            for (const auto& record : accounts[loggedInAccountIndex]->transactionHistory) {
                std::cout << record << "\n";
            }
        }
    }

    void ATM::sortAccountsByBalance() {
        for (size_t i = 0; i < accounts.size() - 1; ++i) {
            for (size_t j = 0; j < accounts.size() - i - 1; ++j) {
                if (accounts[j]->balance < accounts[j + 1]->balance) {
                    std::swap(accounts[j], accounts[j + 1]);
                }
            }
        }
        std::cout << "Accounts sorted by balance.\n";
    }

    void ATM::searchAccountByName() {
        std::string name;
        std::cout << "Enter name to search: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        for (const auto& account : accounts) {
            if (account->accountHolderName == name) {
                std::cout << "Account found: " << account->accountNumber << " - " << account->accountHolderName << " - Balance: $" << account->balance << " - " << (account->type == Checking ? "Checking" : "Savings") << "\n";
                return;
            }
        }
        std::cout << "Account not found.\n";
    }

    void ATM::searchAccountByBalance() {
        double balance;
        std::cout << "Enter balance to search: ";
        std::cin >> balance;
        for (const auto& account : accounts) {
            if (account->balance == balance) {
                std::cout << "Account found: " << account->accountNumber << " - " << account->accountHolderName << " - Balance: $" << account->balance << " - " << (account->type == Checking ? "Checking" : "Savings") << "\n";
                return;
            }
        }
        std::cout << "Account not found.\n";
    }
}