#ifndef GROUP_H_
#define GROUP_H_
#include <string>
#include <vector>
#include "../../Common/Message.hpp"
using namespace std;

// 创建群组
void Group_Srv_CreatGroup();
//解散群组
int Group_Srv_DelGroup(string name);
//加入群组
void Group_Srv_AddGroup();    
//退出群组
int Group_Srv_ExitGroup(string name);
//获取群列表
vector<string> Group_Srv_GetList(string name);
//查看群聊申请
vector<string> Group_Srv_GetApply(string name);
//删除用户
void Group_Srv_Deluser(string name);
//添加管理者
void Group_Srv_AddManager(string name);
//移除管理者
void Group_Srv_DelManager(string name);
//判断群是否存在，存在则获取群主
string Group_Srv_isExists(string name);
//获取历史记录
vector<string> Group_Srv_history(string Gname);

#endif