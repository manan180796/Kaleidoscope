#if !defined(Scanner_hpp)
    #define Scanner_hpp
    #include <fstream>
    #include <string>
namespace kaleidoscope {
    class Scanner {
        std::istream& input_stream;
        int last_char;
        std::ostream& cerr;

      public:
        class Token {
          public:
            enum Type {
                _eof_,
                _def_,
                _extern_,
                _identifier_,
                _number_,
                _char_,
                _if_,
                _else_,
                _then_
            } type;
            struct Content {
                std::string name;
                double value;
                // Content(){};
                // ~Content(){};
            } content;
            // Token();
            // operator Token() const;
            // Token& operator=(const Token& token);
            // Token(const Token& token);
        };
        Scanner(std::istream& cin, std::ostream& cerr);
        Token get_token();
    };
}  // namespace kaleidoscope
#endif