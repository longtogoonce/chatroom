#ifndef PUTFORMAT_H_
#define PUTFORMAT_H_
#include <iostream>
#include <sys/ioctl.h>
#include <limits>
#include <unistd.h>
#include <string>
#include <termios.h>
#include <sstream>
using namespace std;

//输出颜色
typedef enum
{
    black = 30,
    red = 31,
    green = 32,
    yellow = 33,
    blue = 34,
    purple = 35,
    white = 37,
    color_empty = 39,
}PutColor; 

//背景颜色
typedef enum
{
    B_green = 42,
    B_yellow = 43,
    B_white = 47,
    B_empty = 49,
    B_purple = 45,
    B_blue = 44,
} BackColor;

 //格式
typedef enum
{
    type_empty = 0,
    highlight = 1,  // 高亮显示
    underscore = 4, // 下划线
}
TypeColor;

class PutFormat
{
private:
    int width;  //窗口宽度
    int height; //窗口高度
    int linenum;   //当前光标的行号
    int colume; //当前光标的列号
public:
    PutFormat()
    {
         int fd = STDOUT_FILENO;

         struct winsize size;

         // 使用 ioctl 函数获取窗口大小
         if (ioctl(fd, TIOCGWINSZ, &size) == -1 || size.ws_col == 0 || size.ws_row == 0)
         {
             width = 80;
             height = 24;
             return;
        }

        width = size.ws_col;
        height = size.ws_row;
    }
    void stdput(char &);  //规范输出模式
    void stdexit();     //用户退出
    void getCursorPosition();       // 获取当前位置
    void moveCursorToNextLine();    //移动到下一行

    void printFromRight(string text,PutColor color,BackColor bgcolor,TypeColor style);   // 右打印数据
    void printFromLeft(string text,PutColor color,BackColor bgcolor,TypeColor style);    // 左打印数据
    void printFrommid(string text, PutColor color, BackColor bgcolor, TypeColor style); //居中打印
    void printFrommid2(string text); //默认居中打印
    int getwidth()
    {
        return width;
    }
    int getheight(){
        return height;
    }
    int getlinenum(){
        return linenum;
    }
    int getcolume(){
        return colume;
    }
};

inline void PutFormat::stdput(char &choice){
    string line;
   
    while (true)
    {
        cin >> ws >> line;
        if (line.length() == 1)
             break;
        //cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');     
        cout << "\033[F\033[K"; // 返回上一行并清空该行
    }
    choice = line.c_str()[0];
    cout << "\033[F\033[K"; // 返回上一行并清空该行
}

inline void PutFormat::stdexit()
{
    cout << "\n"<< endl;
    printFrommid("Press any key to continue...", red, B_empty, underscore);
    while (getchar() != '\n')
        ;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

inline void PutFormat::printFromLeft(string text,PutColor color,BackColor bgcolor,TypeColor style){

  cout << "\033[" << style << ";" << color << ";" << bgcolor << "m" << text << "\033[0m" << endl;
}

inline void PutFormat::printFromRight(string text,PutColor color,BackColor bgcolor,TypeColor style) {

  // 计算输出文本的起始位置
  int start = width - text.length();

  // 输出空格填充起始位置
  for (int i = 0; i < start; i++) {
    cout << " ";
  }

  cout << "\033[" << style << ";" << color << ";" << bgcolor << "m" << text << "\033[0m" << endl;
  
}

inline void PutFormat::printFrommid(string text, PutColor color, BackColor bgcolor, TypeColor style)
{
  int len = text.length();
  int left = (width - len) / 2;
  cout << string(left, ' ') << "\033[" << style << ";" << color << ";" << bgcolor << "m" << text << "\033[0m" << endl;
}

inline void PutFormat::printFrommid2(string text)
{
    int len = text.length();
    int left = (width - len) / 2;
    cout << string(left, ' ') << text << endl;
}


inline void PutFormat::getCursorPosition()
{
     // 获取终端的属性
    struct termios term;
    tcgetattr(STDOUT_FILENO, &term);
    termios term2 = term;
    term2.c_lflag &= ~(ECHO | ICANON);  // 禁用回显和规范模式
    tcsetattr(STDOUT_FILENO, TCSANOW, &term2);

    // 发送获取光标位置的转义码
    cout << "\033[6n";
    fflush(stdout);  // 刷新输出缓冲区，确保转义码被发送

    // 从标准输入中读取光标位置的响应
    char response[32];
    int i = 0;
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 'R') {
            break;
        }
        response[i++] = c;
    }
    response[i] = '\0';

    // 解析光标位置的响应，获取行号和列号
    if (response[0] == '\033' && response[1] == '[') {
        sscanf(response, "\033[%d;%dR", &linenum, &colume);
    }

    // 恢复终端的属性
    tcsetattr(STDOUT_FILENO, TCSANOW, &term);
}

inline void PutFormat::moveCursorToNextLine()
{
    cout << "\033[" << linenum + 1 << ";" << colume << "H";
}

#endif
