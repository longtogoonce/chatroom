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

TcpSocket Socketfd;   //全局套接字
extern Account Curuser; //当前登陆用户

void Friend_UI_MgtEntry()
{
    vector<Friend> friends = Friend_Srv_GetAllstate();
    char choice;
    do{
        system("clear");
        cout << "\n\t\t===================================================================" << endl;
        cout << "\t\t************************ Friend Management ************************" << endl;
        cout << "\t\t==================================================================="<< endl;
        cout << "\t\t          好友名称          在线状态          拉黑状态        " << endl;
        for(auto& friend_obj:friends){
            cout << "\t\t          " << friend_obj.getname() << "           " << 
            friend_obj.getOnLinestate() << "           " << friend_obj.getBlackstate() << endl;
        }
        cout << "\t\t-------------------------------------------------------------------" << endl;
        cout << "\t\t[Q]好友查询" << endl;
        cout << "\t\t[A]好友添加" << endl;
        cout << "\t\t[D]好友删除" << endl;
        cout << "\t\t[B]拉黑好友" << endl;
        cout << "\t\t[C]好友聊天" << endl;
        cout << "\t\t[P]好友申请" << endl;
        cout << "\t\t[F]解除拉黑" << endl;
        cout << "" << endl;
        cout << "\t\t==================================================================="<< endl;
        cin >> choice;
        switch(choice)
        {
        case 'A':
        case 'a':
            Friend_Srv_Add();
            break;
        case 'D':
        case 'd':
            Friend_Srv_Delete();
            break;
        case 'Q':
        case 'q':
            Friend_Srv_Query();
            break;
        case 'B':
        case 'b':
            Friend_Srv_Black();
            break;
        case 'C':
        case 'c':
            Friend_UI_Chat();
            break;
        case 'P':
        case 'p':
            Friend_Srv_Apply();
            break;
        case 'F':
        case 'f':
            Friend_Srv_NoBlack();
            break;
        }

    } while ('E'!= choice || 'e' != choice);
}

//在聊天的时候选择好友
void Friend_UI_Chat()
{
    PutFormat put;
    string name;
    cout << "\n\t\t请输入名称:" << endl;
    cin >> name;
    vector<string> history = Friend_Srv_Chat(name);

    system("clear");
    for(auto& str :history){
        auto pos = str.find(":");
        string name = str.substr(0, pos);
        string data = str.substr(pos + 1);

        if(name.compare(Curuser.getname()))
        {
            put.printFromRight(name,color_empty,B_empty,type_empty);
            put.printFromRight(data,black,B_white,highlight);
        }
        else
        {
            put.printFromLeft(name,color_empty,B_empty,type_empty);
            put.printFromLeft(data, black, B_purple, highlight);
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

        Message msg(Friend_Chat, Curuser.getname(), name, data);
        string temp = msg.tojson();
        Socketfd.sendMsg(temp);
    }
}
