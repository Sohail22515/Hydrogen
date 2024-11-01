#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <cstdlib>

#include "parser.hpp"
#include "tokenization.hpp"
#include "generation.hpp"


// std::vector<Token> tokenize(const std::string& str){
    
// }

// std::string tokens_to_asm(const std::vector<Token>& tokens){
//     std::stringstream output;
//     output<<"global _start\n_start:\n";
//     for(int i=0;i<tokens.size();i++){
//         const Token& token=tokens.at(i);
//         if(token.type==TokenType::_return){
//             if(i+1<tokens.size() && tokens.at(i+1).type==TokenType::int_lit){
//                 if(i+2<tokens.size() && tokens.at(i+2).type == TokenType::semi){
//                     output<<"    mov rax, 60\n";
//                     output<<"    mov rdi, "<<tokens.at(i+1).value.value()<<"\n";
//                     output<<"    syscall\n";
//                 }
//             }
//         }
//     }
//     return output.str();
// }

int main(int argc, char* argv[]){
    if(argc!=2){
        std::cerr<<"incorrect usagae. Correct usage is..."<<std::endl;
        std::cerr<<"hydro <input.hy>"<<std::endl;
        return EXIT_FAILURE;
    
    }
    std::string contents;
    
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1],std::ios::in);
        contents_stream<<input.rdbuf();
        contents=contents_stream.str();
    }

    // std::cout<<contents<<std::endl;
    
    // if (!input.is_open()) {
    //     std::cerr << "Error: Could not open the file " << argv[1] << std::endl;
    //     return 1;
    // }
    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens=tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<NodeExit>tree=parser.parse();

    if(!tree.has_value()){
        std::cerr<<"No exit statement found"<<std::endl;
        exit(EXIT_FAILURE);
    }
    Generator generator(tree.value());

    {
        std::fstream file("../out.asm", std::ios::out);
        file<<generator.generate();
    }

    // system("nasm -f elf64 -o out.o ../out.asm");
    // system("ld -o out out.o -lSystem -platform_version macos 10.14 11.0");
    system("nasm -f macho64 -o out.o ../out.asm");

    // system("ld -o out out.o -platform_version macos 10.14 11.0");
    system("ld -o out out.o");




    
    return EXIT_SUCCESS;
}




// cd ..  
// cmake --build build/
// cd build
// ./hydro ../test.hy

//echo $?


// otool -l out.o    # List sections and load commands
// otool -tv out.o   # Disassemble the file to view assembly instructions

// hexdump -C out.o