#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>
#include <stdbool.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Program * ProgramSemanticAction(CompilerState * compilerState, Block * block);
Block * BlockSemanticAction(Instruction * instructions);
Instruction * SingleInstructionSemanticAction(Instruction * instruction);
Instruction * MultipleInstructionsSemanticAction(Instruction * instruction, Instruction * instructions);
Instruction * DeclarationInstructionSemanticAction(Declaration * declaration);
Instruction * AssignationInstructionSemanticAction(Assignation * assignation);
Instruction * PrintInstructionSemanticAction(Print * print);
Instruction * FunctionCallInstructionSemanticAction(FunctionCall * functionCall);
Instruction * ReturnStatementInstructionSemanticAction(ReturnStatement * returnStatement);
Instruction * FunctionInstructionSemanticAction(Function * function);
Instruction * ConditionalInstructionSemanticAction(Conditional * conditional);
Instruction * LoopInstructionSemanticAction(Loop * loop);
Declaration * DeclarationSemanticAction(Type * type, Assignation * assignation);
Type * IntTypeSemanticAction();
Type * BoolTypeSemanticAction();
Type * StringTypeSemanticAction();
Assignation * AssignationSemanticAction(char * varName, Expression * expression);
Expression * ArithmeticExpressionSemanticAction(ArithmeticExpression * arithmeticExpression);
Expression * BooleanExpressionSemanticAction(BooleanExpression * booleanExpression);
Expression * StringExpressionSemanticAction(StringExpression * stringExpression);
ArithmeticExpression * AdditionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right);
ArithmeticExpression * SubtractionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right);
ArithmeticExpression * MultiplicationExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right);
ArithmeticExpression * DivisionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right);
ArithmeticExpression * VarNameArithmeticExpressionSemanticAction(char * varName);
ArithmeticExpression * IntegerArithmeticExpressionSemanticAction(int value);
ArithmeticExpression * FunctionCallArithmeticExpressionSemanticAction(FunctionCall * functionCall);
BooleanExpression * AndExpressionSemanticAction(BooleanExpression * left, BooleanExpression * right);
BooleanExpression * OrExpressionSemanticAction(BooleanExpression * left, BooleanExpression * right);
BooleanExpression * NotExpressionSemanticAction(BooleanExpression * expression);
BooleanExpression * ComparisonExpressionSemanticAction(ArithmeticExpression * left, CompareOperator * op, ArithmeticExpression * right);
BooleanExpression * VarNameBooleanExpressionSemanticAction(char * varName);
BooleanExpression * BoolLiteralExpressionSemanticAction(bool value);
BooleanExpression * FunctionCallBooleanExpressionSemanticAction(FunctionCall * functionCall);
StringExpression * StringLiteralExpressionSemanticAction(char * value);
StringExpression * VarNameStringExpressionSemanticAction(char * varName);
StringExpression * FunctionCallStringExpressionSemanticAction(FunctionCall * functionCall);
Print * PrintSemanticAction(Expression * expression);
Function * FunctionSemanticAction(Type * returnType, char * functionName, Parameters * parameters, Block * block);
Parameters * SingleParameterSemanticAction(Parameter * parameter);
Parameters * MultipleParametersSemanticAction(Parameter * parameter, Parameters * parameters);
Parameter * ParameterSemanticAction(Type * type, char * varName);
FunctionCall * FunctionCallSemanticAction(char * functionName, Arguments * arguments, Type * returnType);
Arguments * SingleArgumentSemanticAction(Argument * argument);
Arguments * MultipleArgumentsSemanticAction(Argument * argument, Arguments * arguments);
Argument * ArgumentSemanticAction(Expression * expression);
Conditional * ConditionalSemanticAction(BooleanExpression * condition, Block * ifBlock, Block * elseBlock);
Loop * LoopSemanticAction(char * varName, ArithmeticExpression * start, ArithmeticExpression * end, Block * block);
CompareOperator * GreaterThanSemanticAction();
CompareOperator * LessThanSemanticAction();
CompareOperator * EqualsEqualsSemanticAction();
CompareOperator * NotEqualsSemanticAction();
CompareOperator * GreaterEqualsSemanticAction();
CompareOperator * LessEqualsSemanticAction();
ReturnStatement * ReturnStatementSemanticAction(Expression * expression);

#endif
