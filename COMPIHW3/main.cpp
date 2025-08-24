#include "output.hpp"
#include "nodes.hpp"
#include "semanticVisitor.hpp"
// Extern from the bison-generated parser
extern int yyparse();

extern std::shared_ptr<ast::Node> program;

int main() {
      yyparse();
    semanticVisitor semanticVisitor;
    program->accept(semanticVisitor);
    semanticVisitor.sCount++;
    program->accept(semanticVisitor);
    std::cout << semanticVisitor.printer;
}
