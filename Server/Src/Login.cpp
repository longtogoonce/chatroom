#include "Login.hpp"
#include <iostream>
#include "fstream"
#include <sys/stat.h>
#include "../../Common/File.hpp"
#include "../../Common/Redis.hpp"
#include "../../Common/TcpSocket.hpp"
#include "../../Common/Account.hpp"
#include "../Persistence/Group.hpp"
#include <map>
#include <string>

extern Redis redis;
const string FILEPATH = "../My_File_Server";
extern map<string, pair<int, string>> Onlineuser;
extern map<int, pair<string,off_t>> Onlinefile;

string verify(string name,string data)
{
    int type = redis.isKey(name);
    if(type <=0)
      return string("F");
    if (Onlineuser.find(name) != Onlineuser.end())
      return string("P");
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
    int type6 = redis.isSet(name + "C", Oname);
    int type7 = redis.isSet(name + "M", Oname);
    int type8 = redis.isSet(name + "A", Oname);
    if (type5 > 0 || type6 > 0 || type7 >0 || type8 >0)
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
    int type1 = redis.isSet("Friends", name);
    if (type1 <= 0)
        return string("P");
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
    int type1 = redis.isSet("Friends", name);
    int type2 = redis.isSet("Groups",name);
    if(type1 >0 || type2 >0)
        return string("P");
    Account user(data);
    int type3 = redis.setHash(name, "name", user.getname());
    int type4 = redis.setHash(name, "passwd", user.getpasswd());
    int type5 = redis.setHash(name, "myitbo", user.getmyitbo());
    int type6 = redis.addSet("Friends", name);
    if ((type3 < 0) && (type4 < 0) && (type5 < 0))
        return string("F");
    else
        return string("T");
}

string UserDelete(string name, string data = "")
{
    //将用户的所有数据都删除
    redis.delkey(name);
    redis.delkey(name + "Q");

    redisReply *replyC = redis.getAllSet(name + "C");
    for (int i = 0; i < replyC->elements; i++)
    {
        string Gname = replyC->element[i]->str;
        GroupDelete(name, Gname);
    }
    freeReplyObject(replyC);
    redisReply *replyM = redis.getAllSet(name + "M");
    for (int i = 0; i < replyM->elements; i++)
    {
        string Gname = replyM->element[i]->str;
        GroupDelete(name, Gname);
    }
    freeReplyObject(replyM);
    redisReply *replyA = redis.getAllSet(name + "A");
    for (int i = 0; i < replyA->elements; i++)
    {
        string Gname = replyA->element[i]->str;
        GroupDelete(name, Gname);
    }
    freeReplyObject(replyA);
    redis.delkey(name + "C");
    redis.delkey(name + "M");
    redis.delkey(name + "A");
    redis.delkey(name + "B");
    redis.delkey(name + "F");
    redis.delkey(name + "L");
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

string history(string name,string Oname)
{
    //加入当前用户正在通信的名单
    Onlineuser[name].second = Oname;
    int type = redis.isSet("Groups", Oname);
    vector<string> history;
    if(type > 0){
        string key;
        if (type > 0)
            key = gethiskey(Oname, "");
        else
            key = gethiskey(name, Oname);
        redisReply *reply = redis.getAlllist(key);

        for (int i = 0; i < reply->elements; i++)
        {
            history.push_back(reply->element[i]->str);
        }
        freeReplyObject(reply);
    }
    json Packet = history;
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

string Recfile_info(string name, string data)
{
    //解析数据
    auto pos = data.find(":");
    string dest = data.substr(0, pos);
    string info = data.substr(pos + 1);
    File Curfile(info);
    int fd = Onlineuser[name].first;
    Onlinefile.insert(make_pair(fd,make_pair(Curfile.getname(),Curfile.gettotalRecords())));
    int type = redis.addSet(dest + "L", Curfile.getname());

    //建立文件与目录
    mkdir(FILEPATH.c_str(), 0777);
    string filepath = FILEPATH + "/" + Curfile.getname();
    ifstream stream(filepath);
    if(!stream){
        stream.close();
        return string("T+0");
    }
    stream.seekg(0, std::ios::end);
    int size = stream.tellg();
    stream.close();
    if (size < 0) {
        return string("F");
    }
    if(size == Curfile.gettotalRecords())
        return string("A");
    return string("T+" + to_string(size));
}

string Sendfile_info(string name, string data)
{
    File file(data);
    string filepath = FILEPATH + "/" + file.getname();
    ifstream stream(filepath);
    if(!stream){
        stream.close();
        return string("F");
    }
    stream.seekg(0, std::ios::end);
    int size = stream.tellg();
    stream.close();
    if (size < 0) {
        return string("F");
    }
    return string("FILE+" + to_string(size-file.gettotalRecords()));
}
