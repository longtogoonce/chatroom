#ifndef GROUP_UI_H_
#define GROUP_UI_H_
#include <string>
#include "../../Common/Account.hpp"
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
//查看群聊申请
void Group_UI_QueryApply(string name);

#endif