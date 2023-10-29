#include <iostream>
#include <thread>
#include <mutex>
#include <array>
#include <vector>

class Fork {
public:
    Fork() : owned_(false) {}

    void grab() {
        mutex_.lock();
        owned_ = true;
    }

    void release() {
        if (owned_) {
            mutex_.unlock();
            owned_ = false;
        }
    }

private:
    std::mutex mutex_;
    bool owned_;
    friend class Philosopher;
};

class Philosopher {
public:
    Philosopher(int id, Fork& left_fork, Fork& right_fork) : id_(id), left_fork_(left_fork), right_fork_(right_fork) {}

    void dine() {
        while (true) {
            think();
            eat();
        }
    }

private:
    int id_;
    Fork& left_fork_;
    Fork& right_fork_;

    void think() {
        std::cout << "Philosopher " << id_ << " is thinking" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    void eat() {
        std::cout << "Philosopher " << id_ << " is hungry and trying to grab forks" << std::endl;
        std::scoped_lock lock(left_fork_.mutex_, right_fork_.mutex_);
        std::cout << "Philosopher " << id_ << " is eating" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        left_fork_.release();
        right_fork_.release();
    }
};

int main() {
    const int NUM_PHILOSOPHERS = 5;
    std::array<Fork, NUM_PHILOSOPHERS> forks;
    std::thread threads[NUM_PHILOSOPHERS];
    //     Philosopher(0, forks[0], forks[1]),
    //     Philosopher(1, forks[1], forks[2]),
    //     Philosopher(2, forks[2], forks[3]),
    //     Philosopher(3, forks[3], forks[4]),
    //     Philosopher(4, forks[4], forks[0])
    // };
    // std::array<std::thread, NUM_PHILOSOPHERS> threads;
    // for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
    //     threads[i] = std::thread(&Philosopher::dine, &philosophers[i]);
    // }

    const int num_philosophers = 5;
    // Fork forks[num_philosophers];
    // std::vector<std::thread> threads;
    std::thread phils[num_philosophers];

    // create philosophers
    for (int i = 0; i < num_philosophers; ++i) {
        int left_fork_idx = i;
        int right_fork_idx = (i + 1) % num_philosophers;
        // auto philosopher =  Philosopher(i, forks[left_fork_idx], forks[right_fork_idx]); // set timeout to 5 seconds
        threads[i] = std::thread([i, &forks]() {Philosopher(i, forks[i], forks[(i + 1) % num_philosophers]).dine();});
        // phils[i] = std::thread([i, &forks]() {Philosopher(i, forks[i], forks[(i + 1) % num_philosophers - 1]).dine();});
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}