#ifndef TYPE
#define TYPE
#include "header.hpp"
#include "typenode.hpp"
#include "basicsymbol.hpp"


class FunctionSymbol;
class Type: public BasicSymbol {
protected:
	Type *actualType;

	vector<string> storageModes;
	vector<string> modifiers;
	vector<string> accessModes;
	vector<int> dimensions;

	vector<string> fullName;
	//string name;
	int size;

	map<Type*, FunctionSymbol*> convertTo;

public:

	//string getName() const;
	virtual int getSize() const;
	Type(string name, int size = 0);
	Type();
	Type(TypeNode*);
	Type(Type*);

	const vector<string>& getStorageModes() const;
	const vector<string>& getModifiers() const;
	const vector<string>& getAccessModes() const;
	const vector<int>& getDimensions() const;

	void setStorageModes(vector<string>);
	void setModifiers(vector<string>);
	void setAccesssModes(vector<string>);
	void setDimensions(vector<int>);
	bool isUnqualified();

	const vector<string>& getFullName() const;
	virtual string toString(string shift = "")override;

	bool operator==(const Type &other) const;
	bool operator==(Type *other) const;
	bool modifiersEqual(const Type &other) const;

	virtual bool isArray();
	virtual bool isPointer();

	//void tryToAddConversion(FunctionSymbol *function);
	void addConversion(Type *type, FunctionSymbol *function);
	pair<bool, FunctionSymbol*>  getConversion(Type *type);

	void setActualType(Type* type);
	Type* getActualType();

};

// bool operator==(Type *a, Type *b);


#endif