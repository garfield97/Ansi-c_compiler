#include "ast.hpp"

int main(int argc, char[] argv)
{
    const Unit_Translation *ast=parseAST();

    ast->print(std::cout);
    std::cout<<std::endl;

    return 0;
}
