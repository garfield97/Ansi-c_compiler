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

// ENUMERATOR

// QUALIFIER_TYPE

// POINTER

// LIST_QUALIFIER_TYPE


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
