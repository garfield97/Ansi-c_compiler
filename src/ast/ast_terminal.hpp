#ifndef ast_terminal_hpp
#define ast_terminal_hpp

#include "ast_node.hpp"


class opr_assignment: public Node {
    private:
        std::string opr;
    protected:
        assign(std::string _opr)
            : opr(_opr)
        {}
        
    public:

        std::string getOpr(){
            return opr;
        }
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<opr<<' ';
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};


class opr_unary: public Node {
    private:
        std::string opr;
    protected:
        assign(std::string _opr)
            : opr(_opr)
        {}
        
    public:

        std::string getOpr(){
            return opr;
        }
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<opr<<' ';
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};



#endif
