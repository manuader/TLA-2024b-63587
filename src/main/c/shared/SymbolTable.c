#include "SymbolTable.h"
#include "Logger.h"
#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

static SymbolTable* currentSymbolTable = NULL;
static Logger* logger = NULL;

void initializeSymbolTable() {
    logger = createLogger("SymbolTable");
    currentSymbolTable = calloc(1, sizeof(SymbolTable));
    currentSymbolTable->first = NULL;
    currentSymbolTable->size = 0;
}

void shutdownSymbolTable() {
    if (currentSymbolTable != NULL) {
        Symbol* current = currentSymbolTable->first;
        while (current != NULL) {
            Symbol* next = current->next;
            free(current->name);
            free(current->type);
            free(current);
            current = next;
        }
        free(currentSymbolTable);
        currentSymbolTable = NULL;
    }
    if (logger != NULL) {
        destroyLogger(logger);
    }
}

bool addSymbol(const char* name, SymbolKind kind, Type* type) {
    if (symbolExists(name)) {
        logError(logger, "Symbol '%s' already exists", name);
        return false;
    }

    Symbol* symbol = calloc(1, sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->kind = kind;
    symbol->type = type;
    symbol->next = currentSymbolTable->first;
    currentSymbolTable->first = symbol;
    currentSymbolTable->size++;

    logDebugging(logger, "Added symbol '%s' of type %d", name, type->type);
    return true;
}

Symbol* findSymbol(const char* name) {
    Symbol* current = currentSymbolTable->first;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Type* getSymbolType(const char* name) {
    Symbol* symbol = findSymbol(name);
    return symbol ? symbol->type : NULL;
}

bool symbolExists(const char* name) {
    return findSymbol(name) != NULL;
}

SymbolTable* getCurrentSymbolTable() {
    return currentSymbolTable;
}

bool addFunctionSymbol(const char* name, Type* returnType, Parameters* parameters) {
    if (symbolExists(name)) {
        logError(logger, "Function '%s' already exists", name);
        return false;
    }

    Symbol* symbol = calloc(1, sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->kind = FUNCTION;
    symbol->type = returnType;
    symbol->parameters = parameters;
    symbol->next = currentSymbolTable->first;
    currentSymbolTable->first = symbol;
    currentSymbolTable->size++;

    logDebugging(logger, "Added function '%s' of type %d", name, returnType->type);
    return true;
} 