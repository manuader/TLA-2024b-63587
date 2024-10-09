#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntaxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseBlock(program->block);
		free(program);
	}
}

void releaseBlock(Block * block) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (block != NULL) {
		releaseInstruction(block->instructions);
		free(block);
	}
}

void releaseInstruction(Instruction * instruction) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (instruction != NULL) {
		switch (instruction->type) {
			case DECLARATION_INSTRUCTION:
				releaseDeclaration(instruction->declaration);
				break;
			case ASSIGNATION_INSTRUCTION:
				releaseAssignation(instruction->assignation);
				break;
			case EXPRESSION_INSTRUCTION:
				releaseExpression(instruction->expression);
				break;
			case PRINT_INSTRUCTION:
				releasePrint(instruction->print);
				break;
			case FUNCTION_CALL_INSTRUCTION:
				releaseFunctionCall(instruction->functionCall);
				break;
			case RETURN_STATEMENT_INSTRUCTION:
				releaseReturnStatement(instruction->returnStatement);
				break;
			case FUNCTION_INSTRUCTION:
				releaseFunction(instruction->function);
				break;
			case CONDITIONAL_INSTRUCTION:
				releaseConditional(instruction->conditional);
				break;
			case LOOP_INSTRUCTION:
				releaseLoop(instruction->loop);
				break;
		}
		releaseInstruction(instruction->next);
		free(instruction);
	}
}

void releaseDeclaration(Declaration * declaration) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (declaration != NULL) {
		free(declaration->type);
		free(declaration->varName);
		releaseAssignation(declaration->assignation);
		free(declaration);
	}
}

void releaseAssignation(Assignation * assignation) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (assignation != NULL) {
		free(assignation->varName);
		releaseExpression(assignation->expression);
		free(assignation);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ARITHMETIC:
				releaseArithmeticExpression(expression->arithmeticExpression);
				break;
			case BOOLEAN:
				releaseBooleanExpression(expression->booleanExpression);
				break;
			case STRING:
				releaseStringExpression(expression->stringExpression);
				break;
		}
		free(expression);
	}
}

void releasePrint(Print * print) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (print != NULL) {
		releaseExpression(print->expression);
		free(print);
	}
}

void releaseFunctionCall(FunctionCall * functionCall) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (functionCall != NULL) {
		free(functionCall->functionName);
		releaseArguments(functionCall->arguments);
		free(functionCall);
	}
}

void releaseFunction(Function * function) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (function != NULL) {
		free(function->returnType);
		free(function->functionName);
		releaseParameters(function->parameters);
		releaseBlock(function->block);
		free(function);
	}
}

void releaseConditional(Conditional * conditional) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (conditional != NULL) {
		releaseBooleanExpression(conditional->condition);
		releaseBlock(conditional->ifBlock);
		releaseBlock(conditional->elseBlock);
		free(conditional);
	}
}

void releaseLoop(Loop * loop) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (loop != NULL) {
		free(loop->varName);
		releaseArithmeticExpression(loop->start);
		releaseArithmeticExpression(loop->end);
		releaseBlock(loop->block);
		free(loop);
	}
}

void releaseArithmeticExpression(ArithmeticExpression * arithmeticExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (arithmeticExpression != NULL) {
		switch (arithmeticExpression->type) {
			case ADD:
			case SUB:
			case MUL:
			case DIV:
				releaseArithmeticExpression(arithmeticExpression->left);
				releaseArithmeticExpression(arithmeticExpression->right);
				break;
			case VAR:
				free(arithmeticExpression->varName);
				break;
			case INT:
				// No need to free int value
				break;
		}
		free(arithmeticExpression);
	}
}

void releaseBooleanExpression(BooleanExpression * booleanExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (booleanExpression != NULL) {
		switch (booleanExpression->type) {
			case AND:
			case OR:
				releaseBooleanExpression(booleanExpression->left);
				releaseBooleanExpression(booleanExpression->right);
				break;
			case NOT:
				releaseBooleanExpression(booleanExpression->notExpr);
				break;
			case COMPARISON:
				releaseArithmeticExpression(booleanExpression->leftArith);
				releaseCompareOperator(booleanExpression->op);
				releaseArithmeticExpression(booleanExpression->rightArith);
				break;
			case VAR:
				free(booleanExpression->varName);
				break;
			case BOOL_LITERAL:
				// No need to free bool value
				break;
		}
		free(booleanExpression); 
	}
}

void releaseStringExpression(StringExpression * stringExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (stringExpression != NULL) {
		free(stringExpression->value);
		free(stringExpression);
	}
}

void releaseCompareOperator(CompareOperator * compareOperator) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (compareOperator != NULL) {
		free(compareOperator);
	}
}

void releaseParameters(Parameters * parameters) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (parameters != NULL) {
		releaseParameter(parameters->parameter);
		releaseParameters(parameters->next);
		free(parameters);
	}
}

void releaseParameter(Parameter * parameter) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (parameter != NULL) {
		free(parameter->type);
		free(parameter->varName);
		free(parameter);
	}
}

void releaseArguments(Arguments * arguments) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (arguments != NULL) {
		releaseArgument(arguments->argument);
		releaseArguments(arguments->next);
		free(arguments);
	}
}

void releaseArgument(Argument * argument) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (argument != NULL) {
		releaseExpression(argument->expression);
		free(argument);
	}
}

void releaseReturnStatement(ReturnStatement * returnStatement) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (returnStatement != NULL) {
		releaseExpression(returnStatement->expression);
		free(returnStatement);
	}
}
