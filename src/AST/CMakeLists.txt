set(AST_PATH ../../include/AST)

add_library(Visitor OBJECT
    ${AST_PATH}/Visitor.hpp
    ${AST_PATH}/Declaration.hpp
)
target_include_directories(Visitor PUBLIC ../../include)

add_library(AbstractSyntaxNode OBJECT
    AbstractSyntaxNode.cpp ${AST_PATH}/AbstractSyntaxNode.hpp
)
target_include_directories(AbstractSyntaxNode PUBLIC ../../include)

add_library(BinaryExpression STATIC
    BinaryExpression.cpp ${AST_PATH}/BinaryExpression.hpp
)
target_include_directories(BinaryExpression PUBLIC ../../include)

add_library(CallExpression STATIC
    CallExpression.cpp ${AST_PATH}/CallExpression.hpp
)
target_include_directories(CallExpression PUBLIC ../../include)

add_library(IfElseExpression STATIC
    IfElseExpression.cpp ${AST_PATH}/IfElseExpression.hpp
)
target_include_directories(IfElseExpression PUBLIC ../../include)



add_library(Expression OBJECT
    Expression.cpp ${AST_PATH}/Expression.hpp
)
target_include_directories(Expression PUBLIC ../../include)

add_library(ForExpression STATIC
    ForExpression.cpp ${AST_PATH}/ForExpression.hpp
)
target_include_directories(ForExpression PUBLIC ../../include)

add_library(Function STATIC
    Function.cpp ${AST_PATH}/Function.hpp
)
target_include_directories(Function PUBLIC ../../include)

add_library(NumberExpression STATIC
    NumberExpression.cpp ${AST_PATH}/NumberExpression.hpp
)
target_include_directories(NumberExpression PUBLIC ../../include)



add_library(Printer STATIC
    Printer.cpp ${AST_PATH}/Printer.hpp
)
target_include_directories(Printer PUBLIC ../../include)

add_library(Prototype STATIC
    Prototype.cpp ${AST_PATH}/Prototype.hpp
)
target_include_directories(Prototype PUBLIC ../../include)


add_library(Variable STATIC
    Variable.cpp ${AST_PATH}/Variable.hpp
)
target_include_directories(Variable PUBLIC ../../include)


add_library(AST OBJECT
    ${AST_PATH}/AST.hpp
)
target_include_directories(Variable PUBLIC ../../include)


target_link_libraries(AST
    BinaryExpression
    CallExpression
    Expression
    ForExpression
    Function
    IfElseExpression
    NumberExpression
    Prototype
    Variable
)


target_link_libraries(AbstractSyntaxNode
    Visitor
)

target_link_libraries(BinaryExpression 
    Expression
)

target_link_libraries(CallExpression 
    Expression
)





target_link_libraries(Expression 
    AbstractSyntaxNode
)
target_link_libraries(ForExpression 
    Expression
    Variable
)

target_link_libraries(Function 
    AbstractSyntaxNode
    Expression
)

target_link_libraries(IfElseExpression 
    Expression
)
target_link_libraries(NumberExpression 
    Expression
)

target_link_libraries(Printer 
    AST
    Visitor
)

target_link_libraries(Prototype 
    AbstractSyntaxNode
)


target_link_libraries(Variable 
    Expression
)
