#include <nlohmann/json.hpp>
#include <vector>
#include "../../Common/Account.hpp"
#include "../../Common/Redis.hpp"

extern Redis redis;
using json = nlohmann::json;

string GroupCreat(string name ,string Gname)
{
    int type = redis.isSet("Friends", Gname);
    int type4 = redis.isSet("Groups", Gname);
    if (type > 0 || type4 > 0 )
        return string("P");
    int type1 = redis.addkey(Gname, name);
    int type2 = redis.addSet(name + "C", Gname);
    int type3 = redis.addSet("Groups", Gname);
    if (type1 < 0 || type2 < 0 || type3 < 0 )
        return string("F");
    else
        return string("T");
}

string GroupDelete(string name,string Gname)
{
    string Owner = redis.getkey(Gname);
    if (Owner.compare(name))
        return string("P");

    //从加过该群的人中删除该群
    redis.delSet(Gname, name);
    redis.delSet(name + "C", Gname);
    redisReply *replyM = redis.getAllSet(Gname + "M");
    for (int i = 0; i < replyM->elements; i++)
    {
        string name = replyM->element[i]->str;
        redis.delSet(name + "M",Gname);
    }
    freeReplyObject(replyM);
    redisReply *reply = redis.getAllSet(Gname+"A");
    for (int i = 0; i < reply->elements; i++)
    {
        string name = reply->element[i]->str;
        redis.delSet(name + "A",Gname);
    }
    freeReplyObject(reply);
    // 删除该群的信息
    int type1 = redis.delkey(Gname);
    int type2 = redis.delkey(Gname + "M");
    int type3 = redis.delkey(Gname + "A");
    int type4 = redis.delSet("Groups", Gname);
    
    if (type1<0 || type2<0 || type3<0 || type4<0)
        return string("F");
    else
        return string("T");
}

string GroupExit(string name,string Gname)
{
    string data = redis.getkey(Gname);
    if(!name.compare(data))
        return GroupDelete(name, Gname);

    if(redis.isSet(Gname+"M",name)>0)
    {
        redis.delSet(Gname + "M", name);
        redis.delSet(name + "M", Gname);
    }
    else if(redis.isSet(Gname+"A",name)>0)
    {
        redis.delSet(Gname + "A", name);
        redis.delSet(name + "A", Gname);
    }
    else
    {
        return string("F");
    }
    return string("T");
}

string GroupReApply(string Gname,string data)
{
    auto pos = data.find(":");
    string name = data.substr(0,pos);
    string reply = data.substr(pos + 1);
    redis.delSet(Gname + "Q", name);
    if (!reply.compare("Yes"))
    {
        int type1 = redis.addSet(Gname + "A", name);
        int type2 = redis.addSet(name + "A", Gname);
        if (type1 < 0 || type2 < 0)
            return string("F");
        else
            return string("T");
    }
    return string("F");
}

string GroupAddMan(string Gname, string data)
{
    auto pos = data.find(":");
    string Oname = data.substr(0, pos);
    string name = data.substr(pos + 1);
    string Owner = redis.getkey(Gname);
    if(!redis.isSet(Gname+"A",Oname))
        return string("N");

    if(!name.compare(Owner)){
        int type1 = redis.delSet(Gname + "A", Oname);
        int type2 = redis.delSet(Oname + "A", Gname);
        int type3 = redis.addSet(Gname + "M", Oname);
        int type4 = redis.addSet(Oname + "M", Gname);
        if(type4 <0 || type3 <0)
            return string("F");
        else
            return string("T");
    }
    else
        return string("P");
}

string GroupDelMan(string Gname, string data)
{
    auto pos = data.find(":");
    string Oname = data.substr(0, pos);
    string name = data.substr(pos + 1);
    string Owner = redis.getkey(Gname);
    if(!redis.isSet(Gname+"M",Oname))
        return string("N");
    if(!name.compare(Owner)){
        int type1 = redis.delSet(Gname + "M", Oname);
        int type2 = redis.delSet(Oname + "M", Gname);
        int type3 = redis.addSet(Gname + "A", Oname);
        int type4 = redis.addSet(Oname + "A", Gname);
        if(type1 <0 || type3 <0)
            return string("F");
        else
            return string("T");
    }
    else
        return string("P");
}

string GroupExituser(string Gname, string data)
{
    auto pos = data.find(":");
    string name = data.substr(0, pos);
    string Oname = data.substr(pos + 1);
    string admin = redis.getkey(Gname);
    if(!name.compare(Oname))
        return string("F");
    if (redis.isSet(Gname + "A", Oname))
    {
        if(!admin.compare(name) || redis.isSet(Gname+"M",name)){
            redis.delSet(Gname + "A", Oname);
            redis.delSet(Oname + "A", Gname);
            return string("T");
        }else
            return string("P");
    }
    else if (redis.isSet(Gname + "M", Oname))
    {
        if(!admin.compare(name)){
            redis.delSet(Gname + "M", Oname);
            redis.delSet(Oname + "M", Gname);
            return string("T");
        }else
            return string("P");
    }
    else
        return string("F");
}

string GroupIsExist(string name, string data)
{
    int type1 = redis.isSet(name + "C", data);
    if(type1 > 0)
        return string(name);
    int type2 = redis.isSet(name + "M", data);
    int type3 = redis.isSet(name + "A", data);
    if(type2 > 0 || type3 > 0 ){
        string Owner = redis.getkey(data);
        return string(Owner);
    }else
        return string("");
}

vector<string> getGroupNumber(string Gname,string name)
{
    vector<string> number;
    string admin = redis.getkey(Gname);
    if(admin.compare(name))
        number.push_back(admin);
    redisReply *data1 = redis.getAllSet(Gname + "M");
    for (int i = 0; i < data1->elements; i++)
    {
        if(name.compare(string(data1->element[i]->str)))
            number.push_back(data1->element[i]->str);
    }
    redisReply *data2 = redis.getAllSet(Gname + "A");
    for (int i = 0; i < data2->elements;i++)
    {
         if(name.compare(string(data2->element[i]->str)))
            number.push_back(data2->element[i]->str);
    }
    freeReplyObject(data1);
    freeReplyObject(data2);
    return number;
}

vector<string> getGroupadmin(string Gname)
{
    vector<string> number;
    string admin = redis.getkey(Gname);
    number.push_back(admin);
    redisReply *data1 = redis.getAllSet(Gname + "M");
    for (int i = 0; i < data1->elements; i++)
    {
       number.push_back(data1->element[i]->str);
    }

    freeReplyObject(data1);
    return number;
}