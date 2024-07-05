## Files ##
nsp_template_library.cpp

#Overview#
- This C++ file consists of 3 classes containing poplar functions which avoid re-writing of them everytime we work on them. These can be readily used by including them in your project
- First class is on GRAPHS : For both directed/undirected graphs ,insert, do BFS, DFS, Shortest Path/Walk (negative weights also) , Cycle Detection, etc.
- Second class is on Tries : Insert into trie and do time-efficient search of words in it
- Third class is on KMP : Find occurences of pattern in another long string of text in time-efficient manner
- Fourth class in on BigInt : Does operator overloading to deal with arithmetic operations on very big integers (bigger than unsigned long long int) which can't be handled in C++ . Finds fibonacci , catalan , Factorial numbers, etc.

## NOTE ##
- All these have been written in time and space efficient manner which can be readily included and serve as helper classes in other C++ projects to fasten the process of creating projects
