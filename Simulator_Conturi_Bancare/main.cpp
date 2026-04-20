#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class TransactionError : public runtime_error {
public:
    explicit TransactionError(const string &message) : runtime_error(message) {}
};

class Account {
protected:
    string iban;
    string owner;
    double balance;

public:
    Account(string iban, string owner, double balance)
        : iban(move(iban)), owner(move(owner)), balance(balance) {}

    virtual ~Account() = default;

    const string &getIban() const {
        return iban;
    }

    const string &getOwner() const {
        return owner;
    }

    double getBalance() const {
        return balance;
    }

    virtual string getType() const = 0;

    virtual void deposit(double amount) {
        if (amount <= 0) {
            throw TransactionError("Suma trebuie sa fie pozitiva.");
        }
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (amount <= 0) {
            throw TransactionError("Suma trebuie sa fie pozitiva.");
        }
        if (amount > balance) {
            throw TransactionError("Fonduri insuficiente.");
        }
        balance -= amount;
    }

    virtual void monthUpdate() = 0;

    virtual string extraInfo() const = 0;
};

class SavingsAccount : public Account {
    double interestRate;

public:
    SavingsAccount(string iban, string owner, double balance, double interestRate)
        : Account(move(iban), move(owner), balance), interestRate(interestRate) {}

    string getType() const override {
        return "Economii";
    }

    void monthUpdate() override {
        balance += balance * interestRate / 100.0;
    }

    string extraInfo() const override {
        return "Dobanda: " + to_string(interestRate) + "%";
    }
};

class CreditAccount : public Account {
    double creditLimit;

public:
    CreditAccount(string iban, string owner, double balance, double creditLimit)
        : Account(move(iban), move(owner), balance), creditLimit(creditLimit) {}

    string getType() const override {
        return "Credit";
    }

    void withdraw(double amount) override {
        if (amount <= 0) {
            throw TransactionError("Suma trebuie sa fie pozitiva.");
        }
        if (balance + creditLimit < amount) {
            throw TransactionError("Limita de credit este depasita.");
        }
        balance -= amount;
    }

    void monthUpdate() override {
        if (balance < 0) {
            balance -= 25.0;
        }
    }

    string extraInfo() const override {
        return "Limita credit: " + to_string(creditLimit);
    }
};

class Bank {
    vector<unique_ptr<Account>> accounts;

    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    static int readInt(const string &prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                clearInput();
                return value;
            }
            cout << "Valoare invalida. Incearca din nou.\n";
            clearInput();
        }
    }

    static double readDouble(const string &prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                clearInput();
                return value;
            }
            cout << "Valoare invalida. Incearca din nou.\n";
            clearInput();
        }
    }

    static string readLine(const string &prompt) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
        } while (value.empty());
        return value;
    }

    Account *findAccount(const string &iban) {
        for (auto &account : accounts) {
            if (account->getIban() == iban) {
                return account.get();
            }
        }
        return nullptr;
    }

public:
    void seedData() {
        accounts.push_back(make_unique<SavingsAccount>("RO49BANK0001", "Andreea Marin", 3200.0, 2.4));
        accounts.push_back(make_unique<CreditAccount>("RO49BANK0002", "Mihai Georgescu", 1500.0, 2000.0));
    }

    void listAccounts() const {
        if (accounts.empty()) {
            cout << "Nu exista conturi.\n";
            return;
        }

        cout << left << setw(16) << "IBAN"
             << setw(20) << "Titular"
             << setw(12) << "Tip"
             << setw(14) << "Sold"
             << "Detalii\n";
        cout << string(80, '-') << '\n';

        for (const auto &account : accounts) {
            cout << left << setw(16) << account->getIban()
                 << setw(20) << account->getOwner()
                 << setw(12) << account->getType()
                 << setw(14) << fixed << setprecision(2) << account->getBalance()
                 << account->extraInfo() << '\n';
        }
    }

    void createSavingsAccount() {
        string iban = readLine("IBAN: ");
        if (findAccount(iban) != nullptr) {
            cout << "Exista deja un cont cu acest IBAN.\n";
            return;
        }

        string owner = readLine("Titular: ");
        double balance = readDouble("Sold initial: ");
        double interestRate = readDouble("Dobanda (%): ");
        accounts.push_back(make_unique<SavingsAccount>(iban, owner, balance, interestRate));
        cout << "Contul de economii a fost creat.\n";
    }

    void createCreditAccount() {
        string iban = readLine("IBAN: ");
        if (findAccount(iban) != nullptr) {
            cout << "Exista deja un cont cu acest IBAN.\n";
            return;
        }

        string owner = readLine("Titular: ");
        double balance = readDouble("Sold initial: ");
        double creditLimit = readDouble("Limita credit: ");
        accounts.push_back(make_unique<CreditAccount>(iban, owner, balance, creditLimit));
        cout << "Contul de credit a fost creat.\n";
    }

    void depositMoney() {
        string iban = readLine("IBAN: ");
        double amount = readDouble("Suma depusa: ");

        Account *account = findAccount(iban);
        if (account == nullptr) {
            cout << "Contul nu exista.\n";
            return;
        }

        try {
            account->deposit(amount);
            cout << "Depunere realizata.\n";
        } catch (const TransactionError &error) {
            cout << error.what() << '\n';
        }
    }

    void withdrawMoney() {
        string iban = readLine("IBAN: ");
        double amount = readDouble("Suma retrasa: ");

        Account *account = findAccount(iban);
        if (account == nullptr) {
            cout << "Contul nu exista.\n";
            return;
        }

        try {
            account->withdraw(amount);
            cout << "Retragere realizata.\n";
        } catch (const TransactionError &error) {
            cout << error.what() << '\n';
        }
    }

    void transferMoney() {
        string sourceIban = readLine("IBAN sursa: ");
        string targetIban = readLine("IBAN destinatie: ");
        double amount = readDouble("Suma transferata: ");

        Account *source = findAccount(sourceIban);
        Account *target = findAccount(targetIban);

        if (source == nullptr || target == nullptr) {
            cout << "Unul dintre conturi nu exista.\n";
            return;
        }

        try {
            source->withdraw(amount);
            target->deposit(amount);
            cout << "Transfer realizat.\n";
        } catch (const TransactionError &error) {
            cout << error.what() << '\n';
        }
    }

    void monthUpdateAll() {
        for (auto &account : accounts) {
            account->monthUpdate();
        }
        cout << "Actualizarea lunara a fost aplicata tuturor conturilor.\n";
    }

    void run() {
        seedData();

        while (true) {
            cout << "\n=== Simulator Conturi Bancare ===\n";
            cout << "1. Afiseaza conturi\n";
            cout << "2. Creeaza cont economii\n";
            cout << "3. Creeaza cont credit\n";
            cout << "4. Depunere\n";
            cout << "5. Retragere\n";
            cout << "6. Transfer\n";
            cout << "7. Actualizare lunara\n";
            cout << "0. Iesire\n";

            int option = readInt("Optiune: ");

            switch (option) {
                case 1:
                    listAccounts();
                    break;
                case 2:
                    createSavingsAccount();
                    break;
                case 3:
                    createCreditAccount();
                    break;
                case 4:
                    depositMoney();
                    break;
                case 5:
                    withdrawMoney();
                    break;
                case 6:
                    transferMoney();
                    break;
                case 7:
                    monthUpdateAll();
                    break;
                case 0:
                    cout << "Program incheiat.\n";
                    return;
                default:
                    cout << "Optiune invalida.\n";
                    break;
            }
        }
    }
};

int main() {
    Bank bank;
    bank.run();
    return 0;
}
