#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_node.hpp"
#include "ast/ast_program.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/ast_function_definition.hpp"
#include "ast/ast_specifier_qualifier.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_expression.hpp"



extern const Node *parseAST();

#endif
