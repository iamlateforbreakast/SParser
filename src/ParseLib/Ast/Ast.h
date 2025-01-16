/* Ast.h */

#ifndef _AST_H_
#define _AST_H_

typedef struct Ast Ast;

Ast* Ast_new();
void Ast_delete(Ast* this);
Ast* Ast_copy(Ast* this);
void Ast_print(Ast* this);
int Ast_getSize(Ast* this);

#endif /* #ifndef _AST_H_ */
