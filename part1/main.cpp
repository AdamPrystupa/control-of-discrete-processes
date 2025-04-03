#include <iostream>
#include <vector>

#include "algorithms.hpp" // Adjusted the path to locate the file


int main() {
    std::vector<Task> tasks;
    int currentTime = 0;
    int cmax = 0;
    loadTasksFromFile("../tests/SCHRAGE5.dat", tasks);

    // sortByQj(tasks, true);
    // optimizeGroupsByCmax(tasks, false);
    // printTasks(tasks);
    //findOptimalTaskOrder(tasks);
    //findOptimalTaskOrderWithThreads(tasks, 8);
    //cmax = calculateCmax(tasks);
    //schrage(tasks);
     schrageWithHeap(tasks);
    std::cout << "Cmax: " << cmax << "\n";
    //printTasks(tasks);
    


    return 0;
}