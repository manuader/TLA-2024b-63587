#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include <stdlib.h>
#include <stdbool.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef struct Program Program;
typedef struct Block Block;
typedef struct Instruction Instruction;
typedef struct Declaration Declaration;
typedef struct Assignation Assignation;
typedef struct Expression Expression;
typedef struct Print Print;
typedef struct FunctionCall FunctionCall;
typedef struct Function Function;
typedef struct Conditional Conditional;
typedef struct Loop Loop;
typedef struct ArithmeticExpression ArithmeticExpression;
typedef struct BooleanExpression BooleanExpression;
typedef struct StringExpression StringExpression;
typedef struct CompareOperator CompareOperator;
typedef struct Parameters Parameters;
typedef struct Parameter Parameter;
typedef struct Arguments Arguments;
typedef struct Argument Argument;
typedef struct ReturnStatement ReturnStatement;
typedef struct Type Type;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum InstructionType_T {
    DECLARATION_INSTRUCTION_T,
    ASSIGNATION_INSTRUCTION_T,
    EXPRESSION_INSTRUCTION_T,
    PRINT_INSTRUCTION_T,
    FUNCTION_CALL_INSTRUCTION_T,
    RETURN_STATEMENT_INSTRUCTION_T,
    FUNCTION_INSTRUCTION_T,
    CONDITIONAL_INSTRUCTION_T,
    LOOP_INSTRUCTION_T
};

struct Program {
    Block * block;
};

struct Block {
    Instruction * instructions;
};

struct Instruction {
    union {
        Declaration * declaration;
        Assignation * assignation;
        Expression * expression;
        Print * print;
        FunctionCall * functionCall;
        ReturnStatement * returnStatement;
        Function * function;
        Conditional * conditional;
        Loop * loop;
    };
    enum InstructionType_T type;
    Instruction * next;
};

struct Declaration {
    Type * type;
    char * varName;
    Assignation * assignation;
};

struct Assignation {
    char * varName;
    Expression * expression;
};

struct Expression {
    union {
        ArithmeticExpression * arithmeticExpression;
        BooleanExpression * booleanExpression;
        StringExpression * stringExpression;
    };
    enum ExpressionType { ARITHMETIC_EXPR_T, BOOLEAN_EXPR_T, STRING_EXPR_T } type;
};

struct Print {
    Expression * expression;
};

struct FunctionCall {
    char * functionName;
    Arguments * arguments;
};

struct Function {
    Type * returnType;
    char * functionName;
    Parameters * parameters;
    Block * block;
};

struct Conditional {
    BooleanExpression * condition;
    Block * ifBlock;
    Block * elseBlock;
};

struct Loop {
    char * varName;
    ArithmeticExpression * start;
    ArithmeticExpression * end;
    Block * block;
};

struct ArithmeticExpression {
    union {
        struct {
            ArithmeticExpression * left;
            ArithmeticExpression * right;
        };
        char * varName;
        int value;
        FunctionCall * functionCall;
    };
    enum ArithmeticExpressionType { ADD_T, SUB_T, MUL_T, DIV_T, VAR_ARITH_T, INT_LITERAL_T, FUNC_CALL_ARITH_T } type;
};

struct BooleanExpression {
    union {
        struct {
            BooleanExpression * left;
            BooleanExpression * right;
        };
        struct {
            ArithmeticExpression * leftArith;
            CompareOperator * op;
            ArithmeticExpression * rightArith;
        };
        BooleanExpression * notExpr;
        char * varName;
        bool value;
        FunctionCall * functionCall;
    };
    enum BooleanExpressionType { AND_T, OR_T, NOT_T, COMPARISON_T, VAR_BOOL_T, BOOL_LITERAL_T, FUNC_CALL_BOOL_T } type;
};

struct StringExpression {
    union {
        char * value;
        char * varName;
        FunctionCall * functionCall;
    };
    enum StringExpressionType { STRING_LITERAL_T, VAR_STRING_T, FUNC_CALL_STRING_T } type;
};

struct CompareOperator {
    enum { GREATER_THAN_T, LESS_THAN_T, EQUALS_EQUALS_T, NOT_EQUALS_T, GREATER_EQUALS_T, LESS_EQUALS_T } type;
};

struct Parameters {
    Parameter * parameter;
    Parameters * next;
};

struct Parameter {
    Type * type;
    char * varName;
};

struct Arguments {
    Argument * argument;
    Arguments * next;
};

struct Argument {
    Expression * expression;
};

struct ReturnStatement {
    Expression * expression;
};

struct Type {
    enum DataType { INT_T, BOOL_T, STRING_T } type;
};

/**
 * Node recursive destructors.
 */
void releaseProgram(Program * program);
void releaseBlock(Block * block);
void releaseInstruction(Instruction * instruction);
void releaseDeclaration(Declaration * declaration);
void releaseAssignation(Assignation * assignation);
void releaseExpression(Expression * expression);
void releasePrint(Print * print);
void releaseFunctionCall(FunctionCall * functionCall);
void releaseFunction(Function * function);
void releaseConditional(Conditional * conditional);
void releaseLoop(Loop * loop);
void releaseArithmeticExpression(ArithmeticExpression * arithmeticExpression);
void releaseBooleanExpression(BooleanExpression * booleanExpression);
void releaseStringExpression(StringExpression * stringExpression);
void releaseCompareOperator(CompareOperator * compareOperator);
void releaseParameters(Parameters * parameters);
void releaseParameter(Parameter * parameter);
void releaseArguments(Arguments * arguments);
void releaseArgument(Argument * argument);
void releaseReturnStatement(ReturnStatement * returnStatement);

#endif
