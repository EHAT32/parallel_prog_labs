#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <iostream>
class StatusThread{
    public:
        StatusThread(const int &timePeriod = 1) : timePeriod_(timePeriod), isRunning_(false){};
        void setPeriod(const int& n){
            timePeriod_ = n;
        }

        void start(){
            if (!isRunning_.load()) {
                isRunning_.exchange(true);
                thread_ = std::thread([this] {threadFunction();});
            };
        };

        void stop(){
            if (!isRunning_.load()) {
                return;
            }
            isRunning_.exchange(false);
            cv_.notify_all();
            thread_.join();
        }

    private:
        void threadFunction(){
            std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
            std::unique_lock<std::mutex> lock(mtx_);
            while (isRunning_.load()) {
                cv_.wait_for(lock, std::chrono::seconds(timePeriod_));
                std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
                auto workingTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
                std::cout << "Thread has been running for " << workingTime << " seconds" << std::endl;
            }
        };

        int timePeriod_;
        std::thread thread_;
        std::atomic<bool> isRunning_;
        std::mutex mtx_;
        std::condition_variable cv_;
};


int main(){
    StatusThread td;
    td.setPeriod(2);
    td.start();
    std::this_thread::sleep_for(std::chrono::seconds(9));

    td.stop();


    return 0;
}