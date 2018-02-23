#ifndef ast_specifier_qualifier_hpp
#define ast_specifier_qualifier_hpp

#include "ast_node.hpp"

class specifier_store_class : public Node {
    //SPECIFIER_STORE_CLASS : TYPEDEF
    //                      | EXTERN
    //                      | STATIC
    //                      | AUTO
    //                      | REGISTER
    private:
        std::string val;
    protected:
        specifier_store_class(std::string _arg1)
            : val(_arg1)
        {}
        
    public:
    
        std::string name = "specifier_store_class";

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

class specifier_type : public Node {
    //SPECIFIER_TYPE : VOID
    //               | CHAR
    //               | SHORT
    //               | INT
    //               | LONG
    //               | FLOAT
    //               | DOUBLE
    //               | SIGNED
    //               | UNSIGNED
    //               | SPECIFIER_UNION_OR_STRUCT
    //               | SPECIFIER_ENUM 
    //               | NAME_TYPE
    private:
        std::string ter;
        NodePtr nonT;
    protected:
        specifier_type(std::string _arg1)
            : ter(_arg1)
            , nonT(NULL)
        {}

        specifier_type(NodePtr _arg1)
            : ter(" ")
            , nonT(_arg1)
        {}
        
    public:
    
        std::string name = "specifier_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(ter != " ") dst<<ter<<" ";
            if(nonT != NULL) nonT->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class specifier_union_or_struct : public Node {
    //SPECIFIER_UNION_OR_STRUCT : UNION_OR_STRUCT IDENTIFIER L_BRACE DECLARATION_LIST_STRUCT R_BRACE
    //                          | UNION_OR_STRUCT L_BRACE DECLAR_LIST_STRUCT R_BRACE
    //                          | UNION_OR_STRUCT IDENTIFIER
    private:
        NodePtr us;
        std::string id;
        NodePtr list;
    protected:
        specifier_union_or_struct(NodePtr _arg1, std::string _arg2)
            : us(_arg1)
            , id(_arg2)
            , list(NULL)
        {}

        specifier_union_or_struct(NodePtr _arg1, NodePtr _arg2)
            : us(_arg1)
            , id( " ")
            , list(_arg2)
        {}

        specifier_union_or_struct(NodePtr _arg1, std::string _arg2, NodePtr _arg3)
            : us(_arg1)
            , id(_arg2)
            , list(_arg3)
        {}
        
    public:
    
        std::string name = "specifier_union_or_struct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            us->PrettyPrint(dst);
            if(id != " ") dst<<id<<" ";
            if(list != NULL){
                dst<<"{ ";
                list->PrettyPrint(dst);
                dst<<" } ";
            }
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class union_or_struct : public Node {
    //UNION_OR_STRUCT : STRUCT
    //                | UNION
    private:
        std::string val;
    protected:
        union_or_struct(std::string _arg1)
            : val(_arg1)
        {}
        
    public:
    
        std::string name = "union_or_struct";

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

class list_spec_qual : public Node {
    //LIST_SPEC_QUAL : SPECIFIER_TYPE LIST_SPEC_QUAL
    //               | SPECIFIER_TYPE 
    //               | QUALIFIER_TYPE LIST_SPEC_QUAL
    //               | QUALIFIER_TYPE
    private:
        NodePtr sq;
        NodePtr rec;
    protected:
        list_spec_qual(NodePtr _arg1)
            : sq(_arg1)
            , rec(NULL)
        {}
        list_spec_qual(NodePtr _arg1, NodePtr _arg2)
            : sq(_arg1)
            , rec(_arg2)
        {}
        
    public:
    
        std::string name = "list_spec_qual";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            sq->PrettyPrint(dst);
            if(rec != NULL) rec->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class specifier_enum : public Node {
    // ENUM - keyword
    //SPECIFIER_ENUM : ENUM L_BRACE LIST_ENUMERATOR R_BRACE
    //               | ENUM IDENTIFIER L_BRACE LIST_ENUMERATOR R_BRACE
    //               | ENUM IDENTIFIER
    private:
        NodePtr list;
        std::string id;
    protected:
        specifier_enum(NodePtr _arg2) // ENUM L_BRACE LIST_ENUMERATOR R_BRACE
            : list(_arg2)
            , id(" ")
        {}
        specifier_enum(std::string _arg2, NodePtr _arg3) // ENUM IDENTIFIER L_BRACE LIST_ENUMERATOR R_BRACE
            : list(_arg3)
            , id(_arg2)
        {}
        specifier_enum(std::string _arg2) // ENUM IDENTIFIER
            : list(NULL)
            , id(_arg2)
        {}
    public:
    
        std::string name = "specifier_enum";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<"enum ";
            if(id != " ") dst<<id<<" ";
            if(list != NULL){
                dst<<" { ";
                list->PrettyPrint(dst);
                dst<<" } ";
            }
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

class list_enumerator : public Node {
    //LIST_ENUMERATOR : ENUMERATOR
    //                | LIST_ENUMERATOR ',' ENUMERATOR
    private:
        NodePtr en;
        NodePtr rec;
    protected:
        list_enumerator(NodePtr _arg1)
            : en(_arg1)
            , rec(NULL)
        {}
        list_enumerator(NodePtr _arg1, NodePtr _arg2)
            : en(_arg2)
            , rec(_arg1)
        {}
    public:
    
        std::string name = "list_enumerator";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<" , ";
            }
            en->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};

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
                exp->PrettyPrint(dst);
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
            if(arg1 != NULL) arg1->PrettyPrint(dst);
            if(arg2 != NULL) arg2->PrettyPrint(dst);
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
                rec->PrettyPrint(dst);
            }
            qt->PrettyPrint(dst);
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
            list->PrettyPrint(dst);
            if(elip){
                dst<<", ...";
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
                rec->PrettyPrint(dst);
                dst<<',';
            }
            dp->PrettyPrint(dst);
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
                rec->PrettyPrint(dst);
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
            list->PrettyPrint(dst);
            if(da != NULL) da->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override
        {

        }

        virtual void renderASM(std::ostream &dst) const override
        {

        }
};
#endif
