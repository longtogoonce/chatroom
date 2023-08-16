#ifndef FILE_H_
#define FILE_H_

#include <iostream>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

class File {
private:
    string filename;     // 文件名称
    off_t offset;           // 当前的偏移量
    off_t totalRecords;     //当前文件字节个数

public:

 File() = default;
 ~File() = default;

 File(string filename_, off_t offset_, off_t total)
     : filename(filename_), offset(offset_),totalRecords(total){}
     
 File(string filepath, off_t offset_) : offset(offset_) {
     // 初始化数据与大小
     struct stat file_stat;
     if (stat(filepath.c_str(), &file_stat) == -1) {
         perror("perror");
     }
     totalRecords = file_stat.st_size;

     //从路径中获取名称
     auto pos = filepath.find_last_of("/\\");
     if(pos != string::npos)
         filename = filepath.substr(pos + 1);
     else
         filename = filepath;
 }

 File(string& jsonstring){
    json Packet = json::parse(jsonstring);
    filename = Packet["filename"];
    totalRecords = Packet["totalRecords"];
 }

 string tojson() { 
    json Packet;
    Packet["filename"] = filename;
    Packet["totalRecords"] = totalRecords;

    string filestring = Packet.dump(3);
    return filestring;
 }

 off_t gettotalRecords(){
    return totalRecords;
 }

 string getname(){
   return filename;
 }
 off_t getoffset(){
  return offset;
 }
 void settotal(string count){
  totalRecords = static_cast<off_t>(stoll(count));
 }
 void setoffset(off_t offset_){
  offset = offset_;
 }
 void setname(string file){
  filename = file;
 }

 /*float Recfile(string path,int fd){
  string filepath = path + "/" + filename;
  cout << filepath << endl;

  // 创建文件
  ofstream putfile(filepath);
  if (!putfile) {
    cout << "\t\t\t\t无法创建文件" << endl;
  }
  
  //接受文件
  char buff[1024];
  ssize_t byte;
  while ((byte = recv(fd, buff, sizeof(buff), 0)) > 0) {
    putfile.write(buff, byte);
    offset += byte;
  }
  if (byte == -1) {
      std::cerr << "Error receiving data." << std::endl;
  }
  float progress = static_cast<float>(offset) / (totalRecords);
  if(progress == 1)
    putfile.close();
  return progress;
 }
 */

 void Sendfile(string count,string path,int Socketfd){
  off_t offset = static_cast<off_t>(stoll(count));
  string filepath = path + filename;
  int fd = open(filepath.c_str(), O_RDONLY);
  if(fd == -1){
    cout << "\t\t\t\t打开文件失败" << endl;
  }

   while (offset< totalRecords) {
    ssize_t sent_bytes = sendfile(Socketfd, fd, &offset, totalRecords - offset);
    if (sent_bytes == -1) {
        cout << "\t\t发送文件失败" << endl;
        break;
    }
  }

 }
 };

#endif