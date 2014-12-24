
#ifndef DATA 
#define DATA

#include <iostream>
#include "data_exception.h"
#include "alphabet.h"
#include "header.h"
#include <stack>

class Data{

	istream &in;
	string str;
	//int size;
	bool wasEof;
	bool ready;
	bool errorOccured;

	
	int spacesInTab;
	string badCharacters;


public:

	char currentChar;
	Position sourcePosition;// previousSourcePosition;
	int currentPosition; //previousPosition;
	stack<Position> previousSourcePosition;
	stack<int> previousPosition;
	Data(istream &in = cin):
	in(in)
	{
		//size = 0;
		wasEof = false;
		ready = false;
		currentPosition = -1;
	//	previousPosition = 0;
		previousPosition.push(0);
		currentChar = '\n';
		badCharacters = "";
		errorOccured = false;
	}

	// Data(Data &&other){
	// 	in(move(other.in));
	// 	size = 0;
	// 	wasEof = false;
	// 	ready = false;
	// 	currentPosition = -1;
	// //	previousPosition = 0;
	// 	previousPosition.push(0);
	// 	currentChar = '\n';
	// 	badCharacters = "";
	// 	errorOccured = false;
	// }

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
		//this->size--;
		return tmp;	
	}	

	void push(char c){
		this->str.push_back(c);
		//this->size++;
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
		return this->str.size();
	}
	bool eof(){
		//this->wasEof = (currentChar == -1);
		//this->wasEof = this->wasEof || this->in.eof();
		//return this->wasEof;
		//cout << currentPosition << ' ' << str[str.size() - 2];
		//cout << currentPosition << '!';
		return this->in.eof() && currentPosition  >= static_cast<int>(str.size()) - 1;
	}

	Position getSourcePosition(){
		return this->sourcePosition;
	}

	int getPosition (){
		return this->currentPosition;
	}

	char charAt(int index){
	//	cout << index << endl;
		if(eof() && index >= static_cast<int>(this->str.size())){
			throw DataException("Invalid index of input");
		}

		if(index >= static_cast<int>(this->str.size())){
			while (!eof() && index >= static_cast<int>(this->str.size())) {
				char c = static_cast<char>(in.get());
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
			//cout << "WTF?!?!";
			makeReady();
		}
		//cout << "'" <<currentChar << "'";
		//ignore(this->charAt(currentPosition + 1));
		//cout << "'" <<currentChar << "'";
		return;
	}

	void lock(){
	//	cout << "Size of str: " << this->size << "\n";
	//	cout << "Locked on postion " << currentPosition << "\n";
		this->previousSourcePosition.push(sourcePosition);
		this->previousPosition.push(currentPosition);
	//	cout << "current char : " << currentChar << "\n";
	}

	void restore(){
		if(this->previousPosition.size() > 0){
			this->currentPosition = this->previousPosition.top();
			this->previousPosition.pop();
		}
		else{
			this->currentPosition = 0;
		}
	//	this->sourcePosition = this->previousSourcePosition;
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
	//	cout << "got character: " << currentChar << wasEof << endl;

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

	void close(){
		this->wasEof = true;
	}
};

#endif