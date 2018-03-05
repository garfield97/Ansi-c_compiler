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
    
    freopen("tmp/stderr.txt", "w", stderr);
    
    const Node* ast = parseAST(argv[2]);

    fclose(stderr);

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
        
        dstStream<<"    .text"<<'\n';
        ast->compile(dstStream, CC);
    } 


    // Translate to Python
    // bin/c_compiler --translate [source-file.c] -o [dest-file.py]
    if( mode == "--translate" ){
        TranslateContext TC;
        TC.indent = 0;
        TC.function_dec = false;
        TC.main_exists = false;
        ast->translate(dstStream, TC);

        if(TC.main_exists){
            dstStream<<std::endl;
            dstStream<<"# Boilerplate"<<std::endl;
            dstStream<<"if __name__ == \"__main__\":"<<std::endl;
            dstStream<<"\timport sys"<<std::endl;
            dstStream<<"\tret=main()"<<std::endl;
            dstStream<<"\tsys.exit(ret)"<<std::endl;
        }
    }

    // close output file
    dstStream.close();

    return 0;
}
