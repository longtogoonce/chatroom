#include <iostream>
#include "../../Common/Account.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/TcpSocket.hpp"

extern TcpSocket Socketfd;
extern Account Curuser;

int Login_Srv_Verify(string name, string passwd)
{
    Message msg(User_Verify, name, "", passwd);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare("T")){
        Curuser.setname(name);
        return 1;
    }
    else
        return 0;
}

int Login_Srv_Add(Account user)
{
    string data = user.tojson();
    Message msg(User_Creat, user.getname(), "", data);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if (!temp.compare("T"))
        return 1;
    else
        return 0;
}

int Login_Srv_FindPasswd(string name,string myitbo,string& passwd)
{
    Message msg(User_FindPasswd, name, "", myitbo);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare(myitbo))
        return 1;
    else
        return 0;
}

int DelAccount()
{
    Message msg(User_Delete, Curuser.getname(), "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = Socketfd.recvMsg();
    if(!temp.compare("T")){
        Curuser.setname("");
        return 1;
    }else{
        cout << "删除失败" << endl;
        return 0;
    }
}