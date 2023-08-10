#include <iostream>
#include <unistd.h>
#include <vector>
#include "Friend_UI.hpp"
#include "../Src/Friend.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/Account.hpp"
#include "../../Common/PutFormat.hpp"
using namespace std;

extern TcpSocket Socketfd;   //全局套接字
extern Account Curuser; //当前登陆用户
extern PutFormat put;
extern MessageQueue msgQueue;

void Friend_UI_MgtEntry()
{
    char choice;
    do{
        vector<Friend> friends = Friend_Srv_GetAllstate();
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t************************ Friend Management ************************" << endl;
        cout << "\t\t==================================================================="<< endl;
        cout << "\t\t          好友名称          在线状态          拉黑状态        " << endl;
        for(auto& friend_obj:friends){
            cout << "\t\t            " << friend_obj.getname() << "\t              ";
            if(!friend_obj.getOnLinestate().compare("ON"))
                cout << "\033[32;1m" << "ON" << "\033[0m";
            else
                cout << "\033[31;1m" << "OFF" << "\033[0m";
            cout << "                 ";
            if(!friend_obj.getBlackstate().compare("ON"))
                cout << "\033[32;1m" << "ON" << "\033[0m"<< endl;
            else 
                cout << "\033[31;1m" << "OFF"  << "\033[0m" << endl;
        }
        cout << "\t\t-------------------------------------------------------------------" << endl;
        cout << "\t\t[A]好友添加" << endl;
        cout << "\t\t[D]好友删除" << endl;
        cout << "\t\t[B]拉黑好友" << endl;
        cout << "\t\t[C]好友聊天" << endl;
        cout << "\t\t[P]好友申请" << endl;
        cout << "\t\t[F]解除拉黑" << endl;
        cout << "\t\t[E]退出" << endl;
        cout << "" << endl;
        cout << "\t\t==================================================================="<< endl;
        put.stdput(choice);
        switch (choice)
        {
        case 'A':
        case 'a':
            Friend_Srv_Apply();
            break;
        case 'D':
        case 'd':
            Friend_Srv_Delete();
            break;
        case 'B':
        case 'b':
            Friend_Srv_Black();
            break;
        case 'C':
        case 'c':
            Friend_UI_Chat();
            break;
        case 'p':
        case 'P':
            Friend_UI_QueryApply();
            break;
        case 'F':
        case 'f':
            Friend_Srv_NoBlack();
            break;
        }
    } while ('E'!= choice && 'e' != choice);
}

//在聊天的时候选择好友
void Friend_UI_Chat()
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;
    vector<string> history = Friend_Srv_history(name);
    if(history.empty())
        return;

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
    cout << endl;

    while(1){
        string data;
        getline(cin, data);
        if(!data.compare("q"))
            break;
        cout << "\033[1A\033[K";
        put.printFromRight(Curuser.getname(),color_empty,B_empty,type_empty);
        put.printFromRight(data,black,B_white,highlight);

        Message msg(Friend_Chat, Curuser.getname(), name, data);
        string temp = msg.tojson();
        Socketfd.sendMsg(temp);
    }
    Message msg(Packet_exitchat, Curuser.getname(), "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
}

void Friend_UI_QueryApply()
{
    string choice;
    vector<string> Query = Friend_Srv_GetApply();
    if (!Query.size())
    {
        cout << "\n\t\t当前暂无好友验证消息" << endl;
        put.stdexit();
        return;
    }

    cout << "\n\t\t一共有" << Query.size() << "条好友申请:" << endl;
    for(auto& str :Query){
    do
    {
        cout << "\t\t" << str << "申请添加你为好友,[Yes/No]:";
        cin >> choice;
        cout << "\033[F\033[K";
        cout << "\033[F\033[K" << endl;
    } while (choice.compare("Yes") && choice.compare("No"));

    Message msg(Friend_ReApply, Curuser.getname(), str, choice);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if (!temp.compare("T"))
    cout << "\n\t\t快和" << str << "一起聊天吧" << endl;
    else
    cout << "\n\t\t发送回复失败" << endl;
    }
    put.stdexit();
}
