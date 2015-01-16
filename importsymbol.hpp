#ifndef IMPORTSYMBOL
#define IMPORTSYMBOL

#include "header.hpp"
#include "basicscope.hpp"
#include "basicsymbol.hpp"
#include "compoundnamenode.hpp"

class ImportSymbol: BasicSymbol{
protected:
	CompoundNameNode *moduleName, *scopeName;
	BasicScope *scope;
public:
	BasicScope *getScope();
	void setScope(BasicScope*);

	CompoundNameNode *getModuleName();
	CompoundNameNode *getScopeName();

	void setModuleName(CompoundNameNode*);
	void setScopeName(CompoundNameNode*);
	virtual string toString(string shift = "");
};


#endif