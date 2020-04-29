#if !defined(CallExpression_hpp)
    #define CallExpression_hpp
    #include <AST/Expression.hpp>
    #include <memory>
    #include <vector>
namespace kaleidoscope {
    class CallExpression : public Expression {
        std::string callee;
        std::vector<std::unique_ptr<Expression>> argument;

      public:
        CallExpression(const std::string& callee,
                       std::vector<std::unique_ptr<Expression>>& argument);
        virtual void accept(Visitor& visitor) const override;
        const std::string& get_callee() const;
        const std::vector<std::unique_ptr<Expression>>& get_argument() const;
    };
}  // namespace kaleidoscope

#endif