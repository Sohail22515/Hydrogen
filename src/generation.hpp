#pragma once


#include <string>
#include <utility>
#include <sstream>

#include "parser.hpp"

class Generator{
    public:
        inline Generator(NodeExit root)
            :m_root(std::move(root)){

            }
        [[nodiscard]]std::string generate() const{
            std::stringstream output;
            output<<"gloabal_start\n_start:\n";
            output<<"    mov rax, 60\n";
            if (m_root.expr.int_lit.value.has_value()) {
                output << "    mov rdi, " << m_root.expr.int_lit.value.value() << "\n";
            } else {
                output << "    mov rdi, 0\n"; // Default or error handling
            }
            output<<"    syscall\n";

            return output.str();
        }

    private:
        const NodeExit m_root;
};