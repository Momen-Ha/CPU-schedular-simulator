#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <iomanip>
#include <string>
#include <array>
#include <algorithm> 

using namespace std;


// Struct to represent a process
struct Process {
    int number;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int startTime;
    int finishTime;
    int waitingTime;
    int turnaroundTime;
    char state; // pending -> p  , running -> r, finished -> f, continue -> c
};

vector <Process> processes;
queue <Process> schedular;

bool compareArivalTime(const Process& a, const Process& b);
void displayResults(vector <Process> processes, string s);
void FCFS(vector<Process> processes);
void SRT(vector<Process> processes);
void roundRobin(vector <Process> processes, double timeQuantom);

int main(void)
{

    // reading file
    std::ifstream inputFile("Processes.txt");
    if (!inputFile.is_open()) { 
        cout << "Error opening the file!" << endl; 
        return 1; 
    }
    // skip 1 row:
    string skipRaw;
    getline(inputFile, skipRaw);
    double contextSwitchTime = 0, timeQuantom = 0;
    inputFile >> contextSwitchTime >> timeQuantom;
    //Note: context switch time is in microsecond and will be ignored!!!
    getline(inputFile, skipRaw);
    getline(inputFile, skipRaw);

    //inserting processes into Processes Vector
    int number, arrival, burst;
    while (inputFile >> number >> arrival >> burst)
    {
        Process p;
        p.startTime = 0;
        p.finishTime = 0;
        p.number = number;
        p.arrivalTime = arrival;
        p.burstTime = burst;
        p.remainingTime = burst;
        p.state = 'r';
        processes.push_back(p);
    }
        inputFile.close();
    if (processes.empty()) {
        cout << "No data read from the file!" << endl;
        return 1;
    }

    for (const Process& p : processes )
    {
        cout << p.number << "\t" << p.arrivalTime << "\t" << p.burstTime << endl;
    }
    cout << endl;

    // calling scheduling functions
    FCFS(processes);
    SRT(processes);
    roundRobin(processes, timeQuantom);

    return 0;
}

// comparing fucntion
bool compareArivalTime(const Process& a, const Process& b) {
    return a.arrivalTime < b.arrivalTime;
}


void FCFS( vector<Process> processes)
{
    // sorting based on arival time
    sort(processes.begin(), processes.end(), compareArivalTime);
    int processFinishTime = 0;
    for ( Process& p : processes )
    {
        if(p.arrivalTime > processFinishTime)
        {
            p.startTime = p.arrivalTime; // start time for process equals its arival time if there is no process
        }
        else{
        p.startTime = processFinishTime; // start time for process = finish time for previous process
        }
        p.finishTime = p.burstTime + p.startTime;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime = p.startTime - p.arrivalTime;
        processFinishTime = p.finishTime;
        p.state = 'f';
        schedular.push(p);
    }
    displayResults(processes,"FCFS");
}

void SRT(vector<Process> processes)
{  
     // Sort processes by arrival time initially
    sort(processes.begin(), processes.end(), compareArivalTime);
    int previousProcessNumber = 0;
    int tempStartTime = 0;
    int currentTime = 0;
    int totalProcesses = processes.size();
    int completedProcesses = 0;

    // Main loop until all processes are completed
    while (completedProcesses < totalProcesses)
    {
        int shortestRemainingTime = INT_MAX; // Initialize to maximum possible value
        int shortestIndex = -1; // Index of the process with shortest remaining time

        // Find the process with the shortest remaining time in arrived processes
        for (int i = 0; i < totalProcesses; ++i)
        {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime < shortestRemainingTime && processes[i].state != 'f')
            {
                shortestRemainingTime = processes[i].remainingTime;
                shortestIndex = i; 
            }
        }
        // If a process is found
        if (shortestIndex != -1)
        {
            
            Process& currentProcess = processes[shortestIndex];
  
            //If the process is pending
            if (currentProcess.state == 'p')
            {
                currentProcess.state = 'c';
                tempStartTime = currentTime;
            }

            if(previousProcessNumber >= 1)
                {
                    if(previousProcessNumber != currentProcess.number && (processes[previousProcessNumber - 1].state != 'f' || processes[previousProcessNumber - 1].state == 'c'))
                    {
                        processes[previousProcessNumber - 1].state = 'p';
                        processes[previousProcessNumber - 1].finishTime = currentTime;
                        Process p = processes[previousProcessNumber - 1];
                        p.startTime = tempStartTime;
                        schedular.push(p);
                    }
                }
                
            // Start time of the process (either arrival time or the current time)
            if(currentProcess.number != previousProcessNumber && currentProcess.state != 'p' && currentProcess.state != 'c')
                currentProcess.startTime = currentTime;

            // Execute the process for 1 unit of time
            currentProcess.remainingTime--;

            // Update current time
            currentTime++;

            // If the process is completed
            if (currentProcess.remainingTime == 0)
            {
                currentProcess.finishTime = currentTime;
                currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
                schedular.push(currentProcess); // Add the completed process to the scheduler
                currentProcess.state = 'f';
                completedProcesses++;
            }
            previousProcessNumber = currentProcess.number;
        }
        else 
            if(completedProcesses < totalProcesses)
                currentTime++;
    }

    displayResults(processes, "SRT");
}


void roundRobin(vector <Process> processes, double timeQuantom)
{
    sort(processes.begin(), processes.end(), compareArivalTime);
    vector <Process> temp = processes;
    int currentTime = 0;
    int totalProcesses = processes.size();
    int completedProcesses = 0;

    while(completedProcesses < totalProcesses)
    {
        Process currentProcess = temp.front();
        // if the process time less than quantom time
        if(currentProcess.remainingTime <= timeQuantom && currentProcess.arrivalTime <= currentTime)
        {
            Process *p;
            for(Process& pr : processes)
            {
                if(pr.number == currentProcess.number)
                {
                    p = &pr;
                    break;
                }
                
            }
            // it it's new process
            if(currentProcess.state == 'r')
            {
                currentProcess.state = 'f';
                currentProcess.startTime = p->startTime = currentTime;
                currentProcess.finishTime = p->finishTime = currentTime + currentProcess.burstTime;
                currentProcess.turnaroundTime = p->turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = p->waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
            }
            else // if it's postponed process
            {
                currentProcess.startTime  = currentTime;
                currentProcess.finishTime = p->finishTime = currentTime + currentProcess.burstTime;
                currentProcess.turnaroundTime = p->turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
                p->waitingTime = p->turnaroundTime - p->burstTime;
            }
            schedular.push(currentProcess);
            currentTime += currentProcess.burstTime;
            completedProcesses++;
            temp.erase(temp.begin());
        }
        // if the process time > quantom time
        else if(currentProcess.remainingTime > timeQuantom && currentProcess.arrivalTime <= currentTime)
        {
            if(currentProcess.state == 'r')
            {
                currentProcess.state = 'p';
                for(Process& pr : processes)
                {
                    if(pr.number == currentProcess.number)
                    {
                        pr.startTime = currentProcess.startTime = currentTime;
                        break;
                    }
                }
            }
            
            Process p = currentProcess;
            p.burstTime -= timeQuantom;
            p.remainingTime = currentProcess.burstTime - timeQuantom;//e
            temp.push_back(p);
            currentProcess.startTime = currentTime;
            currentProcess.finishTime = currentProcess.startTime + timeQuantom;

            schedular.push(currentProcess);
            // next unit of the same process
            
            currentTime += timeQuantom;
            temp.erase(temp.begin());
        }
        else
            currentTime++;
    }

    displayResults(processes, "RR");
}

void displayResults(vector <Process> processes, string s)
{
    Process temp;
    int tempEndTime;
    int wastedTime = 0;
    double fullTime = schedular.back().finishTime;
    temp = schedular.front();
    // if no process arrives on time 0
    if(temp.arrivalTime != 0)
    {
        wastedTime += temp.arrivalTime;
    }
    cout << s << ": ";
    cout << " " << temp.startTime << "   P" << temp.number << "   " << temp.finishTime << " ";
    tempEndTime = temp.finishTime;
    schedular.pop();
    // printing all schedular elements.
    while(!schedular.empty())
    {
        temp = schedular.front();
        if(tempEndTime != temp.startTime)
        {
            cout << " " << temp.startTime << " ";
            wastedTime += (temp.startTime - tempEndTime);
        }
        cout << "  P" << temp.number << "  " << temp.finishTime << " ";
        tempEndTime = temp.finishTime;
        
        schedular.pop();
    }
    cout << endl;
    cout << "\nProcess\tFinish time\tWaiting time\tTurnaround time\n";
    for(Process &p : processes)
    {
        cout << p.number << "\t" << p.finishTime << "\t\t" << p.waitingTime << "\t\t" << p.turnaroundTime << endl;
    }
    if(wastedTime == 0)
    {
        cout << "CPU Utilization: 100%" << endl;
    }
    else 
        cout << "CPU Utilization: " << 1 - (wastedTime / fullTime) << endl;
    cout << "\n\n";
}

