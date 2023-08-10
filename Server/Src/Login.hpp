#ifndef LOGIN_H_
#define LOGIN_H_
#include <string>
using namespace std;
//验证用户
string verify(string key,string data);
//找回密码
string findpasswd(string name,string data);
//添加用户
string UserCreat(string name, string data);
//删除用户
string UserDelete(string name, string data);
//获取set集合所有元素
string GetSetAll(string name, string temp);
//获取list列表所有元素
string history(string name, string temp);
//发送验证消息
string Apply(string name, string Oname);
//通过名称生成历史消息键值
string gethiskey(string name1, string name2);
//用户退出聊天界面
string exitchat(string name1, string data);
#endif