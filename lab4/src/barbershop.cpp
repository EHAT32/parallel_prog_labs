#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

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
    queue<Client> clients;
    int chairs;
    mutex mtx;
    condition_variable cv;

    Barbershop(int c) {
        chairs = c;
    }

    void addClient(Client c) {
        unique_lock<mutex> lck(mtx);
        if (clients.size() >= chairs) {
            cout << "Client " << c.id << " left because there were no available chairs.\n";
            return;
        }
        clients.push(c);
        cout << "Client " << c.id << " arrived and is waiting.\n";
        cv.notify_one();
    }

    void barberWork(Barber b) {
        while (true) {
            unique_lock<mutex> lck(mtx);
            while (clients.empty()) {
                cout << "Barber " << b.id << " is sleeping.\n";
                cv.wait(lck);
            }
            Client c = clients.front();
            clients.pop();
            cout << "Barber " << b.id << " is cutting hair of client " << c.id << ".\n";
            lck.unlock();
            this_thread::sleep_for(chrono::seconds(2));
        }
    }
};

int main() {
    int numClients = 10;
    int numChairs = 2;
    int numBarbers = 2;

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

    for (int i = 0; i < numBarbers; i++) {
        barbers[i].join();
    }
    cout << 10 << endl;


    return 0;
}