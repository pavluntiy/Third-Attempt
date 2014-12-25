#include "data.hpp"
	Data::Data(istream &in):
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

	bool Data::wasError(){
		return this->errorOccured;
	}
	string Data::getErrorReport(){
		this->errorOccured = false;
		string tmp = this->badCharacters;
		this->badCharacters = "";
		return tmp;
	}

	char Data::pop(){
		if(str.size() == 0){
			return '\0';
		}
		char tmp = '\0';
		sourcePosition.linePosition++;
		tmp = *(end(str) - 1);
		this->str.pop_back();
		return tmp;	
	}	

	void Data::push(char c){
		this->str.push_back(c);
	}


	void Data::recover (){
		while(!eof() && !Alphabet::is<Alphabet::NEWLINE>(this->currentChar)){
			badCharacters += currentChar;
			consume();
		}
	}
	
	int Data::getSize(){
		return this->str.size();
	}
	bool Data::eof(){
		return this->in.eof() && currentPosition  >= static_cast<int>(str.size()) - 1;
	}

	Position Data::getSourcePosition(){
		return this->sourcePosition;
	}

	int Data::getPosition (){
		return this->currentPosition;
	}

	char Data::charAt(int index){
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

	char Data::operator [] (int index){

		return this->charAt(index);
	}

	void Data::consume(int n){
		for(int i = 0; i < n; ++i){
			consume();
		}
	}

	void Data::consumeNewlineTabs(){

	}

	void Data::makeReady(){
		this->ready = true;
		myIgnore(this->charAt(currentPosition + 1));	
	}

	void Data::pushCharToStream(char c){
		this->push(c);
	}

	void Data::isReady(){
		if(eof()){
			throw DataException("Stream is closed!");
			return;
		}
		if(!ready){
			makeReady();
		}
		return;
	}

	void Data::lock(){
		this->previousSourcePosition.push(sourcePosition);
		this->previousPosition.push(currentPosition);
	}

	void Data::restore(){
		if(this->previousPosition.size() > 0){
			this->currentPosition = this->previousPosition.top();
			this->previousPosition.pop();
		}
		else{
			this->currentPosition = 0;
		}
		this->currentChar = this->charAt(currentPosition);
	}

	Position Data::getPreviousSourcePosition(){
		if(this->previousSourcePosition.size() == 0){
			return Position();
		}
		return this->previousSourcePosition.top();
	}

	string Data::dataDump(){
		return this->str;
	}
	void Data::consume(){

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

	bool Data::get(std::string text){
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

	bool Data::get(char c){
		if(currentChar == c){
			consume();
			return true;
		}

		return false;
	}

	bool Data::find(char c){
		if(currentChar == c){
			return true;
		}
		
		return false;
	}

	bool Data::find(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != this->charAt(currentPosition + i)){
				return false;
			}
		}
		return true;
	}

