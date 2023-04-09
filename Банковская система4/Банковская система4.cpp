#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>

using namespace std;

// Класс для представления счета
class Account {
private:
    string id; // айди счета
    string owner; // владелец счета
    double balance; // баланс счета
    string password; // пароль счета
    vector<string> transactions; // история транзакций
    string purpose; // назначение счета 

public:
    // Конструктор с параметрами
    Account(string owner, string password, string purpose) {
        this->owner = owner;
        this->password = password;
        this->balance = 0.0;
        this->purpose = purpose;
        this->id = generate_id(); // генерируем айди из 16 символов
        cout << "Created a new account for " << owner << " with id " << id << endl;
    }

    // Геттеры и сеттеры для полей класса
    string get_id() {
        return id;
    }

    string get_owner() {
        return owner;
    }

    double get_balance() {
        return balance;
    }

    void set_balance(double amount) {
        balance = amount;
    }

    string get_password() {
        return password;
    }

    void set_password(string new_password) {
        password = new_password;
        cout << "Password has been changed" << endl;
    }

    vector<string> get_transactions() {
        return transactions;
    }

    void add_transaction(string transaction) {
        transactions.push_back(transaction);
    }

    string get_purpose() {
        return purpose;
    }

    void set_purpose(string new_purpose) {
        purpose = new_purpose;
        cout << "Purpose changed" << endl;
    }

     // Метод для пополнения денег на счет
    void change_balance(double amount) {
        balance += amount; // увеличиваем баланс на заданную сумму
        transactions.push_back("Replenishment: " + to_string(amount) + " uah."); // добавляем запись в историю транзакций
        cout << "You have successfully credited your account to " << amount << " uah.\n";
    }




    // Метод для генерации случайного айди из 16 символов (цифры и буквы)
    string generate_id() {
        const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        int len = sizeof(alphanum) - 1;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, len);

        string id = "";
        for (int i = 0; i < 16; i++) {
            id += alphanum[dis(gen)];
        }
        return id;

    }
};

// Класс для представления банка
class Bank {

private:
    map<string, Account*> accounts; // словарь для хранения всех счетов по их айди

public:
    // Метод для регистрации нового пользователя и создания нового счета
    void create_account() {

        cout << "Enter your name: ";
        string name;
        cin >> name;

        cout << "Enter your password: ";
        string password1;
        cin >> password1;

        cout << "Password again: ";
        string password2;
        cin >> password2;

        while (password1 != password2) { // проверяем что пароли совпадают
            cout << "Passwords don’t match. Try again." << endl;

            cout << "Enter your password: ";
            cin >> password1;

            cout << "Password again: ";
            cin >> password2;

        }

        cout << "Enter the purpose of your account: ";
        string purpose;
        cin >> purpose;
        // Создаем новый счет и добавляем его в словарь
        Account* new_account = new Account(name, password1, purpose);
        accounts[new_account->get_id()] = new_account;

    }

    // Метод для просмотра баланса счета по айди и паролю
    void check_balance() {

        cout << "Enter the id of your account: ";
        string id;
        cin >> id;

        cout << "Enter your password: ";
        string password;
        cin >> password;

        if (accounts.find(id) != accounts.end()) { // проверяем что счет существует
            Account* account = accounts[id]; // получаем указатель на счет
            if (account->get_password() == password) { // проверяем что пароль верный
                cout << "Your balance: " << account->get_balance() << endl; // выводим баланс
            }
            else {
                cout << "Invalid password" << endl; // сообщаем об ошибке
            }
        }
        else {
            cout << "Account not found" << endl; // сообщаем об ошибке
        }

    }

    // Метод для перевода денег со счета на счет
    void transfer_money() {

        cout << "Enter the id of your account: ";
        string from_id;
        cin >> from_id;

        cout << "Enter your password: ";
        string password;
        cin >> password;

        if (accounts.find(from_id) != accounts.end()) { // проверяем что отправительский счет существует
            Account* from_account = accounts[from_id]; // получаем указатель на отправительский счет

            if (from_account->get_password() == password) { // проверяем что пароль верный

                cout << "Enter the recipient’s id: ";
                string to_id;
                cin >> to_id;

                if (accounts.find(to_id) != accounts.end()) { // проверяем что получательский счет существует

                    Account* to_account = accounts[to_id]; // получаем указатель на получательский счет

                    cout << "Enter the amount of the transfer: ";
                    double amount;
                    cin >> amount;

                    if (from_account->get_balance() >= amount) {  // проверяем что у отправителя достаточно денег

                        from_account->set_balance(from_account->get_balance() - amount);  // уменьшаем баланс отправителя на сумму перевода

                        to_account->set_balance(to_account->get_balance() + amount);  // увеличиваем баланс получателя на сумму перевода

                        string transaction = "transfer from " + from_account->get_owner() +
                            " к " + to_account->get_owner() +
                            ": -" + to_string(amount);

                        from_account->add_transaction(transaction);  // добавляем транзакцию в историю отправителя

                        transaction = "transfer from " + from_account->get_owner() +
                            " к " + to_account->get_owner() +
                            ": +" + to_string(amount);

                        to_account->add_transaction(transaction);  // добавляем транзакцию в историю получателя

                        cout << "Translation was successful" << endl;  // сообщаем об успехе


                    }
                    else {
                        cout << "Insufficient money in your account" << endl;  // сообщаем об ошибке
                    }

                }
                else {
                    cout << "No recipient found" << endl;  // сообщаем об ошибке
                }

            }
            else {
                cout << "Invalid password" << endl; // сообщаем об ошибке
            }
        }
        else {
            cout << "Account not found" << endl; // сообщаем об ошибке
        }

    }

    // Метод для создания роли админа
    void create_admin() {

        cout << "Enter the name of the admin: ";
        string name;
        cin >> name;

        cout << "Enter your password: ";
        string password1;
        cin >> password1;

        cout << "Repeat the password: ";
        string password2;
        cin >> password2;

        while (password1 != password2) { // проверяем что пароли совпадают
            cout << "Passwords don’t match. Try again." << endl;

            cout << "Enter your password: ";
            cin >> password1;

            cout << "Repeat the password: ";
            cin >> password2;

        }

        // Создаем новый счет для админа с назначением admin
        Account* admin_account = new Account(name, password1, "admin");
        accounts[admin_account->get_id()] = admin_account;
    }

    // Метод для просмотра информации по любому счету админом
    void view_account() {

        cout << "Enter the id of your account: ";
        string id;
        cin >> id;

        cout << "Enter your password: ";
        string password;
        cin >> password;

        if (accounts.find(id) != accounts.end()) { // проверяем что счет существует

            Account* account = accounts[id]; // получаем указатель на счет

            if (account->get_password() == password && account->get_purpose() == "admin") {  // проверяем что пароль верный и назначение счета admin

                cout << "Enter ID of the account you are interested in: ";
                string other_id;
                cin >> other_id;

                if (accounts.find(other_id) != accounts.end()) {  // проверяем что интересующий нас счет существует

                    Account* other_account = accounts[other_id];  // получаем указатель на интересующий нас счет

                    // Выводим всю информацию по этому счету
                    cout << "Account information:" << endl;
                    cout << "Id: " << other_account->get_id() << endl;
                    cout << "Owner: " << other_account->get_owner() << endl;
                    cout << "Balance: " << other_account->get_balance() << endl;
                    cout << "Purpose: " << other_account->get_purpose() << endl;
                    cout << "Transaction History: " << endl;

                    for (string transaction : other_account->get_transactions()) {  // выводим все транзакции по счету
                        cout << transaction << endl;
                    }

                }
                else {
                    cout << "Account not found" << endl;  // сообщаем об ошибке
                }

            }
            else {
                cout << "Incorrect password or insufficient permissions" << endl;  // сообщаем об ошибке
            }

        }
        else {
            cout << "Account not found" << endl;  // сообщаем об ошибке
        }

    }
    
    void deposit_money() {  // метод для пополнения счета

        string id;  // переменная для хранения айди счета
        double amount;  // переменная для хранения суммы пополнения

        cout << "Enter the id of your account:" << endl;
        cin >> id;

        if (accounts.find(id) != accounts.end()) {  // если такой айди есть в базе

            Account* account = accounts[id];  // получаем указатель на объект класса Account по ключу-айди

            cout << "Enter the replenishment amount:" << endl;
            cin >> amount;
            if (amount > 0) {  // проверяем, что сумма положительная

                account->change_balance(amount);  // прибавляем сумму к балансу счета

                cout << "The replenishment was successful!" << endl;

            }
            else {
                cout << "Wrong amount" << endl;  // сообщаем об ошибке
            }

        }
        else {
            cout << "That’s not in the database" << endl;  // сообщаем об ошибке
        }

    }
    
  
   };

   int main() {

       Bank bank;  // создаем объект банка

       bool exit = false;  // флаг выхода из программы

       while (!exit) {  // пока не выходим из программы

           cout << "Select the action:" << endl;
           cout << "1. Create a new account" << endl;
           cout << "2. Check account balance" << endl;
           cout << "3. Transfer money from account to account" << endl;
           cout << "4. Create an admin role" << endl;
           cout << "5. View information on any account (admins only)" << endl;
           cout << "6. Deposit money "  << endl;
           cout << "7. Quit the programme" << endl;

           int choice;
           cin >> choice;

           switch (choice) {  // в зависимости от выбора пользователя

           case 1:
               bank.create_account();  // создаем новый счет
               break;

           case 2:
               bank.check_balance();  // проверяем баланс счета
               break;

           case 3:
               bank.transfer_money();  // переводим деньги со счета на счет
               break;

           case 4:
               bank.create_admin();  // создаем роль админа
               break;

           case 5:
               bank.view_account();  // просматриваем информацию по любому счету
               break;

           case 6:
               bank.deposit_money();  // пополняем баланс счета
               break;

           case 7:
               exit = true;  // выходим из программы
               break;

           default:
               cout << "wrong choice" << endl;  // сообщаем об ошибке
               break;
           }

       }

       return 0;
   }