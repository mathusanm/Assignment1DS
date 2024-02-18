#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono> // For measuring time

using namespace std;
using namespace std::chrono;

struct Product {
    int ID;
    string Name;
    double Price;
    string Category;
};

// Global vector to store products
vector<Product> products;

// Function to load product data from file into a vector of Product structs
void loadProductData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    cout << "File opened successfully." << endl;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Product product;
        string field;
        if (getline(ss, field, ',')) {
            product.ID = stoi(field);
        } else {
            cerr << "Error parsing ID from line: " << line << endl;
            continue;
        }
        if (getline(ss, product.Name, ',')) {
            // Trim leading and trailing whitespaces from Name
            product.Name.erase(0, product.Name.find_first_not_of(" \t\r\n"));
            product.Name.erase(product.Name.find_last_not_of(" \t\r\n") + 1);
        } else {
            cerr << "Error parsing Name from line: " << line << endl;
            continue;
        }
        if (getline(ss, field, ',')) {
            product.Price = stod(field);
        } else {
            cerr << "Error parsing Price from line: " << line << endl;
            continue;
        }
        if (getline(ss, product.Category)) {
            // Trim leading and trailing whitespaces from Category
            product.Category.erase(0, product.Category.find_first_not_of(" \t\r\n"));
            product.Category.erase(product.Category.find_last_not_of(" \t\r\n") + 1);
        } else {
            cerr << "Error parsing Category from line: " << line << endl;
            continue;
        }
        products.push_back(product);
    }

    file.close();
}

// Function to display product details
void displayProduct(const Product& product) {
    cout << "ID: " << product.ID << "\tName: " << product.Name << "\tPrice: " << product.Price << "\tCategory: " << product.Category << endl;
}

// Function to display all products
void displayAllProducts() {
    for (const auto& product : products) {
        displayProduct(product);
    }
}

// Function to insert a new product
void insertProduct(const Product& newProduct) {
    cout << "Inserting product:\n";
    displayProduct(newProduct);
    products.push_back(newProduct);
    cout << "Product inserted successfully.\n";
}

// Function to update product details
void updateProduct(int id, const string& newName, double newPrice, const string& newCategory) {
    cout << "Updating product with ID " << id << ":\n";
    for (auto& product : products) {
        if (product.ID == id) {
            cout << "Old details:\n";
            displayProduct(product);
            product.Name = newName;
            product.Price = newPrice;
            product.Category = newCategory;
            cout << "Updated details:\n";
            displayProduct(product);
            cout << "Product updated successfully.\n";
            return;
        }
    }
    cerr << "Product with ID " << id << " not found for updating." << endl;
}

// Function to delete a product
void deleteProduct(int id) {
    cout << "Deleting product with ID " << id << ":\n";
    auto it = remove_if(products.begin(), products.end(), [id](const Product& p) { return p.ID == id; });
    if (it != products.end()) {
        products.erase(it, products.end());
        cout << "Product deleted successfully.\n";
    } else {
        cerr << "Product with ID " << id << " not found for deletion." << endl;
    }
}

// Function to search for a product by ID
void searchProductByID(int id) {
    cout << "Searching for product with ID " << id << ":\n";
    auto it = find_if(products.begin(), products.end(), [id](const Product& p) { return p.ID == id; });
    if (it != products.end()) {
        cout << "Product found:\n";
        displayProduct(*it);
    } else {
        cerr << "Product with ID " << id << " not found." << endl;
    }
}

// Function to search for a product by Name
void searchProductByName(const string& name) {
    cout << "Searching for product with Name \"" << name << "\":\n";
    auto it = find_if(products.begin(), products.end(), [&name](const Product& p) { return p.Name == name; });
    if (it != products.end()) {
        cout << "Product found:\n";
        displayProduct(*it);
    } else {
        cerr << "Product with Name \"" << name << "\" not found." << endl;
    }
}

// Bubble Sort implementation to sort products by price
void bubbleSortByPrice() {
    int n = products.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (products[j].Price > products[j + 1].Price) {
                swap(products[j], products[j + 1]);
            }
        }
    }
}

// Function to measure time taken for sorting and return duration
microseconds measureSortingTime() {
    auto start = high_resolution_clock::now();
    bubbleSortByPrice();
    auto stop = high_resolution_clock::now();
    return duration_cast<microseconds>(stop - start);
}

int main() {
    // Load product data from file
    loadProductData("product_data.txt");

    // Display all products
    cout << "All Products:\n";
    displayAllProducts();

    // Insert a new product
    Product newProduct{12345, "New Product", 99.99, "New Category"};
    insertProduct(newProduct);

    // Update an existing product
    updateProduct(12345, "Updated Product", 199.99, "Updated Category");

    // Delete a product
    deleteProduct(12345);

    // Search for a product by ID
    searchProductByID(57353);

    // Search for a product by Name
    searchProductByName("Smartphone ILGCU");

    // Measure time taken for sorting
    cout << "Sorting products by price...\n";
    auto sortingTime = measureSortingTime();
    cout << "Time taken for sorting: " << sortingTime.count() << " microseconds" << endl;

    // Display all products after sorting
    cout << "\nAfter sorting:\n";
    displayAllProducts();

    return 0;
}