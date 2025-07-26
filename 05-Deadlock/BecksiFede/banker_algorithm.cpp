#include <iostream>
#include <cstring> // Per memset

bool is_empty(int* array , size_t lenght){
    for(int i = 0 ; i < lenght ; ++i){
        if(array[i] != 0)
            return false;
    }

    return true;
}


bool is_minor(int* request , size_t lenght_request , int* work , size_t lenght_work){
    if(lenght_request != lenght_work)
        throw "Vectors's dimensions are not the same";
    
    for(int i_res = 0; i_res < lenght_request ; ++i_res){
        if(request[i_res] > work[i_res])
            return false;
    }

    return true;
}

bool sum(int* work , size_t lenght_work , int* allocation , size_t lenght_allocation){
    if(lenght_work != lenght_allocation)
        throw "Vectors's dimensions are not the same";
    
    for(int i_res = 0; i_res < lenght_work ; ++i_res){
        work[i_res] += allocation[i_res];
    }

    return true;
}

bool is_safe(int* available, int** allocation, int** need, int n_process, int n_resources) {
    int* work = new int[n_resources];
    bool* finished = new bool[n_process];
    memcpy(work, available, n_resources * sizeof(int));
    memset(finished, false, n_process * sizeof(bool));

    for (int i_proc = 0; i_proc < n_process; ++i_proc) {
        if (is_empty(allocation[i_proc], n_resources)) {
            finished[i_proc] = true;
        }
    }

    bool safe = true;
    for (int i_proc = 0; i_proc < n_process; ++i_proc) {
        if (!finished[i_proc] && is_minor(need[i_proc], n_resources, work, n_resources)) {
            sum(work, n_resources, allocation[i_proc], n_resources);
            finished[i_proc] = true;
            i_proc = -1; // Ricomincia il ciclo per verificare altri processi
        }
    }

    for (int i_proc = 0; i_proc < n_process; ++i_proc) {
        if (!finished[i_proc]) {
            std::cout << "State unsafe\n";
            safe = false;
            break;
        }
    }

    delete[] work;
    delete[] finished;
    std::cout << "Pippo";

    return safe;
}

bool assign_resources(int* available, int** allocation, int** need, int process_id, int* request, int n_resources, int n_process) {
    // Verifica se la richiesta è valida rispetto a `need`
    if (!is_minor(request, n_resources, need[process_id], n_resources)) {
        std::cout << "Request exceeds need.\n";
        return false;
    }

    // Verifica se la richiesta è valida rispetto a `available`
    if (!is_minor(request, n_resources, available, n_resources)) {
        std::cout << "Request cannot be satisfied (not enough resources available).\n";
        return false;
    }

    // Backup dei parametri
    int* allocation_backup = new int[n_resources];
    int* need_backup = new int[n_resources];
    int* available_backup = new int[n_resources];

    for (int i = 0; i < n_resources; ++i) {
        allocation_backup[i] = allocation[process_id][i];
        need_backup[i] = need[process_id][i];
        available_backup[i] = available[i];
    }

    // Simula l'assegnazione
    for (int i = 0; i < n_resources; ++i) {
        allocation[process_id][i] += request[i];
        available[i] -= request[i];
        need[process_id][i] -= request[i];
    }

    // Verifica se il sistema rimane in uno stato sicuro
    bool safe = is_safe(available, allocation, need, n_process, n_resources);

    // Ripristina lo stato precedente se il sistema non è sicuro
    if (!safe) {
        for (int i = 0; i < n_resources; ++i) {
            allocation[process_id][i] = allocation_backup[i];
            need[process_id][i] = need_backup[i];
            available[i] = available_backup[i];
        }
    }

    // Dealloca i backup
    delete[] allocation_backup;
    delete[] need_backup;
    delete[] available_backup;

    return safe;
}

int main() {
    const int n_process = 5;    // Numero di processi
    const int n_resources = 3; // Numero di risorse

    // Inizializza le matrici
    int* available = new int[n_resources]{3, 3, 2}; // Risorse disponibili
    int** allocation = new int*[n_process];         // Risorse allocate
    int** max_request = new int*[n_process];        // Richieste massime
    int** need = new int*[n_process];               // Risorse necessarie
    int** request = new int*[n_process];            // Richieste correnti

    for (int i = 0; i < n_process; ++i) {
        allocation[i] = new int[n_resources]{0}; // Inizializza a 0
        max_request[i] = new int[n_resources]{0};
        need[i] = new int[n_resources]{0};
        request[i] = new int[n_resources]{0};
    }

    // Carica i dati per allocation, max_request e calcola need
    int allocation_data[n_process][n_resources] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int max_request_data[n_process][n_resources] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    // Copia i dati nelle matrici
    for (int i = 0; i < n_process; ++i) {
        for (int j = 0; j < n_resources; ++j) {
            allocation[i][j] = allocation_data[i][j];
            max_request[i][j] = max_request_data[i][j];
            need[i][j] = max_request[i][j] - allocation[i][j];
        }
    }

    // Simula una richiesta di risorse
    int process_id = 1; // Processo che richiede risorse
    int request_data[n_resources] = {1, 0, 2}; // Risorse richieste

    for (int j = 0; j < n_resources; ++j) {
        request[process_id][j] = request_data[j];
    }

    // Stampa lo stato iniziale
    std::cout << "Initial State:\n";
    std::cout << "Available: ";
    for (int i = 0; i < n_resources; ++i) {
        std::cout << available[i] << " ";
    }
    std::cout << "\n";

    std::cout << "Allocation:\n";
    for (int i = 0; i < n_process; ++i) {
        for (int j = 0; j < n_resources; ++j) {
            std::cout << allocation[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Max Request:\n";
    for (int i = 0; i < n_process; ++i) {
        for (int j = 0; j < n_resources; ++j) {
            std::cout << max_request[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Need:\n";
    for (int i = 0; i < n_process; ++i) {
        for (int j = 0; j < n_resources; ++j) {
            std::cout << need[i][j] << " ";
        }
        std::cout << "\n";
    }

    // Esegui il test
    std::cout << "\nSimulating resource request for process " << process_id << "...\n";
    if (assign_resources(available, allocation, need, process_id, request[process_id], n_resources,n_process)) {
        std::cout << "Request granted. System is in a safe state.\n";
    } else {
        std::cout << "Request denied. System would enter an unsafe state.\n";
    }

    // Libera la memoria allocata
    delete[] available;
    for (int i = 0; i < n_process; ++i) {
        delete[] allocation[i];
        delete[] max_request[i];
        delete[] need[i];
        delete[] request[i];
    }
    delete[] allocation;
    delete[] max_request;
    delete[] need;
    delete[] request;

    return 0;
}