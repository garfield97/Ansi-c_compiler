#include <stdio.h>
#include <fstream>

#include "ast.hpp"


int main(int argc, char* argv[])
{
    if(argc < 6){
        fprintf(stderr, "incorrect argument amount\n");
        exit(1);
    }
    if(argv[3] != "-o"){
        fprintf(stderr, "third argument is not '-o'\n");
        exit(1);
    }

    // Build AST (FE)
    // 2nd argument is source C file to compile
    
    const Node* ast = parseAST(argv[2]);
    

    // dest file
    std::ofstream dstStream(argv[4]);

    // Print AST
    // bin/c_compiler -P [source-file.c] -o [dest-file.txt]
    if( argv[1] == "-P" ){
        ast->PrettyPrint(dstStream);
    } 

    // Compile to Assembly
    // bin/c_compiler -S [source-file.c] -o [dest-file.s]
    if( argv[1] == "-S" ){
        ast->renderASM(dstStream);
    } 

    // Translate to Python
    // bin/c_compiler --translate [source-file.c] -o [dest-file.py]
    if( argv[1] == "--translate" ){
        ast->toPY(dstStream);
    }

    // close output file
    dstStream.close();

    return 0;
}
