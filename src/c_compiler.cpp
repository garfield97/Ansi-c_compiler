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


    // Compile to Assembly
    // bin/c_compiler -S [source-file.c] -o [dest-file.s]
   

    // Translate to Python
    // bin/c_compiler --translate [source-file.c] -o [dest-file.py]

    return 0;
}
