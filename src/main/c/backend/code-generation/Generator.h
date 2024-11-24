#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

void initializeGeneratorModule();
void shutdownGeneratorModule();
void generate(CompilerState * compilerState);

// Nuevas funciones para generación de código
static void generateProgram(Program * program);
static void generateBlock(Block * block);
static void generateInstruction(Instruction * instruction);
static void generateDeclaration(Declaration * declaration);
static void generateAssignation(Assignation * assignation);
static void generatePrint(Print * print);
static void generateFunction(Function * function);
static void generateFunctionCall(FunctionCall * functionCall);
static void generateConditional(Conditional * conditional);
static void generateLoop(Loop * loop);
static void generateExpression(Expression * expression);
static void generateArithmeticExpression(ArithmeticExpression * expression);
static void generateBooleanExpression(BooleanExpression * expression);
static void generateStringExpression(StringExpression * expression);
static void generateReturnStatement(ReturnStatement * returnStatement);
static void generateParameters(Parameters * parameters);
static void generateArguments(Arguments * arguments);
static const char * getTypeString(Type * type);
static void generateCompareOperator(CompareOperator * op);

#endif
