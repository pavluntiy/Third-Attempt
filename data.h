
#ifndef DATA 
#define DATA

#include <iostream>
#include "data_exception.h"
#include "alphabet.h"
#include "header.h"

class Data{

	istream in;
	string str;
	int size;
	bool wasEof;
	bool ready;
	bool errorOccured;

	
	int spacesInTab;
	string badCharacters;


public:

	char currentChar;
	Position sourcePosition, previousSourcePosition;
	int currentPosition, previousPosition;
	Data(istream &in = cin):
	in(in.rdbuf())
	{
		size = 0;
		wasEof = false;
		ready = false;
		currentPosition = -1;
		previousPosition = 0;
		currentChar = '\n';
		badCharacters = "";
		errorOccured = false;
	}

	bool wasError(){
		return this->errorOccured;
	}
	string getErrorReport(){
		this->errorOccured = false;
		string tmp = this->badCharacters;
		this->badCharacters = "";
		return tmp;
	}

	char pop(){
		if(str.size() == 0){
			return '\0';
		}
		char tmp = '\0';
		sourcePosition.linePosition++;
		tmp = *(end(str) - 1);
		this->str.pop_back();
		this->size--;
		return tmp;	
	}	

	void push(char c){
		this->str.push_back(c);
		this->size++;
	}


	void recover (){
	//	this->errorOccured = true;

		while(!eof() && !Alphabet::is<Alphabet::NEWLINE>(this->currentChar)){
			badCharacters += currentChar;
			consume();
		}


	// 	cout << "Bad character: " << currentChar << endl;
	// 	if(previousPosition >= this->size){
	// //		cout << "Trash!: " << previousPosition <<  "\n";
	// 		throw DataException("Invalid resetting of position!");
	// 	}

	// 	while(this->size > previousPosition){
	// 		badCharacters.push_back(this->pop());
	// 		// if(previousPosition == -1){
	// 		// 	break;
	// 		// }
	// 	}

	// //	cout << "Size: " << this->size << " \n";

	// //	cout << "Restored to " << previousPosition<< "\n";
	// 	this->currentPosition = previousPosition ;

	// 	if(currentPosition == 0){
	// 		makeReady();
	// 	}
	// 	currentChar = this->charAt(this->currentPosition);

	//	cout << "Current char : " << currentChar << endl;
	}
	
	int getSize(){
		return this->size;
	}
	bool eof(){
		this->wasEof = this->wasEof || this->in.eof();
		return this->wasEof;
	}

	Position getSourcePosition(){
		return this->sourcePosition;
	}

	int getPosition (){
		return this->currentPosition;
	}

	char charAt(int index){
	//	cout << index << endl;
		if(wasEof && index >= size){
			throw DataException("Invalid index of input");
		}

		if(index >= this->size){
			while (!eof() && index >= this->size){
				char c = in.get();
				this->push(c);
			}
		}

		return str[index];
	}

	char operator [] (int index){

		return this->charAt(index);
	}

	void consume(int n){
		for(int i = 0; i < n; ++i){
			consume();
		}
	}

	void consumeNewlineTabs(){

	}

	void makeReady(){
		this->ready = true;
	//	this->currentPosition = 0;
	//	this->str.push_backCharToStream();
		ignore(this->charAt(currentPosition + 1));	
	}

	void pushCharToStream(char c = '\n'){

//		cout << "Pushed to stream!\n";
		this->push(c);
	//	this->size++;
	//	this->currentChar = *(end(str) - 1);
	}

	void isReady(){
		if(eof()){
			throw DataException("Stream is closed!");
			return;
		}
		// if(this->currentPosition == -1){
		// 	this->consume();
		// 	return;
		// }
	//	cout << "adrtkekqerqwdf\n";
		if(!ready){
			makeReady();
		}
		ignore(this->charAt(currentPosition + 1));
		return;
	}

	void lock(){
	//	cout << "Size of str: " << this->size << "\n";
	//	cout << "Locked on postion " << currentPosition << "\n";
		this->previousSourcePosition = sourcePosition;
		this->previousPosition = currentPosition;
	//	cout << "current char : " << currentChar << "\n";
	}

	void restore(){
		this->currentPosition = this->previousPosition;
	//	this->sourcePosition = this->previousSourcePosition;
		this->currentChar = this->charAt(currentPosition);

	}

	string dataDump(){
		return this->str;
	}
	void consume(){

		if (Alphabet::is<Alphabet::NEWLINE>(currentChar)){
			sourcePosition.line ++;
			sourcePosition.linePosition = 0;
		}
		else {
			sourcePosition.linePosition++;
		}

		if (wasEof){
			throw DataException("Unexpected end of file on " + sourcePosition.toString() + "\n");
		}
		
		currentPosition++; 
		
			
	//	cout << "Consuming... ";
		if (wasEof){
			currentChar = EOF;
		}
		else {
			currentChar = this->charAt(currentPosition); 
		}
	//	cout << "got character: " << currentChar << endl;

	}

	bool get(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != this->charAt(currentPosition + i));
				return false;
		}

		for (int i = 0; i < (int) text.size(); i++){
			consume();
		}

		return true;
	}

	bool get(char c){
		if(currentChar == c){
			consume();
			return true;
		}

		return false;
	}

	bool find(char c){
		if(currentChar == c){
			return true;
		}
		
		return false;
	}

	bool find(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != this->charAt(currentPosition + i));
				return false;
		}
		return true;
	}

	void close(){
		this->wasEof = true;
	}
};

#endif