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
    void Printer::new_level(int nodes) { header.push_back(nodes); }
    void Printer::finish_level() { header.pop_back(); }
    void Printer::print_line(const std::string& message) {
        print_header();
        cout << message << std::endl;
    }
    void Printer::visit(const AbstractSyntaxNode& abstract_syntax_node) {}
    void Printer::visit(const BinaryExpression& binary_expression) {
        print_line("<BinaryExpression>");
        new_level(3);
        binary_expression.get_lhs().accept(*this);
        print_line(std::to_string(binary_expression.get_op()));
        binary_expression.get_rhs().accept(*this);
        finish_level();
    }
    void Printer::visit(const CallExpression& call_expression) {
        print_line("<CallExpression> callee = " + call_expression.get_callee());
        new_level(1);
        print_line("Argument : " +
                   std::to_string(call_expression.get_argument().size()));
        new_level(call_expression.get_argument().size());
        for (const auto& arg_exp : call_expression.get_argument()) {
            arg_exp->accept(*this);
        }
        finish_level();
        finish_level();
    }
    void Printer::visit(const Expression& expression) {}
    void Printer::visit(const ForExpression& for_expression) {
        print_line("<ForExpression>");
        new_level(2);
        print_line("Control");
        new_level(3);
        print_line("Initialize");
        new_level(1);
        for_expression.get_initialize().accept(*this);
        finish_level();
        print_line("Condition");
        new_level(1);
        for_expression.get_condition().accept(*this);
        finish_level();
        print_line("Update");
        new_level(1);
        for_expression.get_update().accept(*this);
        finish_level();
        finish_level();
        print_line("Body");
        new_level(1);
        for_expression.get_body().accept(*this);
        finish_level();
        finish_level();
    }
    void Printer::visit(const Function& function) {
        print_line("<Function>");
        new_level(2);
        function.get_prototype().accept(*this);
        function.get_body().accept(*this);
        finish_level();
    }
    void Printer::visit(const NumberExpression& number_expression) {
        print_line("<NumberExpression> value = " +
                   std::to_string(number_expression.get_value()));
    }
    void Printer::visit(const IfElseExpression& if_else_expression) {
        print_line("<IfElseExpression>");
        new_level(3);
        print_line("Condition");
        new_level(1);
        if_else_expression.get_condition_expression().accept(*this);
        finish_level();
        print_line("If Expression");
        new_level(1);
        if_else_expression.get_if_expression().accept(*this);
        finish_level();
        print_line("Else Expression");
        new_level(1);
        if_else_expression.get_else_expression().accept(*this);
        finish_level();
        finish_level();
    }
    void Printer::visit(const Prototype& prototype) {
        print_line("<Prototype> callee = " + prototype.get_callee());
        new_level(1);
        print_line("Argument : " +
                   std::to_string(prototype.get_argument().size()));
        new_level(prototype.get_argument().size());
        for (std::string arg_name : prototype.get_argument()) {
            print_line(arg_name);
        }
        finish_level();
        finish_level();
    }
    void Printer::visit(const Variable& variable) {
        print_line("<Variable> name = " + variable.get_name());
    }

}  // namespace kaleidoscope
#endif