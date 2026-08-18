#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


// ============================================================
// PART 1: SIMPLE PASSWORD HASHING
// ============================================================

string hashPassword(string password)
{
    unsigned long long hash = 0;

    for(int i = 0; i < (int)password.length(); i++)
    {
        hash = hash * 31 + (unsigned char)password[i];
    }

    string result = "";
    string hex = "0123456789abcdef";

    if(hash == 0)
    {
        return "0";
    }

    while(hash > 0)
    {
        int digit = hash % 16;

        result = hex[digit] + result;

        hash = hash / 16;
    }

    return result;
}


// ============================================================
// PART 2: USER CLASS
// ============================================================

class User
{
protected:

    string username;
    string password;
    string role;


public:

    User(string u, string p, string r)
    {
        username = u;
        password = p;
        role = r;
    }


    string getUsername()
    {
        return username;
    }


    string getPassword()
    {
        return password;
    }


    string getRole()
    {
        return role;
    }
};


// ============================================================
// PART 3: FOOD ITEM CLASS
// ============================================================

class FoodItem
{
private:

    int id;
    string name;
    string restaurant;

    double price;
    double discount;

    int quantity;

    string expiry;

    // 0 = normal food
    // 1 = charity donation food
    int donation;


public:

    FoodItem(int i,
             string n,
             string r,
             double p,
             double d,
             int q,
             string e,
             int don)
    {
        id = i;
        name = n;
        restaurant = r;
        price = p;
        discount = d;
        quantity = q;
        expiry = e;
        donation = don;
    }


    int getId()
    {
        return id;
    }


    string getName()
    {
        return name;
    }


    string getRestaurant()
    {
        return restaurant;
    }


    double getPrice()
    {
        return price;
    }


    double getDiscount()
    {
        return discount;
    }


    int getQuantity()
    {
        return quantity;
    }


    string getExpiry()
    {
        return expiry;
    }


    int getDonation()
    {
        return donation;
    }


    void setQuantity(int q)
    {
        quantity = q;
    }


    double getFinalPrice()
    {
        return price - (price * discount / 100);
    }


    void display()
    {
        cout << "ID          : " << id << endl;
        cout << "Food        : " << name << endl;
        cout << "Restaurant  : " << restaurant << endl;
        cout << "Price       : " << price << endl;
        cout << "Discount    : " << discount << "%" << endl;

        if(donation == 0)
        {
            cout << "Final Price : "
                 << getFinalPrice() << endl;
        }
        else
        {
            cout << "Type        : Charity Donation" << endl;
        }

        cout << "Quantity    : " << quantity << endl;
        cout << "Expiry      : " << expiry << endl;

        cout << "-----------------------------" << endl;
    }
};


// ============================================================
// PART 4: CHILD CLASSES
// ============================================================

class Customer : public User
{
public:

    Customer(string u, string p)
        : User(u, p, "Customer")
    {
    }
};


class Restaurant : public User
{
public:

    Restaurant(string u, string p)
        : User(u, p, "Restaurant")
    {
    }
};


class Charity : public User
{
public:

    Charity(string u, string p)
        : User(u, p, "Charity")
    {
    }
};


class Admin : public User
{
public:

    Admin(string u, string p)
        : User(u, p, "Admin")
    {
    }
};


// ============================================================
// PART 5: MAIN SYSTEM CLASS
// ============================================================

class FoodWasteSystem
{
private:

    vector<User> users;

    vector<FoodItem> foods;

    string currentUser;
    string currentRole;


public:

    // ========================================================
    // CONSTRUCTOR
    // ========================================================

    FoodWasteSystem()
    {
        currentUser = "";
        currentRole = "";

        loadUsers();
        loadFoods();
    }


    // ========================================================
    // PART 6: USER FILE LOAD
    // ========================================================

    void loadUsers()
    {
        ifstream file("users.txt");

        // File না থাকলেও program বন্ধ/crash করবে না
        if(!file)
        {
            return;
        }

        string username;
        string password;
        string role;


        while(file >> username >> password >> role)
        {
            users.push_back(
                User(username, password, role)
            );
        }


        file.close();
    }


    // ========================================================
    // PART 7: USER FILE SAVE
    // ========================================================

    void saveUsers()
    {
        ofstream file("users.txt");

        if(!file)
        {
            cout << "Error: Could not save user data!" << endl;
            return;
        }


        for(int i = 0; i < (int)users.size(); i++)
        {
            file << users[i].getUsername() << " "
                 << users[i].getPassword() << " "
                 << users[i].getRole()
                 << endl;
        }


        file.close();
    }


    // ========================================================
    // PART 8: FOOD FILE LOAD
    // ========================================================

    void loadFoods()
    {
        ifstream file("food_items.txt");

        // File না থাকলেও program বন্ধ/crash করবে না
        if(!file)
        {
            return;
        }


        int id;
        int quantity;
        int donation;

        string name;
        string restaurant;
        string expiry;

        double price;
        double discount;


        while(file >> id
                  >> name
                  >> restaurant
                  >> price
                  >> discount
                  >> quantity
                  >> expiry
                  >> donation)
        {
            foods.push_back(
                FoodItem(
                    id,
                    name,
                    restaurant,
                    price,
                    discount,
                    quantity,
                    expiry,
                    donation
                )
            );
        }


        file.close();
    }


    // ========================================================
    // PART 9: FOOD FILE SAVE
    // ========================================================

    void saveFoods()
    {
        ofstream file("food_items.txt");

        if(!file)
        {
            cout << "Error: Could not save food data!" << endl;
            return;
        }


        for(int i = 0; i < (int)foods.size(); i++)
        {
            file << foods[i].getId() << " "
                 << foods[i].getName() << " "
                 << foods[i].getRestaurant() << " "
                 << foods[i].getPrice() << " "
                 << foods[i].getDiscount() << " "
                 << foods[i].getQuantity() << " "
                 << foods[i].getExpiry() << " "
                 << foods[i].getDonation()
                 << endl;
        }


        file.close();
    }


    // ========================================================
    // PART 10: REGISTER
    // ========================================================

    bool registerUser(string username,
                      string password,
                      string role)
    {

        // Empty username/password prevent করা
        if(username.empty() || password.empty())
        {
            cout << "Username and Password cannot be empty!"
                 << endl;

            return false;
        }


        // একই username আগে আছে কিনা check
        for(int i = 0; i < (int)users.size(); i++)
        {
            if(users[i].getUsername() == username)
            {
                cout << "Username already exists!" << endl;

                return false;
            }
        }


        // Password hash করা
        string hashedPassword =
            hashPassword(password);


        // নতুন user add করা
        users.push_back(
            User(
                username,
                hashedPassword,
                role
            )
        );


        // File-এ save করা
        saveUsers();


        cout << "Registration Successful!" << endl;

        return true;
    }


    // ========================================================
    // PART 11: LOGIN
    // ========================================================

    bool login(string username, string password)
    {
        // Empty input হলে login হবে না
        if(username.empty() || password.empty())
        {
            return false;
        }


        // Entered password আবার একইভাবে hash করা
        string hashedPassword =
            hashPassword(password);


        // User list search করা
        for(int i = 0; i < (int)users.size(); i++)
        {

            if(users[i].getUsername() == username &&
               users[i].getPassword() == hashedPassword)
            {

                currentUser = username;

                currentRole =
                    users[i].getRole();


                return true;
            }
        }


        return false;
    }


    // ========================================================
    // PART 12: LOGOUT
    // ========================================================

    void logout()
    {
        currentUser = "";
        currentRole = "";

        cout << "Logout Successful!" << endl;
    }


    // ========================================================
    // PART 13: CURRENT ROLE
    // ========================================================

    string getRole()
    {
        return currentRole;
    }


    // ========================================================
    // PART 14: RESTAURANT ADD FOOD
    // ========================================================

    void addFood()
    {
        string name;
        string expiry;

        double price;
        double discount;

        int quantity;

        int donationChoice;


        cout << "Food Name: ";
        cin >> name;


        cout << "Price: ";
        cin >> price;


        cout << "Discount (%): ";
        cin >> discount;


        cout << "Quantity: ";
        cin >> quantity;


        cout << "Expiry: ";
        cin >> expiry;


        cout << "\nFood Type:\n";
        cout << "1. Normal Food\n";
        cout << "2. Charity Donation\n";

        cout << "Choice: ";
        cin >> donationChoice;


        // Input validation
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid Input!" << endl;
            return;
        }


        if(price < 0 ||
           discount < 0 ||
           discount > 100 ||
           quantity <= 0 ||
           (donationChoice != 1 &&
            donationChoice != 2))
        {
            cout << "Invalid Input!" << endl;

            return;
        }


        int donation;


        if(donationChoice == 1)
            donation = 0;

        else
            donation = 1;


        // ====================================================
        // New Food ID
        // ====================================================

        int id = 1;

        for(int i = 0; i < (int)foods.size(); i++)
        {
            if(foods[i].getId() >= id)
            {
                id = foods[i].getId() + 1;
            }
        }


        // Food object তৈরি করে vector-এ add
        foods.push_back(
            FoodItem(
                id,
                name,
                currentUser,
                price,
                discount,
                quantity,
                expiry,
                donation
            )
        );


        // File-এ save
        saveFoods();


        cout << "\nFood Added Successfully!" << endl;
        cout << "Food ID: " << id << endl;
    }


    // ========================================================
    // PART 15: VIEW NORMAL FOOD
    // ========================================================

    void viewFood()
    {
        bool found = false;


        cout << "\n========== AVAILABLE FOOD ==========\n";


        for(int i = 0; i < (int)foods.size(); i++)
        {

            if(foods[i].getQuantity() > 0 &&
               foods[i].getDonation() == 0)
            {
                foods[i].display();

                found = true;
            }
        }


        if(!found)
        {
            cout << "No Normal Food Available!" << endl;
        }
    }


    // ========================================================
    // PART 16: SEARCH FOOD
    // ========================================================

    void searchFood()
    {
        string keyword;

        cout << "Enter Food Name: ";
        cin >> keyword;


        bool found = false;


        for(int i = 0; i < (int)foods.size(); i++)
        {

            if(foods[i].getName() == keyword &&
               foods[i].getQuantity() > 0 &&
               foods[i].getDonation() == 0)
            {
                foods[i].display();

                found = true;
            }
        }


        if(!found)
        {
            cout << "Food Not Found!" << endl;
        }
    }


    // ========================================================
    // PART 17: CUSTOMER ORDER
    // ========================================================

    void placeOrder()
    {
        int id;
        int quantity;


        cout << "Enter Food ID: ";
        cin >> id;


        cout << "Enter Quantity: ";
        cin >> quantity;


        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid Input!" << endl;
            return;
        }


        if(quantity <= 0)
        {
            cout << "Invalid Quantity!" << endl;

            return;
        }


        // Food ID search
        for(int i = 0; i < (int)foods.size(); i++)
        {

            if(foods[i].getId() == id &&
               foods[i].getDonation() == 0)
            {

                // Quantity check
                if(foods[i].getQuantity() < quantity)
                {
                    cout << "Not Enough Quantity!" << endl;

                    return;
                }


                // Total price
                double total =
                    foods[i].getFinalPrice()
                    * quantity;


                // Quantity কমানো
                foods[i].setQuantity(
                    foods[i].getQuantity()
                    - quantity
                );


                // Updated data save
                saveFoods();


                cout << "\n================================\n";
                cout << "Order Placed Successfully!" << endl;
                cout << "Total Price: " << total << endl;
                cout << "================================\n";


                return;
            }
        }


        cout << "Food ID Not Found!" << endl;
    }


    // ========================================================
    // PART 18: VIEW CHARITY FOOD
    // ========================================================

    void viewDonationFood()
    {
        bool found = false;


        cout << "\n====== CHARITY DONATION FOOD ======\n";


        for(int i = 0; i < (int)foods.size(); i++)
        {

            if(foods[i].getQuantity() > 0 &&
               foods[i].getDonation() == 1)
            {
                foods[i].display();

                found = true;
            }
        }


        if(!found)
        {
            cout << "No Donation Food Available!" << endl;
        }
    }


    // ========================================================
    // PART 19: CHARITY RECEIVE FOOD
    // ========================================================

    void receiveDonation()
    {
        int id;
        int quantity;


        cout << "Enter Donation Food ID: ";
        cin >> id;


        cout << "Enter Quantity: ";
        cin >> quantity;


        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid Input!" << endl;
            return;
        }


        if(quantity <= 0)
        {
            cout << "Invalid Quantity!" << endl;

            return;
        }


        // Donation food search
        for(int i = 0; i < (int)foods.size(); i++)
        {

            if(foods[i].getId() == id &&
               foods[i].getDonation() == 1)
            {

                // Quantity check
                if(foods[i].getQuantity() < quantity)
                {
                    cout << "Not Enough Food Available!"
                         << endl;

                    return;
                }


                // Quantity কমানো
                foods[i].setQuantity(
                    foods[i].getQuantity()
                    - quantity
                );


                // Save updated data
                saveFoods();


                cout << "\n====================================\n";
                cout << "Food Donation Received Successfully!"
                     << endl;

                cout << "Charity: "
                     << currentUser << endl;

                cout << "Quantity Received: "
                     << quantity << endl;

                cout << "====================================\n";


                return;
            }
        }


        cout << "Donation Food ID Not Found!" << endl;
    }


    // ========================================================
    // PART 20: ADMIN WASTE REPORT
    // ========================================================

    void report()
    {
        int available = 0;
        int soldOut = 0;
        int donationFood = 0;


        for(int i = 0; i < (int)foods.size(); i++)
        {

            if(foods[i].getQuantity() == 0)
            {
                soldOut++;
            }

            else
            {
                available++;
            }


            if(foods[i].getDonation() == 1 &&
               foods[i].getQuantity() > 0)
            {
                donationFood++;
            }
        }


        cout << "\n====================================\n";
        cout << "      FOOD WASTE REDUCTION REPORT\n";
        cout << "====================================\n";


        cout << "Available Food : "
             << available << endl;


        cout << "Sold Out Food  : "
             << soldOut << endl;


        cout << "Donation Food  : "
             << donationFood << endl;


        cout << "====================================\n";
    }


    // ========================================================
    // PART 21: CUSTOMER MENU
    // ========================================================

    void customerMenu()
    {
        int choice;


        do
        {
            cout << "\n========== CUSTOMER MENU ==========\n";

            cout << "1. View Food\n";
            cout << "2. Search Food\n";
            cout << "3. Place Order\n";
            cout << "4. Logout\n";


            cout << "Enter Choice: ";
            cin >> choice;


            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');

                cout << "Invalid Choice!" << endl;

                continue;
            }


            if(choice == 1)
                viewFood();


            else if(choice == 2)
                searchFood();


            else if(choice == 3)
                placeOrder();


            else if(choice == 4)
                logout();


            else
                cout << "Invalid Choice!" << endl;


        }while(choice != 4);
    }


    // ========================================================
    // PART 22: RESTAURANT MENU
    // ========================================================

    void restaurantMenu()
    {
        int choice;


        do
        {
            cout << "\n========== RESTAURANT MENU ==========\n";

            cout << "1. Add Food\n";
            cout << "2. View Food\n";
            cout << "3. Logout\n";


            cout << "Enter Choice: ";
            cin >> choice;


            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');

                cout << "Invalid Choice!" << endl;

                continue;
            }


            if(choice == 1)
                addFood();


            else if(choice == 2)
                viewFood();


            else if(choice == 3)
                logout();


            else
                cout << "Invalid Choice!" << endl;


        }while(choice != 3);
    }


    // ========================================================
    // PART 23: CHARITY MENU
    // ========================================================

    void charityMenu()
    {
        int choice;


        do
        {
            cout << "\n========== CHARITY MENU ==========\n";

            cout << "1. View Donation Food\n";
            cout << "2. Receive Food Donation\n";
            cout << "3. Logout\n";


            cout << "Enter Choice: ";
            cin >> choice;


            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');

                cout << "Invalid Choice!" << endl;

                continue;
            }


            if(choice == 1)
                viewDonationFood();


            else if(choice == 2)
                receiveDonation();


            else if(choice == 3)
                logout();


            else
                cout << "Invalid Choice!" << endl;


        }while(choice != 3);
    }


    // ========================================================
    // PART 24: ADMIN MENU
    // ========================================================

    void adminMenu()
    {
        int choice;


        do
        {
            cout << "\n========== ADMIN MENU ==========\n";

            cout << "1. View Food\n";
            cout << "2. Generate Report\n";
            cout << "3. Logout\n";


            cout << "Enter Choice: ";
            cin >> choice;


            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');

                cout << "Invalid Choice!" << endl;

                continue;
            }


            if(choice == 1)
                viewFood();


            else if(choice == 2)
                report();


            else if(choice == 3)
                logout();


            else
                cout << "Invalid Choice!" << endl;


        }while(choice != 3);
    }
};


// ============================================================
// PART 25: MAIN FUNCTION
// ============================================================

int main()
{
    FoodWasteSystem system;

    int choice;


    cout << "========================================\n";
    cout << "      FOOD WASTE REDUCTION SYSTEM\n";
    cout << "========================================\n";


    do
    {
        cout << "\n========== MAIN MENU ==========\n";

        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";


        cout << "Enter Choice: ";
        cin >> choice;


        // Invalid input handle
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid Choice!" << endl;

            continue;
        }


        // ====================================================
        // REGISTER
        // ====================================================

        if(choice == 1)
        {
            string username;
            string password;

            int roleChoice;


            cout << "Username: ";
            cin >> username;


            cout << "Password: ";
            cin >> password;


            cout << "\nSelect Role\n";

            cout << "1. Customer\n";
            cout << "2. Restaurant\n";
            cout << "3. Charity\n";
            cout << "4. Admin\n";


            cout << "Choice: ";
            cin >> roleChoice;


            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');

                cout << "Invalid Role!" << endl;

                continue;
            }


            string role;


            if(roleChoice == 1)
                role = "Customer";


            else if(roleChoice == 2)
                role = "Restaurant";


            else if(roleChoice == 3)
                role = "Charity";


            else if(roleChoice == 4)
                role = "Admin";


            else
            {
                cout << "Invalid Role!" << endl;

                continue;
            }


            system.registerUser(
                username,
                password,
                role
            );
        }


        // ====================================================
        // LOGIN
        // ====================================================

        else if(choice == 2)
        {
            string username;
            string password;


            cout << "Username: ";
            cin >> username;


            cout << "Password: ";
            cin >> password;


            if(system.login(username, password))
            {
                cout << "Login Successful!" << endl;


                if(system.getRole() == "Customer")
                {
                    system.customerMenu();
                }


                else if(system.getRole() == "Restaurant")
                {
                    system.restaurantMenu();
                }


                else if(system.getRole() == "Charity")
                {
                    system.charityMenu();
                }


                else if(system.getRole() == "Admin")
                {
                    system.adminMenu();
                }
            }


            else
            {
                cout << "Invalid Username or Password!"
                     << endl;
            }
        }


        // ====================================================
        // EXIT
        // ====================================================

        else if(choice == 3)
        {
            cout << "Thank You!" << endl;
        }


        else
        {
            cout << "Invalid Choice!" << endl;
        }


    }while(choice != 3);


    return 0;
}
