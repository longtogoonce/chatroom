#ifndef FRIEND_H_
#define FRIEND_H_
#include "../../Common/Account.hpp"
#include <string>
using namespace std;

//好友添加
void Friend_Srv_Apply();
//好友删除
void Friend_Srv_Delete();
//拉黑用户
void Friend_Srv_Black();
//解除拉黑
void Friend_Srv_NoBlack();
//获取用户的聊天状态+聊天对象
vector<string> Friend_Srv_history(string name);
// 获取所有的好友状态
vector<Friend> Friend_Srv_GetAllstate();
//获取好友申请列表
vector<string> Friend_Srv_GetApply();

#endif