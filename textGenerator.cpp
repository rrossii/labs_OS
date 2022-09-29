#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>

using namespace std;
const string possibleWords =
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             " ,.";
int main() {
    int size;
    cin >> size;
    fstream output(R"(C:\Users\annro\CLionProjects\lab3_OS\text.txt)",std::ios::out);

    srand(time(nullptr));
    string str;
    for (int i = 0; i < size; i++) {
        str += possibleWords[rand() % possibleWords.size() - 1];
    }
    output << str;
    output.close();
}
