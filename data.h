
#ifndef DATA 
#define DATA

#include <iostream>
#include "dataexception.hpp"
#include "alphabet.h"
#include "header.h"
#include <stack>

class Data{

	istream &in;
	string str;
	//bool wasEof;
	bool ready;
	bool errorOccured;

	
	int spacesInTab;
	string badCharacters;


public:

	char currentChar;
	Position sourcePosition;
	int currentPosition;
	stack<Position> previousSourcePosition;
	stack<int> previousPosition;
	Data(istream &in = cin):
	in(in)
	{
		//wasEof = false;
		ready = false;
		currentPosition = -1;
		previousPosition.push(0);
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
		return tmp;	
	}	

	void push(char c){
		this->str.push_back(c);
	}


	void recover (){
		while(!eof() && !Alphabet::is<Alphabet::NEWLINE>(this->currentChar)){
			badCharacters += currentChar;
			consume();
		}
	}
	
	int getSize(){
		return this->str.size();
	}
	bool eof(){
		return this->in.eof() && currentPosition  >= static_cast<int>(str.size()) - 1;
	}

	Position getSourcePosition(){
		return this->sourcePosition;
	}

	int getPosition (){
		return this->currentPosition;
	}

	char charAt(int index){
		if(eof() && index >= static_cast<int>(this->str.size())){
			throw DataException("Invalid index of input");
		}

		if(index >= static_cast<int>(this->str.size())){
			while (!eof() && index >= static_cast<int>(this->str.size())) {
			//	cout << "!!!";
				char c = static_cast<char>(in.get());
				this->push(c);
			//	cout << c;
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
		ignore(this->charAt(currentPosition + 1));	
	}

	void pushCharToStream(char c = '\n'){
		this->push(c);
	}

	void isReady(){
		if(eof()){
			throw DataException("Stream is closed!");
			return;
		}
		if(!ready){
			makeReady();
		}
		return;
	}

	void lock(){
		this->previousSourcePosition.push(sourcePosition);
		this->previousPosition.push(currentPosition);
	}

	void restore(){
		if(this->previousPosition.size() > 0){
			this->currentPosition = this->previousPosition.top();
			this->previousPosition.pop();
		}
		else{
			this->currentPosition = 0;
		}
		this->currentChar = this->charAt(currentPosition);
	}

	Position getPreviousSourcePosition(){
		if(this->previousSourcePosition.size() == 0){
			return Position();
		}
		return this->previousSourcePosition.top();
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

		if (eof()){
			throw DataException("Unexpected end of file on " + sourcePosition.toString() + "\n");
		}
		
		currentPosition++; 
		
		if (eof()){
			currentChar = EOF;
		}
		else {
			currentChar = this->charAt(currentPosition); 
		}
	}

	bool get(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != this->charAt(currentPosition + i)){
				return false;
			}
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
			if (text[i] != this->charAt(currentPosition + i)){
				return false;
			}
		}
		return true;
	}

	// void close(){
	// 	this->wasEof = true;
	// }
};

#endif