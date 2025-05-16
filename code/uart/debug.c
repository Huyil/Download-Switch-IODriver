#include "debug.h"
// 函数类型定义
typedef void (*Func0)();
typedef void (*Func1)(void*);
typedef void (*Func2)(void*, void*);
typedef void (*Func3)(void*, void*, void*);

void* func = NULL;
uint8_t isNull[3];
void* argv[3] = {isNull,isNull,isNull};
void DEBUGCB(void *arg)
{
  if(func)
  {
    func = (void*)(((uintptr_t)func) | 1);  
    if(argv[2] && argv[1] && argv[0])
      ((Func3)func)(argv[0],argv[1],argv[2]);
    else if(argv[1] && argv[0])
      ((Func2)func)(argv[0],argv[1]);
    else if(argv[0])
      ((Func1)func)(argv[0]);
    else 
      ((Func0)func)();
    func = NULL;
  }
}

