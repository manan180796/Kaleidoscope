#if !defined(Scanner_cpp)
    #define Scanner_cpp
    #include <Parser/Scanner.hpp>
    #include <cctype>
    #include <iostream>
namespace kaleidoscope {
    Scanner::Scanner(std::istream& cin, std::ostream& cerr)
        : input_stream(cin), cerr(cerr), last_char(' ') {}
    Scanner::Token Scanner::get_token() {
        Token token;
        // Skip any whitespace.
        while (std::isspace(last_char)) last_char = input_stream.get();

        if (std::isalpha(last_char)) {  // identifier: [a-zA-Z][a-zA-Z0-9]*
                                        // std::cout << "hello" << std::endl;
            token.content.name = last_char;
            while (std::isalnum((last_char = input_stream.get())))
                token.content.name += last_char;

            if (token.content.name == "def")
                token.type = Token::_def_;
            else if (token.content.name == "extern")
                token.type = Token::_extern_;
            else if (token.content.name == "if")
                token.type = Token::_if_;
            else if (token.content.name == "else")
                token.type = Token::_else_;
            else if (token.content.name == "then")
                token.type = Token::_then_;
            else if (token.content.name == "for")
                token.type = Token::_for_;
            else if (token.content.name == "do")
                token.type = Token::_do_;
            else
                token.type = Token::_identifier_;

            return token;
        }

        if (std::isdigit(last_char) || last_char == '.') {  // Number: [0-9.]+
            std::string NumStr;
            do {
                NumStr += last_char;
                last_char = input_stream.get();
            } while (std::isdigit(last_char) || last_char == '.');

            token.content.value = std::stod(NumStr);
            token.type = token._number_;
            return token;
        }

        if (last_char == '#') {
            // Comment until end of line.
            do
                last_char = input_stream.get();
            while (!input_stream.eof() && last_char != '\n' &&
                   last_char != '\r');

            if (!input_stream.eof()) return get_token();
        }

        // Check for end of file.  Don't eat the EOF.
        if (input_stream.eof()) {
            token.type = token._eof_;
            return token;
        }

        // Otherwise, just return the character as its ascii value.
        token.content.name = last_char;
        token.type = token._char_;
        last_char = input_stream.get();
        return token;
    }
}  // namespace kaleidoscope
#endif