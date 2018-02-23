#ifndef ast_specifier_qualifier_hpp
#define ast_specifier_qualifier_hpp

#include "ast_node.hpp"

// SPECIFIER_STORE_CLASS

// SPECIFIER_TYPE

// SPECIFIER_UNION_OR_STRUCT

// UNION_OR_STRUCT

// LIST_SPEC_QUAL

// SPECIFIER_ENUM

// LIST_ENUMERATOR


class enumerator : public Node {
    //ENUMERATOR : IDENTIFIER 
    //           | IDENTIFIER ASSIGN EXPR_CONST
    private:
        std::string id;
        NodePtr exp;
    protected:
        enumerator(std::string _id)
            : id(_id)
            , exp(NULL)
        {}
        enumerator(std::string _id, NodePtr _arg2)
            : id(_id)
            , exp(_arg2)
        {}
    public:
    
        std::string name = "enumerator";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<id<<" ";
            if(exp != NULL){
                dst<<"= ";
                exp->PrettyPrint();
            }
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class qualifier_type : public Node {
    //QUALIFIER_TYPE : CONST
    //               | VOLATILE
    private:
        std::string val;
    protected:
        qualifier_type(std::string _val)
            : val(_val)
        {}
    public:
    
        std::string name = "qualifier_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<val<<" ";
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class pointer : public Node {
    //POINTER : OP_MUL
    //        | OP_MUL LIST_QUALIFIER_TYPE
    //        | OP_MUL POINTER
    //        | OP_MUL LIST_QUALIFIER_TYPE POINTER
    private:
        NodePtr arg1;
        NodePtr arg2;
    protected:
        pointer()
            : arg1(NULL)
            , arg2(NULL)
        {}
        pointer(NodePtr _arg1)
            : arg1(_arg1)
            , arg2(NULL)
        {}                
        pointer(NodePtr _arg1, NodePtr _arg2)
            : arg1(_arg1)
            , arg2(_arg2)
        {}
    public:
    
        std::string name = "pointer";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<'*';
            if(arg1 != NULL) arg1->PrettyPrint();
            if(arg2 != NULL) arg2->PrettyPrint();
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class list_qualifier_type : public Node {
    //LIST_QUALIFIER_TYPE : QUALIFIER_TYPE
    //                    | LIST_QUALIFIER_TYPE QUALIFIER_TYPE
    private:
        NodePtr qt;
        NodePtr rec;
    protected:
        list_qualifier_type(NodePtr _qt)
            : qt(_qt)
            , rec(NULL)
        {}
        list_qualifier_type(NodePtr _arg1, NodePtr _qt)
            : qt(_qt)
            , rec(_arg1)
        {}
    public:
    
        std::string name = "list_qualifier_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint();
            }
            qt->PrettyPrint();
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class lsit_param_type : public Node {
    //LIST_PARAM_TYPE : LIST_PARAMETER
    //                | LIST_PARAMETER ',' ELLIPSIS  // ELIPSIS = ...
    private:
        NodePtr list;
        bool elip;
    protected:
        lsit_param_type(NodePtr _arg1, bool _arg2)
            : list(_arg1)
            , elip(_arg2)
        {}
    public:
    
        std::string name = "lsit_param_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            list->PrettyPrint();
            if(elip){
                dst", ...";
            }
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class list_parameter : public Node {
    //LIST_PARAMETER : DECLARATION_PARAMETER
    //               | LIST_PARAMETER ',' DECLARATION_PARAMETER
    private:
        NodePtr dp;
        NodePtr rec;
    protected:
        list_parameter(NodePtr _dp)
            : dp(_dp)
            , rec(NULL)
        {}
        list_parameter(NodePtr _arg1, NodePtr _dp)
            : dp(_dp)
            , rec(_arg1)
        {}
    public:
    
        std::string name = "list_parameter";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint();
                dst<<',';
            }
            dp->PrettyPrint();
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class list_identifier : public Node {
    //LIST_IDENTIFIER : IDENTIFIER 
    //                | LIST_IDENTIFIER ',' IDENTIFIER
    private:
        std::string id;
        NodePtr rec;
    protected:
        list_identifier(std::string _id)
            : id(_id)
            , rec(NULL)
        {}
        list_identifier(NodePtr _arg1, std::string _id)
            : id(_id)
            , rec(_arg1)
        {}
    public:
    
        std::string name = "list_identifier";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint();
                dst<<',';
            }
            dst<<id<<" ";
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class name_type : public Node {
    // NAME_TYPE : LIST_SPEC_QUAL
    //           | LIST_SPEC_QUAL DECLARATOR_ABSTRACT
    private:
        NodePtr list;
        NodePtr da;
    protected:
        name_type(NodePtr _arg1)
            : list(_arg1)
            , da(NULL)
        {}
        name_type(NodePtr _arg1, NodePtr _arg2)
            : list(_arg1)
            , da(_arg2)
        {}
    public:
    
        std::string name = "name_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            list->PrettyPrint();
            if(da != NULL) da->PrettyPrint();
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};
#endif
