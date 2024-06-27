#include "semant.hpp"
#include "common.hpp"
#include <algorithm>

namespace cool {

void Semant::visitProgramStmt(Program* stmt) {

    initialize_constants();

    install_basic_classes();

    construct_ctables(stmt);

    check_inheritance(stmt);

    gather_features(stmt);

    multiple_definition_of_method_checks(stmt);

    // check every class in the program
    for (auto& class_ : stmt->classes) {
        curr_class = class_.get();
        symboltable.enterScope();
        class_->accept(this);
        symboltable.exitScope();
    } 
}

void Semant::visitClassStmt(Class* stmt) {
    // check every feature in each class.
    for (auto& feat: stmt->features) {
        feat->accept(this);
    }    
}

void Semant::visitFeatureExpr(Feature* expr) {
    
    switch (expr->featuretype) {
        case FeatureType::ATTRIBUT:
            check_attribut(expr);
            break;
        case FeatureType::METHOD:
            check_method(expr);
            break;
    }
}

void Semant::visitFormalExpr(Formal* expr) {
    if (symboltable.probe(expr->id.lexeme)) {
        fatal_semant_error(expr->id, expr->id.lexeme + " is a Supplicated name.");
    }
    if (expr->id == self) {
        fatal_semant_error(expr->id, "Can't use keyword 'self'. Preserved");
    }
    if (expr->type_ == SELF_TYPE) {
        fatal_semant_error(expr->type_, "Can't use the keyword 'SELF_TYPE'. Preserved.");
    }
    expr->expr_type = expr->type_;
    symboltable.insert(expr->id.lexeme, expr->expr_type);
}

void Semant::visitAssignExpr(Assign* expr) {
    expr->expr->accept(this);
    Token assign_type = expr->expr->expr_type;
    Token id_type;
    Token *id_type_ptr = symboltable.get(expr->id.lexeme);
    if (id_type_ptr) {
        id_type = *id_type_ptr;
    }
    else {
        Class* target_class = curr_class;
        while (true) {
            Feature* attr = get_feature(target_class, expr->id.lexeme, FeatureType::ATTRIBUT);
            if (attr) {
                id_type_ptr = &id_type;
                *id_type_ptr = attr->expr_type ? attr->expr_type : attr->type_;
                break;       
            }
            Token parent = target_class->superClass;
            if (parent == No_class)
                break;
            target_class = classTable.get(parent.lexeme);
            if (!target_class) 
                fatal_semant_error(expr->id, "Unable to find class `" + parent.lexeme + "`");
        }
    }
    // if still no id_type_ptr also meaning did not find the attribute.
    if (!id_type_ptr) {
        fatal_semant_error(expr->id, "type error in assignement construct");
    }
    if (!conform(assign_type, id_type)) {
        fatal_semant_error(expr->id, "Declared type `" + id_type.lexeme + "` of " + expr->id.lexeme + " does not confom to infered type `" 
            + assign_type.lexeme + "`.");
    }
    expr->expr_type = assign_type;
}

void Semant::visitIfExpr(If* expr) {
    expr->cond->accept(this);
    Token cond_type = expr->cond->expr_type;
    if (cond_type != Bool) {
        fatal_semant_error(cond_type, "predicate inside If branch must have Bool type.");
    }
    expr->thenBranch->accept(this);
    expr->elseBranch->accept(this);
    Token join_type = LCA(expr->thenBranch->expr_type, expr->elseBranch->expr_type);
    expr->expr_type = join_type;
}

void Semant::visitWhileExpr(While* expr) {
    expr->cond->accept(this);
    if (expr->cond->expr_type != Bool) {
        fatal_semant_error(expr->cond->expr_type, "predicate inside while branch must have Bool type.");
    }
    expr->expr->accept(this);
    expr->expr_type = Object;
}

void Semant::visitBinaryExpr(Binary* expr) {

    expr->lhs->accept(this);
    expr->rhs->accept(this);
    switch (expr->op.token_type) {
        case PLUS:
        case MINUS:
        case STAR:
        case SLASH: {
            if (expr->lhs->expr_type != Int || expr->rhs->expr_type != Int) {
                expr->expr_type = Object;
                fatal_semant_error(expr->op, "Binary arithmetic operands must be type Int.");
            }
            expr->expr_type = Int;
            break;
        }
        case LESS:
        case LESS_EQUAL: {
            if (expr->lhs->expr_type != Int || expr->rhs->expr_type != Int) {
                expr->expr_type = Object;
                fatal_semant_error(expr->op, "Binary comparison operands must be type Int.");
            }
            expr->expr_type = Bool;
            break;
        }
        case EQUAL: {
            Token lhs_type = expr->lhs->expr_type;
            Token rhs_type = expr->rhs->expr_type;
            if (lhs_type == Int) {
                if (rhs_type == Int) {
                    expr->expr_type = Bool;
                } else {
                    expr->expr_type = Object;
                    fatal_semant_error(expr->op, "type mismatch in '=' operator.");
                }
            } else if (lhs_type == Bool) {
                if (rhs_type == Bool) {
                    expr->expr_type = Bool;
                } else {
                    expr->expr_type = Object;
                    fatal_semant_error(expr->op, "type mismatch in '=' operator.");
                }
            } else if (lhs_type == Str) {
                if (rhs_type == Str) {
                    expr->expr_type = Bool;
                } else {
                    expr->expr_type = Object;
                    fatal_semant_error(expr->op, "type mismatch in '=' operator.");
                }
            }
            expr->expr_type = Bool;     // !TODO: recheck =. Not sure on the logics.
            break;
        }

    }
}

void Semant::visitUnaryExpr(Unary* expr) {
    expr->expr->accept(this);
    switch (expr->op.token_type) {
        case TILDE: {
            if (expr->expr->expr_type != Int) {
                expr->expr_type = Object;
                fatal_semant_error(expr->op, "Type error in '~' operator.");
            }
            expr->expr_type = Int;
            break;
        }
        case NOT: {
            if (expr->expr->expr_type != Bool) {
                expr->expr_type = Object;
                fatal_semant_error(expr->op, "Type error in 'NOT' operator.");
            }
            expr->expr_type = Bool;
            break;
        }
        case ISVOID: {
            expr->expr_type = Bool;
            break;
        }
    }
}

void Semant::visitVariableExpr(Variable* expr) {
    if (expr->name == self) {
        expr->expr_type = curr_class->name;
        return;
    }

    auto v = symboltable.get(expr->name.lexeme);
    if (v) {
        expr->expr_type = *v;
        return;
    } else {
        auto target_class = curr_class;
        while(true) {
            Feature *attr = get_feature(target_class, expr->name.lexeme, FeatureType::ATTRIBUT);
            if (attr) {
                // in case the feature not been visited yet.
                expr->expr_type = attr->expr_type ? attr->expr_type : attr->type_;
                return;
            }

            Token parent = target_class->superClass;
            if (parent == No_class)
                break;
            target_class = classTable.get(parent.lexeme);
            if (!target_class) 
                fatal_semant_error(expr->name, "Unable to find class `" + parent.lexeme + "`");
        }
    }

    fatal_semant_error(expr->name, "Variable `" + expr->name.lexeme + "` is not defined.");
}
    
void Semant::visitNewExpr(New* expr) {
    if (expr->type_ == SELF_TYPE)
        expr->expr_type = curr_class->name;
    else 
        expr->expr_type = expr->type_;
}

void Semant::visitBlockExpr(Block* expr) {
    Token block_type;
    for (auto& expr_: expr->exprs) {
        expr_->accept(this);
        block_type = expr_->expr_type;
    }
    expr->expr_type = block_type;
}

void Semant::visitGroupingExpr(Grouping* expr) {
    expr->expr->accept(this);
    expr->expr_type = expr->expr->expr_type;
}

void Semant::visitStaticDispatchExpr(StaticDispatch* expr) {
    Feature *feat;
    Class* target_class;

    expr->expr->accept(this);
    if (!conform(expr->expr->expr_type, expr->class_))
        fatal_semant_error(expr->callee_name, "Static dispatch Error: Type infered `" 
        + expr->expr->expr_type.lexeme + "` does not conform to declared type `" + expr->class_.lexeme + "`.");

    target_class = classTable.get(expr->class_.lexeme);
    if (!target_class)
        fatal_semant_error(expr->callee_name, "Static dispatch Error: Unable to find class `" + expr->class_.lexeme + "`");
    while (true) {
        feat = get_feature(target_class, expr->callee_name.lexeme, FeatureType::METHOD);
        if (feat)
            break;
        Token parent = target_class->superClass;
        if (parent == No_class)
            break;
        target_class = classTable.get(parent.lexeme);
        if (!target_class)
            fatal_semant_error(expr->callee_name, "Static dispatch Error: Unable to find class `" + parent.lexeme + "`");
    }

    if (!feat)
        fatal_semant_error(expr->callee_name, "Static dispatch Error: Unable to find class `" + expr->class_.lexeme + "`");

    // We still got the type even if the feature isn't visited yet; hence the ternary.
    Token fun_type = feat->expr_type ? feat->expr_type : feat->type_;
    if (fun_type == SELF_TYPE)
        fun_type = expr->expr->expr_type;

    // check args
    for (size_t i = 0; i < expr->args.size(); i++) {
        expr->args[i]->accept(this);
        if (!conform(expr->args[i]->expr_type, feat->formals[i]->type_)){
            fatal_semant_error(expr->callee_name, "Static Dispatch Error: Type mismatch with args while calling `" + expr->callee_name.lexeme + "`.");
        }
    }
    expr->expr_type = fun_type;

}

void Semant::visitDispatchExpr(Dispatch* expr) {
    Feature *feat;
    Class* target_class;

    expr->expr->accept(this);
    if (expr->expr->expr_type == SELF_TYPE)
        expr->expr->expr_type = curr_class->name;

    target_class = classTable.get(expr->expr->expr_type.lexeme);
    if (!target_class)
        fatal_semant_error(expr->callee_name, "Dynamic Dispatch Error: Unable to find class `" + expr->expr->expr_type.lexeme + "`");
    while (true) {
        feat = get_feature(target_class, expr->callee_name.lexeme, FeatureType::METHOD);
        if (feat)
            break;
        Token parent = target_class->superClass;
        if (parent == No_class)
            break;
        target_class = classTable.get(parent.lexeme);
        if (!target_class)
            fatal_semant_error(expr->callee_name, "Unable to find `" + parent.lexeme + "`");

    }
    if (!feat)
        fatal_semant_error(expr->callee_name, "Dynamic Dispatch Error: Unable to find class `" + expr->expr->expr_type.lexeme + "`");

    // We still got the type even if the feature isn't visited yet; hence the ternary.
    Token fun_type = feat->expr_type ? feat->expr_type : feat->type_;
    if (fun_type == SELF_TYPE)
        fun_type = expr->expr->expr_type;

    // check args
    for (size_t i = 0; i < expr->args.size(); i++) {
        expr->args[i]->accept(this);
        if (!conform(expr->args[i]->expr_type, feat->formals[i]->type_)){
            fatal_semant_error(expr->callee_name, "Dynamic Dispatch Error: Type mismatch with args while calling `" + expr->callee_name.lexeme + "`.");
        }
    }
    expr->expr_type = fun_type;

}

void Semant::visitLiteralExpr(Literal* expr) {
    switch (expr->object.type()) {
        case CoolType::Bool_t:
            expr->expr_type = Bool;
            break;
        case CoolType::Number_t:
            expr->expr_type = Int;
            break;
        case CoolType::String_t:
            expr->expr_type = Str;
            break;
        case CoolType::Void_t:
            expr->expr_type = No_type; // !TODO: to check later.
            break;
    }
}

void Semant::visitLetExpr(Let* expr) {

    symboltable.enterScope();
    for (auto& let: expr->vecAssigns) {
        auto formal = std::get<0>(let).get();
        Expr* let_expr = std::get<1>(let).get(); // Since smart pointer.

        if (formal->id== self) {
            fatal_semant_error(formal->id, "Cannot use self as name.");
        }

        if (let_expr) {
            let_expr->accept(this);
            if (let_expr->expr_type != No_type && !conform(formal->type_, let_expr->expr_type))
                fatal_semant_error(formal->id, "Let Assign Error: the infered `" + let_expr->expr_type.lexeme 
                + "` does not conform to the declared type `" + formal->type_.lexeme + "`.");
            symboltable.insert(formal->id.lexeme, let_expr->expr_type);
        } else {
            symboltable.insert(formal->id.lexeme, formal->type_); // !TODO: doubt on pointer here.
        }
    }

    expr->body->accept(this);
    expr->expr_type = expr->body->expr_type;
    symboltable.exitScope();
}

void Semant::visitCaseExpr(Case* expr) {

    Token join_type = No_type;
    expr->expr->accept(this);
    Token expr0_type = expr->expr->expr_type;
    if (expr0_type == No_type)
        fatal_semant_error(expr0_type, "Case Error: `" + expr0_type.lexeme + "` must be a valid cool type.");
    
    SymbolTable<std::string, Token> casetable; // to track duplicated branches.
    casetable.enterScope();
    for (auto& match: expr->matches) {
        symboltable.enterScope();

        auto formal = std::get<0>(match).get();
        Expr* match_expr = std::get<1>(match).get();

        if (casetable.get(formal->type_.lexeme)) {
            fatal_semant_error(formal->type_, "Case Error: " + formal->type_.lexeme + "` is a duplicated branch.");
        }

        casetable.insert(formal->type_.lexeme, formal->type_);

        formal->accept(this);

        match_expr->accept(this);

        if (join_type == No_type)
            join_type = match_expr->expr_type;
        else 
            join_type = LCA(join_type, match_expr->expr_type);
        
        symboltable.exitScope();

    }
    expr->expr_type = join_type;
}

void Semant::check_attribut(Feature* expr) {
    Token parent;
    Class* target_class;
    Feature* feat;

    if (expr->type_ == SELF_TYPE) {
        expr->expr_type = expr->type_ = curr_class->name;
    }

    if (expr->id.lexeme == "self"){
        fatal_semant_error(expr->id, "Attribute Error: Can't use keyword 'self' as name");
    }

    // ensure there's no attribute override.
    target_class = classTable.get(curr_class->superClass.lexeme);
    if (!target_class)
        fatal_semant_error(expr->id, "Attribut Error: Unable to find class `" + curr_class->superClass.lexeme + "`");
    while (true) {
        feat = get_feature(target_class, expr->id.lexeme, FeatureType::ATTRIBUT);
        if (feat) {
            fatal_semant_error(expr->id, "Attribut Error: `" + expr->id.lexeme + "` is an attribute hence cant be overrided.");
            break;
        } 
        parent = target_class->superClass;
        if (parent == No_class)
            break;
        target_class = classTable.get(parent.lexeme);
        if (!target_class) 
            fatal_semant_error(expr->id, "Attribut Error: unable to find class `" + parent.lexeme + "`");
    }

    if (expr->expr) {
        expr->expr->accept(this);       // check the init.

        Token init_type = expr->expr->expr_type;
        if (init_type != No_type && !conform(init_type, expr->type_)) {
            fatal_semant_error(expr->id, "Attribut Error: Declared type `" 
                + expr->type_.lexeme + "` of `" + expr->id.lexeme + "` does not conform to inferred `" + init_type.lexeme + "`.");
        }
        expr->expr_type = expr->expr->expr_type;
    } else {
        expr->expr_type = expr->type_;
    }
    symboltable.insert(expr->id.lexeme, expr->expr_type);

}

void Semant::check_method(Feature* expr) {
    Feature* feat;
    Class* target_class;
    Token parent;            

    symboltable.enterScope();

    if (!classTable.get(expr->type_.lexeme) && expr->type_ != SELF_TYPE) {
        fatal_semant_error(expr->type_, "Method Error: `" + expr->type_.lexeme + "` is an invalid return type for method `" + expr->id.lexeme + "`.");
    }

    feat = nullptr;
    target_class = classTable.get(curr_class->name.lexeme);
    if (!target_class) 
        fatal_semant_error(expr->id, "Method Error: unable to find class `" + curr_class->name.lexeme + "`");

    while (true) {
        feat = get_feature(target_class, expr->id.lexeme, FeatureType::METHOD);
        if (!feat) 
            break;
        parent = target_class->superClass;
        if (parent == No_class)
            break;
        target_class = classTable.get(parent.lexeme);
        if (!target_class) {
            fatal_semant_error(expr->id, "Method Error: unable to find class `" + parent.lexeme + "`");
        }
    }

    if (feat) {
        if (feat->formals.size() != expr->formals.size()) {
            fatal_semant_error(expr->id, "Method Error: number of arguments of `" + expr->id.lexeme + "` does not match match of the parent method.");
        }

        for (size_t i = 0; i < expr->formals.size(); i++) {
            expr->formals[i]->accept(this);

            if (expr->formals[i]->type_ != feat->formals[i]->type_) {
                fatal_semant_error(expr->id, "Method Error: formal type of `" + expr->id.lexeme + "` must match of the corresponding formal type of its parent method.");
            }
        }
        if (expr->type_ != feat->type_) {
            fatal_semant_error(expr->id, "Method Error: `"+ expr->id.lexeme +"` type must match the one of its parent method.");
        }

    } else {
        for (auto& formal: expr->formals) {
            formal->accept(this);
        }
    } 

    // check the body of the method.
    expr->expr->accept(this);

    // method return type must conform to body expr type.
    if (!conform(expr->expr->expr_type, expr->type_)) {
        fatal_semant_error(expr->id, "Method Error: Body return type of `" + expr->id.lexeme + "` must match the declared returned type.");
    }

    expr->expr_type = expr->type_;
    symboltable.exitScope();
    

}

Feature* Semant::get_feature(Class* stmt, const std::string& name, FeatureType ft) {
    for(auto& feat: stmt->features) {
        if (feat->id.lexeme == name && feat->featuretype == ft)
            return feat.get();
    }
    return nullptr;
}

// !TODO: better error handling. later!
std::ostream& Semant::semant_error() {
    semant_errors++;
    return error_stream;
}

std::ostream& Semant::semant_error(Token& c, const std::string& msg) {
    error_stream << "Error at line : " << c.loc << " " << msg << "\n";
    return semant_error();
}

void Semant::fatal_semant_error(Token& c, const std::string& msg) {
    error_stream << "Fatal error at line : " << c.loc << " " << msg << "\n";
    error_stream << "compilation halted due to semantic errors." << std::endl;
    exit(EXIT_FAILURE);
}


bool Semant::check_parents(Program* stmt) {
    // make sure all classes have parent class.
    bool ret{true};
    Token className, parentClassName;
    for (auto& class_ : stmt->classes) {
        className = class_->name;
        parentClassName = class_->superClass;
        if (!classTable.get(parentClassName.lexeme) && (parentClassName == No_class)) {
            semant_error(class_->name, "Parent class " + parentClassName.lexeme + " of class " + className.lexeme + " is not defined.");
            ret = false;
        }
    }
    return ret;
}

bool Semant::check_DAG(Program* stmt) {
    
    bool ret = true;
    // Build the inheritance_graph

    g.addEdge(IO, Object);
    g.addEdge(Int, Object);
    g.addEdge(Str, Object);
    g.addEdge(Bool, Object);

    for (auto& class_: stmt->classes) {
        g.addEdge(class_->name, class_->superClass);
    }

    if(!g.isDGA()) {
        semant_error();
        error_stream << "Cyclic inheritance detected!";
        ret = false;
    }
    return ret;
}

void Semant::construct_ctables(Program* stmt) {

    for (auto& class_: stmt->classes) {
        Token class_name, parent_name;
        class_name = class_->name;
        curr_class = class_.get();
        std::cout << class_name;
        parent_name = class_->superClass;
        if (class_name == Main) {
            class_main_exist = true;
        }
        if (class_name == SELF_TYPE) {
            semant_error(class_name, "Cannot define a class named SELF_TYPE.");
        }

        if (classTable.get(class_name.lexeme)) {
            semant_error(class_name, class_name.lexeme + " already defined.");
        }

        // We can't inherit from the basic class in Cool
        if (parent_name == Main || parent_name == Int ||
            parent_name == Str  || parent_name == SELF_TYPE) {
                semant_error(class_name, "Class " + class_name.lexeme + "cannot inherits from " + parent_name.lexeme + "\n");

        }

        classTable.insert(class_name.lexeme, class_.get());
    }

    
    //classTable.exitScope();

}

void Semant::gather_features(Program *stmt) {

    earger_features.enterScope();

    for (auto& class_: stmt->classes) {
        for (auto& feat: class_->features) {
            earger_features.insert(feat->id.lexeme, feat->type_);                           
        }
    }

    earger_features.exitScope();
}

void Semant::multiple_definition_of_method_checks(Program *stmt) {

    for (auto& class_: stmt->classes) {
        std::vector<std::string> names{};
        for (auto& feat: class_->features) {
            if (feat->featuretype == FeatureType::METHOD) {
                if (std::find(names.begin(), names.end(), feat->id.lexeme) != names.end()) {
                    semant_error(feat->id, "Method " + feat->id.lexeme + " is multiply defined.");
                } else {
                    names.push_back(feat->id.lexeme);
                }
            }
        }
    }

}



void Semant::check_inheritance(Program* stmt) {
    // check if parents are defined.
    if (!check_parents(stmt)) 
        fatal_semant_error(stmt->classes.at(0)->name, "Failed to check inheritance.");

    // check if the graph does not contains cycle. 
    if (!check_DAG(stmt))
        fatal_semant_error(stmt->classes.at(0)->name, "Failed to check inheritance.");
}

void Semant::install_basic_classes() {

    classTable.enterScope();
    // The tree package uses these globals to annotate the classes built below.
    // curr_lineno  = 0;
    //stringtable().insert({"<basic class>", {TokenType::IDENTIFIER, "<basic_class>"}});
    
    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.
    
    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    // 
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    std::vector<std::unique_ptr<Formal>> abort_formals;
    std::vector<std::unique_ptr<Formal>> typename_formals;
    std::vector<std::unique_ptr<Formal>> copy_formals;
    std::vector<std::unique_ptr<Feature>> feats;
    feats.push_back(std::make_unique<Feature>(cool_abort, std::move(abort_formals), Object, nullptr, FeatureType::METHOD));
    feats.push_back(std::make_unique<Feature>(type_name, std::move(typename_formals), Str, nullptr, FeatureType::METHOD));
    feats.push_back(std::make_unique<Feature>(copy, std::move(copy_formals), SELF_TYPE, nullptr, FeatureType::METHOD));
    set_features_type(feats);

    auto Object_class_ = std::make_unique<Class> (
        Object,
        No_class,
        std::move(feats)
    );
    Object_class = std::move(Object_class_);

    // 
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    std::vector<std::unique_ptr<Formal>> out_string_formals;
    out_string_formals.push_back(std::make_unique<Formal>(arg, Str));
    set_formals_type(out_string_formals);
    std::vector<std::unique_ptr<Formal>> out_int_formals;
    out_int_formals.push_back(std::make_unique<Formal>(arg, Int));
    set_formals_type(out_int_formals);
    std::vector<std::unique_ptr<Formal>> in_string_formals;
    std::vector<std::unique_ptr<Formal>> in_int_formals;

    std::vector<std::unique_ptr<Feature>> io_feats;
    io_feats.push_back(std::make_unique<Feature>(out_string, std::move(out_string_formals), SELF_TYPE, nullptr, FeatureType::METHOD));
    io_feats.push_back(std::make_unique<Feature>(out_int, std::move(out_int_formals), SELF_TYPE, nullptr, FeatureType::METHOD));
    io_feats.push_back(std::make_unique<Feature>(in_string, std::move(in_string_formals), Str, nullptr, FeatureType::METHOD));
    io_feats.push_back(std::make_unique<Feature>(in_int, std::move(in_int_formals), Int, nullptr, FeatureType::METHOD));
    set_features_type(io_feats);


    auto IO_class_ = std::make_unique<Class>(
        IO,
        Object,
        std::move(io_feats)
    ); 
    IO_class = std::move(IO_class_);

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer. 
    //
    std::vector<std::unique_ptr<Formal>> int_attr_formals;
    std::vector<std::unique_ptr<Feature>> int_feats; 
    int_feats.push_back(std::make_unique<Feature>(val, std::move(int_attr_formals), prim_slot, nullptr, FeatureType::ATTRIBUT)); 
    set_features_type(int_feats);

    auto Int_class_ = std::make_unique<Class>(
        Int,
        Object,
        std::move(int_feats)
    );
    Int_class = std::move(Int_class_);

    //
    // Bool also has only the "val" slot.
    //
    std::vector<std::unique_ptr<Formal>> bool_attr_formals;
    std::vector<std::unique_ptr<Feature>> bool_feats;
    bool_feats.push_back(std::make_unique<Feature>(val, std::move(bool_attr_formals), prim_slot, nullptr, FeatureType::ATTRIBUT)); 
    set_features_type(bool_feats);


    auto Bool_class_ = std::make_unique<Class>(
        Bool,
        Object,
        std::move(bool_feats)
    );
    Bool_class = std::move(Bool_class_);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //       

    std::vector<std::unique_ptr<Formal>> val_formals = { };
    std::vector<std::unique_ptr<Formal>> str_field_formals = { };
    std::vector<std::unique_ptr<Formal>> length_formals = { };
    std::vector<std::unique_ptr<Formal>> concat_formals;
    concat_formals.push_back(std::make_unique<Formal>(arg, Str));
    set_formals_type(concat_formals);
    std::vector<std::unique_ptr<Formal>> substr_formals;
    substr_formals.push_back(std::make_unique<Formal>(arg, Int));
    substr_formals.push_back(std::make_unique<Formal>(arg2, Int)); 
    set_formals_type(substr_formals);

    std::vector<std::unique_ptr<Feature>> str_features;
    str_features.push_back(std::make_unique<Feature>(val, std::move(val_formals), Int, nullptr, FeatureType::ATTRIBUT));
    str_features.push_back(std::make_unique<Feature>(str_field, std::move(str_field_formals), prim_slot, nullptr, FeatureType::ATTRIBUT));
    str_features.push_back(std::make_unique<Feature>(length, std::move(length_formals), Int, nullptr, FeatureType::METHOD));
    str_features.push_back(std::make_unique<Feature>(concat, std::move(concat_formals), Str, nullptr, FeatureType::METHOD));
    str_features.push_back(std::make_unique<Feature>(substr, std::move(substr_formals), Str, nullptr, FeatureType::METHOD));
    set_features_type(str_features);

    auto Str_class_ = std::make_unique<Class>(
        Str,
        Object,
        std::move(str_features)
    );
    Str_class = std::move(Str_class_);

    // now add these base classes to the class_table.


    classTable.insert(Object.lexeme, Object_class.get());
    classTable.insert(IO.lexeme, IO_class.get());
    classTable.insert(Int.lexeme, Int_class.get());
    classTable.insert(Bool.lexeme, Bool_class.get());
    classTable.insert(Str.lexeme, Str_class.get());

}

void Semant::set_formals_type(std::vector<std::unique_ptr<Formal>>& formals) {
    for (auto& f: formals) {
        f->expr_type = f->type_;
    }
}

void Semant::set_features_type(std::vector<std::unique_ptr<Feature>>& features) {
for (auto& f: features) {
        f->expr_type = f->type_;
    }
}

// this wrappers is for avoiding curr_class global
// in inheritanceGraph class. [separation of concerns]

bool Semant::conform(Token a, Token b) {
    if (a == SELF_TYPE) {
        a = curr_class->name;
    }
    if (b == SELF_TYPE) {
        b = curr_class->name;
    }
    return g.conform(a, b);
}

Token Semant::LCA(Token a, Token b) {
    if (a == SELF_TYPE) {
        a = curr_class->name;
    }
    if (b == SELF_TYPE) {
        b = curr_class->name;
    }
    return g.lca(a, b);
}


}; // end of namespace