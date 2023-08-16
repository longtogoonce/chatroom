#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/sendfile.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../../Common/Account.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/File.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/PutFormat.hpp"

extern TcpSocket Socketfd;
extern Account Curuser;
extern PutFormat put;
extern MessageQueue msgQueue;
extern File RecFile;

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

void Send_File(string file_path, string dest_name)
{
    //获取文件名称，偏移量
    File file(file_path, 0);
    string data = file.tojson();
    Message msg(Packet_sendfile, Curuser.getname(), dest_name, data);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);

    temp = msgQueue.pop();
    auto pos = temp.find("+");
    string state = temp.substr(0, pos);
    if (!state.compare("T")) {

        string count = temp.substr(pos + 1);
        off_t offset = static_cast<off_t>(stoll(count));

        int fd = open(file_path.c_str(), O_RDONLY);
        if(fd == -1){
            cout << "\t\t打开文件失败" << endl;
            put.stdexit();
            return;
        }

        while (offset< file.gettotalRecords()) {
            ssize_t sent_bytes = sendfile(Socketfd.getfd(), fd, &offset,file.gettotalRecords()-offset);
            offset += sent_bytes;
            if (sent_bytes == -1) {
                cout << "\t\t发送文件失败" << endl;
                break;
            }

              float progress = static_cast<float>(offset) / file.gettotalRecords();
              put.printprogress(progress);
        }

        close(fd);
    } else if(!state.compare("F"))
        cout << "\t\t请输入正确的名称" << endl;
    else if(!state.compare("A"))
        cout << "\t\t文件已经发送成功" << endl;
    else
        cout << "\t\t服务器发生错误" << endl;
    put.stdexit();
}

void Recive_File(string file_name)
{
    //创建用户存储目录
    string dir = "../" + Curuser.getname() + "_File_Client";
    mkdir(dir.c_str(), 0777);

    //判断文件是否存在,并计算偏移量
    off_t offset;
    string filepath = dir + "/" + file_name;

    ifstream input(filepath);
    off_t size = 0;
    if (input.is_open()) {
        input.seekg(0, std::ios::end);
        size = input.tellg();
        input.close();
    }
    File file(file_name, size, size);
    RecFile.setname(filepath);
    RecFile.setoffset(size);
    string data = file.tojson();

    //发送接受文件包
    Message msg(Packet_recivefile, Curuser.getname(), "", data);
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
/*
    temp = msgQueue.pop();
    auto pos = temp.find("+");
    string state = temp.substr(0, pos);
    if (!state.compare("T")) {
        
        string count = temp.substr(pos + 1);
        if(!count.compare(to_string(offset)))
              cout << "\t\t该文件已经存在" << endl;
        else{
            
            cout << "\t\t准备开始接受文件" << endl;
        
    } else
        cout << "\t\t该文件不存在" << endl;
    put.stdexit();
    */
}

vector<std::string> getfiles()
{
    Message msg(Packet_GetSetAll, Curuser.getname()+"L", "","");
    string temp = msg.tojson();
    Socketfd.sendMsg(temp);
    temp = msgQueue.pop();

    json data = json::parse(temp);
    return data.get<vector<string>>();
}