#include <iostream>
#include "Account.hpp"
#include "TcpSocket.hpp"
#include "../Src/Login.hpp"
#include "Login_UI.hpp"
#include "Friend_UI.hpp"
#include "Group_UI.hpp"

using namespace std;

TcpSocket Socketfd;   //全局套接字
Account Curuser;    //当前登陆用户

void Main_Menu(void)
{
    /*
        char *host = (char *)malloc(10);
        gethostname(host, 10);
    */

    //连接服务器，开始登陆
    Socketfd.ConnectToHost(IP, 9000);
    
begin:  if(SysLogin()==0)
		return ;
	
    //登陆成功，进入初始菜单
	char choice;
    do
    {
        system("clear");
        cout << '\n' << "\t\t===================================================================" << endl;
        cout << "\t\t*********************** xiaotian's chatroom ***********************" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "\t\t                   [F]riends               [G]roup                 " << endl;
        cout << "" << endl;
        cout << "\t\t                   [D]elete Account        [E]xit                  " << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "\t\t===================================================================" << endl;
        cout <<"\t\tPlease input your choice:" <<endl;
        cin >> choice;
        switch (choice)
        {
		case 'F':
		case 'f':
			Friend_UI_MgtEntry();
			break;
		case 'G':
		case 'g':
			Group_UI_MgtEntry();
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