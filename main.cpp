
#include <fstream>
#include <iostream>
#include <string>
#include "tree_visitor.h"
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
		err = &cerr;

	}


	 Lexer lexer(*in);
	int i = 0;
		try{

			while(!lexer.closed()){
			//	cout << lexer.closed();
			//while(true){
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
	//Parser parser(Lexer(*in));
	try{
	//	parser.visitTree(out);
	//	parser.buildTree();
	//	parser.printTree(parser.getTree(), out);
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