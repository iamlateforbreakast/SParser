/* Declarator.h */

#ifndef _DECLARATOR_H_
#define _DECLARATOR_H_

typedef enum
{
  E_DEC_FUNCTION,
  E_DEC_VAR,
  E_DEC_TYPE
} DeclaratorType;

typedef struct Declarator Declarator;

Declarator * Declarator_new(DeclaratorType * t)
{
}

void Declarator_delete(Declarator * this)
{
}

#endif /* #ifndef _DECLARATOR_H_
