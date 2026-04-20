#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Product {
protected:
    int id;
    string name;
    double price;
    int stock;
    double discount;

public:
    Product(int id, string name, double price, int stock, double discount)
        : id(id), name(move(name)), price(price), stock(stock), discount(discount) {}

    virtual ~Product() = default;

    int getId() const {
        return id;
    }

    const string &getName() const {
        return name;
    }

    int getStock() const {
        return stock;
    }

    double getPrice() const {
        return price;
    }

    double getDiscount() const {
        return discount;
    }

    double finalPrice() const {
        return price * (100.0 - discount) / 100.0;
    }

    void restock(int quantity) {
        stock += quantity;
    }

    bool sell(int quantity) {
        if (quantity <= 0 || quantity > stock) {
            return false;
        }
        stock -= quantity;
        return true;
    }

    virtual string category() const = 0;
    virtual string details() const = 0;
};

class ElectronicsProduct : public Product {
    int warrantyMonths;

public:
    ElectronicsProduct(int id, string name, double price, int stock, double discount, int warrantyMonths)
        : Product(id, move(name), price, stock, discount), warrantyMonths(warrantyMonths) {}

    string category() const override {
        return "Electronice";
    }

    string details() const override {
        return "Garantie: " + to_string(warrantyMonths) + " luni";
    }
};

class FoodProduct : public Product {
    string expirationDate;

public:
    FoodProduct(int id, string name, double price, int stock, double discount, string expirationDate)
        : Product(id, move(name), price, stock, discount), expirationDate(move(expirationDate)) {}

    string category() const override {
        return "Alimentar";
    }

    string details() const override {
        return "Expira la: " + expirationDate;
    }
};

struct Sale {
    string productName;
    int quantity;
    double total;
};

class Store {
    vector<unique_ptr<Product>> products;
    vector<Sale> sales;
    int nextId = 1;

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

    Product *findProduct(int id) {
        for (auto &product : products) {
            if (product->getId() == id) {
                return product.get();
            }
        }
        return nullptr;
    }

public:
    void seedData() {
        products.push_back(make_unique<ElectronicsProduct>(nextId++, "Laptop", 3500.0, 8, 10.0, 24));
        products.push_back(make_unique<ElectronicsProduct>(nextId++, "Casti wireless", 450.0, 12, 5.0, 12));
        products.push_back(make_unique<FoodProduct>(nextId++, "Cafea boabe", 55.0, 20, 0.0, "12.11.2026"));
        products.push_back(make_unique<FoodProduct>(nextId++, "Ciocolata neagra", 12.5, 6, 15.0, "08.08.2026"));
    }

    void addElectronics() {
        string name = readLine("Nume produs: ");
        double price = readDouble("Pret: ");
        int stock = readInt("Stoc initial: ");
        double discount = readDouble("Discount (%): ");
        int warranty = readInt("Garantie in luni: ");

        products.push_back(make_unique<ElectronicsProduct>(nextId++, name, price, stock, discount, warranty));
        cout << "Produs electronic adaugat.\n";
    }

    void addFood() {
        string name = readLine("Nume produs: ");
        double price = readDouble("Pret: ");
        int stock = readInt("Stoc initial: ");
        double discount = readDouble("Discount (%): ");
        string expiration = readLine("Data expirare: ");

        products.push_back(make_unique<FoodProduct>(nextId++, name, price, stock, discount, expiration));
        cout << "Produs alimentar adaugat.\n";
    }

    void restockProduct() {
        int id = readInt("ID produs: ");
        int quantity = readInt("Cantitate adaugata: ");

        Product *product = findProduct(id);
        if (product == nullptr) {
            cout << "Produsul nu exista.\n";
            return;
        }

        product->restock(quantity);
        cout << "Stoc actualizat.\n";
    }

    void sellProduct() {
        int id = readInt("ID produs: ");
        int quantity = readInt("Cantitate vanduta: ");

        Product *product = findProduct(id);
        if (product == nullptr) {
            cout << "Produsul nu exista.\n";
            return;
        }

        if (!product->sell(quantity)) {
            cout << "Stoc insuficient sau cantitate invalida.\n";
            return;
        }

        double total = product->finalPrice() * quantity;
        sales.push_back({product->getName(), quantity, total});
        cout << "Vanzare inregistrata. Total: " << fixed << setprecision(2) << total << '\n';
    }

    void listProducts() const {
        if (products.empty()) {
            cout << "Nu exista produse.\n";
            return;
        }

        cout << left << setw(6) << "ID"
             << setw(22) << "Produs"
             << setw(14) << "Categorie"
             << setw(10) << "Stoc"
             << setw(12) << "Pret"
             << setw(12) << "Discount"
             << "Detalii\n";
        cout << string(95, '-') << '\n';

        for (const auto &product : products) {
            cout << left << setw(6) << product->getId()
                 << setw(22) << product->getName()
                 << setw(14) << product->category()
                 << setw(10) << product->getStock()
                 << setw(12) << fixed << setprecision(2) << product->getPrice()
                 << setw(12) << product->getDiscount()
                 << product->details() << '\n';
        }
    }

    void lowStockReport() const {
        cout << "\nProduse cu stoc redus\n";
        bool found = false;
        for (const auto &product : products) {
            if (product->getStock() <= 5) {
                found = true;
                cout << product->getName() << " - stoc " << product->getStock() << '\n';
            }
        }
        if (!found) {
            cout << "Nu exista produse cu stoc redus.\n";
        }
    }

    void salesReport() const {
        if (sales.empty()) {
            cout << "Nu exista vanzari inregistrate.\n";
            return;
        }

        double revenue = 0.0;
        cout << "\nRaport vanzari\n";
        for (const auto &sale : sales) {
            cout << sale.productName << " | Cantitate: " << sale.quantity
                 << " | Total: " << fixed << setprecision(2) << sale.total << '\n';
            revenue += sale.total;
        }
        cout << "Venit total: " << fixed << setprecision(2) << revenue << '\n';
    }

    void run() {
        seedData();

        while (true) {
            cout << "\n=== Sistem Gestiune Stocuri ===\n";
            cout << "1. Afiseaza produse\n";
            cout << "2. Adauga produs electronic\n";
            cout << "3. Adauga produs alimentar\n";
            cout << "4. Reaprovizioneaza produs\n";
            cout << "5. Vinde produs\n";
            cout << "6. Raport stoc redus\n";
            cout << "7. Raport vanzari\n";
            cout << "0. Iesire\n";

            int option = readInt("Optiune: ");

            switch (option) {
                case 1:
                    listProducts();
                    break;
                case 2:
                    addElectronics();
                    break;
                case 3:
                    addFood();
                    break;
                case 4:
                    restockProduct();
                    break;
                case 5:
                    sellProduct();
                    break;
                case 6:
                    lowStockReport();
                    break;
                case 7:
                    salesReport();
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
    Store store;
    store.run();
    return 0;
}
