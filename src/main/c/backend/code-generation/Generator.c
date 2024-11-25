#include "Generator.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* MODULE INTERNAL STATE */
const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

static void generateFunctionDeclarations(Block * block) {
	Instruction * current = block->instructions;
	while (current != NULL) {
		if (current->type == FUNCTION_INSTRUCTION_T) {
			generateFunction(current->function);
		}
		current = current->next;
	}
}

static void generateProgram(Program * program) {
    // First generate all function declarations
	generateFunctionDeclarations(program->block);
	
    // Then generate main func with the rest of the instructions
	_output(0, "int main() {\n");
	generateBlock(program->block, 1); 
	_output(0, "}\n\n");
}

static void generateBlock(Block * block, int indentLevel) {
	Instruction * current = block->instructions;
	while (current != NULL) {
		generateInstruction(current, indentLevel);
		current = current->next;
	}
}

static void generateInstruction(Instruction * instruction, int indentLevel) {
	switch (instruction->type) {
		case DECLARATION_INSTRUCTION_T:
			generateDeclaration(instruction->declaration, indentLevel);
			break;
		case ASSIGNATION_INSTRUCTION_T:
			generateAssignation(instruction->assignation, indentLevel);
			break;
		case PRINT_INSTRUCTION_T:
			generatePrint(instruction->print, indentLevel);
			break;
		case FUNCTION_INSTRUCTION_T:
			break;
		case CONDITIONAL_INSTRUCTION_T:
			generateConditional(instruction->conditional, indentLevel);
			break;
		case LOOP_INSTRUCTION_T:
			generateLoop(instruction->loop, indentLevel);
			break;
		case RETURN_STATEMENT_INSTRUCTION_T:
			generateReturnStatement(instruction->returnStatement, indentLevel);
			break;
		case EXPRESSION_INSTRUCTION_T:
			_output(indentLevel, "");
			generateExpression(instruction->expression);
			_output(0, ";\n");
			break;
		case FUNCTION_CALL_INSTRUCTION_T:
			_output(indentLevel, "");
			generateFunctionCall(instruction->functionCall);
			_output(0, ";\n");
			break;
	}
}

static void generateDeclaration(Declaration * declaration, int indentLevel) {
	_output(indentLevel, "%s %s", typeToString(declaration->type->type), declaration->varName);
	if (declaration->assignation != NULL) {
		_output(0, " = ");
		generateExpression(declaration->assignation->expression);
	}
	_output(0, ";\n");
}

static void generateAssignation(Assignation * assignation, int indentLevel) {
	_output(indentLevel, "%s = ", assignation->varName);
	generateExpression(assignation->expression);
	_output(0, ";\n");
}

static void generatePrint(Print * print, int indentLevel) {
	_output(indentLevel, "printf(\"");
	
	switch(print->expression->type) {
		case ARITHMETIC_EXPR_T:
			_output(0, "%%d\\n\", ");
			break;
		case BOOLEAN_EXPR_T:
			_output(0, "%%s\\n\", ");
			break;
		case STRING_EXPR_T:
			_output(0, "%%s\\n\", ");
			break;
	}
	
	generateExpression(print->expression);
	_output(0, ");\n");
}

static void generateExpression(Expression * expression) {
	switch(expression->type) {
		case ARITHMETIC_EXPR_T:
			generateArithmeticExpression(expression->arithmeticExpression);
			break;
		case BOOLEAN_EXPR_T:
			generateBooleanExpression(expression->booleanExpression);
			break;
		case STRING_EXPR_T:
			generateStringExpression(expression->stringExpression);
			break;
	}
}

static void generateArithmeticExpression(ArithmeticExpression * expression) {
	switch(expression->type) {
		case ADD_T:
		case SUB_T:
		case MUL_T:
		case DIV_T:
			_output(0, "(");
			generateArithmeticExpression(expression->left);
			switch(expression->type) {
				case ADD_T:
					_output(0, " + ");
					break;
				case SUB_T:
					_output(0, " - ");
					break;
				case MUL_T:
					_output(0, " * ");
					break;
				case DIV_T:
					_output(0, " / ");
					break;
			}
			generateArithmeticExpression(expression->right);
			_output(0, ")");
			break;
		case INT_LITERAL_T:
			_output(0, "%d", expression->value);
			break;
		case VAR_ARITH_T:
			_output(0, "%s", expression->varName);
			break;
		case FUNC_CALL_ARITH_T:
			generateFunctionCall(expression->functionCall);
			break;
	}
}

static void generateBooleanExpression(BooleanExpression * expression) {
	switch(expression->type) {
		case AND_T:
		case OR_T:
			_output(0, "("); 
			generateBooleanExpression(expression->left);
			_output(0, expression->type == AND_T ? " && " : " || ");
			generateBooleanExpression(expression->right);
			_output(0, ")");
			break;
		case NOT_T:
			_output(0, "!("); 
			generateBooleanExpression(expression->notExpr);
			_output(0, ")");
			break;
		case COMPARISON_T:
			_output(0, "(");
			generateArithmeticExpression(expression->leftArith);
			generateCompareOperator(expression->op);
			generateArithmeticExpression(expression->rightArith);
			_output(0, ")"); 
			break;
		case BOOL_LITERAL_T:
			_output(0, "%s", expression->value ? "true" : "false");
			break;
		case VAR_BOOL_T:
			_output(0, "%s", expression->varName);
			break;
		case FUNC_CALL_BOOL_T:
			generateFunctionCall(expression->functionCall);
			break;
	}
}

static void generateStringExpression(StringExpression * expression) {
	switch(expression->type) {
		case STRING_LITERAL_T:
			_output(0, "\"%s\"", expression->value);
			break;
		case VAR_STRING_T:
			_output(0, "%s", expression->varName);
			break;
		case FUNC_CALL_STRING_T:
			generateFunctionCall(expression->functionCall);
			break;
	}
}

static void generateFunction(Function * function) {
	_output(0, "%s %s(", typeToString(function->returnType->type), function->functionName);
	generateParameters(function->parameters);
	_output(0, ") {\n");
	
	generateBlock(function->block, 1);
	
	_output(0, "}\n\n");
}

static void generateFunctionCall(FunctionCall * call) {
	_output(0, "%s(", call->functionName);
	if (call->arguments != NULL) {
		generateArguments(call->arguments);
	}
	_output(0, ")");
}

static void generateConditional(Conditional * conditional, int indentLevel) {
	_output(indentLevel, "if (");
	generateBooleanExpression(conditional->condition);
	_output(0, ") {\n");
	
	generateBlock(conditional->ifBlock, indentLevel + 1);
	
	if (conditional->elseBlock != NULL) {
		_output(indentLevel, "} else {\n");
		generateBlock(conditional->elseBlock, indentLevel + 1);
	}
	
	_output(indentLevel, "}\n");
}

static void generateLoop(Loop * loop, int indentLevel) {
	_output(indentLevel, "for (int %s = ", loop->varName);
	generateArithmeticExpression(loop->start);
	_output(0, "; %s <= ", loop->varName);
	generateArithmeticExpression(loop->end);
	_output(0, "; %s++) {\n", loop->varName);
	
	generateBlock(loop->block, indentLevel + 1);
	
	_output(indentLevel, "}\n");
}

static void generateParameters(Parameters * parameters) {
	if (parameters == NULL) return;
	
	Parameter * current = parameters->parameter;
	_output(0, "%s %s", typeToString(current->type->type), current->varName);
	
	Parameters * next = parameters->next;
	while (next != NULL) {
		_output(0, ", %s %s", typeToString(next->parameter->type->type), next->parameter->varName);
		next = next->next;
	}
}

static void generateArguments(Arguments * arguments) {
	if (arguments == NULL) return;
	
	generateExpression(arguments->argument->expression);
	
	Arguments * current = arguments->next;
	while (current != NULL) {
		_output(0, ", ");
		generateExpression(current->argument->expression);
		current = current->next;
	}
}

static void generateCompareOperator(CompareOperator * op) {
	switch(op->type) {
		case GREATER_THAN_T:
			_output(0, " > ");
			break;
		case LESS_THAN_T:
			_output(0, " < ");
			break;
		case EQUALS_EQUALS_T:
			_output(0, " == ");
			break;
		case NOT_EQUALS_T:
			_output(0, " != ");
			break;
		case GREATER_EQUALS_T:
			_output(0, " >= ");
			break;
		case LESS_EQUALS_T:
			_output(0, " <= ");
			break;
	}
}

static void generateReturnStatement(ReturnStatement * returnStatement, int indentLevel) {
	_output(indentLevel, "return ");
	generateExpression(returnStatement->expression);
	_output(0, ";\n");
}

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating C code...\n");
	
	//includes and initial setup
	_output(0, "%s",
		"#include <stdio.h>\n"
		"#include <stdbool.h>\n"
		"#include <string.h>\n\n"
	);
	
	generateProgram(compilerState->abstractSyntaxtTree);
	
	logDebugging(_logger, "Code generation completed.");
}
