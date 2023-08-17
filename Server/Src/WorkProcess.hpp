#ifndef WORKPROCESS_H_
#define WORKPROCESS_H_
#include <map>
#include "Login.hpp"
#include "../../Server/Persistence/Friend.hpp"
#include "../../Server/Persistence/Group.hpp"
#include "../../Common/Message.hpp"

map<Protocol, string (*)(string, string)> funcMap = {
    {User_Verify, &verify},
    {User_FindPasswd, &findpasswd},
    {User_Creat, &UserCreat},
    {User_Delete, &UserDelete},

    {Packet_Apply, &Apply},
    {Packet_GetSetAll, &GetSetAll},
    {Packet_history, &history},
    {Packet_exitchat,&exitchat},
    {Packet_sendfile,&Recfile_info},
    {Packet_recivefile,&Sendfile_info},

    {Friend_Delete, &Fri_Delete},
    {Friend_Black, &Fri_Black}, 
    {Friend_NoBlack, &Fri_NoBlack},
    {Friend_ReApply, &Fri_ReApply},
    {Friend_GetAll, &Fri_GetAll},

    {Group_Creat, &GroupCreat},
    {Group_Delete, &GroupDelete},
    {Group_Exit, &GroupExit},
    {Group_ReApply, &GroupReApply},
    {Group_AddMan, &GroupAddMan},
    {Group_DelMan, &GroupDelMan},
    {Group_ExitUser, &GroupExituser},
    {Group_IsExist, &GroupIsExist},
};

map<string,pair<int,string>> Onlineuser;
map<int, pair<string,off_t>> Onlinefile;

void WorkProcess(void *arg);

#endif