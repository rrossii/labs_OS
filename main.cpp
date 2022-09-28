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
void saveResToFile(const string &fileName, ll exeTime);
DWORD WINAPI findLongestSentence(LPVOID param);
vector<string> divideText(const string &text, int numThreads);
VOID multiThread(int numThreads, const string &divText);

string longestSentence;

struct textStruct {
    string text;
};

int main() {
    auto start = chrono::high_resolution_clock::now();

    string inputFile = R"(C:\Users\annro\CLionProjects\lab3_OS\text.txt)";
    string outputFile = R"(C:\Users\annro\CLionProjects\lab3_OS\output.txt)";

//    cout << "Input a number of threads (2, 4, 8, 20, 100, 1000):\n";
//    int numThreads = 1;
    //cin >> numThreads;

    string text = readTextFromFile(inputFile);
    vector<string> dividedText = divideText(text, 4);

    cout << longestSentence;

    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds> (end-start);
    //cout << duration.count();
//    saveResToFile(outputFile, duration.count());

	return 0;
}
vector<string> divideText(const string &text, int numThreads) {
    size_t n = text.size();
    DWORD divIndex = n / numThreads, startIndex = 0, end = 0;
    vector<string> dividedText;

    while (numThreads >= 1) {

        if (text[divIndex] != '.') {
            auto dotIter = text.find_first_of('.', divIndex);
            end = dotIter - startIndex + 1;

            string divText = text.substr(startIndex, end);
            dividedText.emplace_back(divText);

            startIndex = dotIter + 1;
            divIndex = (dotIter + n) / numThreads;

        } else {
            end = divIndex - startIndex + 1;
            string divText = text.substr(startIndex, end);
            dividedText.emplace_back(divText);

            startIndex = divIndex + 1;
            divIndex = (divIndex + n) / numThreads;
        }
        numThreads--;
    }
    return dividedText;
}

DWORD WINAPI findLongestSentence(LPVOID param) {
    int wordsCount = 0, wc = 0;
    int start = 0, end = 0;

    std::string& text = *static_cast<std::string*>(param);

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
   ExitThread(0);
}

VOID multiThread(int numThreads, const string &divText) { // take out numThreads later
    HANDLE threads[numThreads];

    for (int i = 0; i < numThreads; i++) {
        threads[i] = CreateThread(NULL,
                                  0,
                                  findLongestSentence,
                                  (LPVOID) &divText,
                                  0,
                                  NULL);
    }
    WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);
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

void saveResToFile(const string &fileName, ll exeTime) {
    fstream output;
    output.open(fileName);

    output << "The longest sentence in " << fileName << " is " << longestSentence << '\n';
    output << "Execution time: " << exeTime << "ms\n";
    output.close();
}