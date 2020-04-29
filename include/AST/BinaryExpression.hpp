#if !defined(BinaryExpression_hpp)
    #define BinaryExpression_hpp
    #include <AST/Expression.hpp>
    #include <memory>
    #include <unordered_map>
namespace kaleidoscope {
    class BinaryExpression : public Expression {
        char op;
        std::unique_ptr<Expression> lhs, rhs;

      public:
        BinaryExpression(char op, std::unique_ptr<Expression> lhs,
                         std::unique_ptr<Expression> rhs);
        const static std::unordered_map<char, int> precedence;

        static bool is_operator(std::string candidate);
        const Expression& get_lhs() const;
        const Expression& get_rhs() const;
        char get_op() const;
        virtual void accept(Visitor& visitor) const override;
    };
}  // namespace kaleidoscope

#endif