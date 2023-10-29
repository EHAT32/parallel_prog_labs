#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

class Message{
    public:
        void set(const int &val){
            std::lock_guard<std::shared_mutex> lock(mtx_);
            pool_.push_back(val);
            std::cout << "Set " << val << " from " << std::this_thread::get_id() << std::endl;
        }

        int get(){
            std::lock_guard<std::shared_mutex> lock(mtx_);
            if (pool_.empty()) {
                return -1;
            }

            int val = this->pool_.back();
            pool_.pop_back();
            std::cout << "Got " << val << " from " << std::this_thread::get_id() << std::endl;
            return val;
        }
    private:
        std::shared_mutex mtx_;
        std::vector<int> pool_;
};

void writer(Message& msg, int id) {
    for (int i = 0; i < 5; i++) {
        msg.set(id * 10 + i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void reader(Message& msg, int id) {
    for (int i = 0; i < 5; i++) {
        int value = msg.get();
        if (value == -1) {
            std::cout << "Reader " << id << " found empty pool." << std::endl;
        }
        else {
            std::cout << "Reader " << id << " got value " << value << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int main(){
    Message msg;
    std::vector<std::thread> threads;

    //writers
    for (int i = 0; i < 3; i++) {
        threads.emplace_back(writer, std::ref(msg), i);
    }

    //readers
    for (int i = 0; i < 2; i++) {
        threads.emplace_back(reader, std::ref(msg), i);
    }

    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}