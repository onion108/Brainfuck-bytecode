#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char** argv) {
	if(argc == 3) {
		ifstream input_file(argv[1]);
		ofstream output_file(argv[2]);
		output_file << "\x27\x26\x4A";
		output_file.put(0);
		while(!input_file.eof()) {
			int current = input_file.get();
			if(current == -1) {
				input_file.close();
				output_file.close();
				return 0;
			}
			switch(current) {
				case '+':
					output_file << '\x1A';
					break;
				case '-':
					output_file << '\x1B';
					break;
				case '>':
					output_file << '\x1C';
					break;
				case '<':
					output_file << '\x1D';
					break;
				case '[':
					output_file << '\x1E';
					break;
				case ']':
					output_file << '\x1F';
					break;
				case ',':
					output_file << '\x19';
					break;
				case '.':
					output_file << '\x18';
					break;
			}
		}
	}
	return 0;
}

