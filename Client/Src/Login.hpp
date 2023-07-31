#ifndef LOGIN_H_
#define LOGIN_H_
#include <string>
#include "../../Common/Account.hpp"

//验证用户是否存在
int Login_Srv_Verify(std::string UID, std::string passwd);
//添加用户
int Login_Srv_Add(Account user);
//找回密码
int Login_Srv_FindPasswd(std::string name,std::string myitbo,std::string& passwd);
//删除用户
int DelAccount();

#endif