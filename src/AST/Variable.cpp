#if !defined(Variable_cpp)
    #define Variable_cpp
    #include <AST/Variable.hpp>
namespace kaleidoscope {
    Variable::Variable(std::string name) : name(name) {}
    void Variable::accept(Visitor& visitor) const { visitor.visit(*this); }
    const std::string& Variable::get_name() const { return name; }
}  // namespace kaleidoscope

#endif