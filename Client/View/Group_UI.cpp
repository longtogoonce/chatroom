#include <iostream>
#include <Group_UI.hpp>
using namespace std;

void Group_UI_initEntry()
{
    char choice;
    do{
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t************************ Group Management *************************" << endl;
        cout << "\t\t==================================================================="<< endl;
    /*
    
                先画个大饼--------此处显示群列表，加入的，管理的，创建的

    */
        cout << "" << endl;
        cout << "" << endl;
        cout << "[A]创建群聊" << endl;
        cout << "[M]管理群组" << endl;
        cout << "[C]群聊" << endl;
        cout << "" << endl;
        cout << "\t\t==================================================================="<< endl;

        cin >> choice;
        switch(choice)
        {
        case 'M':
        case 'm':
            Group_UI_MgtEntry();
            break;
        case 'C':
        case 'c':
            Group_UI_ChatEntry();
            break;
        case 'A':
        case 'a':
            Group_Srv_CreatGroup();
        }
    } while ('E'!= choice || 'e' != choice);
}


void Group_UI_MgtEntry()
{
    string name;
    cout << "\n\t\t请输入你想管理的群聊:" << endl;
    cin >> name;

    system("clear");
    cout << "\n\t\t===================================================================" << endl;
    cout << "\t\t************************ Group Management ************************" << endl;
    cout << "\t\t==================================================================="<< endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "\t\t==================================================================="<< endl;
}

void Group_UI_ChatEntry()
{

}
