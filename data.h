
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
	bool wasEOF;

	
	int spacesInTab;


public:

	char currentChar;
	Position sourcePosition, previousSourcePosition;
	int currentPosition, previousPosition;
	Data(istream &in = cin):
	in(in.rdbuf())
	{
		size = 0;
		wasEOF = false;
		currentPosition = 0;
		currentChar = '\n';
	}

	void pop(){
		if(str.size() > 0){
			this->str.pop_back();
			this->size--;
		}	
	}

	void restorePosition (int position){
		if(position >= size){
			throw DataException("Invalid resetting of position!");
		}

		while(position <= this->size){
			this->pop();
		}

		cout << "size: " << this->size << " \n";

		cout << position;
		this->currentPosition = position;

	}
	
	int getSize(){
		return this->size;
	}
	bool eof(){
		this->wasEOF = this->wasEOF || this->in.eof();
		return this->wasEOF;
	}

	Position getSourcePosition(){
		return this->sourcePosition;
	}

	int getPosition (){
		return this->currentPosition;
	}

	char charAt(int index){
		if(wasEOF && index >= size){
			throw DataException("Invalid index of input");
		}

		if(index >= size){
			while (!eof() && index >= size){
				char c = in.get();
				str.push_back(c);
				++size;
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
		this->currentPosition = 0;
		ignore(this->charAt(currentPosition + 1));
		this->pushCharToStream();
	}

	void pushCharToStream(char c = '\n'){
		this->str.push_back(c);
		this->size++;
		this->currentChar = str[size - 1];
	}

	void isReady(){
		if(eof()){
			throw DataException("Stream is closed!");
			return;
		}
		if(this->currentPosition == -1){
			this->consume();
			return;
		}
	//	cout << "adrtkekqerqwdf\n";
		ignore(this->charAt(currentPosition + 1));
		return;
	}

	void setBackup(){
		this->previousSourcePosition = sourcePosition;
		this->previousPosition = currentPosition;
	}

	void restore(){
		this->currentPosition = this->previousPosition;
		this->sourcePosition = this->previousSourcePosition;
		this->currentChar = this->charAt(currentPosition);

	}

	void consume(){
		if (Alphabet::is<Alphabet::NEWLINE>(currentChar)){
			sourcePosition.line ++;
			sourcePosition.linePosition = 0;
		}
		else {
			sourcePosition.linePosition++;
		}

		if (wasEOF){
			throw DataException("Unexpected end of file on " + sourcePosition.toString() + "\n");
		}
		
		currentPosition++; 
		
		
		
		if (wasEOF){
			currentChar = EOF;
		}
		else {
			currentChar = this->charAt(currentPosition); 
		}
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
		this->wasEOF = true;
	}
};

#endif