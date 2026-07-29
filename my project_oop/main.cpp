#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// SHA-256 Password Hashing Algorithm
string sha256(const string str) {
    unsigned int h0 = 0x6a09e667, h1 = 0xbb67ae85, h2 = 0x3c6ef372, h3 = 0xa54ff53a;
    unsigned int h4 = 0x510e527f, h5 = 0x9b05688c, h6 = 0x1f83d9ab, h7 = 0x5be0cd19;
    
    unsigned long long totalLen = str.length() * 8;
    string padded = str;
    padded += (char)0x80;
    while ((padded.length() * 8) % 512 != 448) {
        padded += (char)0x00;
    }
    for (int i = 7; i >= 0; --i) {
        padded += (char)((totalLen >> (i * 8)) & 0xFF);
    }
    
    for (size_t chunk = 0; chunk < padded.length() / 64; ++chunk) {
        unsigned int w[64] = {0};
        for (int i = 0; i < 16; ++i) {
            w[i] = ((unsigned char)padded[chunk * 64 + i * 4] << 24) |
                   ((unsigned char)padded[chunk * 64 + i * 4 + 1] << 16) |
                   ((unsigned char)padded[chunk * 64 + i * 4 + 2] << 8) |
                   ((unsigned char)padded[chunk * 64 + i * 4 + 3]);
        }
        for (int i = 16; i < 64; ++i) {
            unsigned int s0 = ((w[i-15] >> 7) | (w[i-15] << 25)) ^ ((w[i-15] >> 18) | (w[i-15] << 14)) ^ (w[i-15] >> 3);
            unsigned int s1 = ((w[i-2] >> 17) | (w[i-2] << 15)) ^ ((w[i-2] >> 19) | (w[i-2] << 13)) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }
        unsigned int a = h0, b = h1, c = h2, d = h3, e = h4, f = h5, g = h6, h = h7;
        unsigned int k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };
        for (int i = 0; i < 64; ++i) {
            unsigned int S1 = ((e >> 6) | (e << 26)) ^ ((e >> 11) | (e << 21)) ^ ((e >> 25) | (e << 7));
            unsigned int ch = (e & f) ^ (~e & g);
            unsigned int temp1 = h + S1 + ch + k[i] + w[i];
            unsigned int S0 = ((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10));
            unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
            unsigned int temp2 = S0 + maj;
            h = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
        }
        h0 += a; h1 += b; h2 += c; h3 += d; h4 += e; h5 += f; h6 += g; h7 += h;
    }
    
    stringstream ss;
    ss << setfill('0') << setw(8) << hex << h0
       << setfill('0') << setw(8) << hex << h1
       << setfill('0') << setw(8) << hex << h2
       << setfill('0') << setw(8) << hex << h3
       << setfill('0') << setw(8) << hex << h4
       << setfill('0') << setw(8) << hex << h5
       << setfill('0') << setw(8) << hex << h6
       << setfill('0') << setw(8) << hex << h7;
    return ss.str();
}

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

    string getUsername() const { return username; }
    string getRole() const { return role; }
    bool checkPassword(string pass) const { return password == pass; }

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
    bool isCharity; // Flag to identify charity food

public:
    FoodItem()
    {
        id = 0;
        originalPrice = 0;
        discountPercentage = 0;
        quantity = 0;
        isCharity = false;
    }

    FoodItem(int fId, string fName, string rName, double price, double discount, int qty, string expiry, bool charity = false)
    {
        id = fId;
        name = fName;
        restaurantName = rName;
        originalPrice = price;
        discountPercentage = discount;
        quantity = qty;
        expiryTime = expiry;
        isCharity = charity;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    string getRestaurantName() const { return restaurantName; }
    double getOriginalPrice() const { return originalPrice; }
    double getDiscountPercentage() const { return discountPercentage; }
    int getQuantity() const { return quantity; }
    string getExpiryTime() const { return expiryTime; }
    bool getIsCharity() const { return isCharity; }

    void setQuantity(int qty) { quantity = qty; }

    double getDiscountedPrice() const
    {
        if (isCharity) return 0.0; // Charity items are free
        return originalPrice * (1.0 - discountPercentage / 100.0);
    }

    void displayFoodDetails() const
    {
        cout << left
             << setw(5) << id
             << setw(20) << name
             << setw(18) << restaurantName
             << "$ " << setw(8) << fixed << setprecision(2) << originalPrice
             << setw(10) << (to_string((int)discountPercentage) + "%")
             << "$ " << setw(10) << getDiscountedPrice()
             << setw(8) << quantity
             << setw(10) << expiryTime
             << setw(10) << (isCharity ? "CHARITY" : "REGULAR")
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
    double adminEarnings; // Tracks admin balance from commissions
    
    string userFile = "users.txt";
    string foodFile = "food_items.txt";
    string adminFile = "admin_data.txt";

public:
    FoodWasteSystem()
    {
        currentLoggedInUser = "";
        currentRole = "";
        adminEarnings = 0.0;
        loadUsersFromFile();
        loadFoodFromFile();
        loadAdminDataFromFile();
    }

    ~FoodWasteSystem()
    {
        saveUsersToFile();
        saveFoodToFile();
        saveAdminDataToFile();
    }

    void loadUsersFromFile();
    void saveUsersToFile();
    void loadFoodFromFile();
    void saveFoodToFile();
    void loadAdminDataFromFile();
    void saveAdminDataToFile();
    
    bool registerUser(string uname, string pass, string role);
    bool loginUser(string uname, string pass);
    void logout();
    
    string getCurrentUser() const { return currentLoggedInUser; }
    string getCurrentRole() const { return currentRole; }
    double getAdminEarnings() const { return adminEarnings; }
    vector<FoodItem>& getFoodList() { return foodList; }

    void addFoodItem(string name, string restaurant, double price, double discount, int quantity, string expiry, bool isCharity = false);
    void viewAvailableFood(bool charityOnly = false);
    void searchFood(string keyword);
    bool placeOrder(int foodId, int quantity);
    void generateWasteReport();
};

void FoodWasteSystem::loadUsersFromFile()
{
    ifstream file(userFile);
    if(!file.is_open()) return;

    string uname, pass, role;
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
    for(auto it = usersCredentials.begin(); it != usersCredentials.end(); ++it)
    {
        file << it->first << " " << it->second << " " << userRoles[it->first] << endl;
    }
    file.close();
}

void FoodWasteSystem::loadFoodFromFile()
{
    ifstream file(foodFile);
    if(!file.is_open()) return;

    int id, qty;
    string name, restaurant, expiry;
    double price, discount;
    bool isCharity;

    while(file >> id >> name >> restaurant >> price >> discount >> qty >> expiry >> isCharity)
    {
        foodList.push_back(FoodItem(id, name, restaurant, price, discount, qty, expiry, isCharity));
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
             << foodList[i].getExpiryTime() << " "
             << foodList[i].getIsCharity()
             << endl;
    }
    file.close();
}

void FoodWasteSystem::loadAdminDataFromFile()
{
    ifstream file(adminFile);
    if(file.is_open())
    {
        file >> adminEarnings;
        file.close();
    }
}

void FoodWasteSystem::saveAdminDataToFile()
{
    ofstream file(adminFile);
    if(file.is_open())
    {
        file << adminEarnings;
        file.close();
    }
}

bool FoodWasteSystem::registerUser(string uname, string pass, string role)
{
    if(usersCredentials.find(uname) != usersCredentials.end())
    {
        cout << "Username already exists!" << endl;
        return false;
    }

    usersCredentials[uname] = sha256(pass);
    userRoles[uname] = role;
    saveUsersToFile();
    cout << "Registration Successful!" << endl;
    return true;
}

bool FoodWasteSystem::loginUser(string uname, string pass)
{
    if(usersCredentials.find(uname) != usersCredentials.end())
    {
        if(usersCredentials[uname] == sha256(pass))
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

void FoodWasteSystem::addFoodItem(string name, string restaurant, double price, double discount, int quantity, string expiry, bool isCharity)
{
    int newId = foodList.empty() ? 1 : foodList.back().getId() + 1;
    
    if (isCharity) {
        price = 0.0;
        discount = 0.0;
    }

    foodList.push_back(FoodItem(newId, name, restaurant, price, discount, quantity, expiry, isCharity));
    saveFoodToFile();
    cout << "\n----------------------------------------" << endl;
    cout << (isCharity ? "Charity Food Donated Successfully!" : "Food Item Added Successfully!") << endl;
    cout << "Food ID : " << newId << endl;
    cout << "----------------------------------------" << endl;
}

void FoodWasteSystem::viewAvailableFood(bool charityOnly)
{
    if(foodList.empty())
    {
        cout << "No Food Items Available!" << endl;
        return;
    }

    cout << "\n====================================================================================================\n";
    cout << left
         << setw(5)  << "ID"
         << setw(20) << "Food"
         << setw(18) << "Restaurant"
         << setw(10) << "Price"
         << setw(10) << "Discount"
         << setw(12) << "Final Price"
         << setw(8)  << "Qty"
         << setw(10) << "Expiry"
         << setw(10) << "Type"
         << endl;
    cout << "====================================================================================================\n";

    bool foundAny = false;
    for(size_t i = 0; i < foodList.size(); i++)
    {
        if(foodList[i].getQuantity() > 0)
        {
            if(!charityOnly || (charityOnly && foodList[i].getIsCharity()))
            {
                foodList[i].displayFoodDetails();
                foundAny = true;
            }
        }
    }
    
    if(!foundAny)
    {
        cout << "No items match your criteria!" << endl;
    }
    cout << "====================================================================================================\n";
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

            double totalPrice = foodList[i].getDiscountedPrice() * quantity;
            
            // Admin Commission System
            if (!foodList[i].getIsCharity()) {
                double commission = totalPrice * 0.05; // 5% platform charge for regular items
                adminEarnings += commission;
                saveAdminDataToFile();
            }

            saveFoodToFile();

            cout << "\n==================================" << endl;
            if(foodList[i].getIsCharity()) {
                cout << "Charity Order Claimed Successfully!" << endl;
                cout << "Total Price : $ 0.00 (Free Donation)" << endl;
            } else {
                cout << "Order Placed Successfully!" << endl;
                cout << "Total Amount : $ " << fixed << setprecision(2) << totalPrice << endl;
            }
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
    cout << "     FOOD WASTE REDUCTION REPORT" << endl;
    cout << "==========================================" << endl;

    int totalSoldOut = 0;
    int totalAvailable = 0;
    int totalCharityItems = 0;

    for(size_t i = 0; i < foodList.size(); i++)
    {
        if(foodList[i].getQuantity() == 0) totalSoldOut++;
        else totalAvailable++;

        if(foodList[i].getIsCharity()) totalCharityItems++;
    }

    cout << "Sold Out / Claimed Items : " << totalSoldOut << endl;
    cout << "Available Food Items    : " << totalAvailable << endl;
    cout << "Total Charity Donations  : " << totalCharityItems << endl;
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
            cout << "1. View All Available Food\n";
            cout << "2. View Free Charity Food Only\n";
            cout << "3. Search Food\n";
            cout << "4. Place Order / Claim Charity Food\n";
            cout << "5. Logout\n";
            cout << "Enter Choice : ";
            cin >> choice;

            switch(choice)
            {
                case 1:
                    system.viewAvailableFood(false);
                    break;

                case 2:
                    system.viewAvailableFood(true);
                    break;

                case 3:
                {
                    string keyword;
                    cout << "Enter Food/Restaurant Name : ";
                    cin >> keyword;
                    system.searchFood(keyword);
                    break;
                }

                case 4:
                {
                    int id, quantity;
                    cout << "Enter Food ID : ";
                    cin >> id;
                    cout << "Enter Quantity : ";
                    cin >> quantity;
                    system.placeOrder(id, quantity);
                    break;
                }

                case 5:
                    system.logout();
                    cout << "Logout Successful!" << endl;
                    break;

                default:
                    cout << "Invalid Choice!" << endl;
            }
        } while(choice != 5);
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
            cout << "1. Add Regular Food Item\n";
            cout << "2. Donate Food to Charity (Free 0% Commission)\n";
            cout << "3. View My / All Food Items\n";
            cout << "4. Logout\n";
            cout << "Enter Choice : ";
            cin >> choice;

            switch(choice)
            {
                case 1:
                {
                    string foodName, expiry;
                    double price, discount;
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
                        cout << "Invalid Input Parameters!" << endl;
                    }
                    else
                    {
                        system.addFoodItem(foodName, username, price, discount, quantity, expiry, false);
                    }
                    break;
                }

                case 2:
                {
                    string foodName, expiry;
                    int quantity;

                    cout << "\n--- DONATE FOOD TO CHARITY ---\n";
                    cout << "Food Name : ";
                    cin >> foodName;
                    cout << "Quantity to Donate : ";
                    cin >> quantity;
                    cout << "Expiry Time : ";
                    cin >> expiry;

                    if(quantity <= 0)
                    {
                        cout << "Invalid Quantity!" << endl;
                    }
                    else
                    {
                        // Price = 0, Discount = 0, isCharity = true
                        system.addFoodItem(foodName, username, 0.0, 0.0, quantity, expiry, true);
                    }
                    break;
                }

                case 3:
                    system.viewAvailableFood();
                    break;

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
            cout << "1. View All Food Items\n";
            cout << "2. Generate Waste Reduction Report\n";
            cout << "3. View System Earnings & Balance\n";
            cout << "4. Logout\n";
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
                    cout << "\n==========================================" << endl;
                    cout << "         ADMIN FINANCIAL DASHBOARD" << endl;
                    cout << "==========================================" << endl;
                    cout << "Total Platform Earnings (5% Commission): $ " 
                         << fixed << setprecision(2) << system.getAdminEarnings() << endl;
                    cout << "Note: Charity orders generate 0% commission." << endl;
                    cout << "==========================================" << endl;
                    break;

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

int main()
{
    FoodWasteSystem system;
    int choice;

    cout << "==========================================" << endl;
    cout << "     FOOD WASTE REDUCTION SYSTEM" << endl;
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
                string username, password;
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
                if(roleChoice == 1) role = "Customer";
                else if(roleChoice == 2) role = "Restaurant";
                else role = "Admin";

                system.registerUser(username, password, role);
                break;
            }

            case 2:
            {
                string username, password;

                cout << "Username : ";
                cin >> username;
                cout << "Password : ";
                cin >> password;

                if(system.loginUser(username, password))
                {
                    User *activeUser = nullptr;

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

                    if(activeUser != nullptr)
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
