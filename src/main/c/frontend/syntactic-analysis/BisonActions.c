#include "BisonActions.h"
#include "../../shared/SymbolTable.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"

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
    instruction->type = DECLARATION_INSTRUCTION_T;
    instruction->declaration = declaration;
    return instruction;
}

Instruction * AssignationInstructionSemanticAction(Assignation * assignation) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = ASSIGNATION_INSTRUCTION_T;
    instruction->assignation = assignation;
    return instruction;
}

Instruction * ExpressionInstructionSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = EXPRESSION_INSTRUCTION_T;
    instruction->expression = expression;
    return instruction;
}

Instruction * PrintInstructionSemanticAction(Print * print) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = PRINT_INSTRUCTION_T;
    instruction->print = print;
    return instruction;
}

Instruction * FunctionCallInstructionSemanticAction(FunctionCall * functionCall) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = FUNCTION_CALL_INSTRUCTION_T;
    instruction->functionCall = functionCall;
    return instruction;
}

Instruction * ReturnStatementInstructionSemanticAction(ReturnStatement * returnStatement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = RETURN_STATEMENT_INSTRUCTION_T;
    instruction->returnStatement = returnStatement;
    return instruction;
}

Instruction * FunctionInstructionSemanticAction(Function * function) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = FUNCTION_INSTRUCTION_T;
    instruction->function = function;
    return instruction;
}

Instruction * ConditionalInstructionSemanticAction(Conditional * conditional) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = CONDITIONAL_INSTRUCTION_T;
    instruction->conditional = conditional;
    return instruction;
}

Instruction * LoopInstructionSemanticAction(Loop * loop) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Instruction * instruction = calloc(1, sizeof(Instruction));
    instruction->type = LOOP_INSTRUCTION_T;
    instruction->loop = loop;
    return instruction;
}

Declaration * DeclarationSemanticAction(Type * type, Assignation * assignation) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    
    if (!addSymbol(assignation->varName, VARIABLE, type)) {
        logError(_logger, "Error: Variable '%s' already declared", assignation->varName);
        // Manejar el error según la política del compilador
    }
    
    Declaration * declaration = calloc(1, sizeof(Declaration));
    declaration->type = type;
    declaration->varName = assignation->varName;
    declaration->assignation = assignation;
    return declaration;
}

Type * IntTypeSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Type * type = calloc(1, sizeof(Type));
    type->type = INT_T;
    return type;
}

Type * BoolTypeSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Type * type = calloc(1, sizeof(Type));
    type->type = BOOL_T;
    return type;
}

Type * StringTypeSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Type * type = calloc(1, sizeof(Type));
    type->type = STRING_T;
    return type;
}

Assignation * AssignationSemanticAction(char * varName, Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    
    Symbol* symbol = findSymbol(varName);
    if (symbol == NULL) {
        logError(_logger, "Error: Variable '%s' not declared", varName);
        // Manejar el error
    } else {
        Type* varType = symbol->type;
        bool typeError = false;
        
        switch (expression->type) {
            case ARITHMETIC_EXPR_T:
                if (varType->type != INT_T) typeError = true;
                break;
            case BOOLEAN_EXPR_T:
                if (varType->type != BOOL_T) typeError = true;
                break;
            case STRING_EXPR_T:
                if (varType->type != STRING_T) typeError = true;
                break;
        }
        
        if (typeError) {
            logError(_logger, "Error: Type mismatch in assignment to '%s'", varName);
            // Manejar el error
        }
    }
    
    Assignation * assignation = calloc(1, sizeof(Assignation));
    assignation->varName = varName;
    assignation->expression = expression;
    return assignation;
}

Expression * ArithmeticExpressionSemanticAction(ArithmeticExpression * arithmeticExpression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = calloc(1, sizeof(Expression));
    expression->type = ARITHMETIC_EXPR_T;
    expression->arithmeticExpression = arithmeticExpression;
    expression->resultType = createType(INT_T);
    return expression;
}

Expression * BooleanExpressionSemanticAction(BooleanExpression * booleanExpression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = calloc(1, sizeof(Expression));
    expression->type = BOOLEAN_EXPR_T;
    expression->booleanExpression = booleanExpression;
    return expression;
}

Expression * StringExpressionSemanticAction(StringExpression * stringExpression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = calloc(1, sizeof(Expression));
    expression->type = STRING_EXPR_T;
    expression->stringExpression = stringExpression;
    return expression;
}

ArithmeticExpression * AdditionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    
    if (left->type == VAR_ARITH_T) {
        Type* leftType = getSymbolType(left->varName);
        if (leftType->type != INT_T) {
            logError(_logger, "Error: Left operand must be of type INT");
            // Manejar el error
        }
    }
    
    if (right->type == VAR_ARITH_T) {
        Type* rightType = getSymbolType(right->varName);
        if (rightType->type != INT_T) {
            logError(_logger, "Error: Right operand must be of type INT");
            // Manejar el error
        }
    }
    
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = ADD_T;
    arithmeticExpression->left = left;
    arithmeticExpression->right = right;
    return arithmeticExpression;
}

ArithmeticExpression * FunctionCallArithmeticExpressionSemanticAction(FunctionCall * functionCall) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = FUNC_CALL_ARITH_T;
    arithmeticExpression->functionCall = functionCall;
    return arithmeticExpression;
}

ArithmeticExpression * SubtractionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = SUB_T;
    arithmeticExpression->left = left;
    arithmeticExpression->right = right;
    return arithmeticExpression;
}

ArithmeticExpression * MultiplicationExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = MUL_T;
    arithmeticExpression->left = left;
    arithmeticExpression->right = right;
    return arithmeticExpression;
}

ArithmeticExpression * DivisionExpressionSemanticAction(ArithmeticExpression * left, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = DIV_T;
    arithmeticExpression->left = left;
    arithmeticExpression->right = right;
    return arithmeticExpression;
}

ArithmeticExpression * VarNameArithmeticExpressionSemanticAction(char * varName) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = VAR_ARITH_T;
    arithmeticExpression->varName = varName;
    return arithmeticExpression;
}
 
ArithmeticExpression * IntegerArithmeticExpressionSemanticAction(int value) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArithmeticExpression * arithmeticExpression = calloc(1, sizeof(ArithmeticExpression));
    arithmeticExpression->type = INT_LITERAL_T;
    arithmeticExpression->value = value;
    return arithmeticExpression;
}

BooleanExpression * VarNameBooleanExpressionSemanticAction(char * varName) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = VAR_BOOL_T;
    booleanExpression->varName = varName;
    return booleanExpression;
}

BooleanExpression * FunctionCallBooleanExpressionSemanticAction(FunctionCall * functionCall) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = FUNC_CALL_BOOL_T;
    booleanExpression->functionCall = functionCall;
    return booleanExpression;
}

BooleanExpression * AndExpressionSemanticAction(BooleanExpression * left, BooleanExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    
    if (left->type == VAR_BOOL_T) {
        Type* leftType = getSymbolType(left->varName);
        if (leftType->type != BOOL_T) {
            logError(_logger, "Error: Left operand must be of type BOOL");
            // Manejar el error
        }
    }
    
    if (right->type == VAR_BOOL_T) {
        Type* rightType = getSymbolType(right->varName);
        if (rightType->type != BOOL_T) {
            logError(_logger, "Error: Right operand must be of type BOOL");
            // Manejar el error
        }
    }
    
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = AND_T;
    booleanExpression->left = left;
    booleanExpression->right = right;
    return booleanExpression;
}

BooleanExpression * OrExpressionSemanticAction(BooleanExpression * left, BooleanExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = OR_T;
    booleanExpression->left = left;
    booleanExpression->right = right;
    return booleanExpression;
}

BooleanExpression * NotExpressionSemanticAction(BooleanExpression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = NOT_T;
    booleanExpression->notExpr = expression;
    return booleanExpression;
}

BooleanExpression * ComparisonExpressionSemanticAction(ArithmeticExpression * left, CompareOperator * op, ArithmeticExpression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = COMPARISON_T;
    booleanExpression->leftArith = left;
    booleanExpression->op = op;
    booleanExpression->rightArith = right;
    return booleanExpression;
}

BooleanExpression * BoolLiteralExpressionSemanticAction(bool value) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
    booleanExpression->type = BOOL_LITERAL_T;
    booleanExpression->value = value;
    return booleanExpression;
}

StringExpression * VarNameStringExpressionSemanticAction(char * varName) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StringExpression * stringExpression = calloc(1, sizeof(StringExpression));
    stringExpression->type = VAR_STRING_T;
    stringExpression->varName = varName;
    return stringExpression;
}

StringExpression * FunctionCallStringExpressionSemanticAction(FunctionCall * functionCall) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StringExpression * stringExpression = calloc(1, sizeof(StringExpression));
    stringExpression->type = FUNC_CALL_STRING_T;
    stringExpression->functionCall = functionCall;
    return stringExpression;
}

StringExpression * StringLiteralExpressionSemanticAction(char * value) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StringExpression * stringExpression = calloc(1, sizeof(StringExpression));
    stringExpression->type = STRING_LITERAL_T;
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
    
    if (!addSymbol(functionName, FUNCTION, returnType)) {
        logError(_logger, "Error: Function '%s' already declared", functionName);
        // Manejar el error según la política del compilador
    }
    
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

FunctionCall * FunctionCallSemanticAction(char * functionName, Arguments * arguments, Type * returnType) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FunctionCall * functionCall = calloc(1, sizeof(FunctionCall));
    functionCall->functionName = functionName;
    functionCall->arguments = arguments;
    functionCall->returnType = returnType;
    
    checkFunctionCallTypes(functionCall);
    
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
    op->type = GREATER_THAN_T;
    return op;
}

CompareOperator * LessThanSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = LESS_THAN_T;
    return op;
}

CompareOperator * EqualsEqualsSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = EQUALS_EQUALS_T;
    return op;
}

CompareOperator * NotEqualsSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = NOT_EQUALS_T;
    return op;
}

CompareOperator * GreaterEqualsSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = GREATER_EQUALS_T;
    return op;
}

CompareOperator * LessEqualsSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    CompareOperator * op = calloc(1, sizeof(CompareOperator));
    op->type = LESS_EQUALS_T;
    return op;
}

ReturnStatement * ReturnStatementSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ReturnStatement * returnStatement = calloc(1, sizeof(ReturnStatement));
    returnStatement->expression = expression;
    return returnStatement;
}

bool validateTypes(Type* expected, Type* actual, const char* context) {
    if (expected->type != actual->type) {
        logError(_logger, "Type mismatch in %s: expected %d, got %d", 
                context, expected->type, actual->type);
        return false;
    }
    return true;
}

Type* inferExpressionType(Expression* expr) {
    switch (expr->type) {
        case ARITHMETIC_EXPR_T:
            return createType(INT_T);
        case BOOLEAN_EXPR_T:
            return createType(BOOL_T);
        case STRING_EXPR_T:
            return createType(STRING_T);
        default:
            logError(_logger, "Unknown expression type");
            return NULL;
    }
}

void checkFunctionCallTypes(FunctionCall* call) {
    Symbol* symbol = findSymbol(call->functionName);
    if (!symbol) {
        logError(_logger, "Function %s not declared", call->functionName);
        return;
    }

    if (symbol->kind != FUNCTION) {
        logError(_logger, "%s is not a function", call->functionName);
        return;
    }

    // Validar tipo de retorno
    call->returnType = symbol->type;

    // Validar argumentos
    Arguments* currentArg = call->arguments;
    Parameters* currentParam = symbol->parameters;  // Necesitamos agregar esto a Symbol

    while (currentArg && currentParam) {
        Type* argType = inferExpressionType(currentArg->argument->expression);
        if (!validateTypes(currentParam->parameter->type, argType, "function argument")) {
            return;
        }
        currentArg = currentArg->next;
        currentParam = currentParam->next;
    }

    if (currentArg || currentParam) {
        logError(_logger, "Wrong number of arguments for function %s", call->functionName);
    }
}
