#include "functionscope.hpp"

void FunctionScope::declareFunction(FunctionSymbol *function){
	throw TypeException("No nested functions are allowed!");
}

void FunctionScope::declareVariable(VariableSymbol *variable){

}

void FunctionScope::declareType(Type *type){
	throw TypeException("No nested types are allowed!");
}