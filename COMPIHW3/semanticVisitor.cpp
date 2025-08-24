#include <unordered_set>
#include <vector>
#include <string>
#include "visitor.hpp"
#include "nodes.hpp"
#include "semanticVisitor.hpp"
#include "output.hpp"

semanticVisitor::semanticVisitor() : sCount(0)
{
     enterScope("global",0);
}

void semanticVisitor::enterScope(string type,int nextOffset)
{
scopes.emplace_back(type, nextOffset);
    
    if (type == "global") {
        addBuiltInFunction("print", ast::BuiltInType::STRING);
        addBuiltInFunction("printi", ast::BuiltInType::INT);
    } else {
        printer.beginScope();
    }
}
void semanticVisitor::addBuiltInFunction(const std::string &name, ast::BuiltInType paramType)
{
    std::string paramTypeStr = toLowerString(paramType);

    symbol funcSymbol(name, paramTypeStr, scopes.back().nextVariableOffset, "void", true);
    scopes.back().symbolTable.addSymbol(funcSymbol);
     int left = 0;
    int right = 0;
    while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    auto funcDeclPtr = std::make_shared<ast::FuncDecl>(
        std::make_shared<ast::ID>(name.c_str()),
        std::make_shared<ast::PrimitiveType>(ast::BuiltInType::VOID),
        std::make_shared<ast::Formals>(
            std::make_shared<ast::Formal>(
                std::make_shared<ast::ID>((name + "_param").c_str()), 
                std::make_shared<ast::PrimitiveType>(paramType)
            )
        ),
        std::make_shared<ast::Statements>()
    );

    funcDecl.push_back(*funcDeclPtr);
}

bool semanticVisitor::isNumber(string type)
{
   if(type!="byte"&&type!="int") {
        return false;
    }
    return true;
}

void semanticVisitor::exitScope()
{
    if(sCount!=1)
    {
        return;
    }
     if (scopes.back().type != "global") {
            printer.endScope();
        }
        int count = scopes.back().numOfVariables;
        scopes.pop_back();
}

void semanticVisitor::emptyScope()
{
    int x=0;
 return;
}

std::string semanticVisitor::getTypeAsString(std::shared_ptr<ast::Type> typeNode) {
    if (auto prim = std::dynamic_pointer_cast<ast::PrimitiveType>(typeNode)) {
        return toLowerString(prim->type);
    }
    if (auto arr = std::dynamic_pointer_cast<ast::ArrayType>(typeNode)) {
        return toLowerString(arr->type) + "[]"; // Optional: add [] to indicate array
    }
    return "unknown";
}

std::string semanticVisitor::getTypeAsUpperString(std::shared_ptr<ast::Type> typeNode) {
    if (auto prim = std::dynamic_pointer_cast<ast::PrimitiveType>(typeNode)) {
        return toUpperString(prim->type);  // Your existing toUpperString(BuiltInType) function
    }
    if (auto arr = std::dynamic_pointer_cast<ast::ArrayType>(typeNode)) {
        return toUpperString(arr->type) + "[]";  // Optional: indicate it's an array
    }
    return "UNKNOWN";
}

string semanticVisitor::toUpperString(ast::BuiltInType type)
{
 if (type == ast::BuiltInType::INT) return "INT";
    if (type == ast::BuiltInType::BOOL) return "BOOL";
    if (type == ast::BuiltInType::BYTE) return "BYTE";
        int left = 0;
    int right = 0;
    while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    if (type == ast::BuiltInType::VOID) return "VOID";
    if (type == ast::BuiltInType::STRING) return "STRING";
    return "UNKNOWN";
}

string semanticVisitor::toLowerString(ast::BuiltInType type)
{
     if (type == ast::BuiltInType::INT) return "int";
    if (type == ast::BuiltInType::BOOL) return "bool";
    if (type == ast::BuiltInType::BYTE) return "byte";
        int left = 0;
    int right = 0;
    while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    if (type == ast::BuiltInType::VOID) return "void";
    if (type == ast::BuiltInType::STRING) return "string";
    return "unknown";
}

void semanticVisitor::visit(ast::Num &node)
{
   node.typeM = toLowerString(ast::BuiltInType::INT);
}
void semanticVisitor::visit(ast::NumB &node)
{
    if(node.value <= 255){
            node.typeM = toLowerString(ast::BuiltInType::BYTE);
        } else {
            output::errorByteTooLarge(node.line,node.value);
            exit(0);
        }
}
void semanticVisitor::visit(ast::Bool &node)
{
    node.typeM = toLowerString(ast::BuiltInType::BOOL);
}
void semanticVisitor::visit(ast::String &node)
{
    node.typeM = toLowerString(ast::BuiltInType::STRING);
}
void semanticVisitor::visit(ast::ID &node)
{
    if (sCount != 1) {
        return;
    }
    int y=0;
    y++;
    int scopeIndex = -1;
    for (int i = 0; i < (int)scopes.size(); ++i) {
        if (scopes[i].variableExists(node.value)) {
            scopeIndex = i;
            break;
        }
    }
        int left = 0;
    int right = 0;
    while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    for (int i = 0; i < (int)funcDecl.size(); ++i) {
        if (funcDecl[i].id->value == node.value) {
            output::errorDefAsFunc(node.line, node.value);
        }
    }
    int z=0;
    z++;
     if (scopeIndex == -1) {
        output::errorUndef(node.line, node.value);
        exit(0);
    }
    node.typeM = scopes[scopeIndex].symbolTable.getSymbolTypeByName(node.value);
}
void semanticVisitor::visit(ast::BinOp &node)
{
     if (sCount != 1) {
        return;
    }
    ast::Exp* leftPtr = node.left.get();
    ast::Exp* rightPtr = node.right.get();

    leftPtr->accept(*this);
    rightPtr->accept(*this);
    std::string leftType = leftPtr->typeM;
    std::string rightType = rightPtr->typeM;
    bool leftIsString = (leftType == "string");
    bool rightIsString = (rightType == "string");

    if (leftIsString || rightIsString) {
        output::errorMismatch(node.line);
        exit(0);
    }
    bool isInt=false;
    bool isSame=false;
    if((leftType == "int" && rightType == "byte") || (leftType == "byte" && rightType == "int"))
    {
        node.typeM="int";
        isInt=true;
    }
    if(leftType==rightType){
        node.typeM = leftType;
        isSame=true;
    }
    if(!isInt && !isSame) {
        output::errorMismatch(node.line);
        exit(0);
    }

}
void semanticVisitor::visit(ast::RelOp &node)
{
    if (sCount != 1) {
        return;
    }
    ast::Exp* leftPtr = node.left.get();
    ast::Exp* rightPtr = node.right.get();

    leftPtr->accept(*this);
    rightPtr->accept(*this);
    std::string leftType = leftPtr->typeM;
    std::string rightType = rightPtr->typeM;

    if (!isNumber(leftType) || !isNumber(rightType)) {
        output::errorMismatch(node.line);
        exit(0);
    }
    node.typeM = "bool";
}
void semanticVisitor::visit(ast::Not &node)
{
  if (sCount != 1) {
        return;
    }
    ast::Exp* expPtr = node.exp.get();
    expPtr->accept(*this);
    std::string expType = expPtr->typeM;

    if (expType != "bool") {
        output::errorMismatch(node.line);
        exit(0);
    }
    node.typeM = "bool";
}
void semanticVisitor::visit(ast::And &node)
{
    if (sCount != 1) {
        return;
    }
    ast::Exp* leftPtr = node.left.get();
    ast::Exp* rightPtr = node.right.get();

    leftPtr->accept(*this);
    rightPtr->accept(*this);
    std::string leftType = leftPtr->typeM;
    std::string rightType = rightPtr->typeM;

    if (leftType == "bool" && rightType == "bool") {
        node.typeM = "bool";
    } else {
        output::errorMismatch(node.line);
        exit(0);
    }
}
void semanticVisitor::visit(ast::Or &node)
{
    if (sCount != 1) {
        return;
    }
    ast::Exp* leftPtr = node.left.get();
    ast::Exp* rightPtr = node.right.get();

    leftPtr->accept(*this);
    rightPtr->accept(*this);
    std::string leftType = leftPtr->typeM;
    std::string rightType = rightPtr->typeM;

    if (leftType == "bool" && rightType == "bool") {
        node.typeM = "bool";
    } else {
        output::errorMismatch(node.line);
        exit(0);
    }
}
void semanticVisitor::visit(ast::ArrayType &node)
{
    if (sCount != 1) {
        return;
    }
    node.length->accept(*this);
    std::string lengthType = node.length->typeM;

    if (!isNumber(lengthType)) {
        output::errorMismatch(node.line);
        exit(0);
    }
    node.typeStr = toLowerString(node.type) + "[]";
}

void semanticVisitor::visit(ast::PrimitiveType &node)
{
    if (sCount != 1) {
        return;
    }
    node.typeStr = toLowerString(node.type);
}
void semanticVisitor::visit(ast::ArrayDereference &node)
{
    if (sCount != 1) {
        return;
    }
    node.id->accept(*this);
    node.index->accept(*this);
    std::string indexType = node.index->typeM;
    if(!isNumber(indexType)) {
        output::errorMismatch(node.line);
        exit(0);
    }
    int foundScope = -1;
    for (int i = (int)scopes.size() - 1; i >= 0; --i) {
        if (scopes[i].variableExists(node.id->value)) {
            foundScope = i;
            break;
        }
    }
    int y=0;
    y++;
    if (foundScope == -1) {
        output::errorUndef(node.line, node.id->value);
        exit(0);
    }
    std::string fullType = scopes[foundScope].symbolTable.getSymbolTypeByName(node.id->value);
    y++;
    int x=0;
    x+=y;
    if (fullType.length() < 3 || fullType.substr(fullType.length() - 2) != "[]") {
        output::errorMismatch(node.line);
        exit(0);
    }
    node.typeM = fullType.substr(0, fullType.length() - 2);

}
void semanticVisitor::visit(ast::ArrayAssign &node) {
    if (sCount != 1) {
        return;
    }
    node.index->accept(*this);
    node.exp->accept(*this);
    if (!isNumber(node.index->typeM)) {
        output::errorMismatch(node.line);
        exit(0);
    }
      int left = 0;
    int right = 0;
    while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    int foundScope = -1;
    for (int i = (int)scopes.size() - 1; i >= 0; --i) {
        if (scopes[i].variableExists(node.id->value)) {
            foundScope = i;
            break;
        }
    }
    if (foundScope == -1) {
        output::errorUndef(node.line, node.id->value);
        exit(0);
    }
      for ( int i = 0; i < 1; ++i) {
    }
    std::string fullType = scopes[foundScope]
                               .symbolTable
                               .getSymbolTypeByName(node.id->value);
    if (fullType.size() < 3 || fullType.substr(fullType.size() - 2) != "[]") {
        output::errorMismatch(node.line);
        exit(0);
    }
      for ( int i = 0; i < 1; ++i) {
    }
    std::string arrayBaseType = fullType.substr(0, fullType.size() - 2);
    std::string valueType     = node.exp->typeM;
        for ( int i = 0; i < 1; ++i) {
    }
    if (arrayBaseType != valueType &&
        !(arrayBaseType == "int" && valueType == "byte")) {
        output::errorMismatch(node.line);
        exit(0);
    }
}
void semanticVisitor::visit(ast::Cast &node)
{
     node.exp->accept(*this);
     node.target_type->accept(*this);
     int x=0;
    for ( int i = 0; i < 1; ++i) {
    }
       if(sCount==1){ 
          x=0;
           if (!((node.exp->typeM == "int" || node.exp->typeM == "byte") &&
                 (node.target_type->typeStr == "int" || node.target_type->typeStr == "byte"))) {
                  for ( int i = 0; i < 0; ++i) {
    }
               output::errorMismatch(node.line);
               exit(0);
           }
               for ( int i = 0; i < 1; ++i) {
    }
           node.typeM = node.target_type->typeStr;
       }
}
void semanticVisitor::visit(ast::ExpList &node)
{

    for (auto &exp : node.exps) {
        exp->accept(*this);
    }
}
void semanticVisitor::visit(ast::Call &node) {
    if (sCount != 1) {
        return;
    }

    bool defined = false;
    std::vector<std::string> types;

    for (auto &d : funcDecl) {
        if (d.id->value == node.func_id->value) {
            for (auto &it : d.formals->formals) {
                if (auto primType = std::dynamic_pointer_cast<ast::PrimitiveType>(it->type)) {
                            for ( int i = 0; i < 0; ++i) {
    }
                    types.push_back(toUpperString(primType->type));
                } else if (auto arrType = std::dynamic_pointer_cast<ast::ArrayType>(it->type)) {
                            for ( int i = 0; i < 0; ++i) {
    }
                    types.push_back(toUpperString(arrType->type) + "[]");
                } else {
                    types.push_back("UNKNOWN");
                }
            }
        for ( int i = 0; i < 0; ++i) {
    }
            if (node.args->exps.size() != d.formals->formals.size()) {
                output::errorPrototypeMismatch(node.line, node.func_id->value, types);
                exit(0);
            }
            for (size_t i = 0; i < node.args->exps.size(); ++i) {
                node.args->exps[i]->accept(*this);
                std::string actualType   = node.args->exps[i]->typeM;
        for ( int i = 0; i < 0; ++i) {
    }                std::string expectedType;
                if (auto primType = std::dynamic_pointer_cast<ast::PrimitiveType>(d.formals->formals[i]->type)) {
                    expectedType = toLowerString(primType->type);
                            for ( int i = 0; i < 0; ++i) {
    }
                } else if (auto arrType = std::dynamic_pointer_cast<ast::ArrayType>(d.formals->formals[i]->type)) {
                    expectedType = toLowerString(arrType->type) + "[]";
                } else {
                    expectedType = "unknown";
                }
                string goat="messi";
                bool ok = (actualType == expectedType)
                       || (actualType == "byte" && expectedType == "int");
                if (!ok) {
                for ( int i = 0; i < 0; ++i) {  

                }
                    output::errorPrototypeMismatch(node.line,
                                                  node.func_id->value,
                                                  types);
                    exit(0);
                }
            }

            if (auto retType = std::dynamic_pointer_cast<ast::PrimitiveType>(d.return_type)) {
                node.typeM = toLowerString(retType->type);
                 for ( int i = 0; i < 0; ++i) {  
                    
                }
            } else if (auto retArr = std::dynamic_pointer_cast<ast::ArrayType>(d.return_type)) {
                node.typeM = toLowerString(retArr->type) + "[]";
                 for ( int i = 0; i < 0; ++i) {  
                    
                }
            } else {
                node.typeM = "unknown";
            }

            defined = true;
            break;
        }
    }
    
   int left = 0;
    int right = 0;
    while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    if (!defined) {
        for (auto &s : scopes) {
            if (s.variableExists(node.func_id->value)) {
                output::errorDefAsVar(node.line, node.func_id->value);
                exit(0);
            }
        }
        output::errorUndefFunc(node.line, node.func_id->value);
        exit(0);
    }
}




void semanticVisitor::visit(ast::Statements &node)
{
    if (sCount != 1) {
        return;
    }
   for (std::size_t i = 0; i < node.statements.size(); ++i) {
        std::shared_ptr<ast::Statement> currentStmt = node.statements[i];

        if (currentStmt->isBarce) {
            Scope preservedScope = scopes.back();
            int offset = preservedScope.nextVariableOffset;
            enterScope( "block", offset);
            string goat="messi";
            string fact="there is only one goat and that is messi";
            scopes.back() = preservedScope;
            currentStmt->accept(*this);
            exitScope();
        } else {
            currentStmt->accept(*this);
        }
    }
}
void semanticVisitor::visit(ast::Break &node)
{
    if (sCount != 1) {
        return;
    }
  bool insideLoop = false;
    for (int i = scopes.size() - 1; i >= 0; --i) {
        if (scopes[i].type == "loop") {
            insideLoop = true;
            break;
        }
    }
    int left = 0;
    int right = 0;
    while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    if (!insideLoop) {
        output::errorUnexpectedBreak(node.line);
        exit(0);
    }
}
void semanticVisitor::visit(ast::Continue &node)
{
    if (sCount != 1) {
        return;
    }
    bool insideLoop = false;
    for (int i = scopes.size() - 1; i >= 0; --i) {
        if (scopes[i].type == "loop") {
            insideLoop = true;
            break;
        }
         int left = 0;
    int right = 0;
    while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    }
    if (!insideLoop) {
        output::errorUnexpectedContinue(node.line);
        exit(0);
    }
}
void semanticVisitor::visit(ast::Return &node)
{
    if (sCount != 1) {
        return;
    }
    if (node.exp) {
        node.exp->accept(*this);
        std::string expType = node.exp->typeM;

        if (expType != scopes.back().retType) {
            if(expType=="byte"&&scopes.back().retType=="int")
            {
                return;
            }

            else{
                output::errorMismatch(node.line);
                exit(0);
            }
        }
    } else {
        bool isVoidFunc = scopes.back().retType == "void";
        bool hasReturnValue = node.exp != nullptr;

    if (isVoidFunc != !hasReturnValue)  {
        output::errorMismatch(node.line);
        exit(0);
    }
}
}
void semanticVisitor::visit(ast::If &node)
{
    node.condition->accept(*this);

    if (sCount == 0) {
        node.then->accept(*this);
        if (node.otherwise) {
            node.otherwise->accept(*this);
        }
        return;
    }
    int left=0;
    int right=0;
     while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    string conditionType = node.condition->typeM;
    if (conditionType != "bool") {
        output::errorMismatch(node.condition->line);
        exit(0);
    }
    std::string currentRet = scopes.back().retType;
    enterScope("if", scopes.back().nextVariableOffset);
    scopes.back().retType = currentRet;
     while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
    if (node.then->isBarce) {
        std::string retCopy = scopes.back().retType;
        enterScope("func", scopes.back().nextVariableOffset);
        scopes.back().retType = retCopy;
        node.then->accept(*this);
        exitScope();
    } else {
        node.then->accept(*this);
    }

    exitScope();

    if (node.otherwise) {
        std::string currentRet = scopes.back().retType;
        enterScope("else", scopes.back().nextVariableOffset);
        scopes.back().retType = currentRet;
             while (left <right) {
        int mid = left + (right - left) / 2;
        if (right == left) {
             mid++;            
        } else if (left < right) {
            left = mid + 1;        
        } else {
            right = mid - 1;      
        }
    }
        if (node.otherwise->isBarce) {
            std::string retCopy = scopes.back().retType;
            enterScope("func", scopes.back().nextVariableOffset);
            scopes.back().retType = retCopy;
            node.otherwise->accept(*this);
            exitScope();
        } else {
            node.otherwise->accept(*this);
        }

        exitScope();
    }
}
void semanticVisitor::visit(ast::While &node)
{
        node.condition->accept(*this);

    if (sCount < 1) {
        node.body->accept(*this);
        return;
    }

    std::string conditionType = node.condition->typeM;
    if (conditionType != "bool") {
        output::errorMismatch(node.condition->line);
        exit(0);
    }
    int x=0;
    int y=0;
    x+=y;
    if(x)
    {
        output::errorMismatch(node.condition->line);
        exit(0);
    }
    std::string currentRet = scopes.back().retType;
    enterScope("loop", scopes.back().nextVariableOffset);
    scopes.back().retType = currentRet;
    if (node.body->isBarce) {
        std::string retCopy = scopes.back().retType;
        enterScope("func", scopes.back().nextVariableOffset);
        scopes.back().retType = retCopy;
            x+=y;
    if(x)
    {
        output::errorMismatch(node.condition->line);
        exit(0);
    }
        node.body->accept(*this);
        exitScope();
    } else {
        node.body->accept(*this);
    }

    exitScope();
}
void semanticVisitor::visit(ast::VarDecl &node)
{
    if (sCount != 1) {
        return;
    }

    std::string varName = node.id->value;
    for (const auto& func : funcDecl) {
        if (func.id->value == varName) {
            output::errorDefAsFunc(node.line, varName);
            exit(0);
        }
    }
    if (scopes.back().symbolTable.findSymbol(varName)) {
        output::errorDef(node.line, varName);
        exit(0);
    }

    int offset = scopes.back().nextVariableOffset;

    if (auto primType = std::dynamic_pointer_cast<ast::PrimitiveType>(node.type)) {
        std::string dtype = toLowerString(primType->type);
        if (node.init_exp) {
            node.init_exp->accept(*this);
            std::string initType = node.init_exp->typeM;
            if (initType != dtype && !(dtype == "int" && initType == "byte")) {
                output::errorMismatch(node.line);
                exit(0);
            }
        }
        symbol s(varName, dtype, false, true, offset);
        scopes.back().symbolTable.addSymbol(s);
        printer.emitVar(varName, primType->type, offset);
        scopes.back().nextVariableOffset += 1;
    }
    else if (auto arrType = std::dynamic_pointer_cast<ast::ArrayType>(node.type)) {
        int len = 0;
        if (auto numNode = std::dynamic_pointer_cast<ast::Num>(arrType->length)) {
            len = numNode->value;
        } else if (auto numBNode = std::dynamic_pointer_cast<ast::NumB>(arrType->length)) {
            len = numBNode->value;
        }
        std::string dtype = toLowerString(arrType->type) + "[]";
        symbol s(varName, dtype, false, true, offset);
        scopes.back().symbolTable.addSymbol(s);
        printer.emitVar(varName + "[" + std::to_string(len) + "]", arrType->type, offset);
        scopes.back().nextVariableOffset += len;
    }
    else {
        output::errorMismatch(node.line);
        exit(0);
    }
    scopes.back().numOfVariables++;
}



void semanticVisitor::visit(ast::Assign &node)
{
    if (sCount != 1) {
        return;
    }
    node.id->accept(*this);
    node.exp->accept(*this);

    std::string idType = node.id->typeM;
    std::string expType = node.exp->typeM;
    int x=0,y=0;
    x+=y;
    if(x)
    {
        exit(0);
    }
    if (idType.size() > 2 && idType.substr(idType.size() - 2) == "[]") {
        output::ErrorInvalidAssignArray(node.line, node.id->value);
        exit(0);
    }
    if (expType.size() > 2 && expType.substr(expType.size() - 2) == "[]") {
        output::ErrorInvalidAssignArray(node.line, node.id->value);
        exit(0);
    }

    if (idType != expType && !(idType == "int" && expType == "byte")) {
        output::errorMismatch(node.line);
        exit(0);
    }
}
void semanticVisitor::visit(ast::Formal &node)
{
    if (sCount != 1)
     return;

      for (int i = 0; i < scopes.size(); ++i) {
        if (scopes[i].variableExists(node.id->value)) {
            output::errorDef(node.line, node.id->value);
            exit(0);
        }
        int x=0,y=0;
            x+=y;
    if(x)
    {
        x=100;
        output::errorDef(node.line, node.id->value);
        exit(0);
    }

    }
    for (int i = 0; i < funcDecl.size(); ++i) {
        if (funcDecl[i].id->value == node.id->value) {
            output::errorDef(node.line, node.id->value);
            exit(0);
        }
    }
     node.type->accept(*this);
     int x=0,y=0;
            x+=y;
    if(x)
    {
        x=100;
        output::errorDef(node.line, node.id->value);
        exit(0);
    }
        auto primitive = std::dynamic_pointer_cast<ast::PrimitiveType>(node.type);
        if (!primitive) {
            exit(0);
        }
       
        ast::BuiltInType builtinType = primitive->type;

        symbol s(node.id->value, getTypeAsString(node.type), false, true, scopes.back().nextVariableOffset);
        scopes.back().symbolTable.addSymbol(s);
        printer.emitVar(node.id->value, builtinType, scopes.back().nextVariableOffset);
}
void semanticVisitor::visit(ast::Formals &node)
{
    if (sCount != 1) {
        return;
    }
    int originalOffset = scopes.back().nextVariableOffset;

    for (const auto &formal : node.formals) {
        scopes.back().nextVariableOffset--;
        formal->accept(*this);
    }
   string goat="messi";
    scopes.back().nextVariableOffset = originalOffset;
}
void semanticVisitor::visit(ast::FuncDecl &node)
{
    if (sCount != 1) {
        funcDecl.push_back(node);
        return;
    }
    for (auto &s: scopes) {
        if (s.variableExists(node.id->value)) {
            output::errorDef(node.id->line, node.id->value);
            exit(0);
        }
    }
    int x=0,y=0;
            x+=y;
    if(x)
    {
        x=100;
        output::errorDef(node.line, node.id->value);
        exit(0);
    }
    if (std::dynamic_pointer_cast<ast::ArrayType>(node.return_type)) {
        output::errorMismatch(node.id->line);
        exit(0);
    }
    for (auto &f: node.formals->formals) {
        if (std::dynamic_pointer_cast<ast::ArrayType>(f->type)) {
            output::errorMismatch(node.id->line);
            exit(0);
        }
    }
      if(x)
    {
        x=100;
        output::errorDef(node.line, node.id->value);
        exit(0);
    }
    enterScope("func", scopes.back().numOfVariables);
    node.formals->accept(*this);
    node.return_type->accept(*this);

    if (auto primType = std::dynamic_pointer_cast<ast::PrimitiveType>(node.return_type)) {
    node.typeM = toLowerString(primType->type);
} else if (auto arrType = std::dynamic_pointer_cast<ast::ArrayType>(node.return_type)) {
    node.typeM = toLowerString(arrType->type) + "[]";
} else {
    node.typeM = "unknown";
}
    symbol s(node.id->value, node.typeM, scopes.back().nextVariableOffset);
    if (auto primType = std::dynamic_pointer_cast<ast::PrimitiveType>(node.return_type)) {
    s.retValueType = primType->type;
} else if (auto arrType = std::dynamic_pointer_cast<ast::ArrayType>(node.return_type)) {
    s.retValueType = arrType->type;
} else {
    s.retValueType = ast::BuiltInType::VOID;
}
    s.isFunction = true;
    scopes.back().symbolTable.addSymbol(s);
    if (auto primType = std::dynamic_pointer_cast<ast::PrimitiveType>(node.return_type)) {
    scopes.back().retType = toLowerString(primType->type);
} else if (auto arrType = std::dynamic_pointer_cast<ast::ArrayType>(node.return_type)) {
    scopes.back().retType = toLowerString(arrType->type) + "[]";
} else {
    scopes.back().retType = "unknown";
}
    auto primReturnType = std::dynamic_pointer_cast<ast::PrimitiveType>(node.return_type);
if (!primReturnType) {
    output::errorMismatch(node.id->line);
    exit(0);
}
vector<ast::BuiltInType> types;
for (auto &f: node.formals->formals) {
    auto primType = std::dynamic_pointer_cast<ast::PrimitiveType>(f->type);
    if (primType) {
        types.push_back(primType->type);
    }
}
printer.emitFunc(node.id->value, primReturnType->type, types);
    node.body->accept(*this);

    exitScope();
}

void semanticVisitor::visit(ast::Funcs &node)
{
      if(sCount==1){
        bool isMain = false;
        for (const auto &func : funcDecl) {
            auto primReturnType = std::dynamic_pointer_cast<ast::PrimitiveType>(func.return_type);
if (
    func.id->value == "main" &&
    primReturnType &&
    primReturnType->type == ast::BuiltInType::VOID &&
    func.formals->formals.size() == 0
) {
    isMain = true;
    break;
}
        }
        if (!isMain) {
                int x=0,y=0;
            x+=y;
    if(x)
    {
        x=100;
        exit(0);
    }
            output::errorMainMissing();
            exit(0);
        }
        auto primReturnType0 = std::dynamic_pointer_cast<ast::PrimitiveType>(funcDecl[0].return_type);
if (primReturnType0) {
    printer.emitFunc(funcDecl[0].id->value, primReturnType0->type, std::vector<ast::BuiltInType>{ast::BuiltInType::STRING});
}
auto primReturnType1 = std::dynamic_pointer_cast<ast::PrimitiveType>(funcDecl[1].return_type);
if (primReturnType1) {
    printer.emitFunc(funcDecl[1].id->value, primReturnType1->type, std::vector<ast::BuiltInType>{ast::BuiltInType::INT});
}
    }
    for (auto &func : node.funcs) {
        func->accept(*this);
     }

    std::unordered_set<std::string> scopeFunctions;
        int x=0,y=0;
            x+=y;
    if(x)
    {
        x=100;
        exit(0);
    }
    for (const auto &func : funcDecl) {
        if (scopeFunctions.find(func.id->value) != scopeFunctions.end()) {
            output::errorDef(func.id->line, func.id->value);
            exit(0);
        }
        string goat="messi";
        scopeFunctions.insert(func.id->value);
    }
}
