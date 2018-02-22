#include <stdio.h>

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

    // Print AST
    // bin/c_compiler -P [source-file.c] -o [dest-file.txt]
    if( argv[0] == "-P" ){
        ast->PrettyPrint(cout);
    } 

    // Compile to Assembly
    // bin/c_compiler -S [source-file.c] -o [dest-file.s]
    if( argv[0] == "-S" ){
        
    } 

    // Translate to Python
    // bin/c_compiler --translate [source-file.c] -o [dest-file.py]
    if( argv[0] == "--translate" ){
        
    }

    return 0;
}
