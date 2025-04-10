#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>   
#include <algorithm> 
#include <thread>
#include <mutex>
#include <chrono>

struct Task {
    int index;
    int rj; 
    int qj; 
    int pj; 
};

void loadTasksFromFile(const std::string& filename, std::vector<Task>& tasks);
void findOptimalOrder(std::vector<Task>& tasks);
void sortByRj(std::vector<Task>& tasks, bool ascending);
void sortByQj(std::vector<Task>& tasks, bool ascending);
void optimizeGroupsByCmax(std::vector<Task>& tasks, bool sortByRj);
int calculateCmax(const std::vector<Task>& tasks);
void printTasks(const std::vector<Task>& tasks);
void findOptimalTaskOrder(std::vector<Task>& tasks);
void findOptimalTaskOrderWithThreads(std::vector<Task>& tasks, int numThreads);
void schrage(std::vector<Task>& tasks);
void schrageWithHeap(std::vector<Task>& tasks);
void ownAlgorithm(std::vector<Task>& tasks);

#endif // ALGORITHMS_HPP