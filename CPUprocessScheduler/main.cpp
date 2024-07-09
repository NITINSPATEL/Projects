#include <bits/stdc++.h>
#include "parser.h"

#define all(v) v.begin(), v.end()

using namespace std;


/** Global Constants **/
const string TRACE = "trace";
const string SHOW_STATISTICS = "stats";
const string ALGORITHMS[9] = {"", "FCFS", "RR-", "SPN", "SRT", "HRRN", "FB-1", "FB-2i", "AGING"};

// differnet ways to give priority in AGING
bool sortByServiceTime(const tuple<string, int, int> &a, const tuple<string, int, int> &b)
{
    return (get<2>(a) < get<2>(b));
}
bool sortByArrivalTime(const tuple<string, int, int> &a, const tuple<string, int, int> &b)
{
    return (get<1>(a) < get<1>(b));
}
bool descendingly_by_response_ratio(tuple<string, double, int> a, tuple<string, double, int> b)
{
    return get<1>(a) > get<1>(b);
}
// comparator of heap : greater -> min heap , a > b : min-heap   ;;;;   comparator for sorting vector : a < b : ascending order sort ;;; 'a' is lhs sorted array and 'b' is rhs unsorted array , sort() is in-place sort ;;; 'b' finds its correst index traversing from lhs 'a' towards right side until returned value is 1
bool byPriorityLevel (const tuple<int,int,int>&a,const tuple<int,int,int>&b){
    if(get<0>(a)==get<0>(b))
        return get<2>(a)> get<2>(b);  // if this also same , higher processIndex element have higher priority due to order of insertion into vector
    return get<0>(a) > get<0>(b);
}

void clear_timeline() // convert each timeline instant to ' '
{
    for(int i=0; i<last_instant; i++)
        for(int j=0; j<process_count; j++)
            timeline[i][j] = ' ';
}

string getProcessName(tuple<string, int, int> &a)
{
    return get<0>(a);
}

int getArrivalTime(tuple<string, int, int> &a)
{
    return get<1>(a);
}

int getServiceTime(tuple<string, int, int> &a)
{
    return get<2>(a);
}

// above are comparator and retrieval functions

void fillInWaitTime(){
    for (int i = 0; i < process_count; i++)
    {
        int arrivalTime = getArrivalTime(processes[i]);
        for (int k = arrivalTime; k < finishTime[i]; k++)
        {
            if (timeline[k][i] != '*')
                timeline[k][i] = '.';
        }
    }
}

// priority is judged by waiting time , more the waiting time , more is the priority of the process
int getPriorityLevel(tuple<string, int, int> &a)
{
    return get<2>(a);
}

// feedback and round robin me arrival_time==0 me if ki jagah while is needed ;  pair<int,int> comparison is first by first value and then second value ; 

// PRE-EMPTIVE
void feedbackQ1()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;     // min-heap is maintained, pair of priority level TO process index, if highest priority element comes , it is stored as highest priority element (at top of heap , pq[0] index ) 
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service time
    int j=0;     // main counter of inputted processes
    while(j<process_count && getArrivalTime(processes[j])==0){
        pq.push(make_pair(0,j)); 
        remainingServiceTime[j]=getServiceTime(processes[j]);
        j++;
    }
    for(int time =0;time<last_instant;time++){
        if(!pq.empty()){
            int priorityLevel = pq.top().first;
            int processIndex =pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();
            while(j<process_count && getArrivalTime(processes[j])==time+1){
                    pq.push(make_pair(0,j));
                    remainingServiceTime[j]=getServiceTime(processes[j]);
                    j++;
            }
            remainingServiceTime[processIndex]--;
            timeline[time][processIndex]='*';
            if(remainingServiceTime[processIndex]==0){
                finishTime[processIndex]=time+1;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
            }else{
                if(pq.size()>=1)
                    pq.push(make_pair(priorityLevel+1,processIndex));
                else  // can be avoided , but for definition purpose , kept it here as follows
                    pq.push(make_pair(priorityLevel,processIndex));
            }
        }
        while(j<process_count && getArrivalTime(processes[j])==time+1){
                pq.push(make_pair(0,j));
                remainingServiceTime[j]=getServiceTime(processes[j]);
                j++;
        }
    }
    fillInWaitTime();
}

// PRE-EMPTIVE , time allote is 2^i
void feedbackQ2i()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //pair of priority level TO process index
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service time
    int j=0;
    while(j<process_count && getArrivalTime(processes[0])==0){
        pq.push(make_pair(0,j));
        remainingServiceTime[j]=getServiceTime(processes[j]);
        j++;
    }
    for(int time =0;time<last_instant;time++){
        if(!pq.empty()){
            int priorityLevel = pq.top().first;
            int processIndex =pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();
            while(j<process_count && getArrivalTime(processes[j])<=time+1){
                    pq.push(make_pair(0,j));
                    remainingServiceTime[j]=getServiceTime(processes[j]);
                    j++;
            }
            int currentQuantum = pow(2,priorityLevel);
            int temp = time;
            while(  currentQuantum && remainingServiceTime[processIndex]){   // temp < last_instant condition is maintained at time=temp-1 , below at the end of outside loop
                currentQuantum--;
                remainingServiceTime[processIndex]--;
                timeline[temp++][processIndex]='*';
            }
            if(remainingServiceTime[processIndex]==0){
                finishTime[processIndex]=temp;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
            }else{
                if(pq.size()>=1)
                    pq.push(make_pair(priorityLevel+1,processIndex));
                else
                    pq.push(make_pair(priorityLevel,processIndex));
            }
            time = temp-1;
        }
        while(j<process_count && getArrivalTime(processes[j])<=time+1){
                pq.push(make_pair(0,j));
                remainingServiceTime[j]=getServiceTime(processes[j]);
                j++;
        }
    }
    fillInWaitTime();
}

// NON-PRE-EMPTIVE FEEDBACK
void aging(int originalQuantum)
{
    vector<tuple<int,int,int>>v; // tuple of priority level, process index and total waiting time
    int j=0,currentProcess=-1;
    for(int time =0;time<last_instant;time++){
        while(j<process_count && getArrivalTime(processes[j])<=time){
            v.push_back(make_tuple(getPriorityLevel(processes[j]),j,0));
            j++;
        }

        for(int i=0;i<v.size();i++){
            if(get<1>(v[i])==currentProcess){
                get<2>(v[i])=0;     // waiting time is re-initialized to 0
                get<0>(v[i])=getPriorityLevel(processes[currentProcess]);   // priority level also becomes least possible , equal to 0
            }
            else{
                get<0>(v[i])++;     // boosting priority
                get<2>(v[i])++;     // increasing waiting time
            }
        }
        sort(v.begin(),v.end(),byPriorityLevel);
        currentProcess=get<1>(v[0]);
        int currentQuantum = originalQuantum;
        while(currentQuantum-- && time<last_instant){
            timeline[time][currentProcess]='*';
            time++;
        }
        time--;
    }
    fillInWaitTime();
}

double calculate_response_ratio(int wait_time, int service_time) // response ratio is calculated by taking the ratio of the waiting time of a process and its burst time
{
    return (wait_time + service_time)*1.0 / service_time;  // standard definition
}

// NON-PRE-EMPTIVE for current process not switches out reguarly AND PRE-EMPTIVE because current process switches out if response ratio of other process become greater at changing time instant , considered as NON-PRE-EMPTIVE MOSTLY
void highestResponseRatioNext()
{

    // Vector of tuple <process_name, process_response_ratio, time_in_service> for processes that are in the ready queue
    vector<tuple<string, double, int>> present_processes;
    int j=0;  // counter of inputted processes (names are encoded into indices in processToIndec map in parser.h file)
    for (int current_instant = 0; current_instant < last_instant; current_instant++)
    {
        while(j<process_count && getArrivalTime(processes[j])<=current_instant){
            present_processes.push_back(make_tuple(getProcessName(processes[j]), 1.0, 0));
            j++;
        }
        // Calculate response ratio for every process
        for (auto &proc : present_processes)    // aliasing avoid memory wastage in copying
        {
            string process_name = get<0>(proc);
            int process_index = processToIndex[process_name];
            int wait_time = current_instant - getArrivalTime(processes[process_index]);
            int service_time = getServiceTime(processes[process_index]);
            get<1>(proc) = calculate_response_ratio(wait_time, service_time);  // get<1> syntax is part of the C++ standard library's std::tuple utility, which allows for accessing the elements of a tuple
        }

        // Sort present processes by highest to lowest response ratio , can be pre-emptive here
        sort(all(present_processes), descendingly_by_response_ratio);

        if (!present_processes.empty())
        {
            int process_index = processToIndex[get<0>(present_processes[0])];
            // proces continues to run if come here till now , non-pre-emptive here
            while(current_instant<last_instant && get<2>(present_processes[0]) != getServiceTime(processes[process_index])){
                timeline[current_instant][process_index]='*';
                current_instant++;
                get<2>(present_processes[0])++;
            }
            current_instant--;
            present_processes.erase(present_processes.begin());
            finishTime[process_index] = current_instant + 1;
            turnAroundTime[process_index] = finishTime[process_index] - getArrivalTime(processes[process_index]);
            normTurn[process_index] = (turnAroundTime[process_index] * 1.0 / getServiceTime(processes[process_index]));
        }
    }
    fillInWaitTime();
}

//  PRE-EMPTIVE
void shortestRemainingTime()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;  // min-heap for remaining time and process_index
    int j = 0;  // main counter for inputted processes
    for (int i = 0; i < last_instant; i++)
    {
        while(j<process_count &&getArrivalTime(processes[j]) == i){
            pq.push(make_pair(getServiceTime(processes[j]), j));
            j++;
        }
        if (!pq.empty())
        {
            int processIndex = pq.top().second;
            int remainingTime = pq.top().first;
            pq.pop();
            int serviceTime = getServiceTime(processes[processIndex]);
            int arrivalTime = getArrivalTime(processes[processIndex]);
            timeline[i][processIndex] = '*';

            if (remainingTime == 1) // process finished
            {
                finishTime[processIndex] = i + 1;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
            }
            else
            {
                pq.push(make_pair(remainingTime - 1, processIndex));
            }
        }
    }
    fillInWaitTime();
}

// NON-PRE-EMPTIVE
void shortestProcessNext()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // pair of service time and index , min-heap bn rhi h yha pe
    int j = 0;  // counter for processes count
    for (int i = 0; i < last_instant; i++)
    {
        while(j<process_count && getArrivalTime(processes[j]) <= i){
            pq.push(make_pair(getServiceTime(processes[j]), j));
            j++;
        }
        if (!pq.empty())
        {
            int processIndex = pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();

            int temp = arrivalTime;
            for (; temp < i; temp++)
                timeline[temp][processIndex] = '.';

            temp = i;
            for (; temp < i + serviceTime; temp++)
                timeline[temp][processIndex] = '*';

            finishTime[processIndex] = (i + serviceTime);
            turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
            normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
            i = temp - 1;
        }
    }
}

// PRE-EMPTIVE
void roundRobin(int originalQuantum)
{
    queue<pair<int,int>>q;
    int j=0;
    while(j<process_count &&  getArrivalTime(processes[j])==0){
        q.push(make_pair(j,getServiceTime(processes[j])));
        j++;
    }
    int currentQuantum = originalQuantum;
    for(int time =0;time<last_instant;time++){
        if(!q.empty()){
            int processIndex = q.front().first;
            q.front().second = q.front().second-1;
            int remainingServiceTime = q.front().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            currentQuantum--;
            timeline[time][processIndex]='*';
            while(j<process_count && getArrivalTime(processes[j])==time+1){  // pushed into queue only when current (time instant + 1 )== next process's (jth counter ka next process, not queue ka next process) arrival time
                q.push(make_pair(j,getServiceTime(processes[j])));
                j++;
            }

            if(currentQuantum==0 && remainingServiceTime==0){ 
                finishTime[processIndex]=time+1;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
                currentQuantum=originalQuantum;
                q.pop();
            }else if(currentQuantum==0 && remainingServiceTime!=0){     // pop from first and push at back
                q.pop();
                q.push(make_pair(processIndex,remainingServiceTime));
                currentQuantum=originalQuantum;
            }else if(currentQuantum!=0 && remainingServiceTime==0){     // job completed
                finishTime[processIndex]=time+1;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
                q.pop();
                currentQuantum=originalQuantum;     // here also , current quantum renewed
            }
        }
        while(j<process_count && getArrivalTime(processes[j])==time+1){
            q.push(make_pair(j,getServiceTime(processes[j])));
            j++;
        }
    }
    fillInWaitTime();
}

// ("process_name",arrival_time instant, service_time(delta T required) )

// NON-PRE-EMPTIVE
void firstComeFirstServe()
{   
    int time = getArrivalTime(processes[0]);
    for (int i = 0; i < process_count; i++)
    {
        int processIndex = i;
        int arrivalTime = getArrivalTime(processes[i]);
        int serviceTime = getServiceTime(processes[i]);
        if(arrivalTime <= time )
            finishTime[processIndex] = (time + serviceTime);
        else
            finishTime[processIndex] = (arrivalTime + serviceTime);

        turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
        normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);

        for (int j = time; j < finishTime[processIndex] && arrivalTime <= time ; j++)
            timeline[j][processIndex] = '*';
        for (int j = arrivalTime; j < time; j++)
            timeline[j][processIndex] = '.';
        for (int j = arrivalTime ; j < finishTime[processIndex] && arrivalTime > time ; j++)
            timeline[j][processIndex] = '*';

        time += serviceTime;
    }
}

void printAlgorithm(int algorithm_index)
{
    int algorithm_id = algorithms[algorithm_index].first - '0';
    if(algorithm_id==2 || algorithm_id == 8)
        cout << ALGORITHMS[algorithm_id] <<"-"<<algorithms[algorithm_index].second <<endl;
    else
        cout << ALGORITHMS[algorithm_id] << endl;
}
void printProcesses()
{
    cout << "Process    ";
    for (int i = 0; i < process_count; i++)
        cout << "|  " << getProcessName(processes[i]) << "  ";
    cout << "|\n";
}
void printArrivalTime()
{
    cout << "Arrival    ";
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ",getArrivalTime(processes[i]));
    cout<<"|\n";
}
void printServiceTime()
{
    cout << "Service    |";
    for (int i = 0; i < process_count; i++)
        printf("%3d  |",getServiceTime(processes[i]));
    cout << " Mean|\n";
}
void printFinishTime()
{
    cout << "Finish     ";
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ",finishTime[i]);
    cout << "|-----|\n";
}
void printTurnAroundTime()
{
    cout << "Turnaround |";
    double sum = 0;
    for (int i = 0; i < process_count; i++)
    {
        printf("%3d  |",turnAroundTime[i]);
        sum += turnAroundTime[i];
    }
    if((1.0 * sum / turnAroundTime.size())>=10)
		printf("%2.2f|\n",(1.0 * sum / turnAroundTime.size()));
    else
	 	printf(" %2.2f|\n",(1.0 * sum / turnAroundTime.size()));
}
void printNormTurn()
{
    cout << "NormTurn   |";
    double sum = 0;
    for (int i = 0; i < process_count; i++)
    {
        if( normTurn[i]>=10 )
            printf("%2.2f|",normTurn[i]);
        else
            printf(" %2.2f|",normTurn[i]);
        sum += normTurn[i];
    }

    if( (1.0 * sum / normTurn.size()) >=10 )
        printf("%2.2f|\n",(1.0 * sum / normTurn.size()));
	else
        printf(" %2.2f|\n",(1.0 * sum / normTurn.size()));
}
void printStats(int algorithm_index)
{
    printAlgorithm(algorithm_index);
    printProcesses();
    printArrivalTime();
    printServiceTime();
    printFinishTime();
    printTurnAroundTime();
    printNormTurn();
}
void printTimeline(int algorithm_index)
{
    for (int i = 0; i <= last_instant; i++)
        cout << i % 10<<" ";
    cout <<"\n";
    cout << "------------------------------------------------\n";
    for (int i = 0; i < process_count; i++)
    {
        cout << getProcessName(processes[i]) << "     |";
        for (int j = 0; j < last_instant; j++)
        {
            cout << timeline[j][i]<<"|";
        }
        cout << " \n";
    }
    cout << "------------------------------------------------\n";
}

void execute_algorithm(char algorithm_id, int quantum,string operation)
{
    switch (algorithm_id)
    {
    case '1':  // DONE
        if(operation==TRACE)cout<<"FCFS  ";
        firstComeFirstServe();
        break;
    case '2':
        if(operation==TRACE)cout<<"RR-"<<quantum<<"  ";
        roundRobin(quantum);
        break;
    case '3':
        if(operation==TRACE)cout<<"SPN   ";
        shortestProcessNext();
        break;
    case '4':
        if(operation==TRACE)cout<<"SRT   ";
        shortestRemainingTime();
        break;
    case '5':
        if(operation==TRACE)cout<<"HRRN  ";
        highestResponseRatioNext();
        break;
    case '6':
        if(operation==TRACE)cout<<"FB-1  ";
        feedbackQ1();
        break;
    case '7':
        if(operation==TRACE)cout<<"FB-2^i";
        feedbackQ2i();
        break;
    case '8':
        if(operation==TRACE)cout<<"Aging";
        aging(quantum);
        break;
    default:
        break;
    }
}

int main()
{
    parse();
    for (int idx = 0; idx < (int)algorithms.size(); idx++)   // algorithms from parser.h , e.g. ('0',2) is one of the element
    {
        clear_timeline();
        execute_algorithm(algorithms[idx].first, algorithms[idx].second,operation);   
        if (operation == TRACE)                 // operation is same for all algorithms
            printTimeline(idx);
        else if (operation == SHOW_STATISTICS)  // operation is same for all algorithms
            printStats(idx);
        cout << "\n";
    }
    return 0;
}
