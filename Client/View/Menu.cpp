#include <iostream>
#include "../Src/Login.hpp"
#include "Login_UI.hpp"
#include "Friend_UI.hpp"
#include "Group_UI.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/Account.hpp"
#include "../../Common/PutFormat.hpp"
#include "../../Common/Message.hpp"
#include <thread>
using namespace std;

TcpSocket Socketfd;   //全局套接字  
PutFormat put;  //规范输出
Account Curuser;    //当前登陆用户
MessageQueue msgQueue; // 消息队列

void ReadThreadFromeTcp();  //读线程

void Main_Menu(void)
{
    /*
        char *host = (char *)malloc(10);
        gethostname(host, 10);
    */

    //连接服务器，开始登陆
    Socketfd.ConnectToHost(IP, 9000);
    thread ReadThread(ReadThreadFromeTcp);
    ReadThread.detach();

begin:
    if (SysLogin() == 0)
        return ;
    //登陆成功，进入初始菜单
	char choice;
    do
    {
        system("clear");
        put.printFrommid2("===================================================================");
        put.printFrommid2("*********************** xiaotian's chatroom ***********************");
        put.printFrommid2("===================================================================");
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        put.printFrommid2("                [F]riends               [G]roup                   ");
        cout << "" << endl;
        put.printFrommid2("                [D]elete Account        [E]xit                   ");
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        put.printFrommid2("===================================================================");
        cout <<"\t\t\t\tPlease input your choice:";
        put.stdput(choice);
        switch (choice)
        {
		case 'F':
		case 'f':
            Friend_UI_MgtEntry();
            break;
		case 'G':
		case 'g':
			Group_UI_initEntry();
			break;
        case 'D':
        case 'd':
            if(DelAccount())
                goto begin;
            else
                break;
        }
    } while ('E' != choice && 'e' != choice);
}

int main()
{
    Main_Menu();
    return 0;
}

void ReadThreadFromeTcp()
{
    while(1){
        string buf = Socketfd.recvMsg();
        cout << "\n";
        auto pos = buf.find(":");
        string state = buf.substr(0, pos);
        string data = buf.substr(pos + 1);
        if(!state.compare("ON")){
            auto pos = data.find("+");
            string name = data.substr(0, pos);
            string msg = data.substr(pos + 1);
            put.printFromLeft(name, color_empty, B_empty, type_empty);
            put.printFromLeft(msg, black, B_purple, highlight);
        }else if(!state.compare("OFF"))
            put.printFrommid("[ "+data+" ]", yellow, B_empty, highlight);
        else
            msgQueue.push(data);
    }
}