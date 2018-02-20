#ifndef ast_unit_translation_hpp
#define ast_unit_translation_hpp

#include "ast_node.hpp"


class Unit_Translation
    : public Node
{
private:
    NodePtr arg;
protected:
    Function(ExpressionPtr _arg)
        : arg(_arg)
    {}
public:
 
    virtual void printPY(std::ostream &dst) const override
    {
        
    }

};



#endif
