#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

struct Task {
    int index;
    int rj; 
    int qj; 
    int pj; 
};


void loadTasksFromFile(const std::string& filename, std::vector<Task>& tasks) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    int n;
    file >> n;
    tasks.resize(n);

    for (int i = 0; i < n; ++i) {
        tasks[i].index = i + 1;
        file >> tasks[i].rj >> tasks[i].pj >> tasks[i].qj;
    }

    file.close();
}

void findOptimalOrder(std::vector<Task>& tasks) {
    std::vector<Task> readyQueue;
    std::vector<Task> result;
    int currentTime = 0;

    sortByRj(tasks);
    while (!tasks.empty() || !readyQueue.empty()) {
        while (!tasks.empty() && tasks.front().rj <= currentTime) {
            readyQueue.push_back(tasks.front());
            tasks.erase(tasks.begin());
        }

        if (readyQueue.empty()) {
            currentTime = tasks.front().rj;
            continue;
        }

        auto maxQjTask = std::max_element(readyQueue.begin(), readyQueue.end(), [](const Task& a, const Task& b) {
            return a.qj < b.qj;
        });

        currentTime += maxQjTask->pj;
        result.push_back(*maxQjTask);
        readyQueue.erase(maxQjTask);
    }

    tasks = result;
}

void sortByRj(std::vector<Task>& tasks) {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.rj < b.rj;
    });
}


void sortByQj(std::vector<Task>& tasks) {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.qj < b.qj;
    });
}

int calculateCmax(const std::vector<Task>& tasks) {
    int currentTime = 0;
    int cmax = 0;

    for (const auto& task : tasks) {
        currentTime = std::max(currentTime, task.rj) + task.pj;
        cmax = std::max(cmax, currentTime + task.qj);
    }

    return cmax;
}

int main() {
    std::vector<Task> tasks;
    int currentTime = 0;
    int cmax = 0;
    loadTasksFromFile("tasks.txt", tasks);
    sortByRj(tasks);
    cmax = calculateCmax(tasks);
    std::cout << "Cmax: " << cmax << "\n";
    


    return 0;
}