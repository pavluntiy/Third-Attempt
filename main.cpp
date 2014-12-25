
#include <fstream>
#include <iostream>
#include <string>
#include "lexer.hpp"
#include "parser.hpp"
//#include "treeprinter.hpp"

using namespace std;

int main (int argc, char **argv) {

	istream *in;
	ostream *out;
	ostream *err;

	if(argc == 1){
		in = &cin;
		out =  &cout;
		err = &cerr;
	}
	else { 
		in = new ifstream(string(argv[1]));
		if(argc > 2){
			out = new ofstream(string(argv[2]));
		}
		else {
			out = &cout;
		}
		err = &cerr;
	}


	 Lexer lexer(*in);
	int i = 0;
		try{

			while(!lexer.closed()){
				try{
					*out << lexer[i];
					out->flush(); 
				}	
				catch(DataException de){
					*err << "Data exception: " << de.what() << '\n';
				}
				++i;
			}
		}
		catch (LexerException e){
			*err << e.what() << '\n';
		}

	TreeVisitor visitor (out);
	Parser parser(lexer, visitor);
	try{
		parser.buildTree();
		parser.pushTree();
		visitor.printTree();
	}
	catch (ParserException e){
		*err << e.what() << '\n';
	}

	parser.pushTree();
	visitor.deleteTree();


	return 0;
}