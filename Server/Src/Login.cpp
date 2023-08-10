#include "Login.hpp"
#include <iostream>
#include "../../Common/Redis.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/Account.hpp"
#include <map>
extern Redis redis;
extern map<string,pair<int,string>> Onlineuser;

string verify(string name,string data)
{
    int type = redis.isKey(name);
    if(type <=0)
        return string("F");
    string passwd = redis.getHash(name, "passwd");
    if (!data.compare(passwd))
        return string("T");
    else
        return string("F");
}

string Apply(string name, string data)
{
    auto pos = data.find(":");
    string Oname = data.substr(0, pos);
    if(!Oname.compare(name))
        return string("N");
    int type1 = redis.isSet("Friends", Oname);
    int type2 = redis.isSet("Groups", Oname);
    if (type1 <= 0 && type2 <= 0)
        return string("P"); //账户不存在
    int type5 = redis.isSet(name + "F", Oname);
    if(type5 >0)
        return string("A");//已经是好友了
    int type3 = redis.isSet(Oname + "Q", name);
    if (type3>0)
        return string("E"); //已经发送过
    int type4 = redis.addSet(Oname + "Q", name);
    if (type4 > 0)
        return string("T");
    else
        return string("F");
}

string findpasswd(string name,string data)
{
    string myitbo = redis.getHash(name, "myitbo");
    if(!myitbo.compare(data))
    {
        string passwd = redis.getHash(name, "passwd");
        return passwd;
    }
    else
        return string("F");
}

string UserCreat(string name, string data)
{
    int type = redis.isSet("Friends", name);
    if(type >0)
        return string("P");
    Account user(data);
    int type1 = redis.setHash(name, "name", user.getname());
    int type2 = redis.setHash(name, "passwd", user.getpasswd());
    int type3 = redis.setHash(name, "myitbo", user.getmyitbo());
    int type4 = redis.addSet("Friends", name);
    if ((type1 < 0) && (type2 < 0) && (type3 < 0))
        return string("F");
    else
        return string("T");
}

string UserDelete(string name,string data="")
{
    int type = redis.delAll(name);
    if(type<0)
        return string("F");
    else
        return string("T");
}

string GetSetAll(string name,string temp = "")
{
    vector<string> data;
    redisReply *reply = redis.getAllSet(name);
    for (int i = 0; i < reply->elements;i++)
    {
        data.push_back(reply->element[i]->str);
    }

    json Packet = data;
    return Packet.dump(3);
}

string history(string name,string data)
{
    //判断是否是好友
    int type = redis.isSet(name + "F",data);
    if(type <= 0)
        return string("F");

    //加入当前用户正在通信的名单
    Onlineuser[name].second = data;

    string key = gethiskey(name, data);
    vector<string> history;
    redisReply *reply = redis.getAlllist(key);

    for (int i = 0; i < reply->elements; i++)
    {
        history.push_back(reply->element[i]->str);
    }
    json Packet = history;
    freeReplyObject(reply);
    return Packet.dump(3);
}

string gethiskey(string name1, string name2)
{
    string temp;
    if (name1 < name2)
        swap(name1, name2);
    hash<string> hasher;
    string combinedStr = name1 + name2;
    size_t hashValue = hasher(combinedStr);
    string key = to_string(hashValue);
    key = key.substr(0, 5);
    return key;
}

string exitchat(string name, string data="")
{
    Onlineuser[name].second = "";
    return string("T");
}
