#ifndef ast_program_hpp
#define ast_program_hpp

#include "ast_node.hpp"

class program : public Node {
    // PROGRAM : DECLARATION_EXTERNAL
    //         | PROGRAM DECLARATION_EXTERNAL
    private:
        NodePtr next;
        NodePtr declaration;
    public:
        program(NodePtr _arg1, NodePtr _arg2)
            : next(_arg1)
            , declaration(_arg2)
        {}
    public:

        std::string name = "program";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            next->PrettyPrint(dst);
            declaration->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};



#endif
