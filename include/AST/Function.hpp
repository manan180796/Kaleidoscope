#if !defined(Function_hpp)
    #define Function_hpp
    #include <AST/AbstractSyntaxNode.hpp>
    #include <AST/Expression.hpp>
    #include <AST/Prototype.hpp>
namespace kaleidoscope {
    class Function : public AbstractSytaxNode {
        Prototype& prototype;
        std::unique_ptr<Expression> body;

      public:
        Function(Prototype& prototype, std::unique_ptr<Expression> body);
        virtual void accept(Visitor& visitor) const override;
        const Prototype& get_prototype() const;
        const Expression& get_body() const;
        const std::string& get_callee() const;
        const std::vector<std::string>& get_arguments() const;
    };
}  // namespace kaleidoscope

#endif