#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <stdexcept>

using namespace std;

struct Transaction
{
    string timestamp;
    string action;
    string details;
};

class Coin
{
protected:
    static double coinPrices[20];
    string cryptoNames[20] = {"Bitcoin", "Ethereum", "Binance Coin", "Cardano", "Solana", "XRP", "Polkadot", "Dogecoin", "Avalanche", "Litecoin", "Chainlink", "Bitcoin Cash", "Algorand", "Cosmos", "Stellar", "VeChain", "TRON", "Theta", "Ethereum Classic", "EOS"};

public:
    void printMarketStatistics() const
    {
        srand(time(0));

        cout << "\033[97m\033[1mAvailable Coins Information:\033[0m" << endl;
        for (int i = 0; i < 20; ++i)
        {
            coinPrices[i] = (rand() % 500 + 1);
            cout << i + 1 << ". " << cryptoNames[i] << ": $" << coinPrices[i] << endl;
        }
    }
};

double Coin::coinPrices[20];

class User : public Coin
{
private:
    string userName;
    string password;
    double currentWalletAmount;
    vector<Transaction> transactionHistory;
    vector<string> currentCoinsWallet;

    friend void printUserDetails(const User &user);

public:
    void setName()
    {
        cout << "Enter your user name: " << endl;
        cin.ignore();
        getline(cin, userName);
    }

    string getName() const
    {
        return userName;
    }

    void setPassword()
    {
        string pass2;
        do
        {
            cout << "Enter your password: " << endl;
            cin.ignore();
            getline(cin, password);

            if (password.length() < 8)
            {
                cout << "\033[31m\033[1mYour password is too weak!!\nPlease set it again!\033[0m" << endl;
                continue;
            }
            else if (password.length() > 24)
            {
                cout << "\033[31m\033[1mToo many characters for password!!\nPlease set it again!\033[0m" << endl;
                continue;
            }
            else if (password.length() >= 8 && password.length() <= 16)
            {
                cout << "\033[33m\033[1mYour password strength is medium\033[0m" << endl;
            }
            else if (password.length() >= 17 && password.length() <= 24)
            {
                cout << "\033[32m\033[1mYour password strength is high\033[0m" << endl;
            }

            cout << "Re-enter password: " << endl;
            cin.ignore();
            getline(cin, pass2);

            if (pass2 != password)
            {
                cout << "\033[31m\033[1mPlease re-enter the correct password\033[0m" << endl;
                continue;
            }
            else
            {
                cout << "\033[32m\033[1mPassword set successfully!\033[0m" << endl;
                break;
            }
        } while (true);
    }

    string getPassword() const
    {
        return password;
    }

    void setWalletAmount()
    {
        cout << "Enter your current wallet amount: " << endl;
        string input;
        cin >> input;
        try
        {
            currentWalletAmount = stod(input);
            if (currentWalletAmount <= 0)
            {
                throw invalid_argument("Invalid amount");
            }
        }
        catch (const invalid_argument &e)
        {
            cout << "\033[31m\033[1mInvalid amount!\033[0m" << endl;
            setWalletAmount();
        }
    }

    double getWalletAmount() const
    {
        return currentWalletAmount;
    }

    void createAccount()
    {
        setName();
        setPassword();
        setWalletAmount();
    }

    void recordAction(const string &action, const string &coinName, double coinPrice)
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        string timestamp = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + " " + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
        transactionHistory.push_back({timestamp, action, coinName + ": $" + to_string(coinPrice)});

        try
        {
            ofstream outfile("user_actions.txt", ios_base::app);
            if (outfile.is_open())
            {
                outfile << "User: " << userName << endl;
                outfile << "Timestamp: " << timestamp << endl;
                outfile << "Action: " << action << endl;
                outfile << "Coin: " << coinName << endl;
                outfile << "Price: $" << coinPrice << endl;
                outfile << endl;
                outfile.close();
            }
            else
            {
                throw runtime_error("Unable to open file");
            }
        }
        catch (const exception &e)
        {
            cerr << "\033[31m\033[1mError: \033[0m" << e.what() << endl;
        }
    }

    void displayTransactionHistory() const
    {
        if (transactionHistory.size() <= 0)
        {
            cout << "\033[31m\033[1mNo Transaction History yet!!\033[0m" << endl;
        }
        else
        {
            cout << "\033[97m\033[1mTransaction History:\033[0m" << endl;
            for (int i = 0; i < transactionHistory.size(); ++i)
            {
                const Transaction &transaction = transactionHistory[i];
                cout << "User: " << userName << endl;
                cout << "Timestamp: " << transaction.timestamp << endl;
                cout << "Action: " << transaction.action << endl;
                cout << "Details: " << transaction.details << endl;
                cout << endl;
            }
        }
    }

    void displayUserInfo() const
    {
        printUserDetails(*this);
    }

    void purchaseCoins()
    {
        printMarketStatistics();

        try
        {
            int choice;
            cout << "Enter the number corresponding to your desired coin (1 to 20): ";
            cin >> choice;

            if (choice < 1 || choice > 20)
            {
                throw out_of_range("\033[31m\033[1mInvalid choice\033[0m");
            }

            double price = coinPrices[choice - 1];
            if (price > getWalletAmount())
            {
                throw runtime_error("\033[31m\033[1mYou do not have enough balance!\033[0m");
            }

            cout << "You chose: " << cryptoNames[choice - 1] << " at the price: " << price << endl;
            currentWalletAmount -= price;
            cout << "Amount subtracted from wallet balance: $" << price << endl;
            cout << "Wallet after the transaction: " << getWalletAmount() << endl;
            recordAction("Buy Coin", cryptoNames[choice - 1], price);
            currentCoinsWallet.push_back(to_string(choice));
        }
        catch (const exception &e)
        {
            cerr << "\033[31m\033[1mError: " << e.what() << "\033[0m" << endl;
        }
    }

    void sellCoins()
    {
        try
        {
            if (currentCoinsWallet.empty())
            {
                throw runtime_error("\033[31m\033[1mYou do not have any coins to sell. Please purchase coins first.\033[0m");
            }

            cout << "\033[97m\033[1mCoins in your wallet:\033[0m" << endl;
            for (int i = 0; i < currentCoinsWallet.size(); ++i)
            {
                int index = stoi(currentCoinsWallet[i]) - 1;
                cout << i + 1 << ". " << cryptoNames[index] << endl;
            }

            int choice;
            cout << "Enter the number corresponding to the coin you want to sell (1 to " << currentCoinsWallet.size() << "): ";
            cin >> choice;

            if (choice < 1 || choice > currentCoinsWallet.size())
            {
                throw out_of_range("\033[31m\033[1mInvalid choice\033[0m");
            }

            int index = stoi(currentCoinsWallet[choice - 1]) - 1;
            double price = coinPrices[index];

            srand(time(0));
            int profitOrLoss = rand() % 2;

            double amountChange = (rand() % 101) / 100.0 * price;

            if (profitOrLoss == 0)
            {
                cout << "\033[31m\033[1mYou may incur a loss from selling " << cryptoNames[index] << ". Amount will decrease by $" << amountChange << "\033[0m" << endl;
            }
            else
            {
                cout << "\033[32m\033[1mYou may make a profit from selling " << cryptoNames[index] << ". Amount will increase by $" << amountChange << "\033[0m" << endl;
            }

            char proceed;
            cout << "Do you still want to proceed with selling? (y/n): ";
            cin >> proceed;

            if (proceed == 'y' || proceed == 'Y')
            {
                double price_change;
                if (profitOrLoss == 0)
                {
                    price_change = price - amountChange;
                    currentWalletAmount += price_change;
                }
                else
                {
                    price_change = price + amountChange;
                    currentWalletAmount += price_change;
                }
                cout << "Amount added/removed from wallet balance: $" << amountChange << endl;
                cout << "\033[97m\033[1mWallet after the transaction: $" << getWalletAmount() << "\033[0m" << endl;
                recordAction("Sell Coin", cryptoNames[index], price);
                currentCoinsWallet.erase(currentCoinsWallet.begin() + choice - 1);
            }
            else
            {
                cout << "\033[31m\033[1mSale canceled. No changes made to wallet.\033[0m" << endl;
            }
        }
        catch (const exception &e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }
    void printCurrentCoins() const
    {
        if (currentCoinsWallet.size() <= 0)
        {
            cout << "\033[31m\033[1mYou have zero coins!!\033[0m" << endl;
        }
        else
        {
            cout << "\033[97m\033[1mCurrent Coins in Wallet:\033[0m" << endl;
            for (const auto &coin : currentCoinsWallet)
            {
                try
                {
                    int index = stoi(coin) - 1;
                    if (index < 0 || index >= 20)
                    {
                        throw out_of_range("Invalid coin index");
                    }
                    cout << cryptoNames[index] << endl;
                }
                catch (const exception &e)
                {
                    cerr << "Error: " << e.what() << endl;
                }
            }
        }
    }
};

void printUserDetails(const User &user)
{
    cout << "\033[97m\033[1mUser Information:\033[0m" << endl;
    cout << "Name: " << user.userName << endl;
    cout << "Password: ";
    for (int i = 0; i < user.password.length(); i++)
    {
        cout << "*";
    }
    cout << endl;
    cout << "Current Wallet Amount: $" << user.currentWalletAmount << endl;
}

int main()
{
    int n, userOption;
    cout << "Enter total number of users: " << endl;
    cin >> n;
    if (n <= 0)
    {
        cout << "\033[31m\033[1mNumber of users is zero(0)" << endl
             << "Therefore exiting the program...\033[0m";
        exit(0);
    }

    vector<User> users(n);

    for (int i = 0; i < n; ++i)
    {
        cout << "Enter the information of user " << i + 1 << endl;
        users[i].createAccount();
    }

    for (int i = 0; i < n; ++i)
    {
        while (true)
        {
            cout << "\033[97m\033[1m======= " << users[i].getName() << " Welcome to Krypro ======\033[0m" << endl;
            cout << "\033[97m\033[1m====================================\033[0m" << endl;

            cout << "\033[97m\033[1m=============== Menu ===============\033[0m" << endl;

            cout << "\033[32m\033[1m1. Print exchange stats" << endl;
            cout << "\033[33m\033[1m2: Sell Coin" << endl;
            cout << "\033[34m\033[1m3: Purchase Coin" << endl;
            cout << "\033[35m\033[1m4: Print User Details" << endl;
            cout << "\033[97m\033[1m5: Print Transaction History" << endl;
            cout << "\033[36m\033[1m6: Print Current Coins" << endl;
            cout << "\033[31m\033[1m7: Exit\033[0m" << endl;
            cout << "\033[97m\033[1m====================================\033[0m" << endl;

            cout << "Choose any option: ";
            cin >> userOption;

            if (cin.fail())
            {
                cerr << "Invalid input" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (userOption >= 1 && userOption <= 7)
            {
                cout << "\033[97m\033[1mYou chose: " << userOption << "\033[0m" << endl;
                cout << "\033[97m\033[1m======================================\033[0m" << endl;
            }
            else
            {
                cout << "\033[31m\033[1mYou chose: \" " << userOption << " \" which is invalid input!!\033[0m" << endl;
                continue;
            }

            switch (userOption)
            {
            case 1:
                users[i].printMarketStatistics();
                break;
            case 2:
                users[i].sellCoins();
                break;
            case 3:
                users[i].purchaseCoins();
                break;
            case 4:
                users[i].displayUserInfo();
                break;
            case 5:
                users[i].displayTransactionHistory();
                break;
            case 6:
                users[i].printCurrentCoins();
                break;
            case 7:
                cout << "\033[31m\033[1mGood Bye " << users[i].getName() << "!!\033[0m" << endl;
                cout << "\033[97m\033[1m======================================\033[0m" << endl;

                break;
            default:
                cout << "\033[31m\033[1mInvalid input" << endl;
                break;
            }

            if (userOption == 7)
            {
                break;
            }
        }
    }

    return 0;
}