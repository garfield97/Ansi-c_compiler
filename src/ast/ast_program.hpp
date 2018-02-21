#ifndef ast_program_hpp
#define ast_program_hpp

#include "ast_node.hpp"


class program : public Node {
    private:
        NodePtr next;
        NodePtr declaration;
    protected:
        program(NodePtr _arg1)
            : next(null)
            , declaration(_arg1)
        {}
        program(NodePtr _arg1, NodePtr _arg2)
            : next(_arg1)
            , declaration(_arg2)
        {}
    public:
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(next != null) next->print();
            declaration->print();
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};



#endif
