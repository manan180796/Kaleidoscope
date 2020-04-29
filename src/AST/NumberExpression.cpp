#if !defined(NumberExpression_cpp)
    #define NumberExpression_cpp
    #include <AST/NumberExpression.hpp>
namespace kaleidoscope {
    NumberExpression::NumberExpression(double value) : value(value) {}
    void NumberExpression::accept(Visitor& visitor) const {
        visitor.visit(*this);
    }
    double NumberExpression::get_value() const { return value; }
}  // namespace kaleidoscope

#endif