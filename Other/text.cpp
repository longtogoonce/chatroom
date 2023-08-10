#include "../mychatroom/Common/PutFormat.hpp"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void printStyledText(string &text, PutColor color, BackColor bgColor, TypeColor style) {

    stringstream buf;
    buf << "\033[" << style << ";" << color << ";" << bgColor << "m" << text << "\033[0m";
    text = buf.str();
}

int main()
{
    PutFormat put;
    cout << put.getwidth() << endl;
    cout << put.getheight() << endl;
    put.printFromRight("YXT:",color_empty,B_empty,type_empty);
    string buf1 = "best wishes to you!";
    put.printFromRight(buf1,black,B_white,highlight);
    put.printFromLeft("xiaoming:",color_empty,B_empty,type_empty);
    string buf2 = "haha,thank you ^-^!";
    put.printFromLeft(buf2,black,B_yellow,highlight);
    put.printFrommid("press [q] to exit", red, B_empty, underscore);
    time_t Curtime;
    time(&Curtime);
    string UID = to_string(Curtime);
    string uid = UID.substr(UID.length() - 5);
    cout << "UID:" << uid << endl;

    while(1){
        string data;
        std::cout << "\033[s";
        getline(cin, data);
        if(!data.compare("q"))
            break;
        cout << "\033[1A\033[K";
        cout << "\033[1;1H";    //移动到第一行
        put.printFrommid(data,black,B_yellow,underscore);
        cout << "\033[1A\033[K";
        std::cout << "\033[u"; // u表示恢复光标位置
        sleep(2);
        
    }
    cout << "congratulation!!!" << endl;
}