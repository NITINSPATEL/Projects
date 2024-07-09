#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <bits/stdc++.h>

using namespace std;

/** This file handles parsing the data we are going to work with **/
/** It also holds all the global variables we parse into         **/


string operation; // trace or stats
int last_instant, process_count;  // explained below
vector<pair<char, int>> algorithms; // inputted algorithms , e.g. ('0',2) is one of the element
vector<tuple<string,int,int>> processes;  // processes info , ("process_name",arrival_time instant, service_time(delta T required) )
vector<vector<char>>timeline;  // timeline of CPU we are handling , each time instant is either '.' or '*' or ' '
unordered_map<string,int>processToIndex;  // encoding of process name from 0 to ( process_count - 1 ) , preferrable to give input process in increasing order of arrival times


//Results
// explained below
vector<int>finishTime;
vector<int>turnAroundTime;
vector<float>normTurn;


void parse_algorithms(string algorithm_chunk)
{
    stringstream stream(algorithm_chunk);
    while (stream.good())  // loop runs as long as stream is in a good state (i.e., not at the end of the stream) 
    {
        string temp_str;
        getline(stream, temp_str, ',');
        stringstream ss(temp_str);
        getline(ss, temp_str, '-');
        char algorithm_id = temp_str[0];
        getline(ss, temp_str, '-');     // getline starts reading ss string from (index+1)th where it left  
        int quantum = temp_str.size() >= 1 ? stoi(temp_str) : -1;   // values are truncated , stoi 
        algorithms.push_back( make_pair(algorithm_id, quantum) );
    }
}

void parse_processes()
{
    string process_chunk, process_name;
    int process_arrival_time, process_service_time;
    for(int i=0; i<process_count; i++)
    {
        cin >> process_chunk;

        stringstream stream(process_chunk);
        string temp_str;
        getline(stream, temp_str, ',');
        process_name = temp_str;
        getline(stream, temp_str, ',');
        process_arrival_time = stoi(temp_str);  // values are truncated , stoi (int conversion)
        getline(stream, temp_str, ',');
        process_service_time = stoi(temp_str);

        processes.push_back( make_tuple(process_name, process_arrival_time, process_service_time) );
        processToIndex[process_name] = i;

    }
}

void parse()
{
    string algorithm_chunk;
    cin >> operation >> algorithm_chunk >> last_instant >> process_count;  // last_instant is basically the total no. of periods for which process will last after t=0 
    parse_algorithms(algorithm_chunk);
    parse_processes();
    finishTime.resize(process_count);    // finish time (t'th second) for each process
    turnAroundTime.resize(process_count);   // ( waiting time + process time ) for each process = delta time
    normTurn.resize(process_count);   // (turnaround time / service time) for each process
    timeline.resize(last_instant);      // resizing timeline of our CPU process
    for(int i=0; i<last_instant; i++)
        for(int j=0; j<process_count; j++)
            timeline[i].push_back(' ');
}


#endif // PARSER_H_INCLUDED
