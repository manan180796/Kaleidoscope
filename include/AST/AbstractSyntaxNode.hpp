#if !defined(AbstractSytaxNode_hpp)
    #define AbstractSytaxNode_hpp
    #include <AST/Visitor.hpp>
namespace kaleidoscope {
    class AbstractSytaxNode {
      protected:
        AbstractSytaxNode() {}

      public:
        virtual void accept(Visitor& visitor) const = 0;
    };
}  // namespace kaleidoscope
#endif