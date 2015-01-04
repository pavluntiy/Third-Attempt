
#include <fstream>
#include <iostream>
#include <string>
#include "lexer.hpp"
#include "parser.hpp"
#include "printvisitor.hpp"
#include "deletevisitor.hpp"
#include "typecheckvisitor.hpp"
#include "scopeprintvisitor.hpp"

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
	// int i = 0;
	// 	try{

	// 		while(!lexer.closed()){
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
	try{
		parser.buildTree();
	}
	catch (ParserException e){
		*err << e.what() << '\n';
	}

	// PrintVisitor *printVisitor = new PrintVisitor(out);

	// parser.getTree()->accept(printVisitor);

	// delete printVisitor;

	TypeVisitor *typeVisitor = new TypeVisitor();

	try{
		parser.getTree()->accept(typeVisitor);
	}
	catch (NoticeException &ne){
		*err << ne.what() << '\n';
	}
	catch (TypeException &te){
		*err << te.what() << '\n';
	}

	// typeVisitor->dump(out);

	ScopePrintVisitor *scopePrintVisitor = new ScopePrintVisitor(out);

	parser.getTree()->accept(scopePrintVisitor);



	DeleteVisitor *deleteVisitor = new DeleteVisitor();

	parser.getTree()->accept(deleteVisitor);
	delete deleteVisitor;


	return 0;
}