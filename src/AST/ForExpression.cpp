#if !defined(ForExpression_cpp)
    #define ForExpression_cpp
    #include <AST/ForExpression.hpp>
    #include <memory>
namespace kaleidoscope {
    ForExpression::ForExpression(std::unique_ptr<Variable> loop_variable,
                                 std::unique_ptr<Expression> initialize,
                                 std::unique_ptr<Expression> condition,
                                 std::unique_ptr<Expression> update,
                                 std::unique_ptr<Expression> body)
        : loop_variable(std::move(loop_variable)),
          initialize(std::move(initialize)),
          condition(std::move(condition)),
          update(std::move(update)),
          body(std::move(body)) {}
    const Variable& ForExpression::get_loop_variable() const {
        return *loop_variable;
    }
    const std::string& ForExpression::get_loop_variable_name() const {
        return loop_variable->get_name();
    }
    const Expression& ForExpression::get_initialize() const {
        return *initialize;
    }
    const Expression& ForExpression::get_condition() const {
        return *condition;
    }
    const Expression& ForExpression::get_update() const { return *update; }
    const Expression& ForExpression::get_body() const { return *body; }

    void ForExpression::accept(Visitor& visitor) const { visitor.visit(*this); }
}  // namespace kaleidoscope
#endif