#include <iostream>
#include <string>
#include <algorithm>
#include "../Common/PutFormat.hpp"
using namespace std;

int main() {

    string choice;
    do
    {
    cout << "\t\t申请添加你为好友,[Yes/No]:";
        cin >> choice;
    cout << "\033[F\033[K";
    cout << "\033[F\033[K" << endl;
    } while (choice.compare("Yes") && choice.compare("No"));
    return 0;
}