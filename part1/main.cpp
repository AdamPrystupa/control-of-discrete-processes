#include <iostream>
#include <vector>

#include "algorithms.hpp" // Adjusted the path to locate the file


int main() {
    std::vector<Task> tasks;
    int currentTime = 0;
    int cmax = 0;
    loadTasksFromFile("../tests/SCHRAGE2.dat", tasks);

    auto start = std::chrono::high_resolution_clock::now();
    //sortByQj(tasks, true);
    //optimizeGroupsByCmax(tasks, false);
    //printTasks(tasks);
    //findOptimalTaskOrderWithThreads(tasks, 8);
    //schrage(tasks);
    //schrageWithHeap(tasks);
    ownAlgorithm(tasks);
    auto end = std::chrono::high_resolution_clock::now();

    cmax = calculateCmax(tasks);
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to find optimal order: " << elapsed.count() << " seconds\n";
    std::cout << "Cmax: " << cmax << "\n";
    //printTasks(tasks);
    


    return 0;
}