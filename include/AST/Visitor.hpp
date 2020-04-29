#if !defined(Visitor_hpp)
    #define Visitor_hpp
    #include <AST/Declaration.hpp>
    #include <memory>
namespace kaleidoscope {
    class Visitor {
      public:
        virtual void visit(const AbstractSyntaxNode& abstract_syntax_node) = 0;
        virtual void visit(const BinaryExpression& binary_expression) = 0;
        virtual void visit(const CallExpression& call_expression) = 0;
        virtual void visit(const Expression& abstract_syntax_node) = 0;
        virtual void visit(const Function& function) = 0;
        virtual void visit(const NumberExpression& number_expression) = 0;
        virtual void visit(const Prototype& prototype) = 0;
        virtual void visit(const Variable& variable) = 0;
        virtual void visit(const IfElseExpression& variable) = 0;
    };
}  // namespace kaleidoscope
#endif