#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <chrono>
using namespace std;

DWORD WINAPI thread1(LPVOID);
DWORD WINAPI thread2(LPVOID);
DWORD WINAPI thread3(LPVOID);
DWORD WINAPI thread4(LPVOID);
DWORD WINAPI thread5(LPVOID);

int main()
{
    auto start = chrono::high_resolution_clock::now();
    thread1(NULL);
    thread2(NULL);
    thread3(NULL);
    thread4(NULL);
    thread5(NULL);

//    HANDLE threads[5];
//    threads[0] = CreateThread(NULL,
//                              0,
//                              thread1,
//                              NULL,
//                              0, NULL);
//    threads[1] = CreateThread(NULL,
//                              0,
//                              thread2,
//                              NULL,
//                              0, NULL);
//    threads[2] = CreateThread(NULL,
//                              0,
//                              thread3,
//                              NULL,
//                              0, NULL);
//    threads[3] = CreateThread(NULL,
//                              0,
//                              thread4,
//                              NULL,
//                              0, NULL);
//    threads[4] = CreateThread(NULL,
//                              0,
//                              thread5,
//                              NULL,
//                              0, NULL);
//
//    WaitForMultipleObjects(5, threads, TRUE, INFINITE);
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds> (end-start);
    cout << duration.count();
    return 0;
}

DWORD WINAPI thread1(LPVOID t)
{
    cout << "First thread\n";
    return 0;
}
DWORD WINAPI thread2(LPVOID t)
{
    cout << "Second thread\n";
    return 0;
}
DWORD WINAPI thread3(LPVOID t)
{
    cout << "Third thread\n";
    return 0;
}
DWORD WINAPI thread4(LPVOID t)
{
    cout << "Fourth thread\n";
    return 0;
}
DWORD WINAPI thread5(LPVOID t)
{
    cout << "Fifth thread\n";
    return 0;
}
