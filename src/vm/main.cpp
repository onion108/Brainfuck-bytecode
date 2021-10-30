// Brainfuck Virtual Machine

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <stack>
#include <vector>
using namespace std;

const int BF_INC = 0x1A;    // +
const int BF_DEC = 0x1B;    // -
const int BF_RSHIFT = 0x1C; // >
const int BF_LSHIFT = 0x1D; // <
const int BF_FLAG = 0x1E;   // [
const int BF_JNZ = 0x1F;    // ]
const int BF_INPUT = 0x19;  // ,
const int BF_OUTPUT = 0x18; // .
const int BF_NOP = 0x00;    // (no character to present)
// Extended Commands - Will not be used in Brainfuck
const int EBF_RAND = 0x20;
const int EBF_EXIT = 0x21;
const int EBF_CALL = 0x22;
const int EBF_RET = 0x23;
const int EBF_PUSH = 0x24;
const int EBF_POP = 0x25;

class VirtualMachine {
	private:
		map<int, function<bool(VirtualMachine&)>> commands;
		int pc;
		void _assertIfEof(istream& is) { // I mean, assert if not eof... welp i'm too lazy to change the name since it is a private method
			if(is.eof()) {
				cerr << "Unexpected EOF while executing the bytecode. Terminated.";
				exit(1);
			}
		}
		int _noEofRead(istream& is) {
			_assertIfEof(is);
			int _result = is.get();
			_assertIfEof(is);
			return _result;
		}
		void _assertIfNotEqual(int a, int b, string errorMessage) {
			if(a != b) {
				cerr << errorMessage;
				exit(1);
			}
		}
		static bool _bf_inc(VirtualMachine& self) {
			self.setCurrent(self.getCurrent() + 1);
			return false;
		}
		static bool _bf_dec(VirtualMachine& self) {
			self.setCurrent(self.getCurrent() - 1);
			return false;
		}
		static bool _bf_rshift(VirtualMachine& self) {
			self.memoryIndex++;
			return false;
		}
		static bool _bf_lshift(VirtualMachine& self) {
			self.memoryIndex--;
			return false;
		}
		static bool _bf_output(VirtualMachine& self) {
			cout.put(self.getCurrent());
			return false;
		}
		static bool _bf_input(VirtualMachine& self) {
			self.setCurrent(cin.get());
			return false;
		}
		static bool _bf_flag(VirtualMachine& self) {
			self.flagStack.push(self.pc - 1);
#ifdef DEBUG
			cerr << "BF_FLAG";
			cerr << self.flagStack.top();
			cerr << endl << self.pc << endl;
#endif
			return false;
		}
		static bool _bf_jnz(VirtualMachine& self) {
			// cerr << "DEBUG";
			if(self.getCurrent() != 0) {
				self._assertNotEmpty(self.flagStack);
#ifdef DEBUG
				cerr << self.flagStack.top() << endl;
#endif
				self.pc = self.flagStack.top();
				self.flagStack.pop();
			}
			return false;
		}
		static bool _ebf_rand(VirtualMachine& self) {
			self.setCurrent(rand());
			return false;
		}
		static bool _ebf_exit(VirtualMachine& unused) {
			return true;
		}
		static bool _ebf_call(VirtualMachine& self) {
			int to = self.getCurrent();
			self.call(to);
			return false;
		}
		static bool _ebf_ret(VirtualMachine& self) {
			self.ret();
			return false;
		}
		static bool _ebf_push(VirtualMachine& self) {
			self.push(self.getCurrent());
			return false;
		}
		static bool _ebf_pop(VirtualMachine& self) {
			self.setCurrent(self.pop());
			return false;
		}
		static bool _bf_nop(VirtualMachine& unused) {
			return false;
		}
		void _assertNotEmpty(stack<int>& s) {
			if(s.empty()) {
				cerr << "Error: Stack empty at: " << std::hex << pc << ".";
				exit(1);
			}
		}
		int lastRunnedProgramVersion = -1;
		stack<int> callStack;
		stack<int> defaultStack;
		stack<int> flagStack;
		vector<int> memory;
		int memoryIndex = 0;
	public:
		void registCommand(int byte, function<bool(VirtualMachine&)> handler) {
			if(commands.find(byte) == commands.end()) {
				commands[byte] = handler;
			}
		}
		bool runCommandIfExist(int byte) {
			if(byte == -1) {
				return true;
			}
			if(commands.find(byte) != commands.end()) {
				return commands[byte](*this);
			} else {
				cerr << std::string("UD2 Exception --- Undefined Command");
				cerr << " at " << pc;
				cerr << "\nCommand: " << std::hex << byte;
				cerr << endl;
				exit(1);
				return false; // Will never reach
			}
		}
		void setPc(int newpc) {
			pc = newpc;
		}
		int getPc() {
			return pc;
		}
		int getLastRunnedProgramVersion() {
			return lastRunnedProgramVersion;
		}
		void exec(istream& bytecode) {
			// Correcting bytecode;
			int currentByte;
			currentByte = _noEofRead(bytecode);
			_assertIfNotEqual(currentByte, 0x27, "Invalid Format.");
			currentByte = _noEofRead(bytecode);
			_assertIfNotEqual(currentByte, 0x26, "Invalid Format.");
			currentByte = _noEofRead(bytecode);
			_assertIfNotEqual(currentByte, 0x4A, "Invalid Format.");
			currentByte = _noEofRead(bytecode);
			lastRunnedProgramVersion = currentByte;
			pc = bytecode.tellg();
#ifdef DEBUG
			cerr << pc << endl;
#endif
			// Start running
			while(true) {
				currentByte = bytecode.get();
				pc++;
#ifdef DEBUG
				cerr << pc << endl;
#endif
				bool shouldExit = runCommandIfExist(currentByte);
				bytecode.seekg(pc, ios::beg);
				if(shouldExit) {
					break;
				}
				if(bytecode.eof()) {
					break;
				}
			}
		}
		void push(int n) {
			defaultStack.push(n);
		}
		int pop() {
			_assertNotEmpty(defaultStack);
			int val = defaultStack.top();
			defaultStack.pop();
			return val;
		}
		void call(int newpc) {
			callStack.push(pc);
			pc = newpc;
		}
		void ret() {
			pc = callStack.top();
			callStack.pop();
		}
		void initMachine() {
			srand(time(NULL));
			this->registCommand(BF_INC, _bf_inc);
			this->registCommand(BF_DEC, _bf_dec);
			this->registCommand(BF_RSHIFT, _bf_rshift);
			this->registCommand(BF_LSHIFT, _bf_lshift);
			this->registCommand(BF_FLAG, _bf_flag);
			this->registCommand(BF_JNZ, _bf_jnz);
			this->registCommand(BF_INPUT, _bf_input);
			this->registCommand(BF_OUTPUT, _bf_output);
			this->registCommand(BF_NOP, _bf_nop);
			// Extended Brainfuck Commands
			this->registCommand(EBF_RAND, _ebf_rand);
			this->registCommand(EBF_EXIT, _ebf_exit);
			this->registCommand(EBF_CALL, _ebf_call);
			this->registCommand(EBF_RET, _ebf_ret);
			this->registCommand(EBF_PUSH, _ebf_push);
			this->registCommand(EBF_POP, _ebf_pop);
		}
		int getCurrent() {
			if(this->memoryIndex >= this->memory.size()) {
				return 0;
			} else {
				return this->memory[this->memoryIndex];
			}
		}
		void setCurrent(int value) {
			if(this->memoryIndex >= this->memory.size()) {
				while(this->memoryIndex >= this->memory.size()) {
					this->memory.push_back(0);
				}
				this->memory[this->memoryIndex] = value;
			} else {
				this->memory[this->memoryIndex] = value;
			}
		}
};

int main(int argc, char** argv) {
	if(argc == 2) {
		ifstream ifs(argv[1]);
		VirtualMachine vm;
		vm.initMachine();
		vm.exec(ifs);
	}
	return 0;
}

