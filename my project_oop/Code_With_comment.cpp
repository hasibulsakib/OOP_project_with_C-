#include <iostream>   // cin, cout এর জন্য
#include <vector>     // vector ব্যবহার করার জন্য
#include <string>     // string data type এর জন্য
#include <fstream>    // File Handling (Read/Write)
#include <map>        // map container এর জন্য
#include <algorithm>  // বিভিন্ন algorithm (sort, find ইত্যাদি)
#include <iomanip>    // Output formatting (setw, setprecision)

using namespace std;

//=========================================================
// Base Class : User
// এই class থেকে Customer, Restaurant, Admin তৈরি হবে
//=========================================================

class User
{
protected:
    // protected দিলে Child Class (Customer, Restaurant, Admin)
    // এগুলো access করতে পারবে
    string username;
    string password;
    string role;

public:

    //=========================
    // Constructor
    // Object তৈরি হওয়ার সাথে সাথে
    // username, password, role initialize করবে
    //=========================
    User(string uname, string pass, string r)
    {
        username = uname;
        password = pass;
        role = r;
    }

    //=========================
    // Virtual Destructor
    // Parent pointer দিয়ে Child object delete করলে
    // সঠিক Destructor call হবে
    //=========================
    virtual ~User(){}

    //=========================
    // Getter Function
    // Username return করবে
    //=========================
    string getUsername() const
    {
        return username;
    }

    //=========================
    // Getter Function
    // User এর Role return করবে
    // যেমনঃ Customer / Restaurant / Admin
    //=========================
    string getRole() const
    {
        return role;
    }

    //=========================
    // Password Check Function
    // Login এর সময় input password
    // stored password এর সাথে মিলিয়ে দেখবে
    //=========================
    bool checkPassword(string pass) const
    {
        return password == pass;
    }

    //=====================================================
    // Pure Virtual Function
    //
    // User class-এর নিজের কোনো Menu নেই।
    // তাই এখানে শুধু declare করা হয়েছে।
    //
    // Customer নিজের Menu বানাবে।
    // Restaurant নিজের Menu বানাবে।
    // Admin নিজের Menu বানাবে।
    //
    // =0 দেওয়ার কারণে User class-এর object
    // তৈরি করা যাবে না।
    //=====================================================
    virtual void displayMenu() = 0;
};

//=========================================================
// FoodItem Class
// প্রতিটি Food Item-এর তথ্য এখানে থাকবে
//=========================================================

class FoodItem
{
private:

    // প্রতিটি Food-এর Unique ID
    int id;

    // Food-এর নাম
    string name;

    // কোন Restaurant এই Food Add করেছে
    string restaurantName;

    // Food-এর Original Price
    double originalPrice;

    // কত % Discount দেওয়া হয়েছে
    double discountPercentage;

    // কত Quantity আছে
    int quantity;

    // Expiry Time
    string expiryTime;

public:

    //=====================================================
    // Default Constructor
    // Object তৈরি হলে প্রথমে সব Value 0 বা Empty থাকবে
    //=====================================================
    FoodItem()
    {
        id = 0;
        originalPrice = 0;
        discountPercentage = 0;
        quantity = 0;
    }

    //=====================================================
    // Parameterized Constructor
    // একসাথে সব তথ্য Initialize করবে
    //=====================================================
    FoodItem(int fId,
             string fName,
             string rName,
             double price,
             double discount,
             int qty,
             string expiry)
    {
        id = fId;
        name = fName;
        restaurantName = rName;
        originalPrice = price;
        discountPercentage = discount;
        quantity = qty;
        expiryTime = expiry;
    }

    //=========================
    // Getter Functions
    // Private Variable বাইরে Return করবে
    //=========================

    int getId() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }

    string getRestaurantName() const
    {
        return restaurantName;
    }

    double getOriginalPrice() const
    {
        return originalPrice;
    }

    double getDiscountPercentage() const
    {
        return discountPercentage;
    }

    int getQuantity() const
    {
        return quantity;
    }

    string getExpiryTime() const
    {
        return expiryTime;
    }

    //=========================
    // Setter Function
    // Quantity Update করার জন্য
    //=========================
    void setQuantity(int qty)
    {
        quantity = qty;
    }

    //=====================================================
    // Discount Price Calculate করবে
    //
    // Example:
    // Price = 200
    // Discount = 25%
    //
    // Return করবে 150
    //=====================================================
    double getDiscountedPrice() const
    {
        return originalPrice * (1 - discountPercentage / 100.0);
    }

    //=====================================================
    // Food-এর সব Information সুন্দরভাবে Print করবে
    //=====================================================
    void displayFoodDetails() const
    {
        cout << left
             << setw(5) << id
             << setw(20) << name
             << setw(20) << restaurantName
             << "$ " << setw(10) << fixed << setprecision(2) << originalPrice
             << setw(10) << (to_string((int)discountPercentage) + "%")
             << "$ " << setw(10) << getDiscountedPrice()
             << setw(10) << quantity
             << setw(10) << expiryTime
             << endl;
    }
};

//=========================================================
// FoodWasteSystem Class
// পুরো System Control করবে
//=========================================================

class FoodWasteSystem
{
private:

    //=====================================================
    // সব Food Item এখানে Store হবে
    //=====================================================
    vector<FoodItem> foodList;

    //=====================================================
    // Username -> Password
    // Example:
    // sakib -> 1234
    //=====================================================
    map<string,string> usersCredentials;

    //=====================================================
    // Username -> Role
    // Example:
    // sakib -> Customer
    //=====================================================
    map<string,string> userRoles;

    // বর্তমানে কে Login করেছে
    string currentLoggedInUser;

    // বর্তমানে Login করা User-এর Role
    string currentRole;

    // Users File
    string userFile = "users.txt";

    // Food File
    string foodFile = "food_items.txt";

public:

    //=====================================================
    // Constructor
    // Program Start হলে File থেকে Data Load করবে
    //=====================================================
    FoodWasteSystem()
    {
        currentLoggedInUser = "";
        currentRole = "";

        loadUsersFromFile();
        loadFoodFromFile();
    }

    //=====================================================
    // Destructor
    // Program বন্ধ হওয়ার আগে সব Data Save করবে
    //=====================================================
    ~FoodWasteSystem()
    {
        saveUsersToFile();
        saveFoodToFile();
    }

    //=====================================================
    // Function Declaration
    // Body পরে লিখবো
    //=====================================================

    void loadUsersFromFile();

    void saveUsersToFile();

    void loadFoodFromFile();

    void saveFoodToFile();

    bool registerUser(string uname,
                      string pass,
                      string role);

    bool loginUser(string uname,
                   string pass);

    void logout();

    string getCurrentUser() const;

    string getCurrentRole() const;

    vector<FoodItem>& getFoodList();

    void addFoodItem(string name,
                     string restaurant,
                     double price,
                     double discount,
                     int quantity,
                     string expiry);

    void viewAvailableFood();

    void searchFood(string keyword);

    bool placeOrder(int foodId,
                    int quantity);

    void generateWasteReport();
};

//=========================================================
// users.txt File থেকে সব User Load করবে
//=========================================================

void FoodWasteSystem::loadUsersFromFile()
{
    // File Open করবে
    ifstream file(userFile);

    // যদি File না থাকে তাহলে Function শেষ
    if(!file.is_open())
    {
        return;
    }

    string uname;
    string pass;
    string role;

    // যতক্ষণ পর্যন্ত File-এ Data আছে
    while(file >> uname >> pass >> role)
    {
        // Username অনুযায়ী Password Save করবে
        usersCredentials[uname] = pass;

        // Username অনুযায়ী Role Save করবে
        userRoles[uname] = role;
    }

    // File Close
    file.close();
}
//=========================================================
// সব User Data users.txt File-এ Save করবে
//=========================================================

void FoodWasteSystem::saveUsersToFile()
{
    // File Open (Write Mode)
    ofstream file(userFile);

    // map-এর প্রতিটি User File-এ Save করবে
    for(map<string,string>::iterator it = usersCredentials.begin();
        it != usersCredentials.end();
        it++)
    {
        file << it->first << " "
             << it->second << " "
             << userRoles[it->first]
             << endl;
    }

    // File Close
    file.close();
}
//=========================================================
// food_items.txt File থেকে সব Food Load করবে
//=========================================================

void FoodWasteSystem::loadFoodFromFile()
{
    // File Open
    ifstream file(foodFile);

    // File না থাকলে Function শেষ
    if(!file.is_open())
    {
        return;
    }

    int id;
    int qty;

    string name;
    string restaurant;
    string expiry;

    double price;
    double discount;

    // যতক্ষণ File-এ Data থাকবে
    while(file >> id
               >> name
               >> restaurant
               >> price
               >> discount
               >> qty
               >> expiry)
    {
        // নতুন Food Object তৈরি করে Vector-এ Save করবে
        foodList.push_back(
            FoodItem(
                id,
                name,
                restaurant,
                price,
                discount,
                qty,
                expiry
            )
        );
    }

    // File Close
    file.close();
}
//=========================================================
// সব Food Data food_items.txt File-এ Save করবে
//=========================================================

void FoodWasteSystem::saveFoodToFile()
{
    // File Open (Write Mode)
    ofstream file(foodFile);

    // Vector-এর প্রতিটি Food File-এ Save করবে
    for(int i = 0; i < foodList.size(); i++)
    {
        file << foodList[i].getId() << " "
             << foodList[i].getName() << " "
             << foodList[i].getRestaurantName() << " "
             << foodList[i].getOriginalPrice() << " "
             << foodList[i].getDiscountPercentage() << " "
             << foodList[i].getQuantity() << " "
             << foodList[i].getExpiryTime()
             << endl;
    }

    // File Close
    file.close();
}

//=========================================================
// নতুন User Register করবে
//=========================================================

bool FoodWasteSystem::registerUser(string uname,
                                   string pass,
                                   string role)
{
    // Username আগে থেকেই আছে কিনা Check করবে
    if(usersCredentials.find(uname) != usersCredentials.end())
    {
        cout << "Username already exists!" << endl;
        return false;
    }

    // Username এবং Password Save করবে
    usersCredentials[uname] = pass;

    // User-এর Role Save করবে
    userRoles[uname] = role;

    // File-এ Save করবে
    saveUsersToFile();

    cout << "Registration Successful!" << endl;

    return true;
}
//=========================================================
// User Login করবে
//=========================================================

bool FoodWasteSystem::loginUser(string uname,
                                string pass)
{
    // Username আছে কিনা Check করবে
    if(usersCredentials.find(uname) != usersCredentials.end())
    {
        // Password মিললে Login Success
        if(usersCredentials[uname] == pass)
        {
            // বর্তমানে কে Login করেছে Save করবে
            currentLoggedInUser = uname;

            // User-এর Role Save করবে
            currentRole = userRoles[uname];

            return true;
        }
    }

    // Username অথবা Password ভুল
    return false;
}
//=========================================================
// বর্তমানে Login করা User Logout করবে
//=========================================================

void FoodWasteSystem::logout()
{
    // Login করা User Remove করবে
    currentLoggedInUser = "";

    // User-এর Role Remove করবে
    currentRole = "";
}

//=========================================================
// বর্তমানে Login করা Username Return করবে
//=========================================================

string FoodWasteSystem::getCurrentUser() const
{
    return currentLoggedInUser;
}

//=========================================================
// বর্তমানে Login করা User-এর Role Return করবে
//=========================================================

string FoodWasteSystem::getCurrentRole() const
{
    return currentRole;
}

//=========================================================
// পুরো Food List Return করবে
//=========================================================

vector<FoodItem>& FoodWasteSystem::getFoodList()
{
    return foodList;
}

//=========================================================
// নতুন Food Item System-এ Add করবে
//=========================================================

void FoodWasteSystem::addFoodItem(string name,
                                  string restaurant,
                                  double price,
                                  double discount,
                                  int quantity,
                                  string expiry)
{
    //=====================================================
    // নতুন ID Generate করবে
    // যদি List Empty হয় তাহলে ID = 1
    // নাহলে Last Food-এর ID + 1
    //=====================================================

    int newId;

    if(foodList.empty())
    {
        newId = 1;
    }
    else
    {
        newId = foodList.back().getId() + 1;
    }

    //=====================================================
    // নতুন Food Object Vector-এ Add করবে
    //=====================================================

    foodList.push_back(
        FoodItem(
            newId,
            name,
            restaurant,
            price,
            discount,
            quantity,
            expiry
        )
    );

    //=====================================================
    // নতুন Data File-এ Save করবে
    //=====================================================

    saveFoodToFile();

    cout << "Food Added Successfully!" << endl;
    cout << "Food ID : " << newId << endl;
}

//=========================================================
// সব Available Food সুন্দরভাবে Display করবে
//=========================================================

void FoodWasteSystem::viewAvailableFood()
{
    // যদি কোনো Food না থাকে
    if(foodList.empty())
    {
        cout << "No Food Available!" << endl;
        return;
    }

    cout << "\n==============================================================================================\n";

    cout << left
         << setw(5)  << "ID"
         << setw(20) << "Food"
         << setw(20) << "Restaurant"
         << setw(10) << "Price"
         << setw(10) << "Discount"
         << setw(12) << "Final Price"
         << setw(8)  << "Qty"
         << setw(10) << "Expiry"
         << endl;

    cout << "==============================================================================================\n";

    // সব Food Display করবে
    for(int i = 0; i < foodList.size(); i++)
    {
        // শুধুমাত্র Available Food দেখাবে
        if(foodList[i].getQuantity() > 0)
        {
            foodList[i].displayFoodDetails();
        }
    }

    cout << "==============================================================================================\n";
}

//=========================================================
// Food Name অথবা Restaurant Name দিয়ে Search করবে
//=========================================================

void FoodWasteSystem::searchFood(string keyword)
{
    cout << "\n========== Search Result ==========\n";

    // কোনো Food পাওয়া গেছে কিনা Check করবে
    bool found = false;

    // সব Food Search করবে
    for(int i = 0; i < foodList.size(); i++)
    {
        // Food Name অথবা Restaurant Name Match হলে
        if((foodList[i].getName().find(keyword) != string::npos ||
            foodList[i].getRestaurantName().find(keyword) != string::npos)
            &&
            foodList[i].getQuantity() > 0)
        {
            foodList[i].displayFoodDetails();

            found = true;
        }
    }

    // কিছু না পেলে Message দেখাবে
    if(!found)
    {
        cout << "No Matching Food Found!" << endl;
    }
}

//=========================================================
// Customer Order Place করবে
//=========================================================

bool FoodWasteSystem::placeOrder(int foodId,
                                 int quantity)
{
    // সব Food Search করবে
    for(int i = 0; i < foodList.size(); i++)
    {
        // Food ID Match করলে
        if(foodList[i].getId() == foodId)
        {
            // Quantity আছে কিনা Check করবে
            if(foodList[i].getQuantity() < quantity)
            {
                cout << "Not Enough Quantity Available!" << endl;

                return false;
            }

            // নতুন Quantity Calculate করবে
            int remainingQuantity =
                foodList[i].getQuantity() - quantity;

            // Quantity Update করবে
            foodList[i].setQuantity(remainingQuantity);

            // File Update করবে
            saveFoodToFile();

            // Total Price Calculate করবে
            double totalPrice =
                foodList[i].getDiscountedPrice() * quantity;

            cout << "==================================" << endl;
            cout << "Order Placed Successfully!" << endl;
            cout << "Total Price : " << totalPrice << endl;
            cout << "==================================" << endl;

            return true;
        }
    }

    // Food ID না পেলে
    cout << "Food ID Not Found!" << endl;

    return false;
}

//=========================================================
// System Report Generate করবে
//=========================================================

void FoodWasteSystem::generateWasteReport()
{
    cout << "\n==========================================" << endl;
    cout << "      FOOD WASTE REDUCTION REPORT" << endl;
    cout << "==========================================" << endl;

    // কতগুলো Food Sold Out হয়েছে
    int totalSoldOut = 0;

    // কতগুলো Food এখনও Available আছে
    int totalAvailable = 0;

    // সব Food Check করবে
    for(int i = 0; i < foodList.size(); i++)
    {
        if(foodList[i].getQuantity() == 0)
        {
            totalSoldOut++;
        }
        else
        {
            totalAvailable++;
        }
    }

    cout << "Sold Out Food Items : "
         << totalSoldOut << endl;

    cout << "Available Food Items : "
         << totalAvailable << endl;

    cout << "==========================================" << endl;
}

//=========================================================
// Customer Class
// User Class থেকে Inherit করেছে
//=========================================================

class Customer : public User
{
private:

    // পুরো System Access করার জন্য Reference
    FoodWasteSystem &system;

public:

    //=====================================================
    // Constructor
    //=====================================================
    Customer(string uname,
             string pass,
             FoodWasteSystem &sys)
        : User(uname, pass, "Customer"), system(sys)
    {

    }

    //=====================================================
    // Customer Menu
    //=====================================================
    void displayMenu() override
    {
        int choice;

        do
        {
            cout << "\n========== CUSTOMER MENU ==========\n";

            cout << "1. View Available Food\n";
            cout << "2. Search Food\n";
            cout << "3. Place Order\n";
            cout << "4. Logout\n";

            cout << "Enter Choice : ";
            cin >> choice;

            switch(choice)
            {
                case 1:
                {
                    system.viewAvailableFood();
                    break;
                }

                case 2:
                {
                    string keyword;

                    cout << "Enter Food/Restaurant Name : ";
                    cin >> keyword;

                    system.searchFood(keyword);

                    break;
                }

                case 3:
                {
                    int id;
                    int quantity;

                    cout << "Enter Food ID : ";
                    cin >> id;

                    cout << "Enter Quantity : ";
                    cin >> quantity;

                    system.placeOrder(id, quantity);

                    break;
                }

                case 4:
                {
                    system.logout();

                    cout << "Logout Successful!" << endl;

                    break;
                }

                default:
                {
                    cout << "Invalid Choice!" << endl;
                }
            }

        }
        while(choice != 4);
    }
};

//=========================================================
// Restaurant Class
// User Class থেকে Inherit করেছে
//=========================================================

class Restaurant : public User
{
private:

    // পুরো System Access করার জন্য Reference
    FoodWasteSystem &system;

public:

    //=====================================================
    // Constructor
    //=====================================================
    Restaurant(string uname,
               string pass,
               FoodWasteSystem &sys)
        : User(uname, pass, "Restaurant"), system(sys)
    {

    }

    //=====================================================
    // Restaurant Menu
    //=====================================================
    void displayMenu() override
    {
        int choice;

        do
        {
            cout << "\n========== RESTAURANT MENU ==========\n";

            cout << "1. Add Food Item\n";
            cout << "2. View Available Food\n";
            cout << "3. Logout\n";

            cout << "Enter Choice : ";
            cin >> choice;

            switch(choice)
            {
                case 1:
                {
                    string foodName;
                    string expiry;

                    double price;
                    double discount;

                    int quantity;

                    cout << "Food Name : ";
                    cin >> foodName;

                    cout << "Original Price : ";
                    cin >> price;

                    cout << "Discount Percentage : ";
                    cin >> discount;

                    cout << "Quantity : ";
                    cin >> quantity;

                    cout << "Expiry Time : ";
                    cin >> expiry;

                    // Invalid Input Check
                    if(price < 0 || discount < 0 || discount > 100 || quantity <= 0)
                    {
                        cout << "Invalid Input!" << endl;
                    }
                    else
                    {
                        system.addFoodItem(
                            foodName,
                            username,
                            price,
                            discount,
                            quantity,
                            expiry
                        );
                    }

                    break;
                }

                case 2:
                {
                    system.viewAvailableFood();
                    break;
                }

                case 3:
                {
                    system.logout();

                    cout << "Logout Successful!" << endl;

                    break;
                }

                default:
                {
                    cout << "Invalid Choice!" << endl;
                }
            }

        }
        while(choice != 3);
    }
};
//=========================================================
// Admin Class
// User Class থেকে Inherit করেছে
//=========================================================

class Admin : public User
{
private:

    // পুরো System Access করার জন্য Reference
    FoodWasteSystem &system;

public:

    //=====================================================
    // Constructor
    //=====================================================
    Admin(string uname,
          string pass,
          FoodWasteSystem &sys)
        : User(uname, pass, "Admin"), system(sys)
    {

    }

    //=====================================================
    // Admin Menu
    //=====================================================
    void displayMenu() override
    {
        int choice;

        do
        {
            cout << "\n========== ADMIN MENU ==========\n";

            cout << "1. View All Food\n";
            cout << "2. Generate Report\n";
            cout << "3. Logout\n";

            cout << "Enter Choice : ";
            cin >> choice;

            switch(choice)
            {
                case 1:
                {
                    system.viewAvailableFood();
                    break;
                }

                case 2:
                {
                    system.generateWasteReport();
                    break;
                }

                case 3:
                {
                    system.logout();

                    cout << "Logout Successful!" << endl;

                    break;
                }

                default:
                {
                    cout << "Invalid Choice!" << endl;
                }
            }

        }
        while(choice != 3);
    }
};
//=========================================================
// Main Function
//=========================================================

int main()
{
    // পুরো System-এর Object তৈরি
    FoodWasteSystem system;

    int choice;

    cout << "==========================================" << endl;
    cout << "    FOOD WASTE REDUCTION SYSTEM" << endl;
    cout << "==========================================" << endl;

    do
    {
        cout << "\n========== MAIN MENU ==========\n";

        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";

        cout << "Enter Choice : ";
        cin >> choice;

        switch(choice)
        {
            //=====================================
            // Register
            //=====================================
            case 1:
            {
                string username;
                string password;

                int roleChoice;

                cout << "Enter Username : ";
                cin >> username;

                cout << "Enter Password : ";
                cin >> password;

                cout << "\nSelect Role\n";
                cout << "1. Customer\n";
                cout << "2. Restaurant\n";
                cout << "3. Admin\n";

                cout << "Choice : ";
                cin >> roleChoice;

                string role;

                if(roleChoice == 1)
                {
                    role = "Customer";
                }
                else if(roleChoice == 2)
                {
                    role = "Restaurant";
                }
                else
                {
                    role = "Admin";
                }

                system.registerUser(username,
                                    password,
                                    role);

                break;
            }

            //=====================================
            // Login
            //=====================================
            case 2:
            {
                string username;
                string password;

                cout << "Username : ";
                cin >> username;

                cout << "Password : ";
                cin >> password;

                if(system.loginUser(username,password))
                {
                    User *activeUser = NULL;

                    if(system.getCurrentRole()=="Customer")
                    {
                        activeUser =
                        new Customer(username,
                                     password,
                                     system);
                    }

                    else if(system.getCurrentRole()=="Restaurant")
                    {
                        activeUser =
                        new Restaurant(username,
                                       password,
                                       system);
                    }

                    else if(system.getCurrentRole()=="Admin")
                    {
                        activeUser =
                        new Admin(username,
                                  password,
                                  system);
                    }

                    if(activeUser!=NULL)
                    {
                        activeUser->displayMenu();

                        delete activeUser;
                    }
                }
                else
                {
                    cout << "Invalid Username or Password!"
                         << endl;
                }

                break;
            }

            //=====================================
            // Exit
            //=====================================
            case 3:
            {
                cout << "Thank You!" << endl;
                break;
            }

            default:
            {
                cout << "Invalid Choice!" << endl;
            }
        }

    }
    while(choice != 3);

    return 0;
}