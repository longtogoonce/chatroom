#ifndef REDIS_H_
#define REDIS_H_

#include <hiredis/hiredis.h>
#include <string>
#include <iostream>
using namespace std;

/*
 *
 *        key的取名规范：
 *                 Account--name    （哈希）
 *                 好友申请--name+Q  （列表）
 *                 聊天消息--name+dest  （列表）
 *                聊天消息（群）--群name   （列表）
 *                 创建的群--name+C   （集合）
 *                 管理的群--name+M    （集合）
 *                 加入的群--name+A   （集合）
 *                    群主--群name      （key-value)
 *                 群管理员--群name+M  （集合）
 *                  群成员--群name+A   (集合)
 *                 白名单--name+B  (集合)
 *                 好友--name+F   （集合）
 */

class Redis
{
public:
    Redis();
    ~Redis();
//    int connectRedis(const char *ip,const int port);    // 连接数据库
    int addkey(const string &key,const string &value);  //添加string键值对
    string getkey(const string &key);   //获取string键值对
    int isKey(const string &key);   //指定的key是否存在
    int delkey(const string &key); // 删除键值
    int delAll(const string &key); //删除所有与key相关的键值

    //处理哈希表
    int delHash(const string &key, const string &field);    //删除对应值
    int setHash(const string &key, const string &field, const string &value);   // 插入哈希表
    int isHash(const string &key, const string &field); //查看指定字段的值是否存在
    string getHash(const string &key, const string &field); // 获取对应的值

    //处理列表
    int addList(const string &key, const string &value);    //插入列表中
    redisReply *getAlllist(const string &key);          // 获取所有的元素
    string getHeadList(const string &key);  //从列表头部获取元素
    string getTailList(const string &key); //从列表尾部获取元素

    //处理集合
    int Setlen(const string &key);  //返回集合中的元素个数
    int addSet(const string &key, const string &value); // 添加到集合中
    int delSet(const string &key, const string &value); //从集合中删除
    int isSet(const string &key, const string &value);  //判断是否在集合中
    redisReply *getAllSet(const string &key);          // 获取所有的元素

private:
    redisContext *connect;  //连接数据库返回的指针
    redisReply *reply;  //数据库返回的消息结构体指针
    string error;   //存储出错信息
};

inline Redis::Redis()
{
    reply = nullptr;
    error = "";

    connect = redisConnect("127.0.0.1", 6379);
    if(connect->err)
        error = connect->errstr;
}

inline Redis::~Redis()
{
    //redisFree(connect);
    //connect = nullptr;
}

/*
int Redis::connectRedis(const char* ip = "127.0.0.1",const int port = 6379)
{
    connect = redisConnect(ip, port);
    if(connect->err)
    {
        error = connect->errstr;
        return -1;
    }
    return 0;
}
*/

inline int Redis::addkey(const string &key,const string &value)
{
    string cmd = "set " + key + " " + value;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int type = reply->type;
    freeReplyObject(reply);
    return type;
}

inline int Redis::isKey(const string &key)
{
    string cmd = "exists  " + key;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int num = reply->integer;
    freeReplyObject(reply);
    return num;
}

inline string Redis::getkey(const string &key)
{
    string cmd = "get  " + key;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    string data = reply->str;
    freeReplyObject(reply);
    return data;
}

inline int Redis::delkey(const string &key)  //删除键值
{
    string cmd = "del " + key;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int type = reply->type;
    freeReplyObject(reply);
    return type;
}

inline int Redis::delAll(const string &key)
{
    string cmd = "scan 0 match " + key + "*";
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    if(reply->type == REDIS_REPLY_ARRAY) {
    for(int i = 0; i < reply->elements; i++) {
        delkey(reply->element[i]->str);
        }
    }
    freeReplyObject(reply);
    return 1;
}

inline int Redis::setHash(const string &key, const string &field, const string &value) //创建哈希表
{
    string cmd = "hset  " + key + " " + field + " " + value;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int type = reply->type;
    freeReplyObject(reply);
    return type;    //type表示处理结果的类类型
}

inline string Redis::getHash(const string &key, const string &field) //获取对应的数据
{
    string cmd = "hget  " + key + "  " + field;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    string data = reply->str;
    freeReplyObject(reply);
    return data;
}

inline int Redis::delHash(const string &key, const string &field)   //删除对应值
{
    string cmd = "hdel  " + key + "  " + field;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int type = reply->type;
    freeReplyObject(reply);
    return type;
}

inline int Redis::isHash(const string &key, const string &field) //查看指定字段的值是否存在
{
    string cmd = "hexists  " + key + "  " + field;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int num = reply->integer;
    freeReplyObject(reply);
    return num;
}

inline int Redis::addList(const string &key, const string &value)  //插入集合的尾部
{
    string cmd = "RPUSH " + key + " " + value;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int type = reply->type;
    freeReplyObject(reply); 
    return type;
}

inline string Redis::getHeadList(const string &key)  //从头部取出元素
{
    string cmd = "lpop " + key;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    string data = reply->str;
    freeReplyObject(reply);
    return data;
}

inline string Redis::getTailList(const string &key) //从列表头部获取元素
{
    string cmd = "rpop " + key;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    string data = reply->str;
    freeReplyObject(reply);
    return data;
}

inline redisReply *Redis::getAlllist(const string &key)      // 获取所有的元素
{
    string cmd = "lrange  " + key + "  0" + "  -1";
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    return reply;
}


inline int Redis::Setlen(const string &key)    //返回集合中的元素个数
{
    string cmd = "scard " + key;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int num = reply->integer;
    freeReplyObject(reply);
    return num;
}

inline int Redis::addSet(const string &key, const string &value)  //插入列表中
{
    string cmd = "sadd " + key + " " + value;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int type = reply->type;
    freeReplyObject(reply);
    return type;
}

inline int Redis::delSet(const string &key, const string &value) //从集合中删除
{
    string cmd = "srem " + key + " " + value;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int type = reply->type;
    freeReplyObject(reply);
    return type;
}

inline int Redis::isSet(const string &key, const string &value)  //判断是否在集合中
{
    string cmd = "sismember " + key + " " + value;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    int num = reply->integer;
    freeReplyObject(reply);
    return num;
}

inline redisReply *Redis::getAllSet(const string &key) // 获取所有的元素
{
    string cmd = "smembers  " + key;
    reply = (redisReply *)redisCommand(connect, cmd.c_str());
    return reply;
}

#endif