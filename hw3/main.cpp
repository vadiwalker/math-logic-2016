#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <fstream>

using namespace std;

std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

const string fileFolder = "supportFiles";
string folder;

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "Russian");

	if (argc != 3) {
		cout << "Введите 2 числа a и b";
	}

	ifstream Base2("supportFiles/Base");
	ifstream Proof1("supportFiles/ProofFirst");
	ifstream SimpleProof2("supportFiles/ProofFirstPart");
	ifstream ProofSecondPart("supportFiles/ProofSecondPart");

	ofstream fout("output.txt");

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int c = b - a;

	if (c >= 0) {
		string A = "0", B = "0", C = "0";
		for (int i = 0; i < a; i++) {
			A += "'";
		}
		for (int i = 0; i < b; i++) {
			B += "'";
		}
		for (int i = 0; i < c; i++) {
			C += "'";
		}

		fout << "|-?p(" + A + "+p)=0" + B << "\n";

		string input;
		while (getline(Base2, input)) {
			fout << input << "\n";
		}

		fout << "@a(a+0=a)->" + A + "+0=" + A << "\n";
		fout << A + "+0=" + A << "\n";
		string proof = "";

		while (getline(Proof1, input)) {
			input = replaceAll(input, "A", A);
			proof += input + "\n";
		}

		string s;
		for (int i = 0; i < c; i++) {
			string tmp = proof;
			tmp = replaceAll(tmp, "#", s);
            fout << tmp << "\n";
            s += "'";
        }

        fout << "(" + A + "+" + C + ")=" + B + "->?p(" + A + "+p)=" + B + "\n" + "?p(" + A + "+p)=" + B << "\n";
	} else {

		string input;
		while (getline(SimpleProof2, input)) {
			fout << input << "\n";
		}
		c = a - b;

		string B = "", C = "";

		for (int i = 0; i < c - 1; i++) {
			if (i < b) {
				B += "'";
			}
			C += "'";
		}

		if (c - 1 < b) {
			for (int i = c - 1; i < b; i++) {
				B += "'";
			}
		}

		while (getline(ProofSecondPart, input)) {
			input = replaceAll(input, "B", B);
			input = replaceAll(input, "C", C);
			input = replaceAll(input, " ", "");
			fout << input << "\n";
		}
	}
	return 0;
}