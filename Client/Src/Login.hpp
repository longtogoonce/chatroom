#ifndef LOGIN_H_
#define LOGIN_H_
#include <string>
#include <vector>
#include "../../Common/Account.hpp"

//验证用户是否存在
int Login_Srv_Verify(std::string UID, std::string passwd);
//添加用户
int Login_Srv_Add(Account user);
//找回密码
void Login_Srv_FindPasswd(std::string name,std::string myitbo);
//删除用户
int DelAccount();
//发送文件
void Send_File(string file_name, string dest_name);
//接受文件
void Recive_File(string file_name);
// 获取所有文件
vector<std::string> getfiles();

#endif