#if !defined(Prototype_cpp)
    #define Prototype_cpp
    #include <AST/Prototype.hpp>
namespace kaleidoscope {

    Prototype::Prototype(const std::string& callee,
                         std::vector<std::string> argument_name)
        : callee(std::move(callee)), argument_name(std::move(argument_name)) {}
    void Prototype::accept(Visitor& visitor) const { visitor.visit(*this); }
    const std::string& Prototype::get_callee() const { return callee; }
    const std::vector<std::string>& Prototype::get_argument() const {
        return argument_name;
    };

}  // namespace kaleidoscope

#endif