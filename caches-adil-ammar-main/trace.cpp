#include "trace.h"

TraceFile::TraceFile(string fileName){
    this->fileName = fileName;
    this->inputStream.open(this->fileName.c_str());
    if(!this->inputStream.is_open()){
        cout << "error opening file: " << this->fileName << endl;
        exit(1);
    }   
}


bool TraceFile::getNextMemoryAccess(unsigned char & memType, int & offset, string & address){
    if(!this->inputStream.is_open()){
        return 0;
    }   
    string input1, input2, input3;
    if(this->inputStream >> input1 >> input2 >> input3){
        if(input1 == "L"){
            memType = LOAD;
        }
        else if(input1 == "S"){
            memType = STORE;
        }
        else{
            return 0;
        }
        istringstream(input2) >> offset;
        address = input3;
        return 1;
    }
    else{
        this->inputStream.close();
        return 0;
    }
}
