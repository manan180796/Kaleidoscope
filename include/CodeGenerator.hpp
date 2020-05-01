#if !defined(CodeGenerator_hpp)
    #define CodeGenerator_hpp
    #include <llvm/IR/Function.h>
    #include <llvm/IR/IRBuilder.h>
    #include <llvm/IR/LLVMContext.h>
    #include <llvm/IR/LegacyPassManager.h>
    #include <llvm/IR/Module.h>
    #include <AST/AST.hpp>
    #include <AST/Visitor.hpp>
    #include <JIT.hpp>
    #include <iostream>
    #include <map>
    #include <memory>
    #include <stack>
namespace kaleidoscope {
    class CodeGenerator : public Visitor {
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;
        std::unique_ptr<llvm::legacy::FunctionPassManager>
            function_pass_manager;
        std::map<std::string, llvm::Value*> named_values;
        std::ostream& cerr;
        std::stack<llvm::Value*> return_value;
        std::stack<llvm::Function*> return_function;
        std::nullptr_t error_logger(const std::string& message);
        std::unique_ptr<JIT> jit;
        std::map<std::string, std::unique_ptr<const Prototype>> prototypes;
        llvm::Function* get_function_code(const std::string& callee);
        void init_module_fpm();

      public:
        CodeGenerator(std::ostream& cerr);
        virtual void visit(
            const AbstractSyntaxNode& abstract_syntax_node) override;
        virtual void visit(const BinaryExpression& binary_expression) override;
        virtual void visit(const CallExpression& call_expression) override;
        virtual void visit(const Expression& expression) override;
        virtual void visit(const ForExpression& for_expression) override;
        virtual void visit(const Function& function) override;
        virtual void visit(const IfElseExpression& if_else_expression) override;
        virtual void visit(const NumberExpression& number_expression) override;
        virtual void visit(const Prototype& prototype) override;
        virtual void visit(const Variable& variable) override;
        const llvm::Module& get_module() const;
        llvm::orc::VModuleKey commit_module();
        void add_declaration(std::unique_ptr<const Prototype> prototype);
        void add_definition(std::pair<std::unique_ptr<const Prototype>,
                                      std::unique_ptr<const Function>>
                                definition);
        void execute();
        void remove_module(llvm::orc::VModuleKey key);
        void get_result(llvm::Value*& value);
        void get_result(llvm::Function*& function);
        void return_result(llvm::Value* value);
        void return_result(llvm::Function* function);
    };
}  // namespace kaleidoscope
#endif