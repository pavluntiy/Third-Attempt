
#include <fstream>
#include <iostream>
#include <string>
#include "lexer.h"

using namespace std;

int main () {
	Lexer lexer(cin);
	int i = 0;
		try{

			while(!lexer.eof()){
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
			cout << e.what() << '\n';
		}
	return 0;
}