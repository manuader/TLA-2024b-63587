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
static void generateBlock(Block * block, int indentLevel);
static void generateInstruction(Instruction * instruction, int indentLevel);
static void generateDeclaration(Declaration * declaration, int indentLevel);
static void generateAssignation(Assignation * assignation, int indentLevel);
static void generatePrint(Print * print, int indentLevel);
static void generateFunction(Function * function);
static void generateFunctionCall(FunctionCall * functionCall);
static void generateConditional(Conditional * conditional, int indentLevel);
static void generateLoop(Loop * loop, int indentLevel);
static void generateExpression(Expression * expression);
static void generateArithmeticExpression(ArithmeticExpression * expression);
static void generateBooleanExpression(BooleanExpression * expression);
static void generateStringExpression(StringExpression * expression);
static void generateReturnStatement(ReturnStatement * returnStatement, int indentLevel);
static void generateParameters(Parameters * parameters);
static void generateArguments(Arguments * arguments);
static const char * getTypeString(Type * type);
static void generateCompareOperator(CompareOperator * op);

#endif
