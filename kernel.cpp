#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>


#define MEMORY_SIZE 1024 //1KB

struct Process {
    int id;
     int arrival_time;
    int cpu_runtime;
    int required_mem;

    Process(int proc_id, int rt, int at, int rm) : id(proc_id), cpu_runtime(rt), arrival_time(at), required_mem(rm) {}
};

// To manage memory allocation and deallocation.
class MemoryManager {
private:
    int memory[MEMORY_SIZE];
    int mem_available;

public:
    MemoryManager() : mem_available(MEMORY_SIZE) {std::fill(std::begin(memory), std::end(memory), -1); }
    
    // Allocate memory space to a process if there is sufficient memory available.
    bool allocate_memory(int pid, int size) {
        if (size > mem_available) return false;
        int mem_allocated = 0;
        for (int i = 0; i < MEMORY_SIZE && mem_allocated < size; ++i) {
            if (memory[i] == -1) {
                memory[i] = pid;
                ++mem_allocated;
            }
        }
        mem_available = mem_available - size;
        return true;
    }

   // Deallocate memory space for processes.
    void free_memory(int pid) {
        for (int i = 0; i < MEMORY_SIZE; ++i) {
            if (memory[i] == pid) {
                memory[i] = -1;
                ++mem_available;
            }
        }
    }
    int getFree_memory() const { return mem_available; }
};

// Manage process sheduling and execution.
class Scheduler {
private:
    std::queue<Process> ready_queue;
    int time;

public:
    Scheduler() : time() {}
    // Add process to the queue for scheduling.
    void addProcess(const Process &process) {
        ready_queue.push(process);
    }
    int getCurrentTime() const { return time; }

    void run() {
        std::cout << " >>>> The process scheduling started :\n";
        while (!ready_queue.empty()) {
            Process current = ready_queue.front();
            ready_queue.pop();
            std::cout << " Process " << current.id << " is running" << ", its CPU runTime is  " << current.cpu_runtime << "\n";
            time = time + current.cpu_runtime;
            std::cout << " Process " << current.id << " completed.\n";
        }
        std::cout << "Total CPU runtime (without interruption) for all processes is " << time << " units.\n";
    }
};

// Manage the OS (including memory allocation and process scheduling ).
class OS_Kernel {
private:
    MemoryManager mem_manager;
    Scheduler scheduler;
    int pid_counter;

public:
    OS_Kernel() : pid_counter() {}
    // Create new processes
    void create_process(int cpu_runtime, int mem_required) {
        if (mem_required > mem_manager.getFree_memory()) {
            std::cerr << "Error: no memory available for process creation.\n";
            return;
        }
        // Add process to the scheduler and allocate the necessary memory for the new process.
        Process new_process(pid_counter++, cpu_runtime, scheduler.getCurrentTime(), mem_required);
        if (mem_manager.allocate_memory(new_process.id, mem_required)) {
            scheduler.addProcess(new_process);
            std::cout << "Process " << new_process.id << " created.\n";
        } else {
            std::cerr << "Error: Memory allocation failed.\n";
        }
    }
    
    // Terminate process and deallocate memory.
    void terminate_process(int pid) {
        mem_manager.free_memory(pid);
        std::cout << "Process ID " << pid << " terminated and memory deallocated .\n";
    }
 
    // start the scheduler 
    void run_scheduler() {
        scheduler.run();
    }
};

int main() {
    OS_Kernel kernel;

    kernel.create_process(6, 100); 
    kernel.create_process(12, 200); 
    kernel.create_process(4, 75);
    kernel.create_process(2, 50); 

    kernel.run_scheduler(); //  To process all tasks

    return 0;
}
