#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Message{
    public:
        void set(const int &val){
            std::lock_guard<std::mutex> lock(mtx);
            pool.push_back(val);
            std::cout << "Set " << val << " from " << std::this_thread::get_id() << std::endl;
        }

        int get(){
            std::lock_guard<std::mutex> lock(mtx);
            if (pool.empty()) {
                return -1;
            }

            int val = this->pool.back();
            pool.pop_back();
            std::cout << "Got " << val << " from " << std::this_thread::get_id() << std::endl;
            return val;
        }
    private:
        std::mutex mtx;
        std::vector<int> pool;
};

Message msg;

int main(){
    std::thread t1([](){msg.set(1);});
    std::thread t2([](){msg.get();});
    
    t1.join();
    t2.join();
    return 0;
}