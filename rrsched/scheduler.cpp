#include "common.h"
#include "scheduler.h"
#include <algorithm>
#include <queue>

void simulate_rr(int64_t quantum, int64_t max_seq_len, std::vector<Process> &processes, std::vector<int> &seq) {
    seq.clear();

    int64_t curr_time = 0;
    int64_t remain_time = 0;
    int cpu = -1; // -1 represents idle CPU
    size_t next_process = 0;

    std::queue<int> ready_queue;
    std::vector<int64_t> remaining_bursts(processes.size());

    for (size_t i = 0; i < processes.size(); ++i) {
        remaining_bursts[i] = processes[i].burst;
    }

    while (true) {
        // Check for arriving processes
        while (next_process < processes.size() && processes[next_process].arrival <= curr_time) {
            ready_queue.push(next_process);
            next_process++;
        }

        // Check for process completion or time slice expiration
        if (cpu != -1) {
            remaining_bursts[cpu]--;
            if (remaining_bursts[cpu] == 0) {
                processes[cpu].finish_time = curr_time; //adjust finish_time
                cpu = -1;   //idle CPU will be denoted by -1
                remain_time = 0;
            } else {
                remain_time--;
                if  (remain_time == 0) {
                ready_queue.push(cpu);
                cpu = -1;
                }
            }
        }

        // Check for idle CPU and assign a process if possible
        if (cpu == -1 && !ready_queue.empty()) {
            cpu = ready_queue.front();
            ready_queue.pop();
            remain_time = quantum; //quantum = time slice
            if (processes[cpu].start_time == -1) {
                processes[cpu].start_time = curr_time; // adjust start_time for each process
            }
        }

        // Check for termination
        if (cpu == -1 && ready_queue.empty()){
            if (next_process >= processes.size()) {
                break;
             }
        }

        // Update the condensed execution sequence
        if (seq.empty() || (seq.back() != cpu && seq.size() < max_seq_len)) {
            seq.push_back(cpu);
        }

        // Increment the current time
        curr_time++;
    }
}
