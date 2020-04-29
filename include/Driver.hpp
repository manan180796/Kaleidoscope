#if !defined(Driver_hpp)
    #define Driver_hpp
    #include <AST/Printer.hpp>
    #include <CodeGenerator.hpp>
    #include <Parser/Parser.hpp>
    #include <iostream>
namespace kaleidoscope {
    class Driver {
        Parser parser;
        Printer printer;
        CodeGenerator code_generator;
        std::ostream& cout;

      public:
        Driver(std::istream& cin, std::ostream& cout, std::ostream& cerr);
        void loop();
        void function();
        void function_declaration();
        void top_expression();
    };
}  // namespace kaleidoscope
#endif