#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include <ctime>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

//定义系统用户类，描述用户账户信息-----------------------------------------------------------
class Account
{
private:
    string UID;     // 用户标识符
    string name;    // 名字
    string passwd;  //密码
    string myitbo;  //密保
    string tele; // 手机号
    
public:
    Account()=default;
    Account(string& jsonstring)
    {
        json Packet = json::parse(jsonstring);
        UID = Packet["UID"];
        name = Packet["name"];
        passwd = Packet["passwd"];
        myitbo = Packet["myitbo"];
    }
    string tojson()
    {
        json Packet;
        Packet["UID"] = UID;
        Packet["name"] = name;
        Packet["passwd"] = passwd;
        Packet["myitbo"] = myitbo;

        string userstring = Packet.dump(7);
        return userstring;
    }

    string getUID(){
        return UID;
    }
    string getname(){
        return name;
    }
    string getpasswd(){
        return passwd;
    }
    void setUID(string UID){
        this->UID = UID;
    }
    string getmyitbo(){
        return myitbo;
    }
   void setname(string name){
       this->name = name;
   }
   void setpasswd(string passwd){
       this->passwd = passwd;
   }
   void settele(string tele){
       this->tele = tele;
   }
    void setmyitbo(string myitbo){
       this->myitbo = myitbo;
    }
};

//定义好友状态类-------------------------------------------------------------------
class Friend
{
private:
    string name;    //姓名
    string OnLinestate;    //在线状态
    string Blackstate;     //拉黑状态
public:
    Friend() = default;
    Friend(string name_, string Online, string Black) : name(name_),
                                                  OnLinestate(Online), Blackstate(Black){};
    Friend(string &jsonstring)
    {
        json Packet = json::parse(jsonstring);
        name = Packet["name"];
        OnLinestate = Packet["OnLinestate"];
        Blackstate = Packet["Blackstate"];
    }

    string tojson()
    {
         json Packet;
         Packet["name"] = name;
         Packet["OnLinestate"] = OnLinestate;
         Packet["Blackstate"] = Blackstate;

         string Friendstring = Packet.dump(3);
         return Friendstring;
    }

    string getname()
    {
        return name;
    }
    string getOnLinestate(){
        return OnLinestate;
    }
    string getBlackstate(){
        return Blackstate;
    }
};


//定义系统群组类----------------------------------------------------------------------
/*
class Group
{
private:
    string g_name;  //群名称
    string g_UID;   //群UID
    string owner_name;  //群主名称
    string admin_name;    //管理员索引
    string num_name; //群成员索引

public:
    Group(string g_name,string owner_name);
    Group(string &json);
    string tojson();

    string getname(){
        return g_name;
    }
};
Group::Group(string g_name,string owner_name)
{
    time_t timestamp;
    time(&timestamp);
    g_UID = to_string(timestamp);
    this->g_name = g_name;
    this->owner_name = owner_name;
    admin_name = g_UID + "A";
    num_name = g_UID + "N";
}

Group::Group(string &jsonstring)
{
    json Packet = json::parse(jsonstring);
    g_name = Packet["g_name"];
    g_UID = Packet["g_UID"];
    owner_name = Packet["owner_name"];
    admin_name = Packet["CEO_name"];
    num_name = Packet["num_name"];
}

string Group::tojson()
{
    json Packet;
    Packet["g_name"] = g_name;
    Packet["g_UID"] = g_UID;
    Packet["owner_name"] = owner_name;
    Packet["CEO_name"] = admin_name;
    Packet["num_name"] = num_name;

    string groupstring = Packet.dump(7);
    return groupstring;
}
    */
#endif