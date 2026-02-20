#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct MenuItem
{
    int id;
    string name;
    double price;
    int satisfaction;
};

vector<MenuItem> menu =
{
    {1, "Burger", 250.0, 70},
    {2, "Pizza", 550.0, 90},
    {3, "Pasta", 300.0, 60},
    {4, "Cold Coffee", 120.0, 60},
    {5, "Fries", 150.0, 60},
    {6, "Steak", 700.0, 100},
    {7, "Noodles", 220.0, 55}
};

void merge(vector<MenuItem>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1, n2 = right - mid;
    vector<MenuItem> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i].price <= R[j].price) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<MenuItem>& arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void getMaxItemsGreedy(const vector<MenuItem>& items, double budget)
{
    vector<MenuItem> sortedItems = items;
    sort(sortedItems.begin(), sortedItems.end(), [](const MenuItem& a, const MenuItem& b)
    {
        return a.price < b.price;
    });

    double total = 0.0;
    cout << "\nItems you can buy within Tk " << budget << ":\n";
    for (const auto& item : sortedItems)
    {
        if (total + item.price <= budget)
        {
            total += item.price;
            cout << fixed << setprecision(2);
            cout << "- " << item.name << " (Tk " << item.price << ")\n";
        }
    }
    cout << "Total Spent: Tk " << total << endl;
}

int knapsackDP(const vector<MenuItem>& items, int budget)
{
    int n = items.size();
    vector<vector<int>> dp(n + 1, vector<int>(budget + 1, 0));
    for (int i = 1; i <= n; i++)
    {
        for (int w = 0; w <= budget; w++)
        {
            if ((int)items[i - 1].price <= w)
                dp[i][w] = max(dp[i - 1][w], items[i - 1].satisfaction + dp[i - 1][w - (int)items[i - 1].price]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }
    return dp[n][budget];
}

int binarySearchByName(const vector<MenuItem>& sortedMenu, const string& key)
{
    int low = 0, high = sortedMenu.size() - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (sortedMenu[mid].name == key)
            return mid;
        else if (sortedMenu[mid].name < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

void showMenu(const vector<MenuItem>& items)
{
    cout << "\n----- MENU -----\n";
    cout << left << setw(5) << "ID" << setw(20) << "Item" << setw(10) << "Price (Tk)" << endl;
    for (const auto& item : items)
    {
        cout << left << setw(5) << item.id << setw(20) << item.name << setw(10) << item.price << endl;
    }
}

bool adminLogin()
{
    string password;
    cout << "\nEnter Admin Password: ";
    cin >> password;
    return password == "123";
}

void adminPanel()
{
    if (!adminLogin())
    {
        cout << "Access Denied! Incorrect Password.\n";
        return;
    }

    cout << "\n--- Welcome to Admin Panel ---\n";
    int id = menu.size() + 1;
    string name;
    double price;
    int satisfaction;

    cout << "Enter new item name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter price: ";
    cin >> price;
    cout << "Enter satisfaction value (1-100): ";
    cin >> satisfaction;

    menu.push_back({id, name, price, satisfaction});
    cout << "Item added successfully!\n";
}

void takeOrder()
{
    vector<MenuItem> orderList;
    int itemId;
    char more = 'y';
    while (more == 'y' || more == 'Y')
    {
        showMenu(menu);
        cout << "Enter Item ID to order: ";
        cin >> itemId;

        auto it = find_if(menu.begin(), menu.end(), [itemId](const MenuItem& item)
        {
            return item.id == itemId;
        });

        if (it != menu.end())
        {
            orderList.push_back(*it);
            cout << it->name << " added to your order.\n";
        }
        else
        {
            cout << "Invalid ID. Try again.\n";
        }

        cout << "Do you want to add more items? (y/n): ";
        cin >> more;
    }

    double total = 0;
    cout << "\n--- Your Order ---\n";
    for (const auto& item : orderList)
    {
        cout << "- " << item.name << " (Tk " << item.price << ")\n";
        total += item.price;
    }
    cout << "Total Amount: Tk " << fixed << setprecision(2) << total << endl;
}

int main()
{
    int choice;
    double budget;
    do
    {
        cout << "\n===== Mama's Restaurant =====\n";
        cout << "1. Admin Panel\n";
        cout << "2. Show Menu\n";
        cout << "3. Sort Menu by Price\n";
        cout << "4. Get Best Items under Budget\n";
        cout << "5. Max Satisfaction under Budget\n";
        cout << "6. Search Item by Name\n";
        cout << "7. Place an Order\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            adminPanel();
            break;
        case 2:
            showMenu(menu);
            break;
        case 3:
            mergeSort(menu, 0, menu.size() - 1);
            cout << "\nMenu Sorted by Price:\n";
            showMenu(menu);
            break;
        case 4:
            cout << "Enter your budget: ";
            cin >> budget;
            getMaxItemsGreedy(menu, budget);
            break;
        case 5:
            cout << "Enter your budget: ";
            cin >> budget;
            cout << "Max Satisfaction you can achieve: " << knapsackDP(menu, (int)budget) << endl;
            break;
        case 6:
        {
            string itemName;
            cout << "Enter item name to search: ";
            cin.ignore();
            getline(cin, itemName);
            vector<MenuItem> sortedMenu = menu;
            sort(sortedMenu.begin(), sortedMenu.end(), [](const MenuItem& a, const MenuItem& b)
            {
                return a.name < b.name;
            });
            int index = binarySearchByName(sortedMenu, itemName);
            if (index != -1)
            {
                cout << "Item Found: " << sortedMenu[index].name << ", Tk " << sortedMenu[index].price << endl;
            }
            else
            {
                cout << "Item not found!\n";
            }
            break;
        }
        case 7:
            takeOrder();
            break;
        case 8:
            cout << "\nThank you for using Mama's Restaurant System!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    while (choice != 8);
    return 0;
}
