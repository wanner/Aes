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





template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(T)*2)
         << std::hex << i;
  return stream.str();
}



int main() {

	expandKey x;
	x.MakeExpandKey();

//	cout<<sizeof(x.expandedKey)<<endl;
int j =0;
	for( int i = 0; i<176; i++){
		if ( j%16 ==0);
cout<< (int(x.expandedKey[i]))<<" ";

		j++;

	}

	x.cipher(x.input,x.output);
cout<<endl<<"Input: ";
	for (int i = 0; i<16;i++){
		cout<< x.input[i];
	}
	cout<<endl;

cout<<endl<<"Cipher: ";
	for (int i = 0; i<16;i++){
			cout<<x.output[i];
		}


cout<<endl;cout<<endl;

	return 0;
}
