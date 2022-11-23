#include "cache.h"
#include <vector>
#include <map>
#include <algorithm>

Cache::Cache(uint32_t cacheSize, uint32_t blockSize, int nWays, bool debug){

    // Cache configuration
    this->cacheSize = cacheSize;
    this->blockSize = blockSize;
    this->nWays = nWays;

    // Cache stats
    this->hits = 0;
    this->totalAccesses = 0;
    this->debug = debug;

    // Cache number of sets
    this->sets = this->cacheSize / (this->blockSize * this->nWays);  // Currently hard coded

    // Address field bits
    this->offsetBits = log2(blockSize);
    //this->indexBits = log2(this->sets);
    this->indexBits = (nWays == 0) ? 0 : log2(this->cacheSize / this->blockSize) - log2(nWays);  // Currently hard coded. 
    this->tagBits = ADDRESS_LENGTH - this->indexBits - this->offsetBits;

    // Do not change the following print statements
    printf("------------------------------\n");
    printf("Cache size: %" PRIu32 " Bytes\nBlock Size: %" PRIu32 " Bytes\nSets: %d\nWays: %d\nTag bits: %d\nIndex bits: %d\nOffset bits: %d\n", 
            this->cacheSize, this->blockSize, this->sets, this->nWays, this->tagBits, this->indexBits, this->offsetBits);
    printf("------------------------------\n");
}


bool Cache::checkIfHit(vector<int> line, int tag){
	
	bool hit = false;

	vector<int>::iterator itr;
	itr = find(line.begin(), line.end(), tag);

	if(itr != line.end())
		hit = true;

	return hit;

}


void Cache::updateCacheLine(vector<int>& line, int tag){
	
        vector<int>::iterator itr;
        itr = find(line.begin(), line.end(), tag);
	
	if(itr != line.end()){
		int value = *itr;
		line.erase(itr);
		line.push_back(value);
	}

}


bool Cache::accessCache(string address, int addressOffset){
    
    // Get address fields
    address_t addr = this->parseAddress(address, addressOffset);

    // Check for cache hit
    bool hit = checkIfHit(cache[addr.index % this->sets], addr.tag);

    if(hit){
        // Cache hit. Yay!
	updateCacheLine(cache[addr.index % this->sets], addr.tag);
    }

    else // If miss, then load cache block
    {
        //cache[addr.index] = addr.tag;
	//if the size of cache line i.e., row is equal to the no ways -> lru
	//else push the latest entry into cache line
	
	if(cache[addr.index].size() == this->nWays){
		cache[addr.index].erase(cache[addr.index % this->sets].begin());
	}

	cache[addr.index % this->sets].push_back(addr.tag);
	
    }

    // Update stats
    if(hit)
      this->hits += 1;
    this->totalAccesses += 1;

    return hit;
}



// DO NOT CHANGE FILE BELOW THIS LINE

address_t Cache::parseAddress(string address, int addressOffset){
    istringstream converter(address);
    uint64_t conversion;
    converter >> std::hex >> conversion;

    // Add the addressOffset to the address
    conversion += addressOffset;

    // Truncate to 32-bit address
    uint32_t conversion32;
    conversion32 = (uint32_t)(conversion);

    /*calculate the offset */ 
    unsigned int mask = (1 << this->offsetBits) - 1; 
    uint32_t offset = conversion32 & mask;

    /*calculate the index */
    mask = (1 << this->indexBits) - 1;
    uint32_t index = (conversion32 >> this->offsetBits) & mask; 

    /*calculate the tag */
    uint32_t tag = conversion32 >> (this->offsetBits + this->indexBits);

    if(this->debug){
        printf("Address: %s\n",address.c_str());
        printf("Offset: %d\n",addressOffset);
        printf("address + offest: %lu\n",(unsigned long)conversion32);
        printf("tag: %" PRIu32 "\n",(uint32_t)tag);
        printf("index: %" PRIu32 "\n",(uint32_t)index);   
        printf("offset: %" PRIu32 "\n",(uint32_t)offset);

    }  

    address_t parts = { (uint32_t)tag, (uint32_t)index, (uint32_t)offset} ;

    return parts;
}
void Cache::printStats(){ 
    if(this->totalAccesses == 0){
        printf("Error, Cannot get rate \n");
    }
    else{
        printf("Hit rate: %.2f%%\n", 100.00 * (double)(this->hits)/this->totalAccesses);
        printf("Miss rate: %.2f%%\n", 100.00 * (1 - (double)(this->hits)/this->totalAccesses));
    } 
    printf("Hits: %d\n", this->hits);
    printf("total: %d\n",this->totalAccesses);
    printf("------------------------------\n\n"); 
}
