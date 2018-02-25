#include <stdio.h>
#include <fstream>

#include "ast.hpp"


int main(int argc, char* argv[])
{
    if(argc != 5){
        fprintf(stderr, "incorrect argument amount\n");
        exit(1);
    }
    std::string reqO = argv[3];
    if(reqO!= "-o"){
        fprintf(stderr, "third argument is not '-o'\n");
        exit(1);
    }

    // Build AST (FE)
    // 2nd argument is source C file to compile
    
    const Node* ast = parseAST(argv[2]);

    int gi = -0x345;
    

    // dest file
    std::ofstream dstStream(argv[4]);

    std::string mode = argv[1];
    
    // Print AST
    // bin/c_compiler -P [source-file.c] -o [dest-file.txt]
    if( mode == "-P" ){
        ast->PrettyPrint(dstStream);
    } 

    // Compile to Assembly
    // bin/c_compiler -S [source-file.c] -o [dest-file.s]
    if( mode == "-S" ){
        CompileContext CC;
        ast->compile(dstStream, CC);
    } 

    // Translate to Python
    // bin/c_compiler --translate [source-file.c] -o [dest-file.py]
    if( mode == "--translate" ){
        TranslateContext TC;
        ast->translate(dstStream, TC);
    }

    // close output file
    dstStream.close();

    return 0;
}
