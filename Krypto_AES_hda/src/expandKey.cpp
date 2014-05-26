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

uint8_t expandKey::getSboxValue(uint8_t val) {
	return SBox[val];
}

uint8_t expandKey::getRsboxValue(uint8_t val) {
	return RSBox[val];
}

void expandKey::rotate(char* tmp) {
	char dummy = tmp[0];
	for (int i = 0; i < 3; i++) {
		tmp[i] = tmp[i + 1];
	}
	tmp[3] = dummy;
}

uint8_t expandKey::getRconValue(uint8_t val) {
	return Rcon[val];
}

void expandKey::subBytes(uint8_t* state){
	 for (int i = 0; i < 16; i++) {
	        state[i] = getSboxValue(state[i]);
	    }
}


void expandKey::invSubBytes(uint8_t* state)
{
    uint8_t i;

    // Substitute all the values from the state with the value in the SBox
    // using the state value as index for the SBox
    for (i = 0; i < 16; i++) {
        state[i] = getRsboxValue(state[i]);
    }
}

void expandKey::expScheduleTask(char* tmp, int value) {
	rotate(tmp);

	for (int i = 0; i < 4; ++i) {
		tmp[i] = getSboxValue(tmp[i]);
	}

	tmp[0] = tmp[0] ^ getRconValue(value); //NUR alle 4 mal beim ersten Byte!!!!!!!!!!!!!
}

void expandKey::MakeExpandKey() {

	int currentSize = 0;
	int rconIteration = 1;

	char tmp[4] = { 0 };

	for (int i = 0; i < SIZE; i++) {
		expandedKey[i] = key[i];
	}

	currentSize += SIZE;

	while (currentSize < EXPANDEDSIZE) {

		for (int i = 0; i < 4; i++) {
			tmp[i] = expandedKey[(currentSize - 4) + i];
		}

		if (currentSize % SIZE == 0) {
			expScheduleTask(tmp, rconIteration++);
		}

		for (int i = 0; i < 4; i++) {
			expandedKey[currentSize] = expandedKey[currentSize - SIZE] ^ tmp[i];
			currentSize++;
		}
	}

}

void expandKey::startShift(uint8_t* state){
	for (int i = 1; i < 4; i++) {		//Starte erst beim 5. Byte
	        shiftRows(state + i * 4, i);
	    }
}

uint8_t expandKey::mulGaloisField2_8(uint8_t a, uint8_t b){
	  uint8_t p = 0;
	  uint8_t hi_bit_set;


	    for (int counter = 0; counter < 8; counter++) {
	        if ((b & 1) == 1)
	            p ^= a;
	        hi_bit_set = (a & 0x80);    // xtime() ...
	        a <<= 1;
	        if (hi_bit_set == 0x80)
	            a ^= 0x1b;              // ...
	        b >>= 1;
	    }

	    return p;
}

void expandKey::shiftRows(uint8_t* state, int pos){

	 for (int i = 0; i < pos; i++) {
	        uint8_t tmp = state[0];
	        for (int j = 0; j < 3; j++) {
	            state[j] = state[j + 1];
	        }
	        state[3] = tmp;
	    }

}

void expandKey::invShiftRow(uint8_t* state, uint8_t nbr)
{
    uint8_t  tmp;

    // Each iteration shifts the row to the right by 1
    for (int i = 0; i < nbr; i++) {
        tmp = state[3];
        for (int j = 3; j > 0; j--) {
            state[j] = state[j - 1];
        }
        state[0] = tmp;
    }
}

void expandKey::invShiftRows(uint8_t* state)
{


    // Iterate over the 4 rows and call invShiftRow() with that row
    for (int i = 1; i < 4; i++) {
        invShiftRow(state + (i * 4), i);
    }
}

void expandKey::mixColumn(uint8_t* column){
	 uint8_t cpy[4];

	    for(int i = 0; i < 4; i++) {
	        cpy[i] = column[i];
	    }

	    column[0] = mulGaloisField2_8(cpy[0], 2) ^
	                mulGaloisField2_8(cpy[1], 3) ^
	                mulGaloisField2_8(cpy[2], 1) ^
	                mulGaloisField2_8(cpy[3], 1);

	    column[1] = mulGaloisField2_8(cpy[0], 1) ^
	                mulGaloisField2_8(cpy[1], 2) ^
	                mulGaloisField2_8(cpy[2], 3) ^
	                mulGaloisField2_8(cpy[3], 1);

	    column[2] = mulGaloisField2_8(cpy[0], 1) ^
	                mulGaloisField2_8(cpy[1], 1) ^
	                mulGaloisField2_8(cpy[2], 2) ^
	                mulGaloisField2_8(cpy[3], 3);

	    column[3] = mulGaloisField2_8(cpy[0], 3) ^
	                mulGaloisField2_8(cpy[1], 1) ^
	                mulGaloisField2_8(cpy[2], 1) ^
	                mulGaloisField2_8(cpy[3], 2);
}

void expandKey::mixColumns(uint8_t* state){
	    uint8_t column[4];

	    for (int i = 0; i < 4; i++) {

	        for (int j = 0; j < 4; j++) {
	            column[j] = state[(j * 4) + i];
	        }

	        mixColumn(column);

	        for (int j = 0; j < 4; j++) {
	            state[(j * 4) + i] = column[j];
	        }
	    }
}

void expandKey::addRoundKey(uint8_t* state, uint8_t* roundKey){
	 for (int i = 0; i < 16; i++) {
	        state[i] = state[i] ^ roundKey[i] ;
	    }
}

void expandKey::createRoundKey(const uint8_t* expandedKey, uint8_t* roundKey){

	    for (int i = 0; i < 4; i++) {

	        for (int j = 0; j < 4; j++) {
	            roundKey[(i + (j * 4))] = expandedKey[(i * 4) + j];
	        }
	    }
}

void expandKey::round(uint8_t* state, uint8_t* roundKey){
	  subBytes(state);
	    startShift(state);
	    mixColumns(state);
	    addRoundKey(state, roundKey);
}

void expandKey::finalRound(uint8_t* state, uint8_t* roundkey){
subBytes(state);
startShift(state);
addRoundKey(state, roundkey);
}

void expandKey::invMixColumn(uint8_t* column)
{
    uint8_t i;
    uint8_t cpy[4];

    for(i = 0; i < 4; i++) {
        cpy[i] = column[i];
    }

    column[0] = mulGaloisField2_8(cpy[0], 0x0E) ^
                mulGaloisField2_8(cpy[1], 0x0B) ^
                mulGaloisField2_8(cpy[2], 0x0D) ^
                mulGaloisField2_8(cpy[3], 0x09);

    column[1] = mulGaloisField2_8(cpy[0], 0x09) ^
                mulGaloisField2_8(cpy[1], 0x0E) ^
                mulGaloisField2_8(cpy[2], 0x0B) ^
                mulGaloisField2_8(cpy[3], 0x0D);

    column[2] = mulGaloisField2_8(cpy[0], 0x0D) ^
                mulGaloisField2_8(cpy[1], 0x09) ^
                mulGaloisField2_8(cpy[2], 0x0E) ^
                mulGaloisField2_8(cpy[3], 0x0B);

    column[3] = mulGaloisField2_8(cpy[0], 0x0B) ^
                mulGaloisField2_8(cpy[1], 0x0D) ^
                mulGaloisField2_8(cpy[2], 0x09) ^
                mulGaloisField2_8(cpy[3], 0x0E);
}

void expandKey::invMixColumns(uint8_t* state)
{

    uint8_t column[4];

    // Iterate over the 4 columns
    for (int i = 0; i < 4; i++) {
        // Construct one column by iterating over the 4 rows
        for (int j = 0; j < 4; j++) {
            column[j] = state[(j * 4) + i];
        }

        // Apply the invMixColumn on one column
        invMixColumn(column);

        // Put the values back into the state
        for (int j = 0; j < 4; j++) {
            state[(j * 4) + i] = column[j];
        }
    }
}

void expandKey::invRound(uint8_t* state, uint8_t* roundKey)
{
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, roundKey);
    invMixColumns(state);
}

void expandKey::aes(uint8_t* state,  uint8_t* expandedKey, uint32_t Nr){
	 uint8_t roundKey[16];

	    createRoundKey(expandedKey, roundKey);

	    addRoundKey(state, roundKey);

	    for (int i = 1; i < Nr; i++) {
	        createRoundKey(expandedKey + 16 * i, roundKey);
	        round(state, roundKey);
	    }

	    createRoundKey(expandedKey + 16 * Nr, roundKey);

	    finalRound(state, roundKey);
}

void expandKey::invfinalRound(uint8_t* state, uint8_t* roundkey){


	invShiftRows(state);
	    invSubBytes(state);
	  //  addRoundKey(state, roundKey);
	    addRoundKey(state,roundkey);
}

void expandKey::invAes(uint8_t* state, const uint8_t* expandedKey, uint32_t Nr)
{

    uint8_t roundKey[16];

    createRoundKey(expandedKey + 16 * Nr, roundKey);

    addRoundKey(state, roundKey);

    for (int i = Nr - 1; i > 0; i--) {
        createRoundKey(expandedKey + 16 * i, roundKey);
        invRound(state, roundKey);
    }

    createRoundKey(expandedKey, roundKey);

    invfinalRound(state, roundKey);
}

void expandKey::cipher(const char* input, char* output){


	    // Iterate over the columns
	    for (int i = 0; i < 4; i++) {
	        // Iterate over the rows
	        for (int j = 0; j < 4; j++) {
	            state[(i + (j * 4))] = input[(i * 4) + j];
	        }
	    }

	    // Encrypt the block using the expandedKey
	    aes(state, expandedKey, 10);

	    // Unmap the block again into the output
	    for (int i = 0; i < 4; i++) {
	        // Iterate over the rows
	        for (int j = 0; j < 4; j++) {
	            output[(i * 4) + j] = state[(i + (j * 4))];
	        }
	    }
}

void expandKey::decipher(const char* input, char* output)
{

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {
           state[(i + (j * 4))] = input[(i * 4) + j];
        }
    }

    invAes(state, expandedKey,10);

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {
            output[(i * 4) + j] = state[(i + (j * 4))];
            outputTextVar[(i * 4) + j] = state[(i + (j * 4))];;
        }

    }


}



void expandKey::showKey(){
	std::cout<<"Key: ";
		for( int i = 0; i<16; i++){

	std::cout<< ((key[i]))<<" ";



		}
		std::cout <<std::endl;
}

void expandKey::inputKey(){

std::cout <<"Input Key 128Bit: ";
std::cin>>key;
}

void expandKey::inputText(){



	}




void expandKey::makecipher(){



}
