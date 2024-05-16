# CPU Scheduler Simulator 💻

Welcome to the CPU Scheduler Simulator, a C++ program designed to simulate three key CPU scheduling algorithms: First Come First Served (FCFS), Shortest Remaining Time First (SRTF), and Round Robin (RR). This tool is invaluable for understanding how different algorithms handle process scheduling by analyzing their performance based on real input scenarios.

## Features 🌟

- **Three Scheduling Algorithms** - Simulate FCFS, SRTF, and RR to see how each handles process scheduling.
- **Input from Text File** - Reads context switch time and time quantum for RR from a provided text file.
- **Detailed Process Simulation** - Manages and simulates processes with details like process number, arrival time, and burst time.
- **Performance Metrics** - Analyze and compare the efficiency of each scheduling algorithm based on the simulation results.

## Getting Started 🚀

Follow these instructions to get a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

This program is written in C++ and requires a compiler that supports C++11 or later. Ensure you have a C++ compiler installed:

```bash
# Example for checking GCC version
g++ --version
```
Usage 📚
Prepare your input text file with the first line containing the context switch time and the time quantum for RR.
Subsequent lines should detail each process's information: process number, arrival time, and burst time.
Execute the program and provide the path to your input file when prompted.
The program will simulate the scheduling of these processes using the specified algorithms and output the results for comparison.
