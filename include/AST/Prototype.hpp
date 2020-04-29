#if !defined(Prototype_hpp)
    #define Prototype_hpp
    #include <AST/AbstractSyntaxNode.hpp>
    #include <memory>
    #include <vector>
namespace kaleidoscope {
    class Prototype : public AbstractSytaxNode {
        std::string callee;
        std::vector<std::string> argument_name;

      public:
        Prototype(const std::string& callee,
                  std::vector<std::string> argument_name);

        virtual void accept(Visitor& visitor) const override;
        const std::string& get_callee() const;
        const std::vector<std::string>& get_argument() const;
    };
}  // namespace kaleidoscope

#endif