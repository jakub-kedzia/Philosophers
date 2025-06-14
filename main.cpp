#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

enum State { THINKING, HUNGRY, EATING };

using namespace std;

vector<State> states;
mutex mtx;
mutex cout_mtx;
condition_variable cv;

int N = 5;

void print_state(int id, const string& message) {
    unique_lock<mutex> lock(cout_mtx);
    cout << "Philosopher " << id << ": " << message << endl;
}

void try_to_eat(int id) {
    if (states[id] == HUNGRY &&
        states[(id + N - 1) % N] != EATING &&
        states[(id + 1) % N] != EATING) {
        states[id] = EATING;
        cv.notify_all();
    }
}

void take_forks(int id) {
    unique_lock<mutex> lock(mtx);
    states[id] = HUNGRY;
//    lock.unlock();
    print_state(id, "hungry and tries to take a fork");

//    lock.lock();
    if (states[id] != EATING) {
        try_to_eat(id);
    }
//    lock.unlock();
    print_state(id, "started eating");
}

void put_forks(int id) {
    unique_lock<mutex> lock(mtx);
    states[id] = THINKING;
//    lock.unlock();
    print_state(id, "puts fork down and starts thinking");
}

void philosopher(int id) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1000, 3000);

    while (true) {
        print_state(id, "is thinking");
        this_thread::sleep_for(chrono::milliseconds(dist(gen)));


        take_forks(id);

        this_thread::sleep_for(chrono::milliseconds(dist(gen)));

        put_forks(id);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Incorrect number of args. \n";
        return 1;
    }

    N = stoi(argv[1]);
    states.resize(N, THINKING);

    vector<thread> philosophers;
    for (int i = 0; i < N; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto& t : philosophers) {
        t.join();
    }

    return 0;
}
