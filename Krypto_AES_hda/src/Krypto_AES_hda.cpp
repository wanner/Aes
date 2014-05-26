//============================================================================
// Name        : Krypto_AES_hda.cpp
// Author      : JPfndrt
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "expandKey.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

template<typename T>
std::string int_to_hex(T i) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex
			<< i;
	return stream.str();
}

int main() {

	expandKey x;
	x.MakeExpandKey();

//x.cipher(x.input,x.output);
//x.decipher(x.output,x.input);

//cout<<x.input<<" "<<endl;

string eingabe;
cout<<"Eingabe: ";
//cin>>eingabe;
std::getline (std::cin,eingabe);
int paddingLength = 16 - (eingabe.length() %16);
int counter = 0;
int inputCounter = 0;
int outputCounter = 0;
string localDecipher;

for (int i = 0; i<paddingLength;i++){
	eingabe+="0";
}

int loopCounter = eingabe.length() / 16;

cout <<eingabe<<" lcounter: "<<loopCounter<<endl;
char test[16];


for (int i = 0; i<loopCounter; i++){

	for(int j =0; j<16;j++){
		x.input[j] = eingabe[inputCounter];

		inputCounter++;

	}
x.cipher(x.input,x.output);
if(i==0)cout<<"Cipher: ";
for(int j=0;j<16;j++){
	cout<<int(x.output[j])<<" ";
}

if(loopCounter==i+1)cout<<endl;

x.decipher(x.output,x.input);

for(int j=0;j<16;j++){

	localDecipher+=x.input[j];
}
}

cout<<endl<<endl;
cout<<"DeCipher: "<<localDecipher;




	return 0;
}
