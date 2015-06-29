#ifndef ARRAYTYPE
#define ARRAYTYPE
#include "type.hpp"
#include "header.hpp"
#include "typenode.hpp"
#include "basicsymbol.hpp"


class FunctionSymbol;
class ArrayType: public Type {
protected:
	Type *basicType;
	Type *actualType;

	vector<int> dimensions;

	//vector<string> fullName;
	//string name;
	//int size;

	//map<Type*, FunctionSymbol*> convertTo;

public:

	ArrayType(Type*);
	ArrayType(TypeNode*);

	//virtual int getSize() override const;

	const vector<int>& getDimensions() const;

	virtual string toString(string shift = "")override;

	bool operator==(const Type &other) const;
	bool operator==(Type *other) const;
//	bool modifiersEqual(const Type &other) const;

	virtual bool isArray() override;
//	void addConversion(Type *type, FunctionSymbol *function);
//	pair<bool, FunctionSymbol*>  getConversion(Type *type);

};


#endif