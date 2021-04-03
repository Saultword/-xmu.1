#include <iostream>
#include "Hamming_Code.h"


using namespace Hamming_Code;


int main() {
	string test = "10101010";
	string out = toHamming_Code(test);
	string fault = "111101001010";
	string right = Hamming_Code_Motify(fault);
	cout << "test:" << test << endl;
	cout << "out:" << out << endl;
	cout << "fault:" << fault << endl;
	cout << "right:" << right << endl;
}