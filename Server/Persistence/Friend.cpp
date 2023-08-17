#include "Friend.hpp"
#include <vector>
#include <nlohmann/json.hpp>
#include "../../Common/Account.hpp"
#include "../../Common/Redis.hpp"

extern Redis redis;
using json = nlohmann::json;

extern map<string, int> Onlineuser;

string Fri_Delete(string name, string Fname)
{
    if(!name.compare(Fname))
        return string("P");
    int type = redis.isSet("Friends", Fname);
    if(type <=0)
        return string("F");
    int type1 = redis.delSet(name + "F", Fname);
    int type2 = redis.delSet(Fname + "F", name);
    int type3 = redis.delSet(name + "B", Fname);
    int type4 = redis.delSet(Fname + "B", name);
    if (type1 < 0|| type2 <0 || type3 <0 || type4 <0)
        return string("F");
    else
        return string("T");
}

string Fri_GetAll(string name,string fname="")
{
    vector<string> friends;
    redisReply *data = redis.getAllSet(name+"F");
    for (int i = 0; i < data->elements; i++)
    {
        string OnLinestate = "OFF";
        string Blackstate = "ON";
        string Fname = data->element[i]->str;
        if(Onlineuser.find(Fname)!=Onlineuser.end())
           OnLinestate = "ON";
        if(redis.isSet(name + "B", Fname)>0)
           Blackstate = "OFF";
        Friend friend_obj(Fname, OnLinestate, Blackstate);
        string Friendstring = friend_obj.tojson();
        friends.push_back(Friendstring);
    }

    json Packet = friends;
    freeReplyObject(data);
    return Packet.dump(3);
}

string Fri_Black(string name, string Fname)
{
    if(!name.compare(Fname))
        return string("P");
    int type = redis.delSet(name + "B", Fname);
    if(type <=0)
        return string("F");
    else
        return string("T");
}

string Fri_NoBlack(string name, string Fname)
{
    if(!name.compare(Fname))
        return string("P");
    int type = redis.addSet(name + "B", Fname);
    if(type <=0)
        return string("F");
    else
        return string("T");
}

string Fri_ReApply(string name, string data)
{
    auto pos = data.find(":");
    string Fname = data.substr(0,pos);
    string reply = data.substr(pos + 1);
    redis.delSet(name+"Q",Fname);
    if (!reply.compare("Yes"))
    {
        int type1 = redis.addSet(name + "F", Fname);
        int type2 = redis.addSet(name + "B", Fname);
        int type3 = redis.addSet(Fname + "F", name);
        int type4 = redis.addSet(Fname + "B", name);
        if (type1 < 0 || type2 < 0 || type3 < 0 || type4 < 0)
           return string("F");
        else
           return string("T");
    }
    return string("F");
}