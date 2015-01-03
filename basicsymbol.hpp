#ifndef BASICSYMBOL
#define BASICSYMBOL

#include "header.hpp"
//#include "type.hpp"
#include "noticeexception.hpp"
#include "typeexception.hpp"

class Type;
class BasicScope;

class BasicSymbol {
protected:

	string name;
	Type *type;
	BasicScope *definitionScope;
	Position position;

public:
	virtual void setName(string name);
	virtual void setType(Type *type);
	virtual void setDefinitionScope(BasicScope *scope);

	virtual string getName();
	virtual Type* getType();
	virtual BasicScope* getDefinitionScope();
	void setPosition(Position position);
	Position getPosition();
	//virtual bool operator== (const BasicSymbol &b) const;
	virtual string toString() = 0;
	BasicSymbol(Position position = {0, 0});
};

//

#endif