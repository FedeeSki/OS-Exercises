#include <iostream>

bool flag[2] = {
    false,
    false
};
int turn = 0;


void enter_critical_section(int i){
    int other_proc = 1 - i;

    flag[i] = true;

    turn = other_proc;

    while (flag[other_proc] && turn == other_proc)
    {
        
    }
}


void exit_critical_section(int i ){
    flag[i] = false;
}


void* thread_function(void *arg){
    int i = *(int *) arg;
    for(int counter = 0 ; counter < 5 ; ++counter){
        enter_critical_section(i);
        std::cout << "Ciao per la " << counter +1  << " volta da Pthread id: " << i << std::endl;
        exit_critical_section(i);
    }

    pthread_exit(nullptr);
}

int main(){
    pthread_t thread1 , thread2;
    int id1 = 0 , id2 = 1;

    volatile int c = 0;
    
    // Crea i thread
    pthread_create(&thread1, nullptr, thread_function, &id1);
    pthread_create(&thread2, nullptr, thread_function, &id2);

    // Attendi la terminazione dei thread
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
}