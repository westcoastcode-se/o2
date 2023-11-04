# o2

[License](LICENSE.txt)
[Primitives](#Primitives)
[Modules](#Modules)
[Type](#Type)
[Const](#Const)
[Func](#Func)
[Trait](#Trait)
[Macro](#Macro)

## Primitives

- [x] `primitives`, such as int32, float32
- [ ] `string` primitive (unicode?)
- [ ] unicode characters
  - should this be `char` or should a wide character type be added?

* `byte`, is 1 byte
* `bool`, is 4 bytes
  * [ ] look into alternate bool types, using bit flags or something similar
* `int8`, is 1 byte
* `uint8`, is 1 byte
* `int16`, is 2 byte
* `uint16`, is 2 byte
* `int32` (alias: `int`), is 4 bytes
* `uint32` (alias: `uint`), is 4 byte
* `int64`, is 8 bytes
* `uint64`, is 8 byte
* `float32` (alias: `float`), is 4 bytes
* `float64`, is 8 bytes

## Modules

- [x] `package` root is the same as the project root
- [x] `package` names are now figured out from the module name and path
  - modules should be put as children under root, unless they are private
- [ ] `module` parser
- [ ] `module` command line tool
- [ ] use `git` to fetch requirements/dependencies

Modules can be considered a collection of packages, where each package contains the actual source code. Source code is
always associated with a module.

Your source code root folder must have a `o2.mod` file. It contains information on what compiler has to be used,
what the name of the module is and what dependencies this module has.

A bare-minimum module file must have a compiler-version and a name, for example:

```
compiler 1.0

name "github.com/westcoastcode-se/o2"
```

### Requirements and dependencies

You can depend on other modules using the `require` syntax. For example:

```
require "github.com/westcoastcode-se/o2-stdio" v1.0
```

This will use `git` and checkout `github.com/westcoastcode-se/o2-stdio` with tag `v1.0`

### Private dependencies

- [ ] private `modules`
    - should give you the power of having multiple versions of the same requirement

If you specify a requirements as `private` then that dependency will not have it's symbols exposed outside
your own module. Example:

```
require "github.com/westcoastcode-se/o2-crypto" v1.0 private
```

Modules requiring the exact same dependency can share symbols from `private` dependencies with each other. Useful
if you have a framework split into multiple modules, but want to share data between them containing private
dependencies.

## Package

A package is, implicitly, figured out by the folder it's located in. Lets' consider the following folder structure:

- mydomain.io/mymodule
  - services
    - service.o2
  - models
    - user.o2
    - roles.o2
  - app
    - app.o2

Then if you import `mydomain.io/mymodule/models` then all files in the `models` directory is imported

## Import

- [x] `import`
- [x] `import` aliases
- [x] `import` statements should be the path to the module + package path

You import a package with the syntax: `import "mymodule.io/module/package"`. You can also specify an alias for your import
using the format `import "<path>" as alias_name`

## Type

- [x] the `type` keyword
- [ ] implicit type deduction for constants
- [ ] implicit type deduction for variables
- [ ] inheritance

### Typedef

- [ ] typedef
- [ ] consider if typedefs should be allowed to extend methods?
  - question: runtime type-information will be lost in some case. Is this okay?
- [ ] consider primitive typedef with extra data: `type Id : int32 { <Fields> }`
  - question: runtime type-information will be lost in some case. Is this okay?

If you create a type that inherits from a primitive or another type then that's considered a typedef. Typedef
are handled by the compiler as two different types, but can be compile-time casted between the type and the
`base` type. 

Consider the following:

```
type Id : int32
```

Then you can 

### Struct

- [x] `type` structs without inheritance
- [ ] structs with inheritance

### Enum

- [ ] add support for `enum` types

## Const

- [x] `const` variables from constant primitive token, such as `0` or `1.0f`
- [ ] `const` variables from other constant name
- [ ] `const` variables from `const func`
- [ ] `const` functions
- [ ] returning a `const` variable from a `func`

You can create constant symbols using the `const` keyword.

### Const variables

Consider the following:

```
const VALUE = 10
```

then the symbol `VALUE` will have the value `10` with the implicit type `int32`. Constant values can't be modified
when your application is running, but instead, can be used in a macro or other pre-compile processes to improve
the performance of your application

### Const functions

Constant functions are guaranteed to be compile-time compliant. They can be used in both normal expressions
and in macro expressions.

```
const func MyFunc() int {
  return 10
}
```

## Func

- [x] simple `func` function definitions

## Trait

- [ ] add `trait` types used as an alternative to template to solve some of the template
  specializations done in C++, for example the `operator<<` for std::ostream

Consider the following:

```
trait Stringify {
  func String() string
}

type Id : int32 {
  func String() string {
    return "My Id"
  }
}

func Println(s Stringify) {
  stdio.Println(s.String())
}

func main() {
  var id = (Id)10
  Println(id)
}
```

Types that comply with a trait's interface will be, compile-time resolved to be fit that trait. Traits, however,
is not the same as an `interface`. Runtime-type information is kept for interfaces, but traits loses that 
information.

The purpose for a trait is to create a modern solution for parts of C++'s template specialization functionality.

## Macro

- [ ] `macro` scope in functions
- [ ] implement `macro` functions

If you are familiar with C and C++, then a macro is a mix between the pre-processor MACRO in C and templates in C++.

A macro is evaluated during the compilation phase. Everything that is
compile-time constant, such as `const` variables, `type` information, `attribute` data, `func` statements etc,
can be used inside a macro.

### Macro scope

Macro scopes can be used to generate source code inside source code during the compilation phase. Macro statements
have the format `macro { <expr> }`.

Consider the following:

```
func f() int {
  var result = 0
  macro {
    for i := 0; i < 3; ++i {
      yield "result++"
    }
  }
  return result
}
```

Will turn into:

```
func f() int {
  var result = 0
  result++
  result++
  result++
  return result
}
```

### Macro functions

Macro functions have the format `macro func <NAME>(<ARGS>) {}`. This type of function can also be written
as `const func <NAME>(<ARGS>) stream<string> {}`.

Consider the following example:

```
macro func M(value i) {
  i = i * 10
  yield `"value is ${i}"`
}
const stringified string = M(2)
```

Will turn into:

```
const stringified string = "value is 20"
```

###  

## Features

### Completed

- [x] recursive symbol querying
    - imports
    - inner types
    - collision detection
- [x] binary and unary operations using basic operators `+`, `-` etc.
- [x] `return` statement
- [x] resolving types should not also calculate the type size
- [x] do not require `;`

### Not complete

- [ ] each new expression must be put on a new line
- [ ] only resolve sizes for unknown types at the end. Some times can be resolved during parse phase
  (only primitives etc.).
    - how should fields in types be handled?
- [ ] resolving `primitive` types can be during parse
- [ ] `if` statement
- [ ] `for` loops
- [ ] `switch` statement
- [ ] `cast` between primitives
- [ ] `date` and `time` operations
- [ ] types that inherits from primitives
- [ ] template syntax using `<` and `>`
- [ ] `yield` stream statement in function
- [ ] `async` and `await` suspension statements
    - should be triggered by interupts from drivers (sockets, io, ...)
- [ ] compile-time `extern` functions linkage
- [ ] runtime `extern` functions linkage
    - if called and not linked then `panic`
- [ ] types in `extern` libraries that use inheritance
    - expose vtable
- [ ] `panic` statements
- [ ] full multi-threaded support
    - [x] multi-thread friendly source code parse
    - [ ] allow for multi-threaded resolve (maybe)
- [ ] argument name specification using `call(name_arg = "test")`
- [ ] code complexity analyser and reporting
    - will be needed anyways for the optimizer to be good

### Consideration / Evaluation

- [ ] exceptions or `error` return value?
    - for example, exceptions should be considered a fatal error so why not panic?

