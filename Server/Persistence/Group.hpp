#ifndef GROUP_H_
#define GROUP_H_
#include <string>
#include <vector>
#include "../../Common/Message.hpp"
using namespace std;


//加入群组
string GroupReApply(string name,string Gname);
//创建一个群
string GroupCreat(string name, string Gname);
//删除一个群
string GroupDelete(string name, string Gname);
//退出一个群
string GroupExit(string name, string Gname);
//增加管理者
string GroupAddMan(string Gname, string data);
//删除管理员
string GroupDelMan(string Gname, string data);
//删除用户
string GroupExituser(string Gname, string data);
//判读群是否存在，如存在，则返回群主
string GroupIsExist(string name, string data);
//获取全部成员
vector<string> getGroupNumber(string Gname,string name);
//获取该群的管理员
vector<string> getGroupadmin(string name);

#endif