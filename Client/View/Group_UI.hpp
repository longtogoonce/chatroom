#ifndef GROUP_UI_H_
#define GROUP_UI_H_
#include <string>
using namespace std;

/*
 *  查看已经加入的群组-----放在Group_UI_MgtEntry界面
 *  管理自己的群----------群主对群组管理人员的添加与删除
 *                      管理人员对普通用户的管理
 */


//初始群界面
void Group_UI_initEntry();
//群管理界面
void Group_UI_MgtEntry();
//群聊界面
void Group_UI_ChatEntry();


// 创建群组
int Group_Srv_CreatGroup();
//解散群组
int Group_Srv_DelGroup(string name);
//加入群组
int Group_Srv_AddGroup(string name);
//退出群组
int Group_Srv_ExitGroup(string name);

//增加/删除管理员
//批准加入/移除
//查看成员列表

#endif