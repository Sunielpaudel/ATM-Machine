#ifndef ATM_HPP
#define ATM_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <string>


namespace atmProject {

    // Enum for account type
    enum class AccountType {
        Checking,
        Savings
    };

    // Struct to represent an account
    struct Account {
        int accountNumber;
        std::string accountHolderName;
        std::string password;
        double balance;
        AccountType type;
        std::vector<std::string> transactionHistory; // To store transaction logs

        Account(int accNum, const std::string& holder, const std::string& pass, double bal, AccountType accType)
            : accountNumber(accNum), accountHolderName(holder), password(pass), balance(bal), type(accType) {}
    };

    // ATM class to manage the accounts and operations
    struct ATM {
        std::vector<std::unique_ptr<Account>> accounts;
        int loggedInAccountIndex = -1;

        // Function declarations
        void loadDataFromFile(const std::string& filename);
        void saveDataToFile(const std::string& filename);
        void displayMenu();
        bool login(int accountNumber, const std::string& password);
        void checkBalance();
        void deposit();
        void withdraw();
        void transfer();
        void viewTransactionHistory();
        void sortAccountsByBalance();  // Custom sorting function
        void searchAccountByName();

    };
}

#endif // ATM_HPP