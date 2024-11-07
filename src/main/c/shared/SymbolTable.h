#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    VARIABLE,
    FUNCTION
} SymbolKind;

typedef struct Symbol {
    char* name;
    SymbolKind kind;
    Type* type;
    struct Symbol* next;
    Parameters* parameters;
} Symbol;

typedef struct {
    Symbol* first;
    int size;
} SymbolTable;

void initializeSymbolTable();

void shutdownSymbolTable();

bool addSymbol(const char* name, SymbolKind kind, Type* type);

Symbol* findSymbol(const char* name);

Type* getSymbolType(const char* name);

bool symbolExists(const char* name);

SymbolTable* getCurrentSymbolTable();

bool addFunctionSymbol(const char* name, Type* returnType, Parameters* parameters);

#endif 