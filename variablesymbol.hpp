#ifndef VARIABLESYMBOL
#define VARIABLESYMBOL

#include "header.hpp"
#include "type.hpp"
#include "basicsymbol.hpp"
#include "abstractscope.hpp"


class VariableSymbol: public BasicSymbol{
protected:
	string base;
	int offset;


public:
	void setBase(string str);
	void setOffset(int offset);

	string getAddress();
	string getBase();
	int getOfsset();
	int getSize();

	VariableSymbol();
	VariableSymbol(Type *type, string name);
	virtual string toString(string shift = "")override;
};

#endif