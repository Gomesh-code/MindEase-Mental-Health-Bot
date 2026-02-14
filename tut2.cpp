#include <iostream>
#include <string>
#include <fstream>

class Product {
private:
    int productId;
    char name[50];
    float price;

public:
    // Method to create a new product
    void createProduct() {
        std::cout << "\nEnter The Product ID: ";
        std::cin >> productId;
        std::cout << "Enter The Name of The Product: ";
        std::cin.ignore(); // To clear the input buffer
        std::cin.getline(name, 50);
        std::cout << "Enter The Price of The Product: ";
        std::cin >> price;
    }

    // Method to display product details
    void showProduct() const {
        std::cout << "\nProduct ID: " << productId;
        std::cout << "\nProduct Name: " << name;
        std::cout << "\nPrice: " << price;
    }

    // Getter methods to access private data
    int getProductId() const {
        return productId;
    }

    float getPrice() const {
        return price;
    }

    const char* getName() const {
        return name;
    }
};
// Global fstream and Product objects
std::fstream file;
Product pr;

// Function to write a new product to the file
void write_product() {
    file.open("database.dat", std::ios::out | std::ios::app | std::ios::binary);
    pr.createProduct();
    // Write the object's data to the file
    file.write(reinterpret_cast<char*>(&pr), sizeof(Product));
    file.close();
    std::cout << "\n\nProduct record has been created.";
}

// Function to display all products from the file
void display_all_products() {
    std::cout << "\n\n--- DISPLAYING ALL RECORDS ---\n";
    file.open("database.dat", std::ios::in | std::ios::binary);
    if (!file) {
        std::cout << "File could not be opened. No products found.";
        return;
    }
    // Read and display each product from the file
    while (file.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        pr.showProduct();
        std::cout << "\n====================================\n";
    }
    file.close();
}

// Function to search for and display a specific product
void display_specific_product(int id) {
    bool found = false;
    file.open("database.dat", std::ios::in | std::ios::binary);
    if (!file) {
        std::cout << "File could not be opened.";
        return;
    }
    while (file.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        if (pr.getProductId() == id) {
            pr.showProduct();
            found = true;
        }
    }
    file.close();
    if (!found) {
        std::cout << "\n\nRecord not found.";
    }
}

// Function to modify a product's record in the file
void modify_product() {
    int id;
    bool found = false;
    std::cout << "\nEnter the Product ID of the record to modify: ";
    std::cin >> id;

    file.open("database.dat", std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        std::cout << "File could not be opened.";
        return;
    }
    while (file.read(reinterpret_cast<char*>(&pr), sizeof(Product)) && !found) {
        if (pr.getProductId() == id) {
            pr.showProduct();
            std::cout << "\n\nPlease Enter The New Details of Product" << std::endl;
            pr.createProduct();
            long long pos = -1 * static_cast<long long>(sizeof(pr));
            file.seekp(pos, std::ios::cur); // Go back one record in the file
            file.write(reinterpret_cast<char*>(&pr), sizeof(Product));
            std::cout << "\n\nRecord Updated.";
            found = true;
        }
    }
    file.close();
    if (!found) {
        std::cout << "\n\nRecord Not Found.";
    }
}


// Function to delete a product's record from the file
void delete_product() {
    int id;
    std::cout << "\nEnter the Product ID to delete: ";
    std::cin >> id;

    std::ifstream inFile("database.dat", std::ios::binary);
    if (!inFile) {
        std::cout << "File could not be opened.";
        return;
    }

    std::ofstream outFile("temp.dat", std::ios::binary);
    // Read from original file and write to temp file, skipping the record to be deleted
    while (inFile.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        if (pr.getProductId() != id) {
            outFile.write(reinterpret_cast<char*>(&pr), sizeof(Product));
        }
    }

    inFile.close();
    outFile.close();

    remove("database.dat");
    rename("temp.dat", "database.dat");
    std::cout << "\n\nRecord Deleted.";
}
void generate_bill() {
    system("clear || cls"); // Clear the console screen
    std::cout << "\n\n\n--- BILLING COUNTER ---\n\n";
    display_all_products();

    char choice;
    int order_arr[50], quantity_arr[50];
    int counter = 0;
    float total_amount = 0;

    do {
        std::cout << "\n\nEnter the Product ID you want to buy: ";
        std::cin >> order_arr[counter];
        std::cout << "Enter the quantity: ";
        std::cin >> quantity_arr[counter];
        counter++;
        std::cout << "\nDo you want to add another product? (y/n): ";
        std::cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    std::cout << "\n\n--- GENERATING BILL ---\n";
    std::cout << "****************************************************\n";
    std::cout << "Pr ID\tName\t\tQuantity \tPrice \tAmount\n";
    std::cout << "****************************************************\n";

    for (int i = 0; i < counter; i++) {
        file.open("database.dat", std::ios::in | std::ios::binary);
        file.seekg(0, std::ios::beg); // Go to the beginning of the file
        while(file.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
            if(pr.getProductId() == order_arr[i]) {
                float amount = pr.getPrice() * quantity_arr[i];
                total_amount += amount;
                std::cout << pr.getProductId() << "\t" << pr.getName() << "\t\t" << quantity_arr[i] << "\t\t" << pr.getPrice() << "\t" << amount << "\n";
            }
        }
        file.close();
    }
    
    std::cout << "****************************************************\n";
    std::cout << "Grand Total: " << total_amount << std::endl;
    std::cout << "****************************************************\n";
    std::cout << "Thank you for shopping!\n";
}
void admin_menu() {
    char choice;
    do {
        system("clear || cls");
        std::cout << "\n\n--- ADMIN MENU ---\n";
        std::cout << "1. CREATE PRODUCT\n";
        std::cout << "2. DISPLAY ALL PRODUCTS\n";
        std::cout << "3. SEARCH FOR PRODUCT\n";
        std::cout << "4. MODIFY PRODUCT\n";
        std::cout << "5. DELETE PRODUCT\n";
        std::cout << "6. BACK TO MAIN MENU\n";
        std::cout << "Please Enter Your Choice (1-6): ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                write_product();
                break;
            case '2':
                display_all_products();
                break;
            case '3':
                int id;
                std::cout << "\nEnter product ID to search: ";
                std::cin >> id;
                display_specific_product(id);
                break;
            case '4':
                modify_product();
                break;
            case '5':
                delete_product();
                break;
            case '6':
                return; // Exit admin menu
            default:
                std::cout << "\aInvalid choice. Please try again.";
        }
        std::cin.ignore();
        std::cin.get(); // Wait for user to press Enter
    } while (choice != '6');
}

int main() {
    char choice;
    do {
        system("clear || cls");
        std::cout << "\n\n--- SUPERMARKET BILLING SYSTEM ---\n";
        std::cout << "1. BILLING\n";
        std::cout << "2. ADMIN MENU\n";
        std::cout << "3. EXIT\n";
        std::cout << "Please select your option (1-3): ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                generate_bill();
                break;
            case '2':
                admin_menu();
                break;
            case '3':
                std::cout << "Exiting program. Goodbye!\n";
                exit(0);
            default:
                std::cout << "\aInvalid choice. Please try again.";
        }
        std::cin.ignore();
        std::cin.get();
    } while (choice != '3');

    return 0;
}
