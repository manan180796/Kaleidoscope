#if !defined(BinaryExpression_cpp)
    #define BinaryExpression_cpp
    #include <AST/BinaryExpression.hpp>
namespace kaleidoscope {

    BinaryExpression::BinaryExpression(char op, std::unique_ptr<Expression> lhs,
                                       std::unique_ptr<Expression> rhs)
        : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
    const std::unordered_map<char, int> BinaryExpression::precedence = {
        {'*', 4}, {'-', 2}, {'+', 2}, {'<', 1}};

    bool BinaryExpression::is_operator(std::string candidate) {
        return candidate.length() == 1 &&
               precedence.find(candidate[0]) != precedence.end();
    }
    const Expression& BinaryExpression::get_lhs() const { return *lhs; }
    const Expression& BinaryExpression::get_rhs() const { return *rhs; }
    char BinaryExpression::get_op() const { return op; }
    void BinaryExpression::accept(Visitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace kaleidoscope

#endif