// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <queue>

// using namespace std;

// class Client {
// public:
//     int id;
//     Client(int i) {
//         id = i;
//     }
// };

// class Barber {
// public:
//     int id;
//     Barber(int i) {
//         id = i;
//     }
// };

// class Barbershop {
// public:
//     Barbershop(int c) {
//         chairs_ = c;
//         allBarbersAsleep = false;
//     }

//     void addClient(Client c) {
//         unique_lock<mutex> lck(mtx_);
//         if (clients_.size() >= chairs_) {
//             cout << "Client " << c.id << " left because there were no available chairs." << endl;
//             return;
//         }
//         clients_.push(c);
//         cout << "Client " << c.id << " arrived and is waiting." << endl;
//         cv.notify_one();
//     }

//     void barberWork(Barber b) {
//         while (true) {
//             unique_lock<mutex> lck(mtx_);
//             while (clients_.empty()) {
//                 cout << "Barber " << b.id << " is sleeping." << endl;;
//                 if (allBarbersAsleep) {
//                     cout << "All barbers are asleep, closing shop." << endl;
//                     return;
//                 }
//                 cv.wait(lck);
//             }
//             Client c = clients_.front();
//             clients_.pop();
//             cout << "Barber " << b.id << " is cutting hair of client " << c.id << endl;
//             lck.unlock();
//             this_thread::sleep_for(chrono::seconds(2));
//         }
//     }

//     condition_variable cv;
//     bool allBarbersAsleep;
// private:
//     queue<Client> clients_;
//     int chairs_;
//     mutex mtx_;
// };

// int main() {
//     int numClients = 10;
//     int numChairs = 3;
//     int numBarbers = 1;

//     Barbershop b(numChairs);

//     thread barbers[numBarbers];
//     for (int i = 0; i < numBarbers; i++) {
//         Barber barber(i);
//         barbers[i] = thread(&Barbershop::barberWork, &b, barber);
//     }

//     for (int i = 0; i < numClients; i++) {
//         Client c(i);
//         b.addClient(c);
//         this_thread::sleep_for(chrono::seconds(1));
//     }

//     b.allBarbersAsleep = true;
//     b.cv.notify_all();

//     for (int i = 0; i < numBarbers; i++) {
//         barbers[i].join();
//     }

//     return 0;
// }

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

mutex cout_message;

class Client {
public:
    int id;
    Client(int i) {
        id = i;
    }
};

class Barber {
public:
    int id;
    Barber(int i) {
        id = i;
    }
};

class Barbershop {
public:
    Barbershop(int c) {
        chairs_ = c;
        allBarbersAsleep = false;
    }

    void addClient(Client c) {
        unique_lock<mutex> lck(mtx_);
        if (clients_.size() >= chairs_) {
            {
                std::lock_guard<mutex> lock(cout_message);
                cout << "Client " << c.id << " left because there were no available chairs." << endl;
            }
            return;
        }
        clients_.push(c);
        {
            std::lock_guard<mutex> lock(cout_message);
            cout << "Client " << c.id << " arrived and is waiting." << endl;
        }
        
        cv.notify_one();
    }

    void barberWork(Barber b) {
        while (true) {
            unique_lock<mutex> lck(mtx_);
            while (clients_.empty()) {
                cout << "Barber " << b.id << " is sleeping." << endl;;
                if (allBarbersAsleep) {
                    {
                        std::lock_guard<mutex> lock(cout_message);
                        cout << "All barbers are asleep, closing shop." << endl;
                    }
                    return;
                }
                cv.wait(lck);
            }
            Client c = clients_.front();
            clients_.pop();
            {
                std::lock_guard<mutex> lock(cout_message);
                cout << "Barber " << b.id << " is cutting hair of client " << c.id << endl;
            }
            // cout << "Barber " << b.id << " is cutting hair of client " << c.id << endl;
            lck.unlock();
            this_thread::sleep_for(chrono::seconds(2));
        }
    }

    condition_variable cv;
    bool allBarbersAsleep;
private:
    queue<Client> clients_;
    int chairs_;
    mutex mtx_;
};

int main() {
    int numClients = 10;
    int numChairs = 3;
    int numBarbers = 1;

    Barbershop b(numChairs);

    thread barbers[numBarbers];
    for (int i = 0; i < numBarbers; i++) {
        Barber barber(i);
        barbers[i] = thread(&Barbershop::barberWork, &b, barber);
    }

    for (int i = 0; i < numClients; i++) {
        Client c(i);
        b.addClient(c);
        this_thread::sleep_for(chrono::seconds(1));
    }

    b.allBarbersAsleep = true;
    b.cv.notify_all();

    for (int i = 0; i < numBarbers; i++) {
        barbers[i].join();
    }

    return 0;
}
