set(Parser_INCLUDE "../../include/Parser")


add_library(Parser 
    Parser.cpp ${Parser_INCLUDE}/Parser.hpp
)
target_include_directories(Parser PUBLIC ../../include)

add_library(Scanner 
    Scanner.cpp ${Parser_INCLUDE}/Scanner.hpp
)
target_include_directories(Scanner PUBLIC ../../include)




add_subdirectory(../AST AST)





target_link_libraries(Parser 
    AST
    Scanner
)


target_link_libraries(Scanner 
)

