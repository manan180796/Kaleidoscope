#if !defined(Variable_hpp)
    #define Variable_hpp
    #include <AST/Expression.hpp>
    #include <string>
namespace kaleidoscope {
    class Variable : public Expression {
      private:
        std::string name;

      public:
        Variable(std::string name);

        virtual void accept(Visitor& visitor) const override;
        const std::string& get_name() const;
    };
}  // namespace kaleidoscope
#endif