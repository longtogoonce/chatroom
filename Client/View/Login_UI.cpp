#include <iostream>
#include "Login_UI.hpp"
#include "../Src/Login.hpp"
#include "Account.hpp"
#include <unistd.h>

using namespace std;
// 系统登陆
int SysLogin()
{
    char choice;
    do{
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t*********************** xiaotian's chatroom ***********************" << endl;
        cout << "\t\t==================================================================="<< endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "\t\t                   [L]ogin in      [F]ind password                 " << endl;
        cout << "" << endl;
        cout << "\t\t                   [S]ign  in      [E]xit                          " << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "\t\t==================================================================="<< endl;
        cin >> choice;
        switch(choice)
        {
        case 'L':
        case 'l':
            if(Login())
                return 1;
            break;
        case 'S':
        case 's':
            Sign();
            break;
        case 'F':
        case 'f':
            FindPasswd();
            break;
        }
    } while ('e' != choice || 'E' != choice);
}

int Login()
{
    string name;
    string passwd;

    system("clear");
    cout << "\n\t\t===================================================================" << endl;
    cout << "\t\t*********************** xiaotian's chatroom ***********************" << endl;
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
    cout << "\t\t请输入你的名字:";
    cin >> name;
    cout << "\n\t\t请输入你的密码:";
    cin >> passwd;
    if(Login_Srv_Verify(name,passwd)){
        return 1;
    }
    else
    {
        cout << "\n\t\t登陆失败" << endl;
        sleep(2);
        return 0;
    }
}

void Sign()
{
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t*********************** xiaotian's chatroom ***********************" << endl;
        cout << "\t\t===================================================================" << endl;
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
        Account user;
        if (Login_Srv_Add(user))
        {
        cout << "\n\t\t恭喜你,账户创建成功,您的UID为:" << user.getUID() << endl;
        }
        else
        {
        cout << "\n\t\t账户创建失败!" << endl;
        }

        cout << "\t\t按任意键退出……" << endl;
        system("pause");
}

void FindPassword()
{
        string name;
        string myitbo;
        string passwd;

        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t*********************** xiaotian's chatroom ***********************" << endl;
        cout << "\t\t===================================================================" << endl;
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
        cout << "\t\t===================================================================" << endl;
        cout << "\t\t请输入用户名:";
        cin >> name;
        cout << "\t\t请输入密保:";
        cin >> myitbo;
        if(Login_Srv_FindPasswd(name,myitbo,passwd))
        {
        cout << "\n\t\t你的密码是:" << passwd << endl;
        }else
        {
        cout << "\n\t\t验证失败" << endl;
        }

        cout << "\t\t按任意键退出……" << endl;
        system("pause");
}