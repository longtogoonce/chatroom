#ifndef FRIEND_H_
#define FRIEND_H_
#include <string>
using namespace std;

// 删除好友
string Fri_Delete(string,string);
//好友聊天
string Fri_Chat(string,string);
//获取好友的状态
string Fri_GetAll(string name, string Fname);
//验证消息的回复
string Fri_ReApply(string name, string data);
//移除拉黑名单
string Fri_NoBlack(string name, string Fname);
//加入拉黑名单
string Fri_Black(string name, string Fname);
#endif