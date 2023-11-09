
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <ctime>
using namespace std;


template <typename T>
class account {
public:
    string custname;
    long int accno;
    T withdrawamount;
    T deposit;
    int type;

    account() {
        cout<<"*****************BANK MANAGEMENT SYSTEM***************************"<<endl;
    }

    virtual void welcome() {
        cout << "Welcome to the account!" << endl;
    }

    virtual void depositMoney() {
        cout << "Enter the deposit:" << endl;
        cin >> deposit;
    }

    virtual void withdrawMoney() {
        cout << "Enter the amount you want to withdraw:" << endl;
        cin >> withdrawamount;
    }

    void getdetail() {
        cout << "Enter customer name:" << endl;
        cin >> custname;
        cout << "Enter account number:" << endl;
        cin >> accno;
    }

    void putdetail() {
        cout << "Customer Name: " << custname << endl;
        cout << "Account Number: " << accno << endl;
    }
};

template <typename T>
class cur_acc : public account<T> {
private:
    static T balance;

public:
    void welcome() override {
        cout << "WELCOME TO THE CURRENT ACCOUNT" << endl;
        cout << "Here you can have a cheque book facility but no interest. Also, you have withdrawal facilities." << endl;
        cout << "If your balance goes below 500, a charge will be applied." << endl;
    }

    void depositMoney() override {
        account<T>::depositMoney();
        balance = balance + account<T>::deposit;
        cout << "The current balance of your account is: " << balance << endl;
    }

    void withdrawMoney() override {
        account<T>::withdrawMoney();
        try {
            if (account<T>::withdrawamount > balance) {
                throw "Insufficient balance for withdrawal";
            }

            balance = balance - account<T>::withdrawamount;
            if (balance < 500) {
                cout << "Withdrawal permitted with a charge on your account..." << endl;
                try {
                    if (balance - 10 < 0) {
                        throw "Insufficient balance after fine";
                    }
                    balance = balance - 10;

                    
                    string fileName = "cur_" + this->custname + "_" + to_string(this->accno) + ".txt";
                    ofstream currentFile(fileName, ios::app);

                    if (currentFile.is_open()) {
                        auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
                        currentFile << "Date & Time: " << ctime(&now);
                        currentFile << "Fine charged to " << this->custname << " | Account Number: " << this->accno << " | Fine Amount: $10" << endl;
                        currentFile.close();
                    } else {
                        cerr << "Error opening " << fileName << " for fine charged information" << endl;
                    }
                } catch (const char *msg) {
                    cerr << "Error: " << msg << ". No withdrawal possible after a fine." << endl;
                    balance = balance + account<T>::withdrawamount; 
                }
            } else {
                cout << "The withdrawal is permitted." << endl;
            }
        } catch (const char *msg) {
            cerr << "Error: " << msg << ". No withdrawal possible." << endl;
        }
    }

    void saveCurrentInfo() {
        
        string fileName = "cur_" + this->custname + "_" + to_string(this->accno) + "_info.txt";
        ofstream currentFile(fileName, ios::app);

        if (currentFile.is_open()) {
            auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
            currentFile << "Date & Time: " << ctime(&now);
            currentFile << "Customer Name: " << this->custname << " | Account Number: " << this->accno << " | Balance: " << balance << endl;
            currentFile.close();
        } else {
            cerr << "Error opening " << fileName << " for current account info" << endl;
        }
    }
};


template <typename T>
T cur_acc<T>::balance = 1000;


template <typename T>
class sav_acc : public account<T> {
public:
    static T bal;
    T p, r, t, ci;

    void welcome() override {
        cout << "WELCOME TO THE SAVINGS ACCOUNT" << endl;
        cout << "Here you can get compound interest and withdrawal facilities." << endl;
    }

    void depositMoney() override {
        account<T>::depositMoney();
        bal = account<T>::deposit + bal;
        cout << "The current balance of your account is: " << bal << endl;
    }

    void withdrawMoney() override {
        account<T>::withdrawMoney();
        bal = bal - account<T>::withdrawamount;
        cout << "The current amount is: " << bal << endl;
    }

    void interest() {
        cout << "Enter principle, rate, and time:" << endl;
        cin >> p >> r >> t;
        ci = p * pow((1 + r / 100), t) - p;
        cout << "Compound interest: " << ci << endl;
        cout << "The final amount with interest is: " << ci + bal << endl;
    }

    void saveSavingsInfo() {
        
        string fileName = "savings_" + this->custname + "_" + to_string(this->accno) + "_info.txt";
        ofstream savingsFile(fileName, ios::app);

        if (savingsFile.is_open()) {
            auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
            savingsFile << "Date & Time: " << ctime(&now);
            savingsFile << "Customer Name: " << this->custname << " | Account Number: " << this->accno << " | Balance: " << bal << endl;
            savingsFile.close();
        } else {
            cerr << "Error opening " << fileName << " for savings account info" << endl;
        }
    }
};

template <typename T>
T sav_acc<T>::bal = 1000;

int main() {
    
    account<float> *acc;
    int type;
    int opt;

    cout << "Enter account type (1 for savings, 2 for current): ";
    cin >> type;

    if (type == 1) {
        acc = new sav_acc<float>;
    } else if (type == 2) {
        acc = new cur_acc<float>;
    } else {
        cout << "Invalid account type" << endl;
        return 1;
    }

    acc->welcome();

    while (true) {
        cout << "Menu" << endl;
        cout << "1. Deposit" << endl;
        cout << "2. Withdraw" << endl;
        cout << "3. Calculate Compound Interest" << endl; 
        cout << "4. Exit" << endl; 
        cout << "Enter choice: ";

        try {
            if (!(cin >> opt)) {
                throw "Invalid input. Please enter a numeric choice.";
            }

            if (opt == 1 || opt == 2) {
                acc->getdetail();
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw "Invalid input. Customer name and account number should be characters.";
                }

                acc->putdetail();
                acc->depositMoney();
                if (type == 1) {
                    static_cast<sav_acc<float>*>(acc)->saveSavingsInfo();
                } else {
                    static_cast<cur_acc<float>*>(acc)->saveCurrentInfo();
                }
            } else if (opt == 3) {
                if (type == 1) {
                    static_cast<sav_acc<float>*>(acc)->interest();
                } else {
                    cout << "Compound interest calculation is available for savings accounts only." << endl;
                }
            } else if (opt == 4) {
                cout << "Exiting..." << endl;
                break;
            } else {
                throw "Invalid choice. Please select a valid option.";
            }
        } catch (const char* error) {
            cerr << "Error: " << error << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    delete acc;

    return 0;
}
