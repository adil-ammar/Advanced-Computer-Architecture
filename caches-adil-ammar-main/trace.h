#ifndef __TRACE_H_
#define __TRACE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

enum MEM_TYPE: unsigned char {LOAD, STORE};

class TraceFile{
    private:
        string fileName;
        ifstream inputStream;

    public:
        TraceFile(string fileName);
        bool getNextMemoryAccess(unsigned char &, int &, string &);
};

#endif
