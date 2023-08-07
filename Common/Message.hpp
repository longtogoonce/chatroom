#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <nlohmann/json.hpp>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;
using json = nlohmann::json;

typedef enum
{
    Packet_Apply = 3,      // 发送验证消息  1.1
    Packet_history = 10, // 获取key列表的所有元素
    Packet_GetSetAll = 11,  // 获取key集合中所有元素
    Packet_exitchat = 25,   //退出聊天

    User_Verify = 12,     // 验证用户
    User_FindPasswd = 13, // 找回密码
    User_Creat = 14,      // 创建账户
    User_Delete = 15,     // 注销账户

    Friend_Delete = 16,   // 删除好友
    Friend_Black = 17,    // 拉黑好友
    Friend_NoBlack = 18, // 解除拉黑
    Friend_ReApply = 4, // 回复验证消息 2.1
    Friend_Chat = 1,    // 好友聊天 3.2
    Friend_GetAll = 19,  // 获取好友列表.状态

    Group_ReApply = 5,  // 回复验证消息 4.1
    Group_Creat = 20,    // 创建群 
    Group_Chat = 2,     // 群聊 5.2
    Group_Delete = 21,   // 删除群
    Group_Exit = 22,     // 退出群
    Group_ExitUser = 23, // 删除用户
    Group_AddMan = 6,   // 增加管理员 6.1
    Group_DelMan = 7,   // 删除管理员 7.1
    Group_IsExist = 24,  // 判断群是否存在
} Protocol;

class Message
{
private:
    Protocol protocol;   //协议号
    string name;         //发送者姓名
    string dest;         //接受者name
    string data;         //数据段
    string time;         //发送时间

public:
    Message(Protocol protocol_, string name_, string dest_, string data_) : protocol(protocol_), name(name_),
                                                                            dest(dest_), data(data_){};
    Message(string &jsonstring)
    {
        json Packet = json::parse(jsonstring);
        protocol = Packet["protocol"];
        name = Packet["name"];
        dest = Packet["dest"];
        data = Packet["data"];
        time = Packet["time"];
    }
    string gettime()
    {
        // 获取当前时间点
        auto now = chrono::system_clock::now();

        // 将时间点转换为 time_t 类型
        time_t time = chrono::system_clock::to_time_t(now);

        // 将 time_t 类型的时间转换为本地时间
        tm* localTime = std::localtime(&time);

        // 将本地时间转换为字符串
        ostringstream oss;
        oss << localTime->tm_year + 1900 << ":" << localTime->tm_mon + 1 << ":" << localTime->tm_mday;
        return oss.str();
    }
    string tojson()
    {
         json Packet;
         Packet["protocol"] = protocol;
         Packet["name"] = name;
         Packet["dest"] = dest;
         Packet["data"] = data;
         Packet["time"] = gettime();

         string jsonstring = Packet.dump(6);
         return jsonstring;
    }

    string getdest()
    {
        return dest;
    }
    string getname(){
        return name;
    }
    Protocol getprotocol(){
        return protocol;
    }
    string getdata(){
        return data;
    }
};

//消息队列----------------------------------------
class MessageQueue
{
public:
    MessageQueue()=default;
    
    void push(string data)
    {
        unique_lock<mutex> lock(mtx);
        Queues.push(data);
        lock.unlock();
        cv.notify_one();
    }
    string pop()
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]{ return !Queues.empty(); });
        string message = Queues.front();
        Queues.pop();
        return message;
    }

private:
    queue<string> Queues;
    mutex mtx;
    condition_variable cv;
};

#endif