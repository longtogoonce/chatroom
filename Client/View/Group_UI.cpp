#include <iostream>
#include "Group_UI.hpp"
#include <vector>
#include "../../Common/Account.hpp"
#include "../../Common/PutFormat.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../Src/Group.hpp"
using namespace std;

extern Account Curuser;
extern PutFormat put;
extern TcpSocket Socketfd;
extern MessageQueue msgQueue;

void Group_UI_initEntry()
{
    vector<string> Group_Creat = Group_Srv_GetList(Curuser.getname()+"C");     //我创建的群
    vector<string> Group_Manager = Group_Srv_GetList(Curuser.getname()+"M");   //我管理的群
    vector<string> Group_Add = Group_Srv_GetList(Curuser.getname()+"A");   //我加入的群

    char choice;
    do{
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t************************ Group Management *************************" << endl;
        cout << "\t\t==================================================================="<< endl;
        cout << "\t\t我创建的群:";
        for(auto& group:Group_Creat){
            cout << group + " ";
        }
        cout << endl;
        cout << "\t\t我管理的群:";
         for(auto& group:Group_Manager){
            cout << group + " ";
         }
        cout << endl;
        cout << "\t\t我加入的群:";
         for(auto& group:Group_Add){
            cout << group + " ";
         }
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
        put.stdput(choice);
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
    } while ('E'!= choice && 'e' != choice);
}


void Group_UI_MgtEntry()
{
    char choice;
    string name;
    cout << "\n\t\t请输入你想管理的群聊:";
    cin >> name;
    cout << endl;
    string Owner = Group_Srv_isExists(name);
    if(Owner.empty()){
        cout << "\t\t请输入正确的群名称" << endl;
        put.stdexit();
        return;
    }

    vector<string> Group_Manager = Group_Srv_GetList(name+"M");    //群的管理者
    vector<string> Group_Add = Group_Srv_GetList(name+"A");    //群的成员
    do{
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t************************ Group Management ************************" << endl;
        cout << "\t\t==================================================================="<< endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "\t\t\t\t\t\t\033[4;33;1m" << name << "\033[0m" << endl;
        cout << "\t\t群主: " << Owner << endl;
        cout << "\t\t群管理员: ";
        for(auto& group:Group_Manager){
            cout << group + " ";
        }
        cout << endl;
        cout << "\t\t群成员:  ";
        for(auto& user:Group_Add){
            cout << user + " ";
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
        cout << "\t\t[G]解散该群" << endl;
        cout << "\t\t[E]返回" << endl;
        cout << "" << endl;
        cout << "\t\t==================================================================="<< endl;
        put.stdput(choice);
        switch(choice)
        {
            case 'Q':
            case 'q':
                Group_UI_QueryApply(name);
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
            case 'G':
            case 'g':
                Group_Srv_DelGroup(name);
                break;
            }
    } while ('E' != choice && 'e' != choice);
}

void Group_UI_QueryApply(string name)
{
    string choice;
    vector<string> Query = Group_Srv_GetApply(name);
    if(!Query.size()){
        cout << "\n\t\t当前暂无群验证消息" << endl;
        put.stdexit();
        return;
    }

    cout << "\n\t\t一共有" << Query.size() << "条群申请" << endl;
    for(auto& str :Query){
    do
    {
        cout << "\t\t" << str << "申请添加你为好友,[Yes/No]:";
        cin >> choice;
        cout << "\033[F\033[K";
        cout << "\033[F\033[K" << endl;
    } while (choice.compare("Yes") && choice.compare("No"));

        Message msg(Group_ReApply, name, str, choice);
        string temp = msg.tojson();
        Socketfd.sendMsg(temp);
        temp = msgQueue.pop();
        if(!temp.compare("T"))
            cout << "\n\t\t快在" << str << "里一起聊天吧" << endl;
        else
            cout << "\n\t\t发送回复失败" << endl;
    }
    put.stdexit();
}

void Group_UI_ChatEntry()
{
     string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;
    vector<string> history = Group_Srv_history(name);

    system("clear");
    for(auto& str :history){
        auto pos = str.find(":");
        string name = str.substr(0, pos);
        string data = str.substr(pos + 1);

        if(!name.compare(Curuser.getname()))
        {
            put.printFromRight(name,color_empty,B_empty,type_empty);
            put.printFromRight(data,black,B_green,highlight);
        }
        else
        {
            put.printFromLeft(name,color_empty,B_empty,type_empty);
            put.printFromLeft(data, black, B_white, highlight);
        }
    }
    put.printFrommid("press [q] to exit", red, B_empty, underscore);

    while(1){
        string data;
        getline(cin, data);
        if(!data.compare("q"))
            break;
        cout << "\033[1A\033[K";
        put.printFromRight(Curuser.getname(),color_empty,B_empty,type_empty);
        put.printFromRight(data,black,B_white,highlight);

        Message msg(Group_Chat, name, Curuser.getname(), data);
        string temp = msg.tojson();
        Socketfd.sendMsg(temp);
    }
}
