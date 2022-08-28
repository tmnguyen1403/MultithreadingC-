#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>

std::mutex values_mtx;
std::mutex cout_mtx;
std::vector<int> values;

int randGen(const int&min, const int&max) {
    static thread_local std::mt19937 
        generator(std::hash<std::thread::id>()(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void threadFnc(int tid) {
    cout_mtx.lock();
    std::cout << "Starting thread " << tid << ".\n";
    cout_mtx.unlock();

    values_mtx.lock();
    int val = values[0];
    values_mtx.unlock();
    int rval = randGen(0, 10);
    val += rval;

    cout_mtx.lock();
    std::cout << "Thread " << tid << " adding " << rval << ". New value: " << val << std::endl;
    cout_mtx.unlock();

    values_mtx.lock();
    values.push_back(val);
    values_mtx.unlock();
}

int main()
{
    values.push_back(42);
    
    std::thread tr1(threadFnc, 1);
    std::thread tr2(threadFnc, 2);
    std::thread tr3(threadFnc, 3);
    std::thread tr4(threadFnc, 4);

    tr1.join();
    tr2.join();
    tr3.join();
    tr4.join();

    std::cout << "Input: " << values[0] << std::endl;
    for (int index = 1; index < 5; ++index) {
        std::cout << "Result " << index << ": " << values[index] << std::endl;
    }
    return 1;
}