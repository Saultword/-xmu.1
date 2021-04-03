#pragma once
#include <string>
namespace Hamming_Code {
	using namespace std;
	void Yihuo(string& s, int a, int b, int c, int d, int e, int f) {
		s[a - 1] = (s[b - 1] - '0') ^ (s[c - 1] - '0') ^
			(s[d - 1] - '0') ^ (s[e - 1] - '0') ^ (s[f - 1] - '0') + '0';
	}

	int check(string s, int a, int b, int c, int d, int e, int f) {
		int jarg = (s[a - 1] - '0') ^ (s[b - 1] - '0') ^ (s[c - 1] - '0') ^
			(s[d - 1] - '0') ^ (s[e - 1] - '0') ^ (s[f - 1] - '0');
		return jarg;
	}

	string toHamming_Code(string s) {
		string hamming = { '0','0',s[0],'0',s[1],s[2],s[3],'0',s[4],s[5],s[6],s[7] };
		Yihuo(hamming, 1, 3, 5, 7, 9, 11);
		Yihuo(hamming, 2, 3, 6, 7, 10, 11);
		Yihuo(hamming, 4, 4, 5, 6, 7, 12);
		Yihuo(hamming, 8, 8, 9, 10, 11, 12);
		return hamming;
	}

	string Hamming_Code_Motify(string s) {
		string tmp = s + "0";
		int position1 = check(tmp, 1, 3, 5, 7, 9, 11);
		int position2 = check(tmp, 2, 3, 6, 7, 10, 11);
		int position3 = check(tmp, 4, 5, 6, 7, 12, 13);
		int position4 = check(tmp, 8, 9, 10, 11, 12, 13);
		int position = position1 * pow(2, 0) + position2 * pow(2, 1) + position3 * pow(2, 2) + position4 * pow(2, 3);
		s[position - 1] = (s[position - 1] - '0') ^ 1 + '0';
		return s;
	}
}
