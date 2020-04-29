#if !defined(CodeGenerator_cpp)
    #define CodeGenerator_cpp
    #include <llvm/ADT/APFloat.h>
    #include <llvm/ADT/STLExtras.h>
    #include <llvm/IR/BasicBlock.h>
    #include <llvm/IR/Constants.h>
    #include <llvm/IR/DerivedTypes.h>
    #include <llvm/IR/Type.h>
    #include <llvm/IR/Verifier.h>
    #include <llvm/Support/TargetSelect.h>
    #include <llvm/Transforms/InstCombine/InstCombine.h>
    #include <llvm/Transforms/Scalar.h>
    #include <llvm/Transforms/Scalar/GVN.h>
    #include <CodeGenerator.hpp>
    #include <Parser/Parser.hpp>
namespace kaleidoscope {
    // returns Value*
    llvm::orc::VModuleKey CodeGenerator::commit_module() {
        auto key = jit->addModule(std::move(module));
        init_module_fpm();
        return key;
    }
    void CodeGenerator::remove_module(llvm::orc::VModuleKey key) {
        return jit->removeModule(key);
    }
    std::nullptr_t CodeGenerator::error_logger(const std::string& message) {
        cerr << message << std::endl;
        cerr.flush();
        return nullptr;
    }
    void CodeGenerator::add_declaration(
        std::unique_ptr<const Prototype> prototype) {
        prototypes[prototype->get_callee()] = std::move(prototype);
    }
    void CodeGenerator::add_definition(
        std::pair<std::unique_ptr<const Prototype>,
                  std::unique_ptr<const Function>>
            definition) {
        add_declaration(std::move(definition.first));
        definition.second->accept(*this);
    }

    void CodeGenerator::execute() {
        auto ExprSymbol = jit->findSymbol(Parser::get_anonymous_name());
        assert(ExprSymbol && "Function not found");

        // Get the symbol's address and cast it to the right type (takes no
        // arguments, returns a double) so we can call it as a native function.
        double (*FP)() = (double (*)())(intptr_t)ExprSymbol.getAddress().get();
        fprintf(stderr, "Evaluated to %f\n", FP());
        prototypes.erase(Parser::get_anonymous_name());
    }
    llvm::Function* CodeGenerator::get_function_code(
        const std::string& callee) {
        if (auto f = module->getFunction(callee)) return f;
        auto f = prototypes.find(callee);
        if (f != prototypes.end()) {
            llvm::Function* llvm_function;
            f->second->accept(*this);
            get_result(llvm_function);
            return llvm_function;
        }
        return nullptr;
    }
    void CodeGenerator::init_module_fpm() {
        module = std::make_unique<llvm::Module>("Kaleidoscope jit", context);
        function_pass_manager =
            std::make_unique<llvm::legacy::FunctionPassManager>(module.get());
        function_pass_manager->add(llvm::createInstructionCombiningPass());
        function_pass_manager->add(llvm::createReassociatePass());
        function_pass_manager->add(llvm::createGVNPass());
        function_pass_manager->add(llvm::createCFGSimplificationPass());
        function_pass_manager->doInitialization();
    }
    CodeGenerator::CodeGenerator(std::ostream& cerr)
        : context(), builder(context), module(), named_values(), cerr(cerr) {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
        jit = std::make_unique<JIT>();

        init_module_fpm();
    }

    const llvm::Module& CodeGenerator::get_module() const { return *module; }

    void CodeGenerator::visit(const AbstractSyntaxNode& abstract_syntax_node) {}

    void CodeGenerator::get_result(llvm::Value*& value) {
        value = return_value.top();
        return_value.pop();
    }
    void CodeGenerator::get_result(llvm::Function*& function) {
        function = return_function.top();
        return_function.pop();
    }
    void CodeGenerator::return_result(llvm::Value* value) {
        return_value.push(value);
    }
    void CodeGenerator::return_result(llvm::Function* function) {
        return_function.push(function);
    }
    // returns Value*
    void CodeGenerator::visit(const BinaryExpression& binary_expression) {
        binary_expression.get_lhs().accept(*this);
        llvm::Value* lhs_value;
        get_result(lhs_value);
        binary_expression.get_rhs().accept(*this);
        llvm::Value* rhs_value;
        get_result(rhs_value);

        if (!lhs_value || !rhs_value) {
            return_result((llvm::Value*)nullptr);
            return;
        }
        switch (binary_expression.get_op()) {
            case '+':
                return return_result(
                    builder.CreateFAdd(lhs_value, rhs_value, "addtmp"));
                break;
            case '-':
                return return_result(
                    builder.CreateFSub(lhs_value, rhs_value, "subtmp"));
                break;
            case '*':
                return return_result(
                    builder.CreateFMul(lhs_value, rhs_value, "multmp"));
                break;
            case '<':
                lhs_value =
                    builder.CreateFCmpULT(lhs_value, rhs_value, "cmptmp");
                return return_result(builder.CreateUIToFP(
                    lhs_value, llvm::Type::getDoubleTy(context), "booltmp"));
                break;
            default:
                return return_result((llvm::Value*)error_logger(
                    "Invalid binary operator : " + binary_expression.get_op()));
        }
    }
    void CodeGenerator::visit(const IfElseExpression& if_else_expression) {
        llvm::Value* condition_value;
        if_else_expression.get_condition_expression().accept(*this);
        get_result(condition_value);
        if (!condition_value) return return_result((llvm::Value*)nullptr);

        condition_value = builder.CreateFCmpONE(
            condition_value, llvm::ConstantFP::get(context, llvm::APFloat(0.0)),
            "ifcond");
        llvm::Function* function = builder.GetInsertBlock()->getParent();

        // enter this block of condition is true
        llvm::BasicBlock* then_block =
            llvm::BasicBlock::Create(context, "then");

        // enter this block if condition is false
        llvm::BasicBlock* else_block =
            llvm::BasicBlock::Create(context, "else");

        // after execution of either blocks control returns to main flow so flow
        // "merges" from then_block and else_block
        llvm::BasicBlock* merge_block =
            llvm::BasicBlock::Create(context, "merge");

        // create a if else branch
        builder.CreateCondBr(condition_value, then_block, else_block);

        // set the new insertion point to the "end" of yet "empty" then block so
        // essentially it is at the begining of the then block
        builder.SetInsertPoint(then_block);

        llvm::Value* then_value;
        if_else_expression.get_if_expression().accept(*this);
        get_result(then_value);

        builder.CreateBr(merge_block);

        // get the last block built in the then codegen
        then_block = builder.GetInsertBlock();

        llvm::Value* else_value;
        if_else_expression.get_else_expression().accept(*this);
        get_result(else_value);

        // get the last block built in the else codegen
        else_block = builder.GetInsertBlock();

        function->getBasicBlockList().push_back(merge_block);
        builder.SetInsertPoint(merge_block);

        llvm::PHINode* phi_node =
            builder.CreatePHI(llvm::Type::getDoubleTy(context), 2, "ifcond");

        phi_node->addIncoming(then_value, then_block);
        phi_node->addIncoming(else_value, else_block);
        return return_result(phi_node);
    }
    // returns Value*
    void CodeGenerator::visit(const CallExpression& call_expression) {
        llvm::Function* callee_f =
            get_function_code(call_expression.get_callee());
        if (!callee_f) {
            return return_result((llvm::Value*)error_logger(
                "Unknown function : " + call_expression.get_callee()));
        } else if (callee_f->arg_size() !=
                   call_expression.get_argument().size()) {
            return return_result((llvm::Value*)error_logger(
                "Number of arguments do not match for " +
                call_expression.get_callee() + " (required:" +
                std::to_string(callee_f->arg_size()) + ",given:" +
                std::to_string(call_expression.get_argument().size()) + ")"));
        } else {
            std::vector<llvm::Value*> arguments;
            for (const auto& arg : call_expression.get_argument()) {
                arg->accept(*this);
                llvm::Value* arg_v;
                get_result(arg_v);
                if (!arg_v) return return_result((llvm::Value*)nullptr);

                arguments.push_back(arg_v);
            }
            return return_result(
                builder.CreateCall(callee_f, arguments, "calltmp"));
        }
    }

    void CodeGenerator::visit(const Expression& abstract_syntax_node) {}

    // returns Function*
    void CodeGenerator::visit(const Function& function) {
        llvm::Function* llvm_function =
            get_function_code(function.get_callee());

        if (!llvm_function) {
            return return_result((llvm::Function*)nullptr);
        }
        if (!llvm_function->empty()) {
            return return_result((llvm::Function*)error_logger(
                "Function " + function.get_callee() + " cannot be redefined"));
        }

        llvm::BasicBlock* basic_block =
            llvm::BasicBlock::Create(context, "entry", llvm_function);
        builder.SetInsertPoint(basic_block);
        named_values.clear();
        for (auto& argument : llvm_function->args()) {
            named_values[argument.getName()] = &argument;
        }
        function.get_body().accept(*this);
        llvm::Value* ret_code;
        get_result(ret_code);
        if (ret_code) {
            builder.CreateRet(ret_code);
            llvm::verifyFunction(*llvm_function);
            function_pass_manager->run(*llvm_function);
            return return_result(llvm_function);
        }
        llvm_function->removeFromParent();
        return return_result((llvm::Function*)nullptr);
    }

    // returns Value*
    void CodeGenerator::visit(const NumberExpression& number_expression) {
        return return_result(llvm::ConstantFP::get(
            context, llvm::APFloat(number_expression.get_value())));
    }

    // returns Function*
    void CodeGenerator::visit(const Prototype& prototype) {
        std::vector<llvm::Type*> args(prototype.get_argument().size(),
                                      llvm::Type::getDoubleTy(context));
        llvm::FunctionType* function_type = llvm::FunctionType::get(
            llvm::Type::getDoubleTy(context), args, false);
        llvm::Function* f = llvm::Function::Create(
            function_type, llvm::Function::ExternalLinkage,
            prototype.get_callee(), module.get());
        unsigned index = 0;
        for (auto& arg : f->args())
            arg.setName(prototype.get_argument()[index++]);
        return return_result(f);
    }

    // returns Function*
    void CodeGenerator::visit(const Variable& variable) {
        llvm::Value* v = named_values[variable.get_name()];
        if (!v) error_logger("Unknown variable : " + variable.get_name());
        return return_result(v);
    }

}  // namespace kaleidoscope
#endif