#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

// Stany filozofów
enum State { THINKING, HUNGRY, EATING };

// Zmienne globalne
std::vector<State> states;
std::mutex mtx;
std::condition_variable cv;

// Liczba filozofów (ustalana na początku)
int N = 5;

void print_state(int id, const std::string& message) {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Filozof " << id << ": " << message << std::endl;
}

// Sprawdza czy filozof może jeść
void try_to_eat(int id) {
    if (states[id] == HUNGRY &&
        states[(id + N - 1) % N] != EATING &&
        states[(id + 1) % N] != EATING) {
        states[id] = EATING;
        cv.notify_all();  // notify others that state has changed
    }
}

void take_forks(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    states[id] = HUNGRY;
    lock.unlock();
    print_state(id, "jest glodny i probuje wziac widelce");

    if (states[id] != EATING) {
        try_to_eat(id);

        std::this_thread::sleep_for(std::chrono::seconds(1));
        // Czekaj aż będzie można jeść
//        cv.wait(lock, [&]() { return states[id] == EATING; });
    }
    print_state(id, "zaczal jesc");
}

void put_forks(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    states[id] = THINKING;
    mtx.unlock();
    print_state(id, "odklada widelce i zaczyna mysleć");
}

void philosopher(int id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1000, 3000);

    while (true) {
        print_state(id, "mysli");
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen)));


        take_forks(id);

        std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen)));

        put_forks(id);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Użycie: " << argv[0] << " <liczba_filozofow>\n";
        return 1;
    }

    N = std::stoi(argv[1]);
    states.resize(N, THINKING);  // initialize all to THINKING

    std::vector<std::thread> philosophers;
    for (int i = 0; i < N; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto& t : philosophers) {
        t.join();
    }

    return 0;
}
