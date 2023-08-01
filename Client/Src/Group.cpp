#include "Group.hpp"
#include <string>
#include <iostream>
#include "../../Common/Account.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/TcpSocket.hpp"

using namespace std;
extern TcpSocket Socketfd;
extern Account Curuser;

//逻辑范式
int result_Group(Protocol protocol,string Gname)
{
    string name;
    cout << "\n\t\t请输入名称:" << endl;
    cin >> name;

    Message msg(protocol, Gname, "",Curuser.getname()+":"+name);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare("T"))
        return 1;
    if(!temp.compare("P"))
        return -1;
    else
        return 0;
}

int result(Protocol protocol)
{
    string name;
    cout << "\n\t\t请输入名称:" << endl;
    cin >> name;

    Message msg(protocol, Curuser.getname(), "", name);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare("T"))
        return 1;
    else
        return 0;
}

//***************************************************************

vector<string> Group_Srv_GetList(string key)
{
    Message msg(Packet_GetSetAll, key, "","");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

vector<string> Group_Srv_GetApply(string name)
{
    Message msg(Packet_GetListAll,name, "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

void Group_Srv_AddGroup()
{
    string name;
    cout << "\n\t\t请输入名称:" << endl;
    cin >> name;

    Message msg(Packet_Apply, name, "", Curuser.getname());
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare("T"))
        cout << "群申请已经发送成功" << endl;
    else
        cout << "群申请发送失败" << endl;
}

void Group_Srv_ExitGroup(string Gname)
{

    Message msg(Group_Exit,Gname, "", Curuser.getname());
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare("T"))
        cout << "群退出成功" << endl;
    else
        cout << "群退出失败" << endl;
}

void Group_Srv_CreatGroup()
{
    if(result(Group_Creat))
        cout << "群创建成功" << endl;
    else
        cout << "群创建失败" << endl;
}

void Group_Srv_DelGroup(string Gname)
{
  int reply = result_Group(Group_Delete,Gname);
    if(reply==1)
        cout << "群解散成功" << endl;
    if(!reply)
        cout << "群解散失败" << endl;
    else
        cout << "你没有操作权限" << endl;
}

void Group_Srv_Deluser(string Gname)
{
    int reply = result_Group(Group_Exit,Gname);
    if(reply==1)
        cout << "用户删除成功" << endl;
    if(!reply)
        cout << "用户删除失败" << endl;
    else
        cout << "你没有操作权限" << endl;
}

void Group_Srv_AddManager(string Gname)
{
    int reply = result_Group(Group_AddMan,Gname);
    if(reply==1)
        cout << "用户添加管理员成功" << endl;
    if(!reply)
        cout << "用户添加管理员失败" << endl;
    else
        cout << "你没有操作权限" << endl;
}

void Group_Srv_DelManager(string Gname)
{
    int reply = result_Group(Group_DelMan,Gname);
    if(reply==1)
        cout << "撤销管理员成功" << endl;
    if(!reply)
        cout << "撤销管理员失败" << endl;
    else
        cout << "你没有操作权限" << endl;
}