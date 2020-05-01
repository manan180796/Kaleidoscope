#if !defined(Printer_hpp)
    #define Printer_hpp
    #include <AST/AST.hpp>
    #include <iostream>
    #include <memory>
    #include <vector>
namespace kaleidoscope {
    class Printer : public Visitor {
        std::ostream& cout;
        std::vector<int> header;
        const std::string t, h, e;
        void print_header();
        void new_level(int nodes);
        void finish_level();
        void print_line(const std::string& message);
        void print_line(const char& message);

      public:
        Printer(std::ostream& cout);
        virtual void visit(
            const AbstractSyntaxNode& abstract_syntax_node) override;
        virtual void visit(const BinaryExpression& binary_expression) override;
        virtual void visit(const CallExpression& call_expression) override;
        virtual void visit(const Expression& expression) override;
        virtual void visit(const ForExpression& for_expression) override;
        virtual void visit(const Function& function) override;
        virtual void visit(const IfElseExpression& function) override;
        virtual void visit(const NumberExpression& number_expression) override;
        virtual void visit(const Prototype& prototype) override;
        virtual void visit(const Variable& variable) override;
    };
}  // namespace kaleidoscope
#endif