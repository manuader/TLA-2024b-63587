%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	Token token;
	int integer;
	char* string;
	bool boolean;

	/** Non-terminals. */

	Expression * expression;
	Program * program;
	Block * block;
	Instruction * instruction;
	Declaration * declaration;
	Assignation * assignation;
	Print * print;
	FunctionCall * functionCall;
	Function * function;
	Conditional * conditional;
	Loop * loop;
	Type * type;
	ArithmeticExpression * arithmeticExpression;
	BooleanExpression * booleanExpression;
	StringExpression * stringExpression;
	CompareOperator * compareOperator;
	Parameters * parameters;
	Parameter * parameter;
	Arguments * arguments;
	Argument * argument;
	ReturnStatement * returnStatement;
}

/** Terminals. */
%token <integer> INTEGER_LITERAL
%token <token> ADD SUB MUL DIV
%token <token> AND OR NOT
%token <token> PROGRAM INT BOOL STRING PRINT IF ELSE FOR IN RETURN
%token <token> OPEN_PARENTHESIS CLOSE_PARENTHESIS SEMICOLON OPEN_BRACE CLOSE_BRACE COMMA EQUALS
%token <token> GREATER_THAN LESS_THAN EQUALS_EQUALS NOT_EQUALS GREATER_EQUALS LESS_EQUALS
%token <string> VAR_NAME FUNCTION_NAME STRING_LITERAL
%token <string> INT_VAR_NAME BOOL_VAR_NAME STRING_VAR_NAME
%token <string> INT_FUNCTION_NAME BOOL_FUNCTION_NAME STRING_FUNCTION_NAME
%token <boolean> BOOL_LITERAL

/** Non-terminals. */
%type <program> program
%type <block> block
%type <instruction> instructions instruction
%type <declaration> declaration
%type <assignation> assignation
%type <expression> expression
%type <print> print
%type <functionCall> function_call int_function_call bool_function_call string_function_call
%type <function> function
%type <conditional> conditional
%type <loop> loop
%type <type> type
%type <arithmeticExpression> arit_exp
%type <booleanExpression> bool_exp
%type <stringExpression> string_exp
%type <compareOperator> compare_op
%type <parameters> parameters
%type <parameter> parameter
%type <arguments> arguments
%type <argument> argument
%type <returnStatement> return_statement

%start program

/* Precedence rules */
%left OR
%left AND
%left EQUALS_EQUALS NOT_EQUALS
%left LESS_THAN GREATER_THAN LESS_EQUALS GREATER_EQUALS
%left ADD SUB
%left MUL DIV
%right NOT

%%

program: PROGRAM block                                              { $$ = ProgramSemanticAction(currentCompilerState(), $2); }
	;

block: OPEN_BRACE instructions CLOSE_BRACE                          { $$ = BlockSemanticAction($2); }
	;

instructions: instruction                                           { $$ = SingleInstructionSemanticAction($1); }
	| instruction instructions                                      { $$ = MultipleInstructionsSemanticAction($1, $2); }
	;

instruction: declaration SEMICOLON                                  { $$ = DeclarationInstructionSemanticAction($1); }
	| assignation SEMICOLON                                         { $$ = AssignationInstructionSemanticAction($1); }
	| print SEMICOLON                                               { $$ = PrintInstructionSemanticAction($1); }
	| function_call SEMICOLON                                       { $$ = FunctionCallInstructionSemanticAction($1); }
	| return_statement SEMICOLON                                    { $$ = ReturnStatementInstructionSemanticAction($1); }
	| function                                                      { $$ = FunctionInstructionSemanticAction($1); }
	| conditional                                                   { $$ = ConditionalInstructionSemanticAction($1); }
	| loop                                                          { $$ = LoopInstructionSemanticAction($1); }
	;

declaration: type assignation                             			{ $$ = DeclarationSemanticAction($1, $2); }
	;

type: INT                                                           { $$ = IntTypeSemanticAction(); }
	| BOOL                                                          { $$ = BoolTypeSemanticAction(); }
	| STRING                                                        { $$ = StringTypeSemanticAction(); }
	;

assignation: VAR_NAME EQUALS expression                             { $$ = AssignationSemanticAction($1, $3); }
	;
	

expression: arit_exp                                                { $$ = ArithmeticExpressionSemanticAction($1); }
	| bool_exp                                                      { $$ = BooleanExpressionSemanticAction($1); }
	| string_exp                                                    { $$ = StringExpressionSemanticAction($1); }
	;

arit_exp: INTEGER_LITERAL                                           { $$ = IntegerArithmeticExpressionSemanticAction($1); }
	| INT_VAR_NAME                                                  { $$ = VarNameArithmeticExpressionSemanticAction($1); }
	| int_function_call                                             { $$ = FunctionCallArithmeticExpressionSemanticAction($1); }
	| arit_exp ADD arit_exp                                     	{ $$ = AdditionExpressionSemanticAction($1, $3); }
	| arit_exp SUB arit_exp                                         { $$ = SubtractionExpressionSemanticAction($1, $3); }
	| arit_exp MUL arit_exp                                         { $$ = MultiplicationExpressionSemanticAction($1, $3); }
	| arit_exp DIV arit_exp                                         { $$ = DivisionExpressionSemanticAction($1, $3); }
	| OPEN_PARENTHESIS arit_exp CLOSE_PARENTHESIS                   { $$ = $2; }
	;

bool_exp: BOOL_LITERAL                                              { $$ = BoolLiteralExpressionSemanticAction($1); }
	| BOOL_VAR_NAME                                                 { $$ = VarNameBooleanExpressionSemanticAction($1); }
	| bool_function_call                                            { $$ = FunctionCallBooleanExpressionSemanticAction($1); }
	| bool_exp AND bool_exp                                     	{ $$ = AndExpressionSemanticAction($1, $3); }
	| bool_exp OR bool_exp                                          { $$ = OrExpressionSemanticAction($1, $3); }
	| NOT bool_exp                                                  { $$ = NotExpressionSemanticAction($2); }
	| arit_exp compare_op arit_exp                                  { $$ = ComparisonExpressionSemanticAction($1, $2, $3); }
	| OPEN_PARENTHESIS bool_exp CLOSE_PARENTHESIS                   { $$ = $2; }
	;

string_exp: STRING_LITERAL                                          { $$ = StringLiteralExpressionSemanticAction($1); }
	| STRING_VAR_NAME                                               { $$ = VarNameStringExpressionSemanticAction($1); }
	| string_function_call                                          { $$ = FunctionCallStringExpressionSemanticAction($1); }
	;

print: PRINT OPEN_PARENTHESIS expression CLOSE_PARENTHESIS          { $$ = PrintSemanticAction($3); }
	;

function: type FUNCTION_NAME OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS block { $$ = FunctionSemanticAction($1, $2, $4, $6); }
	| type FUNCTION_NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS block 	 			 { $$ = FunctionSemanticAction($1, $2, NULL, $5); }
	;

parameters: parameter                                               { $$ = SingleParameterSemanticAction($1); }
	| parameter COMMA parameters                                    { $$ = MultipleParametersSemanticAction($1, $3); }
	;

parameter: type VAR_NAME                                            { $$ = ParameterSemanticAction($1, $2); }
	;

function_call: int_function_call | bool_function_call | string_function_call
	;

int_function_call: INT_FUNCTION_NAME OPEN_PARENTHESIS arguments CLOSE_PARENTHESIS { $$ = FunctionCallSemanticAction($1, $3, IntTypeSemanticAction()); }
	| INT_FUNCTION_NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS					    { $$ = FunctionCallSemanticAction($1, NULL, IntTypeSemanticAction()); }
	;

bool_function_call: BOOL_FUNCTION_NAME OPEN_PARENTHESIS arguments CLOSE_PARENTHESIS { $$ = FunctionCallSemanticAction($1, $3, BoolTypeSemanticAction()); }
	| BOOL_FUNCTION_NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS					      { $$ = FunctionCallSemanticAction($1, NULL, BoolTypeSemanticAction()); }
	;

string_function_call: STRING_FUNCTION_NAME OPEN_PARENTHESIS arguments CLOSE_PARENTHESIS { $$ = FunctionCallSemanticAction($1, $3, StringTypeSemanticAction()); }
	| STRING_FUNCTION_NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS					        { $$ = FunctionCallSemanticAction($1, NULL, StringTypeSemanticAction()); }
	;

arguments: argument                                                 { $$ = SingleArgumentSemanticAction($1); }
	| argument COMMA arguments                                      { $$ = MultipleArgumentsSemanticAction($1, $3); }
	;

argument: expression                                                { $$ = ArgumentSemanticAction($1); }
	;

conditional: IF OPEN_PARENTHESIS bool_exp CLOSE_PARENTHESIS block ELSE block { $$ = ConditionalSemanticAction($3, $5, $7); }
	;

loop: FOR VAR_NAME IN OPEN_PARENTHESIS arit_exp COMMA arit_exp CLOSE_PARENTHESIS block { $$ = LoopSemanticAction($2, $5, $7, $9); }
	;

compare_op: GREATER_THAN                                            { $$ = GreaterThanSemanticAction(); }
	| LESS_THAN                                                     { $$ = LessThanSemanticAction(); }
	| EQUALS_EQUALS                                                 { $$ = EqualsEqualsSemanticAction(); }
	| NOT_EQUALS                                                    { $$ = NotEqualsSemanticAction(); }
	| GREATER_EQUALS                                                { $$ = GreaterEqualsSemanticAction(); }
	| LESS_EQUALS                                                   { $$ = LessEqualsSemanticAction(); }
	;

return_statement: RETURN expression                                 { $$ = ReturnStatementSemanticAction($2); }
	;

%%
