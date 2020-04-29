#if !defined(IfElseExpression_hpp)
    #define IfElseExpression_hpp
    #include <AST/Expression.hpp>
    #include <memory>
namespace kaleidoscope {
    class IfElseExpression : public Expression {
        std::unique_ptr<Expression> if_expression;
        std::unique_ptr<Expression> else_expression;
        std::unique_ptr<Expression> condition_expression;

      public:
        IfElseExpression(std::unique_ptr<Expression> condition_expression,
                         std::unique_ptr<Expression> if_expression,
                         std::unique_ptr<Expression> else_expression = nullptr);
        const Expression& get_if_expression() const;
        const Expression& get_else_expression() const;
        const Expression& get_condition_expression() const;
        virtual void accept(Visitor& visitor) const override;
    };
}  // namespace kaleidoscope
#endif