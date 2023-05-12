// COMPILE USING -std=c++11

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

const int ARRAY_SIZE = 20;
int num_array[ARRAY_SIZE];
int even_sum = 0;
int odd_sum = 0;
int total_sum = 0;

mutex mtx;
condition_variable even_cv;
condition_variable odd_cv;

bool evenFinished = false;
bool oddFinished = false;

void createArray(bool random);
void evenThread();
void oddThread();
void sumThread();

int main() {
    srand(time(0));
    createArray(true);

    thread b(evenThread);
    thread c(oddThread);
    thread d(sumThread);

    b.join();
    c.join();
    d.join();

    cout << "The even sum is: " << even_sum << endl;
    cout << "The odd sum is: " << odd_sum << endl;
    cout << "The total sum is: " << total_sum << endl;

    return 0;
}

// Allows for array to be filled with random numbers or 0 to ARRAY_SIZE.
void createArray(bool random) {
    if (random) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            num_array[i] = rand() % 100 + 1;
        }
    }
    else {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            num_array[i] = i;
        }
    }
}

// Sum even numbers.
void evenThread() {
    int sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i += 2) {
        sum += num_array[i];
    }

    // This thread has a lock to update shared variable.
    lock_guard<mutex> lock(mtx);
    even_sum = sum;
    evenFinished = true;

    // Notify thread d that thread b is complete.
    even_cv.notify_one();
}

// Sum odd numbers.
void oddThread() {
    int sum = 0;
    for (int i = 1; i < ARRAY_SIZE; i += 2) {
        sum += num_array[i];
    }

    // This thread has a lock to update shared variable.
    lock_guard<mutex> lock(mtx);
    odd_sum = sum;
    oddFinished = true;

    // Notify thread d that thread b is complete.
    odd_cv.notify_one();
}

// Sum all numbers
void sumThread() {
    unique_lock<mutex> lock(mtx);
    even_cv.wait(lock, [] { return evenFinished; });
    odd_cv.wait(lock, [] { return oddFinished; });

    total_sum = odd_sum + even_sum;
}