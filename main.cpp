
#include <fstream>
#include <iostream>
#include <string>
#include "manager.h"
#include "lexer.h"

using namespace std;

int main () {

	// istream *in;
	// ostream *out;
	// ostream *err;

	// if(argc == 0){
	// 	in = &cin;
	// 	out = &cout;
	// 	err = &cerr;
	// }

	ifstream fin("input.in");


	Lexer lexer(cin);
	int i = 0;
		try{

			while(!lexer.eof()){
			//while(true){
				try{
					cout << lexer[i];
					cout.flush(); 
				}	
				catch(DataException de){
					cout << "Data exception: " << de.what() << '\n';
				}
				++i;
			}
		}
		catch (LexerException e){
			cerr << e.what() << '\n';
		}
	return 0;
}