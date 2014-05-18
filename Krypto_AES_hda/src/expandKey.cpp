/*
 * expandKey.cpp
 *
 *  Created on: 18.05.2014
 *      Author: joergpfendert
 */

#include "expandKey.h"

expandKey::expandKey() {
	// TODO Auto-generated constructor stub

}

expandKey::~expandKey() {
	// TODO Auto-generated destructor stub
}


uint8_t expandKey::getSboxValue(uint8_t val){
	return SBox[val];
};

void expandKey::rotateWord(char* tmp){
	 char dummy = tmp[0];
	    for (int i = 0; i < 3; i++) {
	        tmp[i] = tmp[i + 1];
	    }
	    tmp[3] = dummy;
}

char expandKey::getRconValue(char val){
	 return Rcon[val];
}

void expandKey::keyScheduleCore(char* tmp, int iteration){
	rotateWord(tmp);

	    for (int i = 0; i < 4; ++i) {
	        tmp[i] = getSboxValue(tmp[i]);
	    }

	    tmp[0] = tmp[0] ^ getRconValue(iteration); //NUR alle 4 mal beim ersten Byte!!!!!!!!!!!!! TEST XOR!!!!
}

void expandKey::MakeExpandKey(){

	int currentSize = 0;
	    int rconIteration = 1;

	    char tmp[4] = { 0 };   // Temporary 4-byte variable

	    // Set the 16,24,32 bytes of the expanded key to the input key
	    for (int i = 0; i < SIZE; i++) {
	        expandedKey[i] = key[i];
	    }

	    currentSize += SIZE;

	    while (currentSize < EXPANDEDSIZE) {
	        // Assign the previous 4 bytes to the temporary value t
	        for (int i = 0; i < 4; i++) {
	            tmp[i] = expandedKey[(currentSize - 4) + i];
	        }

	        // Every 16,24,32 bytes we apply the core schedule to t
	        // and increment rconIteration afterwards
	        if(currentSize % SIZE == 0) {
	            keyScheduleCore(tmp, rconIteration++);
	        }



	        for(int i = 0; i < 4; i++) {
	            expandedKey[currentSize] = expandedKey[currentSize - SIZE] ^ tmp[i];
	            currentSize++;
	        }
	    }


}
