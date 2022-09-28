#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
using namespace std;

typedef long long ll;

string readTextFromFile(const string &fileName);
void saveResToFile(const string &fileName, const string &longestSentence, ll exeTime);
string findLongestSentence(const string &text);
string divideText(const string &text, int numThreads);
VOID multiThread(int numThreads, const string &divText);

int main() {
    auto start = chrono::high_resolution_clock::now();

    string inputFile = R"(C:\Users\annro\CLionProjects\lab3_OS\text.txt)";
    string outputFile = R"(C:\Users\annro\CLionProjects\lab3_OS\output.txt)";

    cout << "Input a number of threads (2, 4, 8, 20, 100, 1000):\n";
    int numThreads = 1;
    cin >> numThreads;

//    multiThread(numThreads);

    string text;
    text = readTextFromFile(inputFile);

    string longestSentence;
    longestSentence = findLongestSentence(text);

    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds> (end-start);
    saveResToFile(outputFile, longestSentence, duration.count());

	return 0;
}


string findLongestSentence(const string &text) {
    string longestSentence;
    int wordsCount = 0, wc = 0;
    int start = 0, end = 0;

    for (int i = 0; i < text.size(); i++) {
        if (text[i] == '.') {
            if (i > 0 && text[i-1] != ' ') {
                wordsCount++;
            }

            if (wordsCount > wc) {
                end = i - start + 1; // number of chars
                longestSentence = text.substr(start, end);
                wc = wordsCount;
            }
            start = i+1;
            wordsCount = 0;

        } else if (i > 0 && text[i] == ' ' && text[i-1] != '.') {
            wordsCount++;
        }
    }
    return longestSentence;
}

string readTextFromFile(const string &fileName) {
    ostringstream ss;
    fstream input;
    input.open(fileName);

    if (!input.is_open()) {
        cout << "Cannot open the file! - " << fileName << "!\n";
        exit(EXIT_FAILURE); // ?
    }

    ss << input.rdbuf();
    input.close();
    return ss.str();
}

void saveResToFile(const string &fileName, const string &longestSentence, ll exeTime) {
    fstream output;
    output.open(fileName);

    output << "The longest sentence in " << fileName << " is " << longestSentence << '\n';
    output << "Execution time: " << exeTime << "ms\n";
    output.close();
}