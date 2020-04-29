#if !defined(Parser_cpp)
    #define Parser_cpp
    #include <Parser/Parser.hpp>
    #include <stack>
namespace kaleidoscope {
    const std::string Parser::anonymous_name = "__anon__";
    const std::string& Parser::get_anonymous_name() { return anonymous_name; }
    Parser::Parser(std::istream& cin, std::ostream& cerr)
        : scanner(cin, cerr), cerr(cerr) {}
    const Scanner::Token& Parser::get_last_token() const { return last_token; }
    Scanner::Token Parser::update_token() {
        last_token = scanner.get_token();
        return last_token;
    }
    std::nullptr_t Parser::error_logger(const std::string& message) {
        cerr << message << std::endl;
        cerr.flush();
        return nullptr;
    }
    std::unique_ptr<NumberExpression> Parser::number_expression() {
        auto e = std::make_unique<NumberExpression>(last_token.content.value);
        update_token();
        return e;
    }
    std::unique_ptr<Expression> Parser::identifier_expression() {
        std::string name = last_token.content.name;

        update_token();
        // Variable expression
        if (last_token.content.name != "(")
            return std::make_unique<Variable>(name);

        // call expression
        std::vector<std::unique_ptr<Expression>> arguments;
        // for eating open parenthesis
        update_token();
        while (last_token.content.name != ")") {
            if (auto argument = expression()) {
                arguments.push_back(std::move(argument));
            } else {
                return nullptr;
            }
            if (last_token.content.name != "," &&
                last_token.content.name != ")") {
                return error_logger("Expected ')' or ',' in argument list");
            } else if (last_token.content.name == ",") {
                update_token();
            }
            // update_token();
        }
        update_token();
        return std::make_unique<CallExpression>(name, arguments);
        // return nullptr;
    }
    std::unique_ptr<Expression> Parser::parenthesis_expression() {
        update_token();
        auto exp = expression();
        if (last_token.content.name != ")") {
            return error_logger("Expected )");
        }
        update_token();
        return exp;
        // return nullptr;
    }
    std::unique_ptr<Expression> Parser::primary_expression() {
        switch (last_token.type) {
            case Scanner::Token::_identifier_:
                return identifier_expression();
            case Scanner::Token::_number_:
                return number_expression();
            case Scanner::Token::_if_:
                return if_else_expression();
            case Scanner::Token::_char_:
                if (last_token.content.name == "(") {
                    return parenthesis_expression();
                }

            default:
                return error_logger(
                    "unknown token when expecting an expression");
        }
        return nullptr;
    }
    std::unique_ptr<Expression> Parser::binary_expression() {
        std::stack<std::unique_ptr<Expression>> operands;
        std::stack<char> operators;
        while (true) {
            auto exp = primary_expression();
            if (exp)
                operands.push(std::move(exp));
            else {
                return nullptr;
            }
            int pres = 0;
            char op = '\0';
            if (BinaryExpression::is_operator(last_token.content.name)) {
                op = last_token.content.name[0];
                pres = BinaryExpression::precedence.at(op);
            } else {
                pres = 0;
            }
            while (!operators.empty() &&
                   BinaryExpression::precedence.at(operators.top()) >= pres) {
                auto rhs = std::move(operands.top());
                operands.pop();
                auto lhs = std::move(operands.top());
                operands.pop();
                operands.push(std::make_unique<BinaryExpression>(
                    operators.top(), std::move(lhs), std::move(rhs)));
                operators.pop();
            }
            if (pres == 0) break;
            update_token();
            operators.push(op);
        }
        return std::move(operands.top());
        return nullptr;
    }
    std::unique_ptr<Expression> Parser::expression() {
        return binary_expression();
    }
    std::unique_ptr<IfElseExpression> Parser::if_else_expression() {
        // read if condition
        if (last_token.type != Scanner::Token::_if_) {
            return error_logger("expected \"if\" keyword");
        }
        update_token();
        auto condition = expression();
        if (!condition) return nullptr;

        // read if expression
        if (last_token.type != Scanner::Token::_then_) {
            return error_logger("expected \"then\" keyword");
        }
        update_token();
        auto if_exp = expression();
        if (!if_exp) return nullptr;

        // read else statement if it exists;
        if (last_token.type != Scanner::Token::_else_) {
            return error_logger("expected \"else\" keyword");
        }
        update_token();
        auto else_exp = expression();
        if (!else_exp) return nullptr;
        return std::make_unique<IfElseExpression>(
            std::move(condition), std::move(if_exp), std::move(else_exp));
    }
    std::unique_ptr<Prototype> Parser::prototype() {
        if (last_token.type != last_token._identifier_) {
            return error_logger("Expected function name in prototype");
        }
        std::string name = last_token.content.name;
        update_token();
        if (last_token.content.name != "(") {
            return error_logger("Expected '(' in prototype");
        }
        std::vector<std::string> arguments;
        do {
            update_token();
            if (last_token.type != last_token._identifier_) break;
            arguments.push_back(last_token.content.name);
        } while (true);
        if (last_token.content.name != ")") {
            return error_logger("Expected ')' in prototype");
        }
        update_token();
        return std::make_unique<Prototype>(name, std::move(arguments));
    }
    std::pair<std::unique_ptr<Prototype>, std::unique_ptr<Function>>
    Parser::function() {
        update_token();
        auto proto = prototype();
        if (!proto) return std::make_pair(nullptr, nullptr);
        auto body = expression();
        if (!body) return std::make_pair(nullptr, nullptr);
        return std::make_pair(std::move(proto), std::make_unique<Function>(
                                                    *proto, std::move(body)));
    }
    std::unique_ptr<Prototype> Parser::function_declaration() {
        update_token();
        return prototype();
    }
    std::pair<std::unique_ptr<Prototype>, std::unique_ptr<Function>>
    Parser::top_expression() {
        if (auto exp = expression()) {
            // Make an anonymous proto.
            auto proto = std::make_unique<Prototype>(
                get_anonymous_name(), std::vector<std::string>());
            return std::make_pair(
                std::move(proto),
                std::make_unique<Function>(*proto, std::move(exp)));
        }
        return std::make_pair(nullptr, nullptr);
    }


}  // namespace kaleidoscope
#endif