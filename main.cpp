#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>

using namespace std;

typedef long long ll;

string readTextFromFile(const string &fileName);
void saveResToFile(const string &fileName, const string &text, ll exeTime, size_t numberOfThreads);
DWORD WINAPI findLongestSentence(LPVOID threadData);
vector<string> divideTextIntoChunksOfSentences(const string &text, size_t parts);
VOID runCalculatingThreads(vector<string> &chunksOfSentences, int priority);
size_t CountWords(const string &sentence);
void RunNThreads(size_t numberOfThreads);
void deleteUnnecessarySymbols(string &text);
string LongestSentenceInVector(const vector<string> &sentences);
DWORD progressUpdateInPercents(size_t nowSizeOfVectorSentences, size_t finalSizeOfVectorSentences);
void limitThreads(size_t numOfThreads, vector<string> &chunksOfSentences);

vector<string> sentencesFoundByThreads;
HANDLE hMutex;
HANDLE hSemaphore;

uint64_t CurrentTimeMillis() {
    uint64_t ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    return ms;
}

int main() {
//    RunNThreads(1);
//    RunNThreads(2);
    //RunNThreads(4);
//    RunNThreads(8);
    RunNThreads(20);
//    RunNThreads(100);
//    RunNThreads(1000);
    return 0;
}

vector<string> divideTextIntoChunksOfSentences(const string &text, size_t parts) {
    vector<size_t> endOfSentence; // indexes of dots
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '.') {
            endOfSentence.emplace_back(i);
        }
    }

    vector<string> dividedText; // multiSentence

    size_t numberOfSentencesInText = endOfSentence.size();

    size_t startOfSentence = 0;
    int index = 0;
    size_t indexOfEndOfSentence = endOfSentence[index];
    if (endOfSentence.size() <= parts) {
        for (size_t i = 0; i < numberOfSentencesInText; i++) {
            size_t numOfCharsToEmplace = indexOfEndOfSentence - startOfSentence + 1;
            dividedText.emplace_back(text.substr(startOfSentence, numOfCharsToEmplace));

            startOfSentence = indexOfEndOfSentence + 1;
            index++;
            indexOfEndOfSentence = endOfSentence[index];
        }
    } else {
        index = -1; // -1 + numberOfSentencesInOneMultiSentence (на початку речення, коли індекс 0, рахуватиме неправильно)
        size_t numberOfSentencesInOneMultiSentence = numberOfSentencesInText / parts;
        size_t leftoverSentences = numberOfSentencesInText % parts;

        for (size_t i = 0; i < parts; i++) {
            if (index + 1 >= numberOfSentencesInText) {
                break;
            }
            string multiSentence;
            indexOfEndOfSentence = endOfSentence[index + numberOfSentencesInOneMultiSentence];
            size_t numOfCharsToEmplace = indexOfEndOfSentence - startOfSentence + 1;

            multiSentence += text.substr(startOfSentence, numOfCharsToEmplace);
            index += numberOfSentencesInOneMultiSentence;
            startOfSentence = indexOfEndOfSentence;

            dividedText.emplace_back(multiSentence);
        }

        for (size_t i = 0; i < leftoverSentences; i++) {
            if (index + 1 >= numberOfSentencesInText) {
                break;
            }
            string multiSentence;
            indexOfEndOfSentence = endOfSentence[index + 1];
            size_t numOfCharsToEmplace = indexOfEndOfSentence - startOfSentence + 1;

            multiSentence += text.substr(startOfSentence + 1, numOfCharsToEmplace); // was startOfSentence
            index++;
            startOfSentence = indexOfEndOfSentence;

            dividedText[i] += multiSentence;
        }
    }

    assert(dividedText.size() <= parts);
    return dividedText;
}

string LongestSentenceInVector(const vector<string> &sentences) {
    size_t maxWords = 0;
    string result;

    for (auto &sentence : sentences) {
        size_t currentCountWords = CountWords(sentence);
        if (currentCountWords > maxWords) {
            result = sentence;
            maxWords = currentCountWords;
        }
    }

    return result;
}

string FindLongestSentenceInText(const string &multiSentence) {
    vector<string> sentences;

    size_t startIndex = 0;
    for (size_t i = 0; i < multiSentence.size(); i++) {
        if (multiSentence[i] == '.') {
            size_t numOfCharsToEmplace = i - startIndex + 1;
            string sentence = multiSentence.substr(startIndex, numOfCharsToEmplace);
            sentences.emplace_back(sentence);

            startIndex = i+1;
        }
    }

    return LongestSentenceInVector(sentences);
}

size_t CountWords(const string &sentence) {
    size_t countWords = 0;

    for (int i = 1; i < sentence.size(); i++) {
        if (sentence[i] == ' ' || (sentence[i] == '.' && sentence[i-1] != ' ')) {
            countWords++;
        }
    }
    return countWords;
}

void deleteUnnecessarySymbols(string &text) {
    string::iterator iter = unique(text.begin(), text.end(), [](auto lhs, auto rhs){
        return lhs == rhs && lhs == ' '; // add later \n also
    });
    text.erase(iter, text.end() );
}

void RunNThreads(size_t numberOfThreads) {
//    string inputFile = R"(C:\Users\annro\CLionProjects\lab3_OS\text.txt)";
//    string inputFile = R"(C:\Users\annro\CLionProjects\lab3_OS\text1.txt)";
    string inputFile = R"(C:\Users\annro\CLionProjects\lab3_OS\text2.txt)";
    string text = readTextFromFile(inputFile);

    deleteUnnecessarySymbols(text);
    vector<string> chunksOfSentences = divideTextIntoChunksOfSentences(text, numberOfThreads);

    auto startTime = CurrentTimeMillis();

    runCalculatingThreads(chunksOfSentences, THREAD_PRIORITY_HIGHEST);

    auto endTime = CurrentTimeMillis();

    auto longest = LongestSentenceInVector(sentencesFoundByThreads);

    string outputFile = R"(C:\Users\annro\CLionProjects\lab3_OS\output.txt)";
    saveResToFile(outputFile, longest, endTime - startTime, numberOfThreads);
    sentencesFoundByThreads.clear();

    startTime = CurrentTimeMillis();

    size_t limitedNumOfThread = 5;
    limitThreads(limitedNumOfThread, chunksOfSentences);

    endTime = CurrentTimeMillis();

    longest = LongestSentenceInVector(sentencesFoundByThreads);

    saveResToFile(outputFile, longest, endTime - startTime, limitedNumOfThread);
    sentencesFoundByThreads.clear();
}

DWORD progressUpdateInPercents(size_t nowSizeOfVectorSentences, size_t finalSizeOfVectorSentences) {
    return (nowSizeOfVectorSentences * 100) / finalSizeOfVectorSentences;
}

struct SentenceAndThread {
    string sentence;
    size_t numberOfChunks{};
    size_t thread{};
};

void limitThreads(size_t numOfThreads, vector<string> &chunksOfSentences) {
    if (numOfThreads > chunksOfSentences.size()) {
        cerr << "Cannot divide text where the number of sentences (" << chunksOfSentences.size() << ") is less than number of threads (" << numOfThreads << ")\n";
        return;
    }

    HANDLE threads[numOfThreads];
    hSemaphore = CreateSemaphore(
            NULL,
            0, //or 1
            numOfThreads,
            NULL);

    size_t chunkSize = chunksOfSentences.size();
    size_t sentencesInChunk = chunkSize / numOfThreads;
    size_t k = 0;
    SentenceAndThread sentenceAndNumberOfThread;
    for (size_t i = 0; i < numOfThreads; i++) {
        string multiSentence;

        for (size_t j = k; j < sentencesInChunk; j++) {
            multiSentence += chunksOfSentences[j];
        }

        k = sentencesInChunk;
        sentencesInChunk += chunkSize / numOfThreads;

        if (chunkSize - sentencesInChunk < chunkSize / numOfThreads) {
            sentencesInChunk -= chunkSize / numOfThreads; // back
            for (size_t l = sentencesInChunk; l < chunkSize; l++) { // or sentInChunk+1
                multiSentence += chunksOfSentences[l];
            }
        }

        sentenceAndNumberOfThread.sentence = multiSentence;

        //WaitForSingleObject(hSemaphore, INFINITE);

        threads[i] = CreateThread(
                NULL,
                0,
                findLongestSentence,
                (LPVOID) &sentenceAndNumberOfThread,
                NULL,
                NULL
        );
        WaitForSingleObject(threads[i], INFINITE);

        //ReleaseSemaphore(hSemaphore, 1, NULL); // or not null
    }

    WaitForMultipleObjects(numOfThreads, threads, TRUE, INFINITE);

    for (size_t i = 0; i < numOfThreads; i++) {
        CloseHandle(threads[i]);
    }
    CloseHandle(hSemaphore);
}

DWORD WINAPI findLongestSentence(LPVOID threadData) {
    //WaitForSingleObject(hSemaphore, INFINITE); //not inf but 0L
    WaitForSingleObject(hMutex, INFINITE);
    struct SentenceAndThread *tData =(struct SentenceAndThread *) threadData;
    string text = tData->sentence;
    assert(tData);
    ReleaseMutex(hMutex);
//    size_t finalSizeOfVectorSentences = tData->numberOfChunks;
//    size_t thread = tData->thread;

    string longest = FindLongestSentenceInText(text); // the longest sentence in chunk
    delete tData;

   // ReleaseSemaphore(hSemaphore, 1, NULL);
    WaitForSingleObject(hMutex, INFINITE);

    sentencesFoundByThreads.push_back(longest);

    ReleaseMutex(hMutex);
    ExitThread(0);
}

VOID runCalculatingThreads(vector<string> &chunksOfSentences, int priority) {
    size_t numberOfChunks = chunksOfSentences.size();
    HANDLE threads[numberOfChunks];

    hMutex = CreateMutex(
            NULL,
            FALSE,
            "longestSentence");

    if (hMutex == NULL) {
        cout << "CreateMutex error: " << GetLastError() << '\n';
        exit(1);
    }

    SentenceAndThread sentenceAndNumberOfThread;
    sentenceAndNumberOfThread.numberOfChunks = numberOfChunks;
    for (size_t i = 0; i < numberOfChunks; i++) {
        sentenceAndNumberOfThread.sentence = chunksOfSentences[i];
        sentenceAndNumberOfThread.thread = i;

        threads[i] = CreateThread(NULL,
                                  0,
                                  findLongestSentence,
                                  (LPVOID) &sentenceAndNumberOfThread,
                                  NULL,
                                  NULL);

        if (threads[i] == NULL) {
            cout << GetLastError() << '\n';
            ExitProcess(3);
        }
        //SetThreadPriority(threads[i], priority);
    }

    bool threadsWorkFinished;
    do {
        WaitForSingleObject(hMutex, INFINITE);
        cout << "progress: " << progressUpdateInPercents(sentencesFoundByThreads.size(), chunksOfSentences.size())
             << "%\n";

        auto comparator = [](const string &current, const string &next){
            return current.size() > next.size();
        };
        string currentLongestSentence = *max_element(sentencesFoundByThreads.begin(), sentencesFoundByThreads.end(), comparator);
        cout << "The longest sentence by now: " << currentLongestSentence << "\n\n";
        threadsWorkFinished = sentencesFoundByThreads.size() >= chunksOfSentences.size();
        ReleaseMutex(hMutex);

        Sleep(10);
    } while (!threadsWorkFinished);

    WaitForMultipleObjects(numberOfChunks, threads, TRUE, INFINITE);
    cout << '\n';

    for (size_t i = 0; i < numberOfChunks; i++) {
        CloseHandle(threads[i]);
    }
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

void saveResToFile(const string &fileName, const string &text, ll exeTime, size_t numberOfThreads) {
    fstream output;
    output.open(fileName, std::ios_base::app);

    output << "Longest sentence size = [" << CountWords(text) << "]\nSentence = [" << text << "]\n";
    output << "Executing time for [" <<  numberOfThreads << "] threads is [" << exeTime << "]ms\n\n";

    output.close();
}