#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

class User
{
protected:
    string username;
    string password;
    string role;

public:
    User(string uname, string pass, string r)
    {
        username = uname;
        password = pass;
        role = r;
    }

    virtual ~User(){}

    string getUsername() const
    {
        return username;
    }

    string getRole() const
    {
        return role;
    }

    bool checkPassword(string pass) const
    {
        return password == pass;
    }

    virtual void displayMenu() = 0;
};

class FoodItem
{
private:
    int id;
    string name;
    string restaurantName;
    double originalPrice;
    double discountPercentage;
    int quantity;
    string expiryTime;

public:
    FoodItem()
    {
        id = 0;
        originalPrice = 0;
        discountPercentage = 0;
        quantity = 0;
    }

    FoodItem(int fId, string fName, string rName, double price, double discount, int qty, string expiry)
    {
        id = fId;
        name = fName;
        restaurantName = rName;
        originalPrice = price;
        discountPercentage = discount;
        quantity = qty;
        expiryTime = expiry;
    }

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

    void setQuantity(int qty)
    {
        quantity = qty;
    }

    double getDiscountedPrice() const
    {
        return originalPrice * (1 - discountPercentage / 100.0);
    }

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

class FoodWasteSystem
{
private:
    vector<FoodItem> foodList;
    map<string,string> usersCredentials;
    map<string,string> userRoles;
    string currentLoggedInUser;
    string currentRole;
    string userFile = "users.txt";
    string foodFile = "food_items.txt";

public:
    FoodWasteSystem()
    {
        currentLoggedInUser = "";
        currentRole = "";
        loadUsersFromFile();
        loadFoodFromFile();
    }

    ~FoodWasteSystem()
    {
        saveUsersToFile();
        saveFoodToFile();
    }

    void loadUsersFromFile();
    void saveUsersToFile();
    void loadFoodFromFile();
    void saveFoodToFile();
    bool registerUser(string uname, string pass, string role);
    bool loginUser(string uname, string pass);
    void logout();
    string getCurrentUser() const;
    string getCurrentRole() const;
    vector<FoodItem>& getFoodList();
    void addFoodItem(string name, string restaurant, double price, double discount, int quantity, string expiry);
    void viewAvailableFood();
    void searchFood(string keyword);
    bool placeOrder(int foodId, int quantity);
    void generateWasteReport();
};

void FoodWasteSystem::loadUsersFromFile()
{
    ifstream file(userFile);
    if(!file.is_open())
    {
        return;
    }

    string uname;
    string pass;
    string role;

    while(file >> uname >> pass >> role)
    {
        usersCredentials[uname] = pass;
        userRoles[uname] = role;
    }
    file.close();
}

void FoodWasteSystem::saveUsersToFile()
{
    ofstream file(userFile);
    for(map<string,string>::iterator it = usersCredentials.begin(); it != usersCredentials.end(); it++)
    {
        file << it->first << " "
             << it->second << " "
             << userRoles[it->first]
             << endl;
    }
    file.close();
}

void FoodWasteSystem::loadFoodFromFile()
{
    ifstream file(foodFile);
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

    while(file >> id >> name >> restaurant >> price >> discount >> qty >> expiry)
    {
        foodList.push_back(FoodItem(id, name, restaurant, price, discount, qty, expiry));
    }
    file.close();
}

void FoodWasteSystem::saveFoodToFile()
{
    ofstream file(foodFile);
    for(size_t i = 0; i < foodList.size(); i++)
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
    file.close();
}

bool FoodWasteSystem::registerUser(string uname, string pass, string role)
{
    if(usersCredentials.find(uname) != usersCredentials.end())
    {
        cout << "Username already exists!" << endl;
        return false;
    }

    usersCredentials[uname] = pass;
    userRoles[uname] = role;
    saveUsersToFile();
    cout << "Registration Successful!" << endl;
    return true;
}

bool FoodWasteSystem::loginUser(string uname, string pass)
{
    if(usersCredentials.find(uname) != usersCredentials.end())
    {
        if(usersCredentials[uname] == pass)
        {
            currentLoggedInUser = uname;
            currentRole = userRoles[uname];
            return true;
        }
    }
    return false;
}

void FoodWasteSystem::logout()
{
    currentLoggedInUser = "";
    currentRole = "";
}

string FoodWasteSystem::getCurrentUser() const
{
    return currentLoggedInUser;
}

string FoodWasteSystem::getCurrentRole() const
{
    return currentRole;
}

vector<FoodItem>& FoodWasteSystem::getFoodList()
{
    return foodList;
}

void FoodWasteSystem::addFoodItem(string name, string restaurant, double price, double discount, int quantity, string expiry)
{
    int newId;
    if(foodList.empty())
    {
        newId = 1;
    }
    else
    {
        newId = foodList.back().getId() + 1;
    }

    foodList.push_back(FoodItem(newId, name, restaurant, price, discount, quantity, expiry));
    saveFoodToFile();
    cout << "Food Added Successfully!" << endl;
    cout << "Food ID : " << newId << endl;
}

void FoodWasteSystem::viewAvailableFood()
{
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

    for(size_t i = 0; i < foodList.size(); i++)
    {
        if(foodList[i].getQuantity() > 0)
        {
            foodList[i].displayFoodDetails();
        }
    }
    cout << "==============================================================================================\n";
}

void FoodWasteSystem::searchFood(string keyword)
{
    cout << "\n========== Search Result ==========\n";
    bool found = false;

    for(size_t i = 0; i < foodList.size(); i++)
    {
        if((foodList[i].getName().find(keyword) != string::npos ||
            foodList[i].getRestaurantName().find(keyword) != string::npos)
            && foodList[i].getQuantity() > 0)
        {
            foodList[i].displayFoodDetails();
            found = true;
        }
    }

    if(!found)
    {
        cout << "No Matching Food Found!" << endl;
    }
}

bool FoodWasteSystem::placeOrder(int foodId, int quantity)
{
    for(size_t i = 0; i < foodList.size(); i++)
    {
        if(foodList[i].getId() == foodId)
        {
            if(foodList[i].getQuantity() < quantity)
            {
                cout << "Not Enough Quantity Available!" << endl;
                return false;
            }

            int remainingQuantity = foodList[i].getQuantity() - quantity;
            foodList[i].setQuantity(remainingQuantity);
            saveFoodToFile();

            double totalPrice = foodList[i].getDiscountedPrice() * quantity;
            cout << "==================================" << endl;
            cout << "Order Placed Successfully!" << endl;
            cout << "Total Price : " << totalPrice << endl;
            cout << "==================================" << endl;
            return true;
        }
    }

    cout << "Food ID Not Found!" << endl;
    return false;
}

void FoodWasteSystem::generateWasteReport()
{
    cout << "\n==========================================" << endl;
    cout << "      FOOD WASTE REDUCTION REPORT" << endl;
    cout << "==========================================" << endl;

    int totalSoldOut = 0;
    int totalAvailable = 0;

    for(size_t i = 0; i < foodList.size(); i++)
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

    cout << "Sold Out Food Items : " << totalSoldOut << endl;
    cout << "Available Food Items : " << totalAvailable << endl;
    cout << "==========================================" << endl;
}

class Customer : public User
{
private:
    FoodWasteSystem &system;

public:
    Customer(string uname, string pass, FoodWasteSystem &sys)
        : User(uname, pass, "Customer"), system(sys) {}

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
                    system.viewAvailableFood();
                    break;

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
                    system.logout();
                    cout << "Logout Successful!" << endl;
                    break;

                default:
                    cout << "Invalid Choice!" << endl;
            }
        } while(choice != 4);
    }
};

class Restaurant : public User
{
private:
    FoodWasteSystem &system;

public:
    Restaurant(string uname, string pass, FoodWasteSystem &sys)
        : User(uname, pass, "Restaurant"), system(sys) {}

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

                    if(price < 0 || discount < 0 || discount > 100 || quantity <= 0)
                    {
                        cout << "Invalid Input!" << endl;
                    }
                    else
                    {
                        system.addFoodItem(foodName, username, price, discount, quantity, expiry);
                    }
                    break;
                }

                case 2:
                    system.viewAvailableFood();
                    break;

                case 3:
                    system.logout();
                    cout << "Logout Successful!" << endl;
                    break;

                default:
                    cout << "Invalid Choice!" << endl;
            }
        } while(choice != 3);
    }
};

class Admin : public User
{
private:
    FoodWasteSystem &system;

public:
    Admin(string uname, string pass, FoodWasteSystem &sys)
        : User(uname, pass, "Admin"), system(sys) {}

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
                    system.viewAvailableFood();
                    break;

                case 2:
                    system.generateWasteReport();
                    break;

                case 3:
                    system.logout();
                    cout << "Logout Successful!" << endl;
                    break;

                default:
                    cout << "Invalid Choice!" << endl;
            }
        } while(choice != 3);
    }
};

int main()
{
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

                system.registerUser(username, password, role);
                break;
            }

            case 2:
            {
                string username;
                string password;

                cout << "Username : ";
                cin >> username;
                cout << "Password : ";
                cin >> password;

                if(system.loginUser(username, password))
                {
                    User *activeUser = NULL;

                    if(system.getCurrentRole() == "Customer")
                    {
                        activeUser = new Customer(username, password, system);
                    }
                    else if(system.getCurrentRole() == "Restaurant")
                    {
                        activeUser = new Restaurant(username, password, system);
                    }
                    else if(system.getCurrentRole() == "Admin")
                    {
                        activeUser = new Admin(username, password, system);
                    }

                    if(activeUser != NULL)
                    {
                        activeUser->displayMenu();
                        delete activeUser;
                    }
                }
                else
                {
                    cout << "Invalid Username or Password!" << endl;
                }
                break;
            }

            case 3:
                cout << "Thank You!" << endl;
                break;

            default:
                cout << "Invalid Choice!" << endl;
        }
    } while(choice != 3);

    return 0;
}