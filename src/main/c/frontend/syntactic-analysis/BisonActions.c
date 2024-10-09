#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Program * ProgramSemanticAction(CompilerState * compilerState, Block * block) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Program * program = calloc(1, sizeof(Program));
    program->block = block;
    compilerState->abstractSyntaxtTree = program;
    if (0 < flexCurrentContext()) {
        logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
        compilerState->succeed = false;
    } else {
        compilerState->succeed = true;
    }
    return program;
}

Block * BlockSemanticAction(Instruction * instructions) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Block * block = calloc(1, sizeof(Block));
    block->instructions = instructions;
    return block;
}

Instruction * SingleInstructionSemanticAction(Instruction * instruction) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return instruction;
}

Instruction * MultipleInstructionsSemanticAction(Instruction * instruction, Instruction * instructions) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    instruction->next = instructions;
    return instruction;
}

Instruction * DeclarationInstructionSemanticAction(Declaration * declaration) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = DECLARATION_INSTRUCTION;
    instruction->declaration = declaration;
    return instruction;
}

Instruction * AssignationInstructionSemanticAction(Assignation * assignation) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = ASSIGNATION_INSTRUCTION;
    instruction->assignation = assignation;
    return instruction;
}

Instruction * ExpressionInstructionSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = EXPRESSION_INSTRUCTION;
    instruction->expression = expression;
    return instruction;
}

Instruction * PrintInstructionSemanticAction(Print * print) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = PRINT_INSTRUCTION;
    instruction->print = print;
    return instruction;
}

Instruction * FunctionCallInstructionSemanticAction(FunctionCall * functionCall) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = FUNCTION_CALL_INSTRUCTION;
    instruction->functionCall = functionCall;
    return instruction;
}

Instruction * ReturnStatementInstructionSemanticAction(ReturnStatement * returnStatement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = RETURN_STATEMENT_INSTRUCTION;
    instruction->returnStatement = returnStatement;
    return instruction;
}

Instruction * FunctionInstructionSemanticAction(Function * function) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = FUNCTION_INSTRUCTION;
    instruction->function = function;
    return instruction;
}

Instruction * ConditionalInstructionSemanticAction(Conditional * conditional) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = CONDITIONAL_INSTRUCTION;
    instruction->conditional = conditional;
    return instruction;
}

Instruction * LoopInstructionSemanticAction(Loop * loop) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = LOOP_INSTRUCTION;
    instruction->loop = loop;
    return instruction;
}

Declaration * DeclarationSemanticAction(Type * type, char * varName, Assignation * assignation) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Declaration * declaration = calloc(1, sizeof(Declaration));
    declaration->type = type;
    declaration->varName = varName;
    declaration->assignation = assignation;
    return declaration;
}

Type * IntTypeSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Type * type = calloc(1, sizeof(Type));
    type->type = INT_TYPE;
    return type;
} 

Type * BoolTypeSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Type * type = calloc(1, sizeof(Type));
    type->type = BOOL_TYPE;
    return type;
}

Type * StringTypeSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Type * type = calloc(1, sizeof(Type));
    type->type = STRING_TYPE;
    return type;
}

Assignation * AssignationSemanticAction(char * varName, Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Assignation * assignation = calloc(1, sizeof(Assignation));
    assignation->varName = varName;
    assignation->expression = expression;
    return assignation;
}

Expression * ArithmeticExpressionSemanticAction(ArithmeticExpression * arithmeticExpression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = calloc(1, sizeof(Expression));
    expression->type = ARITHMETIC_EXPR;
    expression->arithmeticExpression = arithmeticExpression;
    return expression;
}

Expression * BooleanExpressionSemanticAction(BooleanExpression * booleanExpression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = calloc(1, sizeof(Expression));
    expression->type = BOOLEAN_EXPR;
    expression->booleanExpression = booleanExpression;
    return expression;
}

Expression * StringExpressionSemanticAction(StringExpression * stringExpression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = calloc(1, sizeof(Expression));
    expression->type = STRING_EXPR;
    expression->stringExpression = stringExpression;
    return expression;
}

ArithmeticExpression * AdditionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = ADD;
    arithmeticExpression->left = left;
    arithmeticExpression->right = right;
    return arithmeticExpression;
}

ArithmeticExpression * SubtractionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = SUB;
    arithmeticExpression->left = left;
    arithmeticExpression->right = right;
    return arithmeticExpression;
}

ArithmeticExpression * MultiplicationExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = MUL;
    arithmeticExpression->left = left;
    arithmeticExpression->right = right;
    return arithmeticExpression;
}

ArithmeticExpression * DivisionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = DIV;
    arithmeticExpression->left = left;
    arithmeticExpression->right = right;
    return arithmeticExpression;
}

ArithmeticExpression * VarNameArithmeticExpressionSemanticAction(char * varName) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = VAR_ARITH;
    arithmeticExpression->varName = varName;
    return arithmeticExpression;
}
 
ArithmeticExpression * IntegerArithmeticExpressionSemanticAction(int value) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = INT_TYPE;
    arithmeticExpression->value = value;
    return arithmeticExpression;
}

BooleanExpression * AndExpressionSemanticAction(BooleanExpression * left, BooleanExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = AND;
    booleanExpression->left = left;
    booleanExpression->right = right;
    return booleanExpression;
}

BooleanExpression * OrExpressionSemanticAction(BooleanExpression * left, BooleanExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = OR;
    booleanExpression->left = left;
    booleanExpression->right = right;
    return booleanExpression;
}

BooleanExpression * NotExpressionSemanticAction(BooleanExpression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = NOT;
    booleanExpression->notExpr = expression;
    return booleanExpression;
}

BooleanExpression * ComparisonExpressionSemanticAction(ArithmeticExpression * left, CompareOperator * op, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = COMPARISON;
    booleanExpression->leftArith = left;
    booleanExpression->op = op;
    booleanExpression->rightArith = right;
    return booleanExpression;
}

BooleanExpression * VarNameBooleanExpressionSemanticAction(char * varName) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = VAR_BOOL;
    booleanExpression->varName = varName;
    return booleanExpression;
}

BooleanExpression * BoolLiteralExpressionSemanticAction(bool value) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = BOOL_LITERAL;
    booleanExpression->value = value;
    return booleanExpression;
}

StringExpression * StringLiteralExpressionSemanticAction(char * value) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StringExpression * stringExpression = calloc(1, sizeof(StringExpression));
    stringExpression->value = value;
    return stringExpression;
}

Print * PrintSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Print * print = calloc(1, sizeof(Print));
    print->expression = expression;
    return print;
}

Function * FunctionSemanticAction(Type * returnType, char * functionName, Parameters * parameters, Block * block) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Function * function = calloc(1, sizeof(Function));
    function->returnType = returnType;
    function->functionName = functionName;
    function->parameters = parameters;
    function->block = block;
    return function;
}

Parameters * SingleParameterSemanticAction(Parameter * parameter) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Parameters * parameters = calloc(1, sizeof(Parameters));
    parameters->parameter = parameter;
    parameters->next = NULL;
    return parameters;
}

Parameters * MultipleParametersSemanticAction(Parameter * parameter, Parameters * parameters) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Parameters * newParameters = calloc(1, sizeof(Parameters));
    newParameters->parameter = parameter;
    newParameters->next = parameters;
    return newParameters;
}

Parameter * ParameterSemanticAction(Type * type, char * varName) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Parameter * parameter = calloc(1, sizeof(Parameter));
    parameter->type = type;
    parameter->varName = varName;
    return parameter;
}

FunctionCall * FunctionCallSemanticAction(char * functionName, Arguments * arguments) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FunctionCall * functionCall = calloc(1, sizeof(FunctionCall));
    functionCall->functionName = functionName;
    functionCall->arguments = arguments;
    return functionCall;
}

Arguments * SingleArgumentSemanticAction(Argument * argument) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Arguments * arguments = calloc(1, sizeof(Arguments));
    arguments->argument = argument;
    arguments->next = NULL;
    return arguments;
}

Arguments * MultipleArgumentsSemanticAction(Argument * argument, Arguments * arguments) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Arguments * newArguments = calloc(1, sizeof(Arguments));
    newArguments->argument = argument;
    newArguments->next = arguments;
    return newArguments;
}

Argument * ArgumentSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Argument * argument = calloc(1, sizeof(Argument));
    argument->expression = expression;
    return argument;
}

Conditional * ConditionalSemanticAction(BooleanExpression * condition, Block * ifBlock, Block * elseBlock) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Conditional * conditional = calloc(1, sizeof(Conditional));
    conditional->condition = condition;
    conditional->ifBlock = ifBlock;
    conditional->elseBlock = elseBlock;
    return conditional;
}

Loop * LoopSemanticAction(char * varName, ArithmeticExpression * start, ArithmeticExpression * end, Block * block) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Loop * loop = calloc(1, sizeof(Loop));
    loop->varName = varName;
    loop->start = start;
    loop->end = end;
    loop->block = block;
    return loop;
}

CompareOperator * GreaterThanSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = GREATER_THAN;
    return op;
}

CompareOperator * LessThanSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = LESS_THAN;
    return op;
}

CompareOperator * EqualsEqualsSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = EQUALS_EQUALS;
    return op;
}

CompareOperator * NotEqualsSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = NOT_EQUALS;
    return op;
}

CompareOperator * GreaterEqualsSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = GREATER_EQUALS;
    return op;
}

CompareOperator * LessEqualsSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = LESS_EQUALS;
    return op;
}

ReturnStatement * ReturnStatementSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ReturnStatement * returnStatement = calloc(1, sizeof(ReturnStatement));
    returnStatement->expression = expression;
    return returnStatement;
}
