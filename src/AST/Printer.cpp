#if !defined(Printer_cpp)
    #define Printer_cpp
    #include <AST/Printer.hpp>
    #include <memory>
namespace kaleidoscope {
    Printer::Printer(std::ostream& cout) : cout(cout) {}
    void Printer::print_header() {
        for (int i = 0; i < int(header.size()) - 1; ++i) {
            if (header[i] > 0)
                cout << "│   ";
            else
                cout << "    ";
        }
        if (!header.empty()) {
            if (header.back() == 1)
                cout << "└── ";
            else
                cout << "├── ";
            header.back()--;
        }
    }
    void Printer::visit(const AbstractSyntaxNode& abstract_syntax_node) {}
    void Printer::visit(const BinaryExpression& binary_expression) {
        print_header();
        cout << "<BinaryExpression>" << std::endl;
        header.push_back(3);
        binary_expression.get_lhs().accept(*this);
        print_header();
        cout << binary_expression.get_op() << std::endl;
        binary_expression.get_rhs().accept(*this);
        header.pop_back();
    }
    void Printer::visit(const CallExpression& call_expression) {
        print_header();
        cout << "<CallExpression> callee = " << call_expression.get_callee()
             << std::endl;
        header.push_back(1);
        print_header();
        cout << "Argument : " << call_expression.get_argument().size()
             << std::endl;
        header.push_back(call_expression.get_argument().size());
        for (const auto& arg_exp : call_expression.get_argument()) {
            arg_exp->accept(*this);
        }
        header.pop_back();
        header.pop_back();
    }
    void Printer::visit(const Expression& abstract_syntax_node) {}
    void Printer::visit(const Function& function) {
        print_header();
        cout << "<Function>" << std::endl;
        header.push_back(2);
        function.get_prototype().accept(*this);
        function.get_body().accept(*this);
        header.pop_back();
    }
    void Printer::visit(const NumberExpression& number_expression) {
        print_header();
        cout << "<NumberExpression> value = " << number_expression.get_value()
             << std::endl;
    }
    void Printer::visit(const IfElseExpression& if_else_expression) {
        print_header();
        cout << "<IfElseExpression>" << std::endl;
        header.push_back(3);
        print_header();
        cout << "Condition" << std::endl;
        header.push_back(1);
        if_else_expression.get_condition_expression().accept(*this);
        header.pop_back();
        print_header();
        cout << "If Expression" << std::endl;
        header.push_back(1);
        if_else_expression.get_if_expression().accept(*this);
        header.pop_back();
        print_header();
        cout << "Else Expression" << std::endl;
        header.push_back(1);
        if_else_expression.get_else_expression().accept(*this);
        header.pop_back();
        header.pop_back();
    }
    void Printer::visit(const Prototype& prototype) {
        print_header();
        cout << "<Prototype> callee = " << prototype.get_callee() << std::endl;
        header.push_back(1);
        print_header();
        cout << "Argument : " << prototype.get_argument().size() << std::endl;
        header.push_back(prototype.get_argument().size());
        for (std::string arg_name : prototype.get_argument()) {
            print_header();
            cout << arg_name << std::endl;
        }
        header.pop_back();
        header.pop_back();
    }
    void Printer::visit(const Variable& variable) {
        print_header();
        cout << "<Variable> name = " << variable.get_name() << std::endl;
    }

}  // namespace kaleidoscope
#endif