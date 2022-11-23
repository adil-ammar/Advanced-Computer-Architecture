#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include "trace.h"
#include "cache.h"

using namespace std;

uint32_t convertSizeInput(string input);

int main(int argc, char ** argv){
    
    int opt;
    string fileName = "";
    string cacheString = "2KB";
    string blockString = "128B";
    int nWays = 1;
    bool debug = false;

	while ((opt = getopt(argc,argv,"i:c:b:w:d")) != EOF){
        switch(opt){
            case 'i': fileName.assign(optarg); break;
            case 'c': cacheString.assign(optarg); break;
            case 'b': blockString.assign(optarg); break;
            case 'w': nWays = atoi(optarg); break;
            case 'd': debug = true; break;
            case '?': printf("usage is ./cachesim -i <tracefile> -c <cache size> -b <block size> -w <number of ways>\n -i : input trace fileName \n -c : cache size\n -b : block size\n -w : N-ways\n cache and block size supports B, KB, MB, GB suffix\n");
            default: exit(1);
        }
    }
    if(fileName.empty()){
        printf("usage is ./cachesim -i <tracefile> -c <cache size> -b <block size> -w <number of ways>\n -i : input trace fileName \n -c : cache size\n -b : block size\n -w : N-ways\n cache and block size supports B, KB, MB, GB suffix\n");
	exit(1);
    }

    uint32_t cacheSize = convertSizeInput(cacheString);
    uint32_t blockSize = convertSizeInput(blockString);
    if(cacheSize == 0 || blockSize == 0 || (cacheSize < blockSize)){
        printf("Error assigning cacheSize or block size\n");
        return -1;
    }

    // Initialize Trace File
    TraceFile trace(fileName);

    unsigned char memType;
    int offset;
    bool hit;
    string address = "";

    // Initialize Cache
    Cache cache(cacheSize, blockSize, nWays, debug);

    while(trace.getNextMemoryAccess(memType, offset, address)){
       hit = cache.accessCache(address, offset);
       if(debug){
         printf("%s %d(0x%s) %s!\n\n", memType ? "STORE" : "LOAD", offset, address.c_str(), hit ? "HIT" : "MISS");
       }
    }

    // Print results
    cache.printStats();

    return 0;
}





uint32_t convertSizeInput(string input){
    uint32_t result = 0;
    unsigned int position = 0;
    for(position = 0; position < input.size(); ++position){
        if(!isdigit(input.at(position))){
            break;
        }
    }
    result = atoi(input.substr(0,position).c_str());
    if(ceil(log2(result)) != floor(log2(result))){
        return 0;
    }
    int bits = log2(result);
    string suffix = input.substr(position);
    if(suffix == "" || suffix == "B"){
        bits += 0;
    }
    else if(suffix == "KB"){
        bits += 10;
    }
    else if(suffix == "MB"){
        bits += 20;
    }
    else if(suffix == "GB"){
        bits += 30;
    }
    else{
        return 0;
    }
    return pow(2, bits);
}

