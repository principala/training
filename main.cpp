//
//  main.cpp
//  cpptest
//
//  Created by Vladimir Yanov on 1/23/15.
//  Copyright (c) 2015 Vladimir Yanov. All rights reserved.
//

#include <iostream>
#include <thread>
#include <future>
#include <condition_variable>

struct Base
{
    char array[13];
    int myfun(int x, int y, int z)
    {
        int a = x + y + z;
        return a;
    }
    
    void fun1()
    {
    }
    void fun2()
    {
    }
};


std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_id (int id) {
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    // ...
    std::cout << "thread " << id << '\n';
}

void go() {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}

int main ()
{
    std::thread threads[10];
    // spawn 10 threads:
    for (int i=0; i<10; ++i)
        threads[i] = std::thread(print_id,i);
    
    std::cout << "10 threads ready to race...\n";
    go();                       // go!
    
    threads[0].join();
    threads[1].join();
    threads[2].join();
    
//    for (auto& th : threads)
//        th.join();
    
    return 0;
}