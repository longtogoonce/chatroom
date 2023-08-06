#include <iostream>
#include "Friend.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/Account.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/PutFormat.hpp"
#include <vector>
#include "nlohmann/json.hpp"

using namespace std;
extern TcpSocket Socketfd;
extern Account Curuser;
extern PutFormat put;
extern MessageQueue msgQueue;

int result1(Protocol protocol)
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(protocol, Curuser.getname(), "", name);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        return 1;
    else
        return 0;
}

vector<Friend> Friend_Srv_GetAllstate()
{
    Message msg(Friend_GetAll, Curuser.getname(), "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    vector<Friend> friend_list;
    json data = json::parse(temp);
    vector<string> friends = data.get<vector<string>>();
    for(auto& str:friends){
        Friend friend_obj(str);
        friend_list.push_back(friend_obj);
    }
    return friend_list;
}

vector<string> Friend_Srv_Chat(string name)
{
    Message msg(Packet_GetListAll, Curuser.getname()+name, "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

vector<string> Friend_Srv_GetApply()
{
    Message msg(Packet_GetListAll, Curuser.getname()+"Q", "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

void Friend_Srv_Apply()
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(Packet_Apply, Curuser.getname(), name, "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        cout << "\t\t好友验证已经发送成功" << endl;
    else if(!temp.compare("P"))
        cout << "\t\t请输入正确的账户名称" << endl;
    else
        cout << "\t\t好友申请发送失败" << endl;
    put.stdexit();
}

void Friend_Srv_Delete()
{
    if(result1(Friend_Delete))
        cout << "\t\t好友删除成功" << endl;
    else
        cout << "\t\t好友删除失败" << endl;
}

void Friend_Srv_Black()
{
    if(result1(Friend_Black))
        cout << "\t\t成功加入黑名单" << endl;
    else
        cout << "\t\t加入黑名单失败" << endl;
}

void Friend_Srv_NoBlack()
{
    if(result1(Friend_NoBlack))
        cout << "\t\t成功加入白名单" << endl;
    else
        cout << "\t\t加入白名单失败" << endl;
}
