#include <stdio.h>
#include <fstream>

#include "ast.hpp"


int main(int argc, char* argv[])
{
    if(argc < 4){
        fprintf(stderr, "incorrect argument amount\n");
        exit(1);
    }
    if(argv[2] != "-o"){
        fprintf(stderr, "third argument is not '-o'\n");
        exit(1);
    }

    // Build AST (FE)
    // 2nd argument is source C file to compile
    FILE* fp = fopen(argv[1], "r");
    const Node* ast = parseAST(fp);
    fclose(fp);

    // dest file
    FILE* dst = fopen(argv[3], "w+");
    std::fstream dstStream(dst);

    // Print AST
    // bin/c_compiler -P [source-file.c] -o [dest-file.txt]
    if( argv[0] == "-P" ){
        ast->PrettyPrint(dstStream);
    } 

    // Compile to Assembly
    // bin/c_compiler -S [source-file.c] -o [dest-file.s]
    if( argv[0] == "-S" ){
        ast->renderASM(dstStream);
    } 

    // Translate to Python
    // bin/c_compiler --translate [source-file.c] -o [dest-file.py]
    if( argv[0] == "--translate" ){
        ast->toPY(dstStream);
    }

    // close output file
    fclose(dst);

    return 0;
}
