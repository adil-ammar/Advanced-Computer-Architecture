#include "predictor.h"
#include <sstream>
#include <iomanip>
#include <map>
#include <math.h>


Predictor::Predictor(unsigned int m, unsigned int n, unsigned int addrLength, bool debug){
    this->historyBits = m; // Size of global history
    this->globalHistory = 0; 
    this->addrBits = addrLength;
    this->debug = debug;
    this->correct = 0;
    this->total = 0;
    this->n = n;
    //this->bHT = new map<int, vector<int>>;
    printf("BHT: %u-bit\n", n);
    printf("Address length: %u\n", this->addrBits);
    printf("History length: %u\n", this->historyBits);
    printf("Debug mode: %u\n", this->debug);
};


void Predictor::updateGlobalHistory(bool expected){
    globalHistory = globalHistory << 1;
    globalHistory = globalHistory | expected;
    unsigned int mask = (1 << this->historyBits) - 1;
    globalHistory = globalHistory & mask; 
}


int Predictor::getLSMbitsFromGHB(){
	int answer = 0;
	
	answer = globalHistory & ((1 << historyBits) - 1);

	return answer;
}

int Predictor::doOneBitPrediction(int current , bool actual){
	
	int next_state = 0;

	// 1-bit predictor
       	if(actual) next_state = 1;

	return next_state;
}

int Predictor::doTwoBitPrediction(int current , bool actual){
	
	int next_state = 0;
 
 	// 2-bit predictor
        switch(current){
		
		case 0:
			if(actual == 1) next_state = 1;
			break;

                case 1:
			if(actual == 1) next_state = 3;
			break;

                case 2:
			if(actual == 1) next_state = 3;
			break;

                case 3:
			if(actual == 1) next_state = 3;
			else next_state = 2;
			break;
	}

	return next_state;
}


long unsigned int Predictor::countBHTUsed(){
	
	long unsigned int bht_used = 0;

	for(map<int, map<int, entry>>::iterator it = bHT.begin(); it != bHT.end(); it++){
		for(map<int, entry>::iterator itr = it->second.begin(); itr != it->second.end(); itr++){
			if(itr->second.flag == true) bht_used++;
		}
	}

	return bht_used;
}


bool Predictor::makePrediction(string input, bool expected){
    // Convert Hex address to integer address
    int address = truncateAddress(hexToInt(input));

    // Currently, this simple branch predictor simulator simply takes 
    // the previous observed branch direction as the next prediction.
    // Predict branch based on last observed branch
    // bool predicted = globalHistory & 1;
    
    bool predicted = false;
    bool flag = false;
    int tableNumber = getLSMbitsFromGHB();
    int current, next_state;
    
    if(n == 1){

 	    for(map<int, map<int, entry>>::iterator it = bHT.begin(); it != bHT.end(); it++){
                if(it->first == address){
                        flag = true;
                }
            }

            if(!flag){
                for(int i = 0; i < pow(2, historyBits); i++){
                        bHT[address][i].value = 0;
                }
            }

	    
	    
	    // 1-bit predictor
	    current = bHT[address][tableNumber].value;
	    if(current == 1)
		    predicted = true;
	    else 
		    predicted = false;

	    next_state = doOneBitPrediction(current, expected);
    }
    
    else{
	    // 2-bit predictor

	    for(map<int, map<int, entry>>::iterator it = bHT.begin(); it != bHT.end(); it++){
	    	if(it->first == address){
			flag = true;
		}	
	    }

	    if(!flag){
	    	for(int i = 0; i < pow(2, historyBits); i++){
			bHT[address][i].value = 0;
		}
	    }

	    current = bHT[address][tableNumber].value;

	    if(current > 1)
		    predicted = true;
	    else
		    predicted = false;

	    next_state = doTwoBitPrediction(current, expected);
    }

    bHT[address][tableNumber].value = next_state;
    bHT[address][tableNumber].flag = true;


    // Update global history 
    updateGlobalHistory(expected);

    // Update statistics
    if(predicted == expected){
        this->correct++;
    }

    this->total++;

    return predicted;
}

/*
 * Print out branch predictor statistics
 */
void Predictor::printStats(){ 
    printf("------------------------------------------\n"); 
    if(total == 0){
        printf("Error, Cannot get rate \n");
    }
    else{
        printf("Misclassification rate: %.2f%%\n", 100.00 * (1 - (double)(this->correct)/this->total));
    } 
    
    printf("correct: %d\n", this->correct);
    printf("total: %d\n",this->total);


    // Update the following line to print out the number of BHT entires used.
    long unsigned int bht_used = countBHTUsed();
    printf("BHT used %lu entries\n", bht_used);
}

/*
 * DO NOT MODIFY BELOW THIS
 */


/*
 * Convert Hex string from trace file to integer address
 */
unsigned int Predictor::hexToInt(string input){
    istringstream converter(input);
    unsigned int result;
    converter >> std::hex >> result; 
    return result;
}

/*
 * Truncate Address to specified number of address bits
 */
unsigned int Predictor::truncateAddress(unsigned int input){
    unsigned int mask = (1 << this->addrBits) - 1;
    unsigned int result = input & mask;

    return result;
}

