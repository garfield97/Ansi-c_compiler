#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_Node.hpp"

#include "ast/ast_unit_translation.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/ast_function_definition.hpp"
#include "ast/ast_specifier_qualifier.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_expression.hpp"
#include "ast/ast_constant.hpp"



extern const Node *parseAST();

#endif
