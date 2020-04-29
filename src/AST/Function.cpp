#if !defined(Function_cpp)
    #define Function_cpp
    #include <AST/Function.hpp>
namespace kaleidoscope {

    Function::Function(Prototype& prototype,
                       std::unique_ptr<Expression> body)
        : prototype(prototype), body(std::move(body)) {}
    void Function::accept(Visitor& visitor) const { visitor.visit(*this); }
    const Prototype& Function::get_prototype() const { return prototype; }
    const Expression& Function::get_body() const { return *body; }
    const std::string& Function::get_callee() const {
        return prototype.get_callee();
    }
    const std::vector<std::string>& Function::get_arguments() const {
        return prototype.get_argument();
    }

}  // namespace kaleidoscope

#endif