/* Class. h */

#ifndef _CLASS_H_
#define _CLASS_H_

typedef struct Class Class;

struct Object;
typedef struct Object* (*Constructor)();
typedef void (*Destructor)(struct Object*);
typedef struct Object* (*Copy_Operator)(struct Object*);
typedef int (*Comp_Operator)(struct Object*, struct Object*);
typedef char* (*Printer)(struct Object*);

struct Class
{
  Constructor f_new;
  Destructor f_delete;
  Copy_Operator f_copy;
  Comp_Operator f_comp;
  Printer f_print;
};

#endif /* _CLASS_H_ */
