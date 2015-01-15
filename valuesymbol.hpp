#ifndef VALUESYMBOL
#define VALUESYMBOL

#include "header.hpp"
#include "type.hpp"
#include "basicsymbol.hpp"
#include "abstractscope.hpp"


class ValueSymbol: public BasicSymbol{
protected:
	string value;
public:
	void setBase(string str);
	void setOffset(int offset);

	string getAddress();
	string getBase();
	int getOfsset();
	int getSize();

	ValueSymbol(string value = "");

	void setType(Type *type);
	Type* getType();


	virtual string toString(string shift = "") override;
};

#endif