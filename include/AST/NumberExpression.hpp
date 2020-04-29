#if !defined(NumberExpression_hpp)
    #define NumberExpression_hpp
    #include <AST/Expression.hpp>
namespace kaleidoscope {
    class NumberExpression : public Expression {
      private:
        double value;

      public:
        NumberExpression(double value);

        virtual void accept(Visitor& visitor) const override;
        double get_value() const;
    };
}  // namespace kaleidoscope
#endif