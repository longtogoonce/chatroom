#include <fcntl.h>
#include <iostream>
#include <csignal>
#include <fstream>
#include "../Src/Login.hpp"
#include "Login_UI.hpp"
#include "Friend_UI.hpp"
#include "Group_UI.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/Account.hpp"
#include "../../Common/PutFormat.hpp"
#include "../../Common/File.hpp"
#include "../../Common/Message.hpp"
#include <thread>
using namespace std;

TcpSocket Socketfd;   //全局套接字  
PutFormat put;  //规范输出
Account Curuser;    //当前登陆用户
MessageQueue msgQueue; // 消息队列
File RecFile;
int RecState = 0;  // 服务器接受状态

void ReadThreadFromeTcp();  //读线程

void Main_Menu(void)
{
    //连接服务器，开始登陆
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
        put.printFrommid2("                [F]riends               [G]roup                   ");
        cout << "" << endl;
        put.printFrommid2("                [D]elete Account        [E]xit                   ");
        cout << "" << endl;
        put.printFrommid2("                [S]end File             [R]ecive File            ");
        cout << "" << endl;
        cout << "" << endl;
        put.printFrommid2("===================================================================");
        cout <<"\t\t\t\tPlease input your choice:";
        put.stdput(choice);
        switch (choice)
        {
        case 'S':
        case 's':
            Send_File_MgtEntry();
            break;
        case 'R':
        case 'r':
            Recive_File_MgtEntry();
            break;
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

int main(int argc,char **argv)
{

    signal(SIGINT, SIG_IGN);
    
    if(argc !=3){
        cout << "\t\t输入格式为: ./Client IP port " << endl;
    }
    char* endptr;
    unsigned long data = std::strtoul(argv[2], &endptr, 10);
    unsigned short port = static_cast<unsigned short>(data);
    if(Socketfd.ConnectToHost(argv[1], port)<0){
        cout << "\t\t请输入正确的IP与PORT" << endl;
        return 0;
    }
    
    //Socketfd.ConnectToHost(IP, 9000);
    Main_Menu();
    return 0;
}

void ReadThreadFromeTcp()
{
    while(1){
       if(RecState){
        Socketfd.recvFile(RecFile.getname(),RecFile.getoffset(),RecFile.gettotalRecords());
        RecState = 0;
        put.stdexit();
       }
        string buf = Socketfd.recvMsg();
        if(!buf.substr(0,4).compare("FILE")){
            RecState = 1;
            auto pos = buf.find(":");
            RecFile.settotal(buf.substr(pos + 1));
        } else {
            cout << "\n";
            auto pos = buf.find(":");
            string state = buf.substr(0, pos);
            string data = buf.substr(pos + 1);
            if (!state.compare("ON")) {
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
}