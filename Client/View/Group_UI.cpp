#include <iostream>
#include <Group_UI.hpp>
#include <vector>
#include "../../Common/Account.hpp"
#include "../../Common/PutFormat.hpp"
#include "../Src/Group.hpp"
using namespace std;
extern Account Curuser;
extern PutFormat put;

void Group_UI_initEntry()
{
    vector<string> Group_Creat = Group_Srv_GetList(Curuser.getname()+"CG");     //我创建的群
    vector<string> Group_Manager = Group_Srv_GetList(Curuser.getname()+"MG");   //我管理的群
    vector<string> Group_Add = Group_Srv_GetList(Curuser.getname()+"AG");   //我加入的群

    char choice;
    do{
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t************************ Group Management *************************" << endl;
        cout << "\t\t==================================================================="<< endl;
        cout << "\t\t我创建的群:";
        for(auto& group:Group_Creat){
            cout << group + "";
        }
        cout << endl;
        cout << "\t\t我管理的群:";
         for(auto& group:Group_Manager){
            cout << group + "";
         }
        cout << endl;
        cout << "\t\t我加入的群:";
         for(auto& group:Group_Add){
            cout << group + "";
         }
        cout << endl;
        cout << "" << endl;
        cout << "\t\t-------------------------------------------------------------------" << endl;
        cout << "" << endl;
        cout << "\t\t[C]创建群聊" << endl;
        cout << "\t\t[A]加入群" << endl;
        cout << "\t\t[M]管理群组" << endl;
        cout << "\t\t[T]群聊" << endl;
        cout << "\t\t[E]退出" << endl;
        cout << "" << endl;
        cout << "\t\t==================================================================="<< endl;

        cin >> choice;
        switch(choice)
        {
        case 'M':
        case 'm':
            Group_UI_MgtEntry();
            break;
        case 'T':
        case 't':
            Group_UI_ChatEntry();
            break;
        case 'C':
        case 'c':
            Group_Srv_CreatGroup();
            break;
        case 'A':
        case 'a':
            Group_Srv_AddGroup();
            break;
        }
    } while ('E'!= choice || 'e' != choice);
}


void Group_UI_MgtEntry()
{
    char choice;
    string name;
    cout << "\n\t\t请输入你想管理的群聊:" << endl;
    cin >> name;
    vector<string> Group_Creat = Group_Srv_GetList(name);   //群的创建者
    vector<string> Group_Manager = Group_Srv_GetList(name+"MG");    //群的管理者
    vector<string> Group_Add = Group_Srv_GetList(name+"AG");    //群的成员
    Group_Srv_Getpermission(name);  //获得当前群的群身份
    do{
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t************************ Group Management ************************" << endl;
        cout << "\t\t==================================================================="<< endl;
        cout << "" << endl;
        put.printFrommid(name, yellow, B_empty, underscore);
        cout << "" << endl;
        cout << "群主： ";
        for(auto& group:Group_Manager)
            {
                cout << group + "";
            }
        cout << endl;
        cout << "群管理员: ";
        for(auto& group:Group_Creat){
            cout << group + "";
        }
        cout << endl;
        cout << "群成员： ";
        for(auto& group:Group_Manager){
            cout << group + "";
        }
        cout << endl;
        cout << "" << endl;
        cout << "\t\t-------------------------------------------------------------------" << endl;
        cout << "" << endl;
        cout << "\t\t[Q]查看申请" << endl;
        cout << "\t\t[D]删除用户" << endl;
        cout << "\t\t[A]添加管理者" << endl;
        cout << "\t\t[P]移除管理者" << endl;
        cout << "\t\t[S]退出该群" << endl;
        cout << "\t\t[E]返回" << endl;
        cout << "" << endl;
        cout << "\t\t==================================================================="<< endl;
        cin >> choice;
        switch(choice)
        {
            case 'Q':
            case 'q':
                Group_Srv_QueryApply(name);
                break;
            case 'D':
            case 'd':
                Group_Srv_Deluser(name);
                break;
            case 'A':
            case 'a':
                Group_Srv_AddManager(name);
                break;
            case 'P':
            case 'p':
                Group_Srv_DelManager(name);
                break;
            case 'S':
            case 's':
                Group_Srv_ExitGroup(name);
                break;
            }
    } while ('E' != choice || 'e' != choice);
}

void Group_UI_ChatEntry()
{

}
