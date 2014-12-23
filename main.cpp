
#include <fstream>
#include <iostream>
#include <string>
#include "manager.h"
#include "lexer.h"
#include "parser.h"
#include <memory>

using namespace std;

int main (int argc, char **argv) {

	//weak_ptr<istream> in;
	istream *in;
	ostream *out;
	ostream *err;
	//weak_ptr<ostream> out, err;

	if(argc == 1){
		in = &cin;
		out =  &cout;
		err = &cerr;
	}
	else { 
//	in("input.in");
		in = new ifstream(string(argv[1]));
		if(argc > 2){
			out = new ofstream(string(argv[2]));
		}
		else {
			out = &cout;
		}

	}


	Lexer lexer(*in);
	// int i = 0;
	// 	try{

	// 		while(!lexer.closed()){
	// 		//while(true){
	// 			try{
	// 				*out << lexer[i];
	// 				out->flush(); 
	// 			}	
	// 			catch(DataException de){
	// 				*err << "Data exception: " << de.what() << '\n';
	// 			}
	// 			++i;
	// 		}
	// 	}
	// 	catch (LexerException e){
	// 		*err << e.what() << '\n';
	// 	}

	Parser parser(lexer);
//	Parser parser(Lexer(*in));
	try{
		parser.buildTree();
		parser.printTree(parser.getTree(), out);
	}
	catch (ParserException e){
		*err << e.what() << '\n';
	}

	parser.deleteTree();

	return 0;
}