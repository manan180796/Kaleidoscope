#if !defined(Parser_hpp)
    #define Parser_hpp
    #include <AST/AST.hpp>
    #include <Parser/Scanner.hpp>
namespace kaleidoscope {
    class Parser {
        Scanner::Token last_token;
        Scanner scanner;
        static const std::string anonymous_name;
        std::ostream& cerr;
        std::nullptr_t error_logger(const std::string& message);

      public:
        Parser(std::istream& cin, std::ostream& cerr);
        Scanner::Token update_token();
        const Scanner::Token& get_last_token() const;

        std::unique_ptr<Expression> expression();
        std::unique_ptr<NumberExpression> number_expression();
        std::unique_ptr<Expression> identifier_expression();
        std::unique_ptr<Expression> parenthesis_expression();
        std::unique_ptr<Expression> primary_expression();
        std::unique_ptr<Expression> binary_expression();
        std::unique_ptr<Prototype> prototype();
        std::unique_ptr<IfElseExpression> if_else_expression();
        std::unique_ptr<ForExpression> for_expression();
        std::pair<std::unique_ptr<Prototype>, std::unique_ptr<Function>>
        function();
        std::pair<std::unique_ptr<Prototype>, std::unique_ptr<Function>>
        top_expression();
        std::unique_ptr<Prototype> function_declaration();
        static const std::string& get_anonymous_name();
    };
}  // namespace kaleidoscope
#endif