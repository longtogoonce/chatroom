#include <iostream>
#include "Login_UI.hpp"
#include <string>
#include "../Src/Login.hpp"
#include "../../Common/Account.hpp"
#include "../../Common/PutFormat.hpp"
#include "../../Common/File.hpp"
#include <limits>
#include <unistd.h>

using namespace std;
extern PutFormat put;
extern File RecFile;
// 系统登陆
int SysLogin()
{
    char choice = '0';
    do
    {
        system("clear");
        cout << "\n" << endl;
        put.printFrommid2("===================================================================");
        put.printFrommid2("*********************** xiaotian's chatroom ***********************");
        put.printFrommid2("===================================================================");
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        put.printFrommid2("                [L]ogin in         [F]ind password                   ");
        cout << "" << endl;
        put.printFrommid2("                [S]ign  in         [E]xit                            ");
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        put.printFrommid2("===================================================================");
        put.stdput(choice);
        switch (choice)
        {
        case 'L':
        case 'l':
            if(Login())
                return 1;
            break;
        case 'S':
        case 's':
            Sign();
            break;
        case 'F':
        case 'f':
            FindPassword();
            break;
        }
    } while ('e' != choice || 'E' != choice);
    return 0;
}

int Login()
{
    string name;
    string passwd;

    system("clear");
    cout << "\n"<< endl;
    put.printFrommid2("===================================================================");
    put.printFrommid2("*********************** xiaotian's chatroom ***********************");
    put.printFrommid2("===================================================================");
    cout << "" << endl;
    cout << "" << endl;
    std::cout << "\t\t\t\t\t\t | |    ___   __ _(_)_ __  " << std::endl;
    std::cout << "\t\t\t\t\t\t | |   / _ \\ / _` | | '_ \\ " << std::endl;
    std::cout << "\t\t\t\t\t\t | |__| (_) | (_| | | | | |" << std::endl;
    std::cout << "\t\t\t\t\t\t |_____\\___/ \\__, |_|_| |_|" << std::endl;
    std::cout << "\t\t\t\t\t\t               |___/" << std::endl;
    cout << "" << endl;
    cout << "" << endl;
    put.printFrommid2("===================================================================");
    cout << "\t\t\t\t请输入你的账户名称:";
    cin >> name;
    cout << "\n\t\t\t\t请输入你的密码:";
    cin >> passwd;
    int temp = Login_Srv_Verify(name, passwd);
    if(temp == 1)
        return 1;
    else if(!temp)
        cout << "\n\t\t\t\t该用户已经登陆" << endl;
    else
        cout << "\n\t\t\t\t登陆失败" << endl;
    put.stdexit();
    return 0;
}

void Sign()
{
    string name;
    string passwd1;
    string passwd2;
    string tele;
    string myitbo;
    system("clear");
    cout << "\n" << endl;
    put.printFrommid2("===================================================================");
    put.printFrommid2("*********************** xiaotian's chatroom ***********************");
    put.printFrommid2("===================================================================");
    cout << "" << endl;
    cout << "" << endl;
    cout <<"\t\t\t                                 _                          \n"
        << "\t\t\t                   __      _____| | ___ ___  _ __ ___   ___ \n"
        << "\t\t\t                   \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n"
        << "\t\t\t                    \\ V  V /  __/ | (_| (_) | | | | | |  __/\n"
        << "\t\t\t                     \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|\n";
    
    cout << "" << endl;
    cout << "" << endl;
    put.printFrommid2("===================================================================");

    Account user;
    time_t Curtime;
    time(&Curtime);
    string UID = to_string(Curtime);
    UID = UID.substr(UID.length() - 5);
    user.setUID(UID);
    cout << "\t\t\t\t\t\t*请输入你的账户名称：";
    cin >> name;
    cout << endl;
    user.setname(name);
    do
    {
        cout << "\t\t\t\t\t\t*请输入你的密码：";
        cin >> passwd1;
        cout << endl;
        cout << "\t\t\t\t\t\t*请再次输入你的密码：";
        cin >> passwd2;
        cout << endl;
    } while (passwd2.compare(passwd1));
    user.setpasswd(passwd2);
    cout << "\t\t\t\t\t\t*请输入你的手机号：";
    cin >> tele;
    cout << endl;
    user.settele(tele);
    cout << "\t\t\t\t\t\t*请输入你的密保：";
    cin >> myitbo;
    cout << endl;
    user.setmyitbo(myitbo);
    Login_Srv_Add(user);
}

void FindPassword()
{
        string name;
        string myitbo;
        string passwd;

        system("clear");
        cout << "\n" << endl;
        put.printFrommid2("===================================================================");
        put.printFrommid2("*********************** xiaotian's chatroom ***********************");
        put.printFrommid2("===================================================================");
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "\t\t\t\t\t   __| | ___  _ __ | |_( ) |_   / _| ___  _ __ __ _  ___ | |_" <<std::endl;
        cout << "\t\t\t\t\t  / _` |/ _ \\| '_ \\| __|/| __| | |_ / _ \\| '__/ _` |/ _ \\| __|"<<std::endl;
        cout << "\t\t\t\t\t | (_| | (_) | | | | |_  | |_  |  _| (_) | | | (_| | (_) | |_" << std::endl;
        cout << "\t\t\t\t\t  \\__,_|\\___/|_| |_|\\__|  \\__| |_|  \\___/|_|  \\__, |\\___/ \\__|" << std::endl;
        cout << "\t\t\t\t\t                                                |___/" << std::endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        put.printFrommid2("===================================================================");
        cout << "\t\t\t\t请输入用户名:";
        cin >> name;
        cout << "\t\t\t\t请输入密保:";
        cin >> myitbo;
        if (Login_Srv_FindPasswd(name, myitbo, passwd))
        {
        cout << "\n\t\t\t\t你的密码是:";
        put.printFromLeft(passwd, yellow, B_empty, underscore);
        }
        else
        {
        cout << "\n\t\t\t\t验证失败" << endl;
        }
        put.stdexit();
}

void Send_File_MgtEntry()
{
    string file_path;
    string dest_name;

     system("clear");
     cout << "\n" << endl;
     put.printFrommid2("===================================================================");
     put.printFrommid2("*********************** xiaotian's chatroom ***********************");
     put.printFrommid2("===================================================================");
     cout << "" << endl;
     cout << "" << endl;
     cout << "" << endl;
     cout << "\t\t\t\t\t  ____                 _    __ _ _    " << std::endl;
     cout << "\t\t\t\t\t / ___|  ___ _ __   __| |  / _(_) | ___" << std::endl;
     cout << "\t\t\t\t\t \\___ \\ / _ \\ '_ \\ / _` | | |_| | |/ _ \\" << std::endl;
     cout << "\t\t\t\t\t  ___) |  __/ | | | (_| | |  _| | |  __/" << std::endl;
     cout << "\t\t\t\t\t |____/ \\___|_| |_|\\__,_| |_| |_|_|\\___|" << std::endl;
     cout << "" << endl;
     cout << "" << endl;
     cout << "" << endl;
     cout << "" << endl;
     put.printFrommid2("===================================================================");
     do {
        cout << "\t\t\t\t请输入文件名称:";
        cin.ignore();
        cin >> file_path;
        cout << "\033[F\033[K";
        cout << "\033[F\033[K" << endl;
     } while (access(file_path.c_str(), F_OK));
     cout << "\t\t\t\t请输入发送的对象:";
     cin >> dest_name;
     cout << endl;
     Send_File(file_path,dest_name);
}

void Recive_File_MgtEntry()
{
     string filename;
        system("clear");
        cout << "\n" << endl;
        put.printFrommid2("===================================================================");
        put.printFrommid2("*********************** xiaotian's chatroom ***********************");
        put.printFrommid2("===================================================================");
        cout << "" << endl;
        cout << "" << endl;
        cout << "\t\t\t\t\t  ____           _              __ _ _      \n";
        cout << "\t\t\t\t\t |  _ \\ ___  ___(_)_   _____   / _(_) | ___ \n";
        cout << "\t\t\t\t\t | |_) / _ \\/ __| \\ \\ / / _ \\ | |_| | |/ _ \\\n";
        cout << "\t\t\t\t\t |  _ <  __/ (__| |\\ V /  __/ |  _| | |  __/\n";
        cout << "\t\t\t\t\t |_| \\_\\___|\\___|_| \\_/ \\___| |_| |_|_|\\___|\n";
        cout << "" << endl;
        cout << "" << endl;
        cout << "" << endl;
        put.printFrommid2("===================================================================");
        vector<string> files = getfiles();
        if (!files.size()){
            cout << "\n\t\t当前暂无接受到文件" << endl;
            put.stdexit();
            return;
        }
        for(auto ptr : files){
            put.printFrommid2("File Name: " + ptr);
        }
        put.printFrommid2("-------------------------------------------------------------------");
        cout << "\t\t\t\t请输入你想接受的文件名称:";
        cin >> filename;
        auto it = find(files.begin(), files.end(), filename);
        if(it == files.end()){
            cout << "\t\t\t\t请输入正确的文件名称" << endl;
            put.stdexit();
            return;
        }
        Recive_File(filename);
}