#include "Group.hpp"
#include <string>
#include <iostream>
#include "../../Common/Account.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/PutFormat.hpp"

using namespace std;
extern TcpSocket Socketfd;
extern Account Curuser;
extern PutFormat put;

//逻辑范式
int result_Group(Protocol protocol,string Gname)
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

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
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

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
    Message msg(Packet_GetListAll,name+"Q", "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

string Group_Srv_isExists(string Gname)
{
    Message msg(Group_IsExist, Curuser.getname(), "", Gname);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    return temp;
}

void Group_Srv_AddGroup()
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(Packet_Apply, name, "", Curuser.getname());
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare("T"))
        cout << "\t\t群申请已经发送成功" << endl;
    else if(!temp.compare("P"))
        cout << "\t\t请输入正确的群名称" << endl;
    else
        cout << "\t\t群申请发送失败" << endl;
    put.stdexit();
}

void Group_Srv_ExitGroup(string Gname)
{

    Message msg(Group_Exit,Gname, "", Curuser.getname());
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare("T"))
        cout << "\t\t群退出成功" << endl;
    else
        cout << "\t\t群退出失败" << endl;
    put.stdexit();
}

void Group_Srv_CreatGroup()
{
    if(result(Group_Creat))
        cout << "\t\t群创建成功" << endl;
    else
        cout << "\t\t群创建失败" << endl;
    put.stdexit();
}

void Group_Srv_DelGroup(string Gname)
{
  int reply = result_Group(Group_Delete,Gname);
    if(reply==1)
        cout << "\t\t群解散成功" << endl;
    else if(!reply)
        cout << "\t\t群解散失败" << endl;
    else
        cout << "\t\t你没有操作权限" << endl;
    put.stdexit();
}

void Group_Srv_Deluser(string Gname)
{
    int reply = result_Group(Group_ExitUser,Gname);
    if(reply==1)
        cout << "\t\t用户删除成功" << endl;
    else if(!reply)
        cout << "\t\t用户删除失败" << endl;
    else
        cout << "\t\t你没有操作权限" << endl;
    put.stdexit();
}

void Group_Srv_AddManager(string Gname)
{
    int reply = result_Group(Group_AddMan,Gname);
    if(reply==1)
        cout << "\t\t用户添加管理员成功" << endl;
    else if(!reply)
        cout << "\t\t用户添加管理员失败" << endl;
    else
        cout << "\t\t你没有操作权限" << endl;
    put.stdexit();
}

void Group_Srv_DelManager(string Gname)
{
    int reply = result_Group(Group_DelMan,Gname);
    if(reply==1)
        cout << "\t\t撤销管理员成功" << endl;
    else if(!reply)
        cout << "\t\t撤销管理员失败" << endl;
    else
        cout << "\t\t你没有操作权限" << endl;
    put.stdexit();
}