
add_library(CodeGenerator 
    CodeGenerator.cpp ../include/CodeGenerator.hpp
    ../include/AST/Visitor.hpp
    ../include/AST/Declaration.hpp
)
target_include_directories(CodeGenerator PUBLIC ../include)

add_library(Driver 
    Driver.cpp ../include/Driver.hpp
)
target_include_directories(Driver PUBLIC ../include)

add_library(JIT 
    JIT.cpp ../include/JIT.hpp
)
target_include_directories(JIT PUBLIC ../include)




add_subdirectory(Parser)






target_link_libraries(CodeGenerator 
    AST
    Visitor
    JIT
)

target_link_libraries(Driver 
    Parser
    Printer
    CodeGenerator
)


