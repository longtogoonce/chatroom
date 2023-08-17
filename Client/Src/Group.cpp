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
extern MessageQueue msgQueue;

vector<string> Group_Srv_GetList(string key)
{
    Message msg(Packet_GetSetAll, key, "","");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

vector<string> Group_Srv_GetApply(string name)
{
    Message msg(Packet_GetSetAll,name+"Q", "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

vector<string> Group_Srv_history(string Gname)
{
    Message msg(Packet_history, Curuser.getname(),"" , Gname);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}

string Group_Srv_isExists(string Gname)
{
    Message msg(Group_IsExist, Curuser.getname(), "", Gname);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    return temp;
}

void Group_Srv_AddGroup()
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
        cout << "\t\t群申请已经发送成功" << endl;
    else if(!temp.compare("P"))
        cout << "\t\t请输入正确的群名称" << endl;
    else
        cout << "\t\t群申请发送失败" << endl;
    put.stdexit();
}

void Group_Srv_ExitGroup(string Gname)
{
  
    Message msg(Group_Exit, Curuser.getname(), "",Gname);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        cout << "\t\t群退出成功" << endl;
    else
        cout << "\t\t群退出失败" << endl;
    put.stdexit();
}

void Group_Srv_CreatGroup()
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(Group_Creat, Curuser.getname(), "", name);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        cout << "\t\t群创建成功" << endl;
    else if(!temp.compare("P"))
        cout << "\t\t该名称已经存在" << endl;
    else
        cout << "\t\t群创建失败" << endl;
    put.stdexit();
}

void Group_Srv_DelGroup(string Gname)
{

    Message msg(Group_Delete, Curuser.getname(), "",Gname);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        cout << "\t\t该群已经删除成功" << endl;
    else if (!temp.compare("P"))
        cout << "\t\t你没有操作权限" << endl;
    else
        cout << "\t\t群删除失败" << endl;
    put.stdexit();
}

void Group_Srv_Deluser(string Gname)
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(Group_ExitUser, Gname, "",Curuser.getname()+":"+name);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        cout << "\t\t该用户删除成功" << endl;
    else if (!temp.compare("P"))
        cout << "\t\t你没有操作权限" << endl;
    else if(!temp.compare("F"))
        cout << "\t\t你不能删除自己" << endl;
    else
        cout << "\t\t删除失败" << endl;
    put.stdexit();
}

void Group_Srv_AddManager(string Gname)
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(Group_AddMan, Gname, name,Curuser.getname());
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        cout << "\t\t管理员添加成功" << endl;
    else if (!temp.compare("P"))
        cout << "\t\t你没有操作权限" << endl;
    else if (!temp.compare("N"))
        cout << "\t\t查无此人" << endl;
    else
        cout << "\t\t管理员添加失败" << endl;
    put.stdexit();
}

void Group_Srv_DelManager(string Gname)
{
    string name;
    cout << "\n\t\t请输入名称:";
    cin >> name;
    cout << endl;

    Message msg(Group_DelMan, Gname, name,Curuser.getname());
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T"))
        cout << "\t\t管理员撤销成功" << endl;
    else if (!temp.compare("P"))
        cout << "\t\t你没有操作权限" << endl;
    else if (!temp.compare("N"))
        cout << "\t\t查无此人" << endl;
    else
        cout << "\t\t管理员撤销失败" << endl;
    put.stdexit();
}