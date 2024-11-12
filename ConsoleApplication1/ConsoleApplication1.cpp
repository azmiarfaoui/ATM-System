#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";

void ShowMainMenue();
void ShowQuickWithdrawScreen();
void Login();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};
sClient CurrentClient;
vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}
vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}
void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);


    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount, vClients);

}
void ShowNormalWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";


    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    double Amount = 0;
    do
    {
        cout << "\nPlease enter withdraw amount (multiple of 5) : ";
        cin >> Amount;
    }while((int)Amount % 5 != 0);

    //Validate that the amount does not exceeds the balance
    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients);

}
enum enMainMenueOptions { eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogOut = 5 };

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}
short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short ReadQuickWithdrawOption()
{
    cout << "Choose what do you want to do? [1 to 9]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short QuickWithdrawAmount(short Option)
{
    switch (Option)
    {
    case 1 :
        return 20;
        break;
    case 2 :
        return 50;
        break;
    case 3:
        return 100;
        break;
    case 4:
        return 200;
        break;
    case 5:
        return 400;
        break;
    case 6:
        return 600;
        break;
    case 7:
        return 800;
        break;
    case 8:
        return 1000;
        break;
    default:
        break;
    }
}
void PerfromQuickWithdrawOption(short Option)
{
    if (Option == 9)
        return;

    double Amount = QuickWithdrawAmount(Option);

    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\tThe amount exceeds your balance, make another choice.\n";
        cout << "Press any key to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients);
    CurrentClient.AccountBalance -= Amount;
}
void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t [2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}
bool ShowBalanceScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tBalance Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient& C : vClients)
    {
        if (C.AccountNumber == CurrentClient.AccountNumber)
        {
            cout << "\n\nYour balance is: " << C.AccountBalance;

            return true;
        }

    }
    return false;

}
void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eNormalWithdraw:
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eCheckBalance:
        system("cls");
        ShowBalanceScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eLogOut:
        system("cls");
        Login();
        break;
    }

}
void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] LogOut.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}
bool LoadClientInfo(string AccountNumber, string PinCode)
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    if (FindClientByAccountNumber(AccountNumber, vClients, CurrentClient))
        return true;
    else
        return false;
}
void Login()
{
    bool LoginFailed = false;
    string AccountNumber, PinCode;
    do
    {
        system("cls");
        cout << "===========================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "===========================================\n";
        if (LoginFailed)
        {
            cout << "Invalid AccountNumber/PinCode!\n";
        }
        cout << "Please enter Account Number : " << endl;
        cin >> AccountNumber;
        cout << "Please enter PinCode : " << endl;
        cin >> PinCode;
        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);
    } while (LoginFailed);

    ShowMainMenue();
}

int main()

{
    Login();
    system("pause>0");

    return 0;
}