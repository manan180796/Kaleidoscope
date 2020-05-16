#if !defined(Driver_cpp)
    #define Driver_cpp
    #include <Driver.hpp>
namespace kaleidoscope {

    Driver::Driver(std::istream& cin, std::ostream& cout, std::ostream& cerr)
        : parser(cin, cerr), printer(cout), cout(cout), code_generator(cerr) {}
    void Driver::loop() {
        cout << "ready> ";
        parser.update_token();
        while (parser.get_last_token().type != Scanner::Token::_eof_) {
            switch (parser.get_last_token().type) {
                case Scanner::Token::_char_:  // ignore top-level semicolons.
                    parser.update_token();
                    break;
                case Scanner::Token::_def_:
                    function();
                    break;
                case Scanner::Token::_extern_:
                    function_declaration();
                    break;
                default:
                    top_expression();
                    break;
            }
            cout << "ready> ";
            cout.flush();
        }
        code_generator.get_module().print(llvm::outs(), nullptr);
        cout.flush();
    }
    void Driver::function() {
        auto f = parser.function();
        // f.second->accept(printer);
        code_generator.add_definition(std::move(f));
        code_generator.commit_module();
    }
    void Driver::function_declaration() {
        auto f = parser.function_declaration();
        // f->accept(printer);
        f->accept(code_generator);
        code_generator.add_declaration(std::move(f));
        code_generator.commit_module();
    }
    void Driver::top_expression() {
        auto f = parser.top_expression();
        // f.second->accept(printer);
        code_generator.add_definition(std::move(f));
        // llvm::Function* func;
        // code_generator.get_result(func);
        // func->print(llvm::outs());
        auto m = code_generator.commit_module();
        code_generator.execute();
        code_generator.remove_module(m);
    }

}  // namespace kaleidoscope
#endif