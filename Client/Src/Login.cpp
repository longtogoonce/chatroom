#include <iostream>
#include "../../Common/Account.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/PutFormat.hpp"

extern TcpSocket Socketfd;
extern Account Curuser;
extern PutFormat put;
extern MessageQueue msgQueue;

int Login_Srv_Verify(string name, string passwd)
{
    Message msg(User_Verify, name, "", passwd);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    if(!temp.compare("T")){
        Curuser.setname(name);
        return 1;
    }else if(!temp.compare("P"))
        return 0;
    else
        return -1;
}

void Login_Srv_Add(Account user)
{
    string data = user.tojson();
    Message msg(User_Creat, user.getname(), "", data);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if (!temp.compare("T"))
        cout << "\n\t\t\t\t\t\t\t恭喜你,账户创建成功,您的UID为:" << user.getUID() << endl;
    else if(!temp.compare("P"))
        cout << "\n\t\t\t\t\t\t\t该用户账户已经存在,请重新注册" << endl;
    else
        cout << "\n\t\t\t\t\t\t\t账户创建失败,请重新注册" << endl;
    put.stdexit();
}

int Login_Srv_FindPasswd(string name,string myitbo,string& passwd)
{
    Message msg(User_FindPasswd, name, "", myitbo);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare(myitbo)){
        passwd = temp;
        return 1;
    }
    else
        return 0;
}

int DelAccount()
{
    Message msg(User_Delete, Curuser.getname(), "", "");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();
    if(!temp.compare("T")){
        Curuser.setname("");
        return 1;
    }else{
        cout << "删除失败" << endl;
        return 0;
    }
}