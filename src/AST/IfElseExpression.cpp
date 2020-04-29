#if !defined(IfElseExpression_cpp)
    #define IfElseExpression_cpp
    #include <AST/IfElseExpression.hpp>
    #include <memory>
namespace kaleidoscope {
    IfElseExpression::IfElseExpression(
        std::unique_ptr<Expression> condition_expression,
        std::unique_ptr<Expression> if_expression,
        std::unique_ptr<Expression> else_expression)
        : if_expression(std::move(if_expression)),
          else_expression(std::move(else_expression)),
          condition_expression(std::move(condition_expression)) {}
    const Expression& IfElseExpression::get_if_expression() const {
        return *if_expression;
    }
    const Expression& IfElseExpression::get_else_expression() const {
        return *else_expression;
    }
    const Expression& IfElseExpression::get_condition_expression() const {
        return *condition_expression;
    }
    void IfElseExpression::accept(Visitor& visitor) const {
        visitor.visit(*this);
    }
}  // namespace kaleidoscope
#endif