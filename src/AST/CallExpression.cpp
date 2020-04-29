#if !defined(CallExpression_cpp)
    #define CallExpression_cpp
    #include <AST/CallExpression.hpp>
namespace kaleidoscope {

    CallExpression::CallExpression(
        const std::string& callee,
        std::vector<std::unique_ptr<Expression>>& argument)
        : callee(std::move(callee)), argument(std::move(argument)) {}
    void CallExpression::accept(Visitor& visitor) const {
        visitor.visit(*this);
    }
    const std::string& CallExpression::get_callee() const { return callee; }
    const std::vector<std::unique_ptr<Expression>>&
    CallExpression::get_argument() const {
        return argument;
    }

}  // namespace kaleidoscope

#endif