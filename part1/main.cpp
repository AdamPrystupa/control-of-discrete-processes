#include <iostream>
#include <vector>

#include "algorithms.hpp" // Adjusted the path to locate the file


int main() {
    std::vector<Task> tasks;
    int currentTime = 0;
    int cmax = 0;
    loadTasksFromFile("tests/test.dat", tasks);

    // sortByQj(tasks, true);
    // optimizeGroupsByCmax(tasks, false);
    // printTasks(tasks);
    findOptimalTaskOrder(tasks);
    //findOptimalTaskOrderWithThreads(tasks, 8);
    cmax = calculateCmax(tasks);
    std::cout << "Cmax: " << cmax << "\n";
    


    return 0;
}