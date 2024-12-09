#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <memory>
#include <string>

using namespace std;

// Базовый класс Product
class Product {
protected:
    string name;
    double price;
    int quantity;

public:
    Product(const string& name, double price, int quantity)
        : name(name), price(price), quantity(quantity) {
    }

    virtual ~Product() = default;

    virtual void displayInfo() const {
        cout << "Name: " << name << ", Price: " << price << ", Quantity: " << quantity << endl;
    }

    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
};

// Шаблонный класс Pair
template <typename T1, typename T2>
class Pair {
    T1 first;
    T2 second;

public:
    Pair(const T1& first, const T2& second) : first(first), second(second) {}

    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }
};

// Производный класс Clothing
class Clothing : public Product {
private:
    string size;

public:
    Clothing(const string& name, double price, int quantity, const string& size)
        : Product(name, price, quantity), size(size) {
    }

    void displayInfo() const override {
        cout << "Clothing - ";
        Product::displayInfo();
        cout << "Size: " << size << endl;
    }
};

// Производный класс Electronics
class Electronics : public Product {
private:
    string brand;

public:
    Electronics(const string& name, double price, int quantity, const string& brand)
        : Product(name, price, quantity), brand(brand) {
    }

    void displayInfo() const override {
        cout << "Electronics - ";
        Product::displayInfo();
        cout << "Brand: " << brand << endl;
    }
};

// Класс для управления магазином
class Store {
private:
    vector<shared_ptr<Product>> products;

public:
    void addProduct(shared_ptr<Product> product) {
        products.push_back(product);
    }

    void displayAllProducts() const {
        for (const auto& product : products) {
            product->displayInfo();
            cout << endl;
        }
    }

    void searchProduct(const string& name) const {
        auto it = find_if(products.begin(), products.end(),
            [&name](const shared_ptr<Product>& product) {
                return product->getName() == name;
            });
        if (it != products.end()) {
            cout << "Product found:" << endl;
            (*it)->displayInfo();
        }
        else {
            cout << "Product not found." << endl;
        }
    }

    void sortProductsByPrice() {
        sort(products.begin(), products.end(),
            [](const shared_ptr<Product>& a, const shared_ptr<Product>& b) {
                return a->getPrice() < b->getPrice();
            });
    }

    void saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error opening file for writing." << endl;
            return;
        }

        for (const auto& product : products) {
            outFile << product->getName() << ","
                << product->getPrice() << ","
                << product->getQuantity() << endl;
        }
        outFile.close();
        cout << "Data saved to " << filename << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error opening file for reading." << endl;
            return;
        }

        products.clear();
        string name;
        double price;
        int quantity;
        while (inFile >> name >> price >> quantity) {
            products.push_back(make_shared<Product>(name, price, quantity));
        }
        inFile.close();
        cout << "Data loaded from " << filename << endl;
    }

    void modifyProduct(const string& name) {
        auto it = find_if(products.begin(), products.end(),
            [&name](const shared_ptr<Product>& product) {
                return product->getName() == name;
            });
        if (it != products.end()) {
            cout << "Enter new price: ";
            double newPrice;
            cin >> newPrice;
            cout << "Enter new quantity: ";
            int newQuantity;
            cin >> newQuantity;
            (*it)->setPrice(newPrice);
            (*it)->setQuantity(newQuantity);
            cout << "Product updated successfully." << endl;
        }
        else {
            cout << "Product not found." << endl;
        }
    }
};

// Главное меню
void menu(Store& store) {
    int choice;
    do {
        cout << "\nMenu:\n"
            << "1. Add Product\n"
            << "2. Display All Products\n"
            << "3. Search Product\n"
            << "4. Sort Products by Price\n"
            << "5. Modify Product\n"
            << "6. Save to File\n"
            << "7. Load from File\n"
            << "0. Exit\n"
            << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter product type (1 - Clothing, 2 - Electronics): ";
            int type;
            cin >> type;

            cout << "Enter name, price, and quantity: ";
            string name;
            double price;
            int quantity;
            cin >> name >> price >> quantity;

            if (type == 1) {
                cout << "Enter size: ";
                string size;
                cin >> size;
                store.addProduct(make_shared<Clothing>(name, price, quantity, size));
            }
            else if (type == 2) {
                cout << "Enter brand: ";
                string brand;
                cin >> brand;
                store.addProduct(make_shared<Electronics>(name, price, quantity, brand));
            }
            else {
                cout << "Invalid type." << endl;
            }
            break;
        }
        case 2:
            store.displayAllProducts();
            break;
        case 3: {
            cout << "Enter product name to search: ";
            string name;
            cin >> name;
            store.searchProduct(name);
            break;
        }
        case 4:
            store.sortProductsByPrice();
            cout << "Products sorted by price." << endl;
            break;
        case 5: {
            cout << "Enter product name to modify: ";
            string name;
            cin >> name;
            store.modifyProduct(name);
            break;
        }
        case 6:
            store.saveToFile("output.txt");
            break;
        case 7:
            store.loadFromFile("output.txt");
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice, try again." << endl;
        }
    } while (choice != 0);
}

// Точка входа
int main() {
    Store store;
    menu(store);
    return 0;
}
