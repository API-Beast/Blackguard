# Coding conventions

## Use of capital and small initial letters in classes and functions

* Class names: `EpicClass`<br>
* Methods: `void doSthFancy()`<br>
* Properties and variables: `int awesomeness`<br>
* Functions: `void ImNotAMethod()`
* Do not use Hungarian notion.

## Namespaces

All classes are put into the GRG namespace, bigger modules use their own sub-namespaces

Example: `namespace GRG::Audio`

## File naming

Files use CamelCase.

Example: `EpicClass.cpp`

## Includes

When possible keep the amount of includes in a header down to a minimum to keep the compile time low.

## Nested if's

When nesting if's put { and } braces after every if.

**Wrong:**
if(expressionA+expressionB) if(expressionC) doStuff();

**Right:**
if(expressionA+expressionB) {
  if(expressionC) {
    doStuff();
  }
}

## Error handling

We use for error handling exceptions!

## Headers

Keep headers as clutter free as possible. This means no implementations inside headers! For implementations of templates create a new header "EpicClass.impl.h" which you include at the end of the header file.

EpicClass.h:
``` cpp
template<class T>
class EpicClass
{
  void foo();
}
#include "EpicClass.impl.h"
```

EpicClass.impl.h:
``` cpp
template<class T>
void EpicClass<T>::foo()
{
  [...]
}
```
