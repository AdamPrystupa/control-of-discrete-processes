#include "algorithms.hpp"


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


void sortByRj(std::vector<Task>& tasks, bool ascending = true) {
    std::sort(tasks.begin(), tasks.end(), [ascending](const Task& a, const Task& b) {
        return ascending ? (a.rj < b.rj) : (a.rj > b.rj);
    });
}


void sortByQj(std::vector<Task>& tasks, bool ascending = true) {
    std::sort(tasks.begin(), tasks.end(), [ascending](const Task& a, const Task& b) {
        return ascending ? (a.qj < b.qj) : (a.qj > b.qj);
    });
}

void optimizeGroupsByCmax(std::vector<Task>& tasks, bool sortByRj) {
    // Iterate through the sorted tasks and find groups with the same rj or qj
    for (size_t i = 0; i < tasks.size();) {
        size_t j = i + 1;
        while (j < tasks.size() && 
               ((sortByRj && tasks[j].rj == tasks[i].rj) || 
                (!sortByRj && tasks[j].qj == tasks[i].qj))) {
            ++j;
        }

        // Optimize the group [i, j) locally
        if (j > i + 1) {
            std::vector<Task> group(tasks.begin() + i, tasks.begin() + j);
            std::sort(group.begin(), group.end(), [](const Task& a, const Task& b) {
                // Sort by the order that minimizes Cmax
                std::vector<Task> tempA = {a, b};
                std::vector<Task> tempB = {b, a};
                return calculateCmax(tempA) < calculateCmax(tempB);
            });

            // Replace the original group with the optimized group
            std::copy(group.begin(), group.end(), tasks.begin() + i);
        }

        // Move to the next group
        i = j;
    }
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

void printTasks(const std::vector<Task>& tasks) {
    for (const auto& task : tasks) {
        std::cout << "Task " << task.index << ": rj=" << task.rj << ", pj=" << task.pj << ", qj=" << task.qj << "\n";
    }
}



void findOptimalTaskOrder(std::vector<Task>& tasks) {
    std::vector<Task> bestOrder;
    int bestCmax = std::numeric_limits<int>::max();

    auto start = std::chrono::high_resolution_clock::now();

    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.index < b.index;
    });

    do {
        int currentCmax = calculateCmax(tasks);
        if (currentCmax < bestCmax) {
            bestCmax = currentCmax;
            bestOrder = tasks;
        }
    } while (std::next_permutation(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.index < b.index;
    }));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time taken to find optimal order: " << elapsed.count() << " seconds\n";

    tasks = bestOrder;
}



void findOptimalTaskOrderWithThreads(std::vector<Task>& tasks, int numThreads) {
    std::vector<Task> bestOrder;
    int bestCmax = std::numeric_limits<int>::max();
    std::mutex mutex;

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Generate all permutations of tasks
    std::vector<std::vector<Task>> taskChunks;
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.index < b.index;
    });

    do {
        taskChunks.push_back(tasks);
    } while (std::next_permutation(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.index < b.index;
    }));

    size_t chunkSize = taskChunks.size() / numThreads;
    std::vector<std::thread> threads;

    auto processChunk = [&](size_t startIdx, size_t endIdx) {
        for (size_t i = startIdx; i < endIdx; ++i) {
            int currentCmax = calculateCmax(taskChunks[i]);
            std::lock_guard<std::mutex> lock(mutex);
            if (currentCmax < bestCmax) {
                bestCmax = currentCmax;
                bestOrder = taskChunks[i];
            }
        }
    };

    for (int t = 0; t < numThreads; ++t) {
        size_t startIdx = t * chunkSize;
        size_t endIdx = (t == numThreads - 1) ? taskChunks.size() : startIdx + chunkSize;
        threads.emplace_back(processChunk, startIdx, endIdx);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Print elapsed time
    std::cout << "Time taken to find optimal order with threads: " << elapsed.count() << " seconds\n";

    // Update tasks with the best order
    tasks = bestOrder;
}


void schrage(std::vector<Task>& tasks) {
    std::vector<Task> G;
    std::vector<Task> N = tasks;
    sortByRj(N);

    int currentTime = 0;
    int cmax = 0;

    while (!G.empty() || !N.empty()) {
        while (!N.empty() && N.front().rj <= currentTime) {
            G.push_back(N.front());
            N.erase(N.begin());
        }

        if (G.empty()) {
            currentTime = N.front().rj;
        } else {
            auto it = std::max_element(G.begin(), G.end(), [](const Task& a, const Task& b) {
                return a.qj < b.qj;
            });

            Task task = *it;
            G.erase(it);
            currentTime += task.pj;
            cmax = std::max(cmax, currentTime + task.qj);
        }
    }

    std::cout << "Cmax: " << cmax << "\n";
}


void schrageWithHeap(std::vector<Task>& tasks) {
    std::vector<Task> G;
    std::vector<Task> N = tasks;
    sortByRj(N);

    int currentTime = 0;
    int cmax = 0;

    auto compareQj = [](const Task& a, const Task& b) {
        return a.qj < b.qj;
    };

    std::make_heap(N.begin(), N.end(), compareQj);

    while (!G.empty() || !N.empty()) {
        while (!N.empty() && N.front().rj <= currentTime) {
            G.push_back(N.front());
            std::push_heap(G.begin(), G.end(), compareQj);
            N.erase(N.begin());
            std::make_heap(N.begin(), N.end(), compareQj);
        }

        if (G.empty()) {
            currentTime = N.front().rj;
        } else {
            std::pop_heap(G.begin(), G.end(), compareQj);
            Task task = G.back();
            G.pop_back();
            currentTime += task.pj;
            cmax = std::max(cmax, currentTime + task.qj);
        }
    }

    std::cout << "Cmax: " << cmax << "\n";
}

void ownAlgorithm(std::vector<Task>& tasks) {
    std::vector<Task> buffer = tasks; // Tworzenie bufora

    std::sort(buffer.begin(), buffer.end(), [](const Task& a, const Task& b) {
        int sumA = a.rj + a.qj;
        int sumB = b.rj + b.qj;

        if (sumA == sumB) {
            return a.pj < b.pj; // Jeśli sumy są równe, sortuj według pj
        }
        return sumA < sumB; // Sortuj według sumy rj + qj
    });

    tasks = buffer; // Nadpisanie oryginalnego wektora
}