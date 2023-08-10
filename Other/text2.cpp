#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "../Common/PutFormat.hpp"

// Define a global message queue
std::queue<std::string> messageQueue;
std::mutex mtx;
std::condition_variable cv;
PutFormat put;

// Function to handle user input
void userInputThread() {
    std::string input;
    while (true) {
       
        std::getline(std::cin, input);
        messageQueue.push(input);
        cv.notify_one();
    }
}

// Function to generate random strings every 2 seconds
void randomStringGeneratorThread() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> charDistribution(97, 122); // ASCII values for lowercase letters

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::string randomString;
        for (int i = 0; i < 10; ++i) {
            char randomChar = static_cast<char>(charDistribution(gen));
            randomString += randomChar;
        }

        std::lock_guard<std::mutex> lock(mtx);
        messageQueue.push(randomString);
        cv.notify_one();
    }
}

// Function to check if the cursor is at the first column
int main() {
    std::cout << "你们可以开始聊天了" << std::endl;
    std::cin.sync_with_stdio(false);
    // Create and start the threads
    std::thread userInputThreadObj(userInputThread);
    std::thread randomStringGeneratorThreadObj(randomStringGeneratorThread);

    // Main thread to process messages
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !messageQueue.empty(); });

        std::string message = messageQueue.front();
        messageQueue.pop();

        // Check if the cursor is at the first column
            //std::cout << "收到消息： " << message << std::endl;
            put.printFromRight(message, yellow, B_empty, highlight);
           std::cout << "\033[1A";  // 将光标移动到上一行
           std::cout << "\033[30X"; // 清空前10列
            std::cout << "\033[1E";
    }

    // The program will never reach this point, but we'll join the threads to prevent any issues.
    userInputThreadObj.join();
    randomStringGeneratorThreadObj.join();

    return 0;
}

