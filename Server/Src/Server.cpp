#include "sys/epoll.h"
#include <netinet/tcp.h>
#include "WorkProcess.hpp"
#include "../../Server/Persistence/Group.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/ThreadPool.hpp"
#include "../../Common/Message.hpp"
#include "../../Common/Redis.hpp"
#include "../../Common/File.hpp"

Redis redis;
const string FILEPATH = "../My_File_Server";

int main()
{   
    //初始化服务器
    struct epoll_event ev, evs[1024];
    ThreadPool threadpool(20);

    TcpServer server;
    int opt = 1; // 端口复用
    setsockopt(server.getfd(), SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(int));
    server.setListen(9000);

    //依据现有监听套接字创建epoll模型
    int epfd = epoll_create(1024);
    ev.events = EPOLLIN;
    ev.data.fd = server.getfd();
    epoll_ctl(epfd, EPOLL_CTL_ADD, server.getfd(), &ev);

    //持续检查
    while(1)
    {
        int num = epoll_wait(epfd, evs, 1024, 1000);
        for (int i = 0; i < num; i++)
        {
            int fd = evs[i].data.fd;
            // 判读是连接请求还是数据请求
            if(fd == server.getfd()){

                TcpSocket* New = server.acceptConnect();
                New->toNoBlack(); 

                /*   // 开启保活，1分钟内探测不到，断开连接
                   int keep_alive = 1;
                   int keep_idle = 3;
                   int keep_interval = 1;
                   int keep_count = 30;
                   if (setsockopt(New.getfd(), SOL_SOCKET, SO_KEEPALIVE, &keep_alive, sizeof(keep_alive)))
                   {
                       perror("Error setsockopt(SO_KEEPALIVE) failed");
                       exit(1);
                   }
                   if (setsockopt(New.getfd(), IPPROTO_TCP, TCP_KEEPIDLE, &keep_idle, sizeof(keep_idle)))
                   {
                       perror("Error setsockopt(TCP_KEEPIDLE) failed");
                       exit(1);
                   }
                   if (setsockopt(New.getfd(), SOL_TCP, TCP_KEEPINTVL, (void *)&keep_interval, sizeof(keep_interval)))
                   {
                       perror("Error setsockopt(TCP_KEEPINTVL) failed");
                       exit(1);
                   }
                   if (setsockopt(New.getfd(), SOL_TCP, TCP_KEEPCNT, (void *)&keep_count, sizeof(keep_count)))
                   {
                       perror("Error setsockopt(TCP_KEEPCNT) failed");
                       exit(1);
                   }
               */
                // 添加新的epoll模型
                ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                ev.data.fd = New->getfd();
                epoll_ctl(epfd, EPOLL_CTL_ADD,New->getfd(), &ev);

            }
            else if(evs[i].events & EPOLLRDHUP) {
                 close(fd);
                 epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);

                 auto it = find_if(Onlineuser.begin(), Onlineuser.end(),
                 [fd](const pair<string, pair<int, string>> &p){ return p.second.first == fd; });
                 if (it != Onlineuser.end())
                     Onlineuser.erase(it);
            }
            else if(Onlinefile.find(fd) != Onlinefile.end()){
                 string filename = Onlinefile[fd].first;
                 string filepath = FILEPATH + "/" + filename;
                 TcpSocket tcp1(fd);
                 off_t total =  tcp1.recvFile2(filepath);
                 cout << "Curtotal:" << total << endl;
                 cout << "Total:" << Onlinefile[fd].second << endl;
                 if (total == Onlinefile[fd].second)
                     Onlinefile.erase(fd);
            } else {
                 Task task;
                 task.function = WorkProcess;
                 task.arg = &evs[i].data.fd;
                 threadpool.addTask(task);
            }
        }
    }
}

void WorkProcess(void *arg)
{
    //获取初始信息
    int *temp = (int *)arg;
    int src = temp[0];
    TcpSocket tcp1(src);
    string buf = tcp1.recvMsg();
    Message msg(buf);

    // 判断是数据转发还是任务处理
    if(msg.getprotocol()==Packet_recivefile){
        cout << "protocol: " << msg.getprotocol() << " name: " << msg.getname()
             << " Fname: " << msg.getdest() << " data: " << msg.getdata()
             << endl;
        // 判读用户是否有接受文件的权限
        string data = msg.getdata();
        File file(data);
        string filepath = FILEPATH + "/" + file.getname();
        ifstream stream(filepath);
        if(!stream){
            stream.close();
            tcp1.sendMsg("F");
        }else{
            stream.seekg(0, std::ios::end);
            tcp1.sendMsg("FILE:" + to_string(stream.tellg()));
            tcp1.sendFile(filepath, file.gettotalRecords());
            stream.close();
        }

    } else if (msg.getprotocol() == Friend_Chat ||msg.getprotocol() == Group_Chat) {

        string key;
        vector<string> number;
        if(msg.getprotocol()==Friend_Chat){
            
            //判断是否是好友
            int type1 = redis.isSet(msg.getname() + "F", msg.getdest());
            if(type1 <= 0){
                tcp1.sendMsg("OFF:You are not friends yet!!!");
                return;
            }

            //是否被拉黑
            int type = redis.isSet(msg.getdest() + "B", msg.getname());
            if(type <=0){
                tcp1.sendMsg("OFF:You've been blocked!!!");
                return;
            }

            key = gethiskey(msg.getname(), msg.getdest());
            number.push_back(msg.getdest());

        }else{
            key = gethiskey(msg.getdest(), "");
            number = getGroupNumber(msg.getdest(),msg.getname());
        }
        redis.addList(key, msg.getname() + ":" + msg.getdata());

        for(auto& ptr:number){
            if (Onlineuser.find(ptr) != Onlineuser.end()){
                int dest = Onlineuser[ptr].first;
                TcpSocket tcp(dest);
                string name = Onlineuser[ptr].second; // 当前用户正在通信的用户
                if(redis.isSet("Groups", name) >0){
                     string name1 = Onlineuser[msg.getname()].second;
                     if(!name1.compare(name))
                         name = msg.getname();
                }
                if (!name.compare(msg.getname()))
                     tcp.sendMsg("ON:" + msg.getname() + "+" + msg.getdata());
                else
                     tcp.sendMsg("OFF:" + msg.getname() + " sent you a message ");
            }
        }
        //既有任务的处理又有对不同身份人的消息发送
    } else if (msg.getprotocol() < 10) {
        vector<string> number;
        cout << "protocol: " << msg.getprotocol() << " name: " << msg.getname() << " Fname: " << msg.getdest() << " data: " << msg.getdata() <<endl;
        string result = funcMap[msg.getprotocol()](msg.getname(), msg.getdest()+":"+msg.getdata());
        cout << "return :" << result << endl;
        tcp1.sendMsg(":"+result);

        if (redis.isSet("Groups", msg.getdest()) && msg.getprotocol() == Packet_Apply)
            number = getGroupadmin(msg.getdest());
        else
            number.push_back(msg.getdest());

        for (auto ptr : number)
        {
            if (Onlineuser.find(ptr) != Onlineuser.end() && !result.compare("T"))
            {
                int dest = Onlineuser[ptr].first;
                TcpSocket tcp2(dest);
                switch (msg.getprotocol())
                {
                case Packet_Apply:
                     tcp2.sendMsg("OFF:you got a application from " + msg.getname());
                     break;
                case Group_AddMan:
                     tcp2.sendMsg("OFF:you are set as the adminnistrator of group " + msg.getname());
                     break;
                case Group_DelMan:
                     tcp2.sendMsg("OFF:you were removed as adminnistrator of group " + msg.getname());
                     break;
                case Friend_ReApply:
                     tcp2.sendMsg("OFF:" + msg.getname() + " have been added as a friend");
                     break;
                case Group_ReApply:
                     tcp2.sendMsg("OFF:" + msg.getname() + " has approved your membership");
                     break;
                case Packet_sendfile:
                    tcp2.sendMsg("OFF: " + msg.getname() + "sent you a file");
                }
            }
        }
        //单纯的任务处理
    } else {
        cout << "protocol: " << msg.getprotocol() << " name: " << msg.getname() << " data: " << msg.getdata() << endl;
        string result = funcMap[msg.getprotocol()](msg.getname(), msg.getdata());
        cout << "return :" << result << endl;

        //将用户加入当前现在在线列表中
        if(msg.getprotocol()==User_Verify && !result.compare("T")){
            Onlineuser.insert({msg.getname(), make_pair(src,"")});
            cout << "Online:" << msg.getname() << endl;
        }
        tcp1.sendMsg(":" + result);
    }
}
