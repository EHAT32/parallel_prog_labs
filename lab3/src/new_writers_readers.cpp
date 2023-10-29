#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>

class Message {
public:
    void writer(int id) {
        for (int i = 0; i < 5; ++i) {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            pool_.push_back(id * 10 + i);
            std::cout << "Writer " << id << " wrote " << (id * 10 + i) << std::endl;
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void reader(int id) {
        for (int i = 0; i < 5; ++i) {
            std::shared_lock<std::shared_mutex> lock(mutex_);
            if (pool_.empty()) {
                std::cout << "No values" << std::endl;
            }
            else {
                int value = pool_.back();
                std::cout << "Reader " << id << " read " << value << std::endl;
                pool_.pop_back();
            }
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

private:
    std::vector<int> pool_;
    std::shared_mutex mutex_;
};

int main() {
    Message msg;
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(&Message::writer, &msg, i);
    }
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(&Message::reader, &msg, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}