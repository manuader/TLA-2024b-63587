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

/** PRIVATE FUNCTIONS */

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
	// Primera pasada: generar solo las funciones
	while (current != NULL) {
		if (current->type == FUNCTION_INSTRUCTION_T) {
			generateFunction(current->function);
		}
		current = current->next;
	}
}

static void generateNonFunctionInstructions(Block * block) {
	Instruction * current = block->instructions;
	// Segunda pasada: generar el resto de las instrucciones
	while (current != NULL) {
		if (current->type != FUNCTION_INSTRUCTION_T) {
			generateInstruction(current);
		}
		current = current->next;
	}
}

static void generateProgram(Program * program) {
	// Primero generar todas las declaraciones de funciones
	generateFunctionDeclarations(program->block);
	
	// Luego generar la función main
	_output(0, "int main() {\n");
	generateNonFunctionInstructions(program->block);
	_output(0, "}\n\n");
}

static void generateBlock(Block * block) {
	Instruction * current = block->instructions;
	while (current != NULL) {
		generateInstruction(current);
		current = current->next;
	}
}

static void generateInstruction(Instruction * instruction) {
	switch (instruction->type) {
		case DECLARATION_INSTRUCTION_T:
			generateDeclaration(instruction->declaration);
			break;
		case ASSIGNATION_INSTRUCTION_T:
			generateAssignation(instruction->assignation);
			break;
		case PRINT_INSTRUCTION_T:
			generatePrint(instruction->print);
			break;
		case FUNCTION_INSTRUCTION_T:
			generateFunction(instruction->function);
			break;
		case CONDITIONAL_INSTRUCTION_T:
			generateConditional(instruction->conditional);
			break;
		case LOOP_INSTRUCTION_T:
			generateLoop(instruction->loop);
			break;
		case RETURN_STATEMENT_INSTRUCTION_T:
			generateReturnStatement(instruction->returnStatement);
			break;
		case EXPRESSION_INSTRUCTION_T:
			generateExpression(instruction->expression);
			_output(0, ";\n");
			break;
		case FUNCTION_CALL_INSTRUCTION_T:
			generateFunctionCall(instruction->functionCall);
			_output(0, ";\n");
			break;
	}
}

static void generateDeclaration(Declaration * declaration) {
	_output(1, "%s %s", typeToString(declaration->type->type), declaration->varName);
	if (declaration->assignation != NULL) {
		_output(0, " = ");
		generateExpression(declaration->assignation->expression);
	}
	_output(0, ";\n");
}

static void generateAssignation(Assignation * assignation) {
	_output(1, "%s = ", assignation->varName);
	generateExpression(assignation->expression);
	_output(0, ";\n");
}

static void generatePrint(Print * print) {
	_output(1, "printf(\"");
	
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
			generateArithmeticExpression(expression->left);
			_output(0, " + ");
			generateArithmeticExpression(expression->right);
			break;
		case SUB_T:
			generateArithmeticExpression(expression->left);
			_output(0, " - ");
			generateArithmeticExpression(expression->right);
			break;
		case MUL_T:
			generateArithmeticExpression(expression->left);
			_output(0, " * ");
			generateArithmeticExpression(expression->right);
			break;
		case DIV_T:
			generateArithmeticExpression(expression->left);
			_output(0, " / ");
			generateArithmeticExpression(expression->right);
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
			generateBooleanExpression(expression->left);
			_output(0, " && ");
			generateBooleanExpression(expression->right);
			break;
		case OR_T:
			generateBooleanExpression(expression->left);
			_output(0, " || ");
			generateBooleanExpression(expression->right);
			break;
		case NOT_T:
			_output(0, "!");
			generateBooleanExpression(expression->notExpr);
			break;
		case COMPARISON_T:
			generateArithmeticExpression(expression->leftArith);
			generateCompareOperator(expression->op);
			generateArithmeticExpression(expression->rightArith);
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
	
	generateBlock(function->block);
	
	_output(0, "}\n\n");
}

static void generateFunctionCall(FunctionCall * call) {
	_output(0, "%s(", call->functionName);
	if (call->arguments != NULL) {
		generateArguments(call->arguments);
	}
	_output(0, ")");
}

static void generateConditional(Conditional * conditional) {
	_output(1, "if (");
	generateBooleanExpression(conditional->condition);
	_output(0, ") {\n");
	
	generateBlock(conditional->ifBlock);
	
	if (conditional->elseBlock != NULL) {
		_output(1, "} else {\n");
		generateBlock(conditional->elseBlock);
	}
	
	_output(1, "}\n");
}

static void generateLoop(Loop * loop) {
	_output(1, "for (int %s = ", loop->varName);
	generateArithmeticExpression(loop->start);
	_output(0, "; %s <= ", loop->varName);
	generateArithmeticExpression(loop->end);
	_output(0, "; %s++) {\n", loop->varName);
	
	generateBlock(loop->block);
	
	_output(1, "}\n");
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

static void generateReturnStatement(ReturnStatement * returnStatement) {
	_output(1, "return ");
	generateExpression(returnStatement->expression);
	_output(0, ";\n");
}

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating C code...\n");
	
	// Generar includes y setup inicial
	_output(0, "%s",
		"#include <stdio.h>\n"
		"#include <stdbool.h>\n"
		"#include <string.h>\n\n"
	);
	
	// Generar el programa
	generateProgram(compilerState->abstractSyntaxtTree);
	
	logDebugging(_logger, "Code generation completed.");
}
