#include <iostream>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <cstdlib>

using namespace std;

class Philosopher {
public:
    Philosopher(int id, timed_mutex& left_fork, timed_mutex& right_fork, int timeout) :
        id_(id), left_fork_(left_fork), right_fork_(right_fork), timeout_(timeout) {}

    void operator()() {
        while (true) {
            // pick up left fork
            left_fork_.lock();
            cout << "Philosopher " << id_ << " picked up left fork" << endl;;

            // try to pick up right fork
            bool got_right_fork = right_fork_.try_lock_for(chrono::milliseconds(timeout_));
            if (!got_right_fork) {
                //put down left fork if timeout
                left_fork_.unlock();
                cout << "Philosopher " << id_ << " timed out while waiting for right fork" << endl;
                this_thread::sleep_for(chrono::seconds(1)); // wait for 1 second before trying again
                continue;
            }

            cout << "Philosopher " << id_ << " picked up right fork and is eating" << endl;
            this_thread::sleep_for(chrono::seconds(2)); // eat for 2 seconds

            // put down both forks after eating
            left_fork_.unlock();
            right_fork_.unlock();
            // start thinking
            cout << "Philosopher " << id_ << " put down both forks and is thinking" << endl;
            this_thread::sleep_for(chrono::seconds(1)); // think for 1 second
        }
    }

private:
    int id_;
    timed_mutex& left_fork_;
    timed_mutex& right_fork_;
    int timeout_;
};

int main() {
    const int num_philosophers = 5;
    timed_mutex forks[num_philosophers];
    vector<thread> threads;

    // create philosophers
    for (int i = 0; i < num_philosophers; ++i) {
        int left_fork_idx = i;
        int right_fork_idx = (i + 1) % num_philosophers;
        Philosopher philosopher(i, forks[left_fork_idx], forks[right_fork_idx], rand() + 1000); // set timeout to 5 seconds
        threads.emplace_back(philosopher);
    }

    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}