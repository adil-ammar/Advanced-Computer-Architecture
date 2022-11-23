#ifndef __BRANCH_H_
#define __BRANCH_H_

#include <iostream>
#include <map>
using namespace std;

class Predictor{
    private:

        unsigned int addrBits; //number of bits of address used to index  
        unsigned int historyBits; //number of bits used for global history
        unsigned int globalHistory; //record of global histroy
        int correct;
        int total;
        bool debug;
	unsigned int hexToInt(string);
        unsigned int truncateAddress (unsigned int);
        void updateGlobalHistory(bool); //update the history
	int n;
	struct entry {
		int value;
		bool flag = false;
	};

	map<int, map<int, entry>> bHT;
	int doTwoBitPrediction(int, bool);
	int doOneBitPrediction(int, bool);
	int getLSMbitsFromGHB();
	long unsigned int countBHTUsed();

    public:
        Predictor(unsigned int, unsigned int, unsigned int, bool);
        bool makePrediction(string, bool);
        void printStats();
};

#endif
