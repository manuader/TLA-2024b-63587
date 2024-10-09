%{

#include "FlexActions.h"
#include "FlexExport.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
%x STRING

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"PROGRAM"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), PROGRAM); }
"INT"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), INT); }
"BOOL"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), BOOL); }
"STRING"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), STRING); }
"AND"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), AND); }
"OR"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), OR); }
"NOT"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), NOT); }
"PRINT"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), PRINT); }
"IF"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), IF); }
"ELSE"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ELSE); }
"FOR"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FOR); }
"IN"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), IN); }
"RETURN"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), RETURN); }

"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }
"-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }

">"									{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER_THAN); }
"<"									{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS_THAN); }
"=="								{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), EQUALS_EQUALS); }
"!="								{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), NOT_EQUALS); }
">="								{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER_EQUALS); }
"<="								{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS_EQUALS); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
"{"									{ return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
"}"									{ return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }
";"									{ return PunctuationLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }
","									{ return PunctuationLexemeAction(createLexicalAnalyzerContext(), COMMA); }
"="									{ return AssignmentLexemeAction(createLexicalAnalyzerContext()); }

[a-zA-Z_][a-zA-Z0-9_]*				{ return IdentifierLexemeAction(createLexicalAnalyzerContext()); }
[0-9]+								{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

\"									{ BEGIN(STRING); BeginStringLexemeAction(createLexicalAnalyzerContext()); }
<STRING>[^\"\n\\]*					{ StringContentLexemeAction(createLexicalAnalyzerContext()); }
<STRING>\\[\\\"nrt]				    { EscapedCharacterLexemeAction(createLexicalAnalyzerContext()); }
<STRING>\"							{ return EndStringLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<STRING>\n							{ return UnexpectedCharacterLexemeAction(createLexicalAnalyzerContext()); }
<STRING>.							{ return UnexpectedCharacterLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%