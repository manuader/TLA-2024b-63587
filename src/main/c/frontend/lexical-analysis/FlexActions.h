#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */ 
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token KeywordLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token BooleanLiteralLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, boolean value);
Token ComparisonOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token BraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token PunctuationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token AssignmentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void BeginStringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void StringContentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EscapedCharacterLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token EndStringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token UnexpectedCharacterLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif
