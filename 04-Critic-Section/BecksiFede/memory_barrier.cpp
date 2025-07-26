#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> shared_var(0);

void thread_function() {
    shared_var.store(1, std::memory_order_release); // Scrittura con rilascio
    while (shared_var.load(std::memory_order_acquire) != 2) {
        // Attendi che un altro thread aggiorni shared_var
    }
    std::cout << "Thread ha rilevato shared_var = 2\n";
}

int main() {
    std::thread t1(thread_function);
    std::thread t2([]() {
        shared_var.store(2, std::memory_order_release); // Aggiorna shared_var
    });
    
    t1.join();
    t2.join();
    return 0;
}