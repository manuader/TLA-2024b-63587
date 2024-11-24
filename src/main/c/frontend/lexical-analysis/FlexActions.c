#include "FlexActions.h"
#include "../../shared/SymbolTable.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;
Type* _type = NULL;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
	_type = NULL;
}
 
void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
	destroyType(_type);
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
	return INTEGER_LITERAL;
}

Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return YYUNDEF;
}

Token KeywordLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return token;
}

Token BooleanLiteralLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, boolean value) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->boolean = value;
    return BOOL_LITERAL;
}

Token ComparisonOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return token;
}

Token BraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return token;
}

Token PunctuationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return token;
}

Token AssignmentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return EQUALS;
}

Token TypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    if (_type != NULL) {
        destroyType(_type);
    }
    logDebugging(_logger, "%s -- TIPO DE VARIABLE: %d ", __FUNCTION__, token);
    _type = createType(token);
    switch (token) {
        case INT_T:
            return INT;
        case BOOL_T:
            return BOOL;
        case STRING_T:
            return STRING;
        default:
            return YYUNDEF;
    }
}

Token VariableIdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    
    char* identifier = strdup(lexicalAnalyzerContext->lexeme);
    lexicalAnalyzerContext->semanticValue->string = identifier;
    Type* identifierType = NULL;
    
    Symbol* symbol = findSymbol(identifier);
    
    if (symbol != NULL) {   
        // If the identifier already exists in the symbol table use its type
        logDebugging(_logger, "%s -- IDENTIFIER ENCONTRADO: %s", __FUNCTION__, identifier);
        if (symbol->kind == VARIABLE) identifierType = symbol->type;

    } else {
        // If the identifier does not exist in the symbol table, a new symbol is created
        logDebugging(_logger, "%s -- IDENTIFIER NO ENCONTRADO: %s", __FUNCTION__, identifier);
        logDebugging(_logger, "%s -- TIPO DE VARIABLE: %d", __FUNCTION__, _type == NULL ? -1 : _type->type);
        if (_type != NULL) {
            identifierType = createType(_type->type);
            addSymbol(identifier, VARIABLE, identifierType);
        }
    }

    if (identifierType != NULL) {
        switch (identifierType->type) {
            case INT_T:
                logDebugging(_logger, "%s -- TOKEN ENVIADO: INT_VAR_NAME", __FUNCTION__);
                return INT_VAR_NAME;
            case BOOL_T:
                logDebugging(_logger, "%s -- TOKEN ENVIADO: BOOL_VAR_NAME", __FUNCTION__);
                return BOOL_VAR_NAME;
            case STRING_T:
                logDebugging(_logger, "TOKEN ENVIADO: STRING_VAR_NAME");
                return STRING_VAR_NAME;
        }
    }

    return YYUNDEF;                           
}

Token FunctionIdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    char* identifier = strdup(lexicalAnalyzerContext->lexeme);
    lexicalAnalyzerContext->semanticValue->string = identifier;
    Type* identifierType = _type;
    
    Symbol* symbol = findSymbol(identifier);
    
    if (symbol != NULL) {   
        // If the identifier already exists in the symbol table use its type
        if (symbol->kind == FUNCTION) identifierType = symbol->type;

    } else {
        // If the identifier does not exist in the symbol table, a new symbol is created
        return FUNCTION_NAME;
    }

    switch (identifierType->type) {
        case INT_T:
            return INT_FUNCTION_NAME;
        case BOOL_T:
            return BOOL_FUNCTION_NAME;
        case STRING_T:
            return STRING_FUNCTION_NAME;
    }    
}

Token LoopVariableIdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return INT_VAR_NAME;
}

void BeginStringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->string = strdup("");
}

void StringContentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    char* newContent = realloc(lexicalAnalyzerContext->semanticValue->string, 
                                strlen(lexicalAnalyzerContext->semanticValue->string) + 
                                strlen(lexicalAnalyzerContext->lexeme) + 1);
    if (newContent) {
        lexicalAnalyzerContext->semanticValue->string = newContent;
        strcat(lexicalAnalyzerContext->semanticValue->string, lexicalAnalyzerContext->lexeme);
    }
}

void EscapedCharacterLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    char escapedChar;
    switch(lexicalAnalyzerContext->lexeme[1]) {
        case 'n': escapedChar = '\n'; break;
        case 'r': escapedChar = '\r'; break;
        case 't': escapedChar = '\t'; break;
        default: escapedChar = lexicalAnalyzerContext->lexeme[1];
    }
    char* newContent = realloc(lexicalAnalyzerContext->semanticValue->string, 
                                strlen(lexicalAnalyzerContext->semanticValue->string) + 2);
    if (newContent) {
        lexicalAnalyzerContext->semanticValue->string = newContent;
        strncat(lexicalAnalyzerContext->semanticValue->string, &escapedChar, 1);
    }
}

Token EndStringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return STRING_LITERAL;
}

Token UnexpectedCharacterLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return YYUNDEF;
}
