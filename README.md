# o2

## Table of content

1. [License](LICENSE.txt)
2. [Why](#Why)
3. [Primitives](#Primitives)
4. [Modules](#Modules)
5. [Type](#Type)
6. [Const](#Const)
7. [Func](#Func)
8. [Interface](#Interface)
9. [Lambda](#Lambda)
10. [Trait](#Trait)
11. [Macro](#Macro)
12. [Attributes](#Attributes)

## Why?

This is primarily a hobby project of mine. I've been a C++ developer for most of my life and I've always
felt that something is missing from that language:

* C++ takes a long time to compile
* C++ syntax is hard to read (especially the later versions of C++)
    * most modern languages suffer from this (Rust, Node JS etc)
* No good support for compile-time code processing using attributes
    * Engines (web-frameworks, game engines etc) could really use good
      compile-time source code generation for JSON serialization and other similar things
* No easy way of interpreting the source code with tools
* Can't really run C++ as a scripting language
    * Consider a game that makes it possible for mods being written in the same
      language that the game is written in and those mods can actually use the
      runtime-type information, inheritance and other such features
* Built-in support for dependency management between different projects/modules
* No built-in support for suspensions using `async` and `await`
    * Threads are not really a good way of using your computers resources
* Why not?

So my goal is to make a programming language that's modern, compiles quickly enough and
creates machine-code. Then who knows what happens from there

## Getting started

Tools needed are:

* Compiler (Visual Studio, clang or gcc)
    * For example, download clang https://github.com/llvm/llvm-project/releases and configure it
      for clion: https://www.jetbrains.com/help/clion/quick-tutorial-on-configuring-clion-on-windows.html#clang-cl
* CMake >= 3.12
* Git

Start by cloning `llvm-project` and checking out the appropriate version:

```bash
git clone --depth 1 https://github.com/llvm/llvm-project.git -b llvmorg-17.0.5
cd llvm-project
```

then compile and install it for your computer. For example, on widows do the following:

```bash
cd llvm-project
cmake -S llvm -B build -G "Visual Studio 17 2022"
cmake --build build -j 4
cmake --build . --target install
```

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

- [x] allow for operating system specific source codes using filename suffixes
    - `_win.o2`, `_osx.o2`, `_linux.o2`

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
- [ ] resolve references when a package's imports are all resolved
    - should be possible to do in parallel parsing source code since we know, for sure, that a reference
      can only be resolved into something that's imported
    - if we allow for resolving references during parse phase, how would pre-compile annotation driven
      syntax tree manipulation work? Or should we only allow for post-resolve processing?
    -

You import a package with the syntax: `import "mymodule.io/module/package"`. You can also specify an alias for your
import using the format `import "<path>" as alias_name`.

## Type

- [x] the `type` keyword
- [x] implicit type deduction for constants
- [ ] implicit type deduction for variables
- [ ] implicit types deducted from constants can be automatically resolved during parse phase
    - `var name = 10` is always known to be an `int32`
- [x] inheriting from primitives
- [x] inheriting from types
- [ ] prevent inheriting from multiple structs
- [ ] inheriting from macro types
- [ ] inheriting from interfaces
- [ ] macro types
- [ ] lookup of inherited symbols must be done with "closest" first
    - search one level at a time, beginning from the left of the inheritance list
- [ ] add a specific `*void` type called `rawptr`, or `ptr` or something similar

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
- [x] require the use of `var` for local variables
- [x] add `static` scope for static variables and functions
- [x] add `static` variable definitions
- [ ] add `static` function definitions
- [ ] structs with inheritance
- [ ] `constructor` definition
- [ ] `destructor` definition
- [ ] `move` definition
- [ ] default non-args and all-args `constructor` if no constructor added in definition

### Enum

- [ ] add support for `enum` types

## Const

- [x] `const` variables from constant primitive token, such as `0` or `1.0f`
- [ ] `const` variables from other constant name
- [ ] `const` variables from `const func`
- [ ] setting a `const` variable from a `const func` call
- [ ] `const` functions
- [ ] returning a `const` variable from a `func`
- [ ] allow for a struct `type` to be used as constant

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
- [ ] `extern` function definitions
    - use attribute to specify exactly how an `extern` static and/or dynamically linked dependency
      handles arguments and return values
    - `extern` should only be used for statically link with c libraries or dynamically link
      with c, c++ and o2 libraries
- [ ] `const` functions that can be compile-time evaluated
- [ ] consider functions with implicit return type based on `return` statement
    - common syntax from other languages would be: `func Pow2(value int) -> value * value`
- [ ] add support for local `var` statements
- [ ] add support for short-hand `decl_assign` statement for creating one or more variables
- [ ] add support for multiple `var` statements using the `,` delimiter
    - example: `var i,j = 0,1`
- [ ] add support for multiple `decl_assign` statements using the `,` delimiter
    - example: `i,j := 0,1`
- [ ] add access privileges for functions: `module` (default), `public`, `private`
    - a function marked with the `module` accessor should only be accessible from within the same module
    - a function marked with the `public` accessor should only be accessible from everywhere
    - a function marked with the `private` accessor should only be accessible from within the package
- [ ] polymorphism for functions
    - allow for selecting the best matched function when more than one symbol is found during the resolve phase

### Package level function

- [x] add `global` level functions at package level
- [ ] add `global` level functions at function level
- [ ] add support for `variadic` arguments
- [ ] add support for optional arguments
    - allow for a syntax like `MyFunc(10, ArgWithName=5)` if you have multiple optional arguments

You can add functions at the package level. This type of function is called a `global` function.

Consider the following:

```
func Get10() int {
    return 10
}
```

The example above creates a `global` function called `Get10` that returns the value `10` as an `int` primitive

### Type level functions

- [ ] add `static` functions at type level
- [x] add `method` level functions at type level
- [x] add default `this` argument
- [ ] add custom `this` argument name
- [ ] older languages differ between pointer accessors and non-pointer accessors using `->` and `.`
    - can't we always use `.`? IfWhen is it appropriate to use `.` on a pointer
- [ ] add access privileges for functions: `module` (default), `public`, `private`
    - a function marked with the `module` accessor should only be accessible from within the same module
    - a function marked with the `public` accessor should only be accessible from everywhere
    - a function marked with the `private` accessor should only be accessible from within the package

You can have two different types of functions on the type level. The first one is `static` functions.
`static` functions do not require a pointer to a memory block in which the type resides. You can access it
using the syntax: `<type>.<function name>()` - accessibility keywords apply normally. A `static` function is only
allowed to read `static` data and has to be put inside a `static` scope

You can also have `method` functions. Method functions are functions that's put directly inside the type scope. A
`method` function have a `this` pointer and a `base` pointer available inside the scope of the function.
You have to use `this` to access data that's available inside memory of the type.
You can rename how you access `this` by adding a `this` argument (see example below).

Consider the following:

```
import "stdio"

type User {
    var name string
    
    func new(name string) {
        this.name = name
    }
    
    func delete() {
        // do something
    }
    
    private innerGetName() string {
        return this.name
    }
        
    // alternate name for the this pointer calling another method in this type
    func GetName(u this) string {
        return u.innerGetName()
    }
    
    static {
        var five = 5
        
        func Get5() int {
            return User.five
        }    
    }
}

func main() {
    user := User("o2")
    
    print(`Hello ${user.GetName()}\n`)
    print(`Value is ${User.Get5()}`)
}
```

The `Get5` function is a static function and is available without a `User` instance. It does not have
access to the type's internal memory. If you mark the first argument as `this` then that function will be turned
into a `method` function.

The example itself will have the output:

```
Hello o2
Value is 5
```

If you want to call a method of the same type

### Virtual method functions

- [ ] add `virtual`, `abstract`, `override` and `final` prefix for functions
- [ ] add `vtable` for function calls when using inheritance
- [ ] add support for preventing overriding `final` functions
- [ ] add `base` keywords that represents the base class

Consider the following:

```
type Component {
    virtual func Init() bool {
        return true
    }
    
    abstract func Update(dt float32) string
}

type AnimatedComponent : Component {
    skeleton *Skeleton
    
    final func Init() bool {
        if (!base.Init()) {
            return false
        }
        // do something
        return true
    }
    
    override func Update(dt float32) {
        this.skeleton.Animate(dt)
    }
}
```

### Anonymous functions

- [ ] add support for anonymous functions using `f := func (){}` syntax
- [ ] add support for returning function pointers and anonymous functions
    - suggestions: `func GetFunc() Func<(int) string>`

### Delegates

- [ ] add support for delegate functions
- [ ] investigate how to handle memory
- [ ] investigate what to do if the method to be called is a virtual
- [ ] investigate if it should be possible to create a delegate from an interface pointer

Consider the following:

```
import "stdio"

type User {
    func GetName() string const {
        return "o2"
    }
}

func main() {
    user := User()
    d := user.GetName
    print(d())
}
```

The example itself will have the output:

```
o2
```

## Interface

- [ ] add support for `interface` symbol

Consider the following:

```
interface Service {
    func Call(name string)
}
```

The `Service` is an interface. We know this because of the `interface` keyword.

The content of an `interface` can only be virtual `method` functions, thus all functions are automatically
marked as `virtual` with a `this` argument.

Interface can be inherited using the type inheritance syntax. With the given example above
then well allowed to do:

```
type WebService : Service {
    override func Call(w this, name string) {
        // Do something
    }
}
```

## Lambda

- [ ] add support for anonymous function with scope using `l := lambda (){}`
- [ ] automatically create scope if no scope is provided
- [ ] allow for specifying scope manually for some use-cases
- [ ] add support for returning lambda expressions
    - suggestion: `func GetLambda() Lambda<(int)bool>`

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

## Attributes

- [x] add support for the `@attribute` syntax for functions
- [x] add support for the `@attribute` syntax for types
- [ ] add support for the `@attribute` syntax for type fields
- [ ] add support for the `@attribute` syntax for type methods
- [ ] add support for the `@attribute` syntax for type static functions
- [ ] add support for the `@attribute` syntax for constant variables
- [ ] add support for the `@attribute` syntax for function arguments
- [ ] add support for the `@attribute` syntax for global variables
- [ ] consider adding support for the `@attribute` syntax for local variables
- [x] custom attribute types: `type custom_attribute : attribute{}`
- [ ] add support for a way to configure attribute limitations, for example
  so that an attribute is only allowed to be added on functions and nothing else

### Attributes on types

Considering the following:

```
@custom
type my_type {}
```

This will add the attribute "custom" to the type. Attributes can be used efficiently after the resolve
phase. The compiler will raise an error if you specify a type where `attribute` isn't a base type.

### Custom Attributes

The language allows for custom attributes. A custom attribute **must** have the base type `attribute`.

Consider the following:

```
import "stdlib"

type custom : attribute {}
```

This will create an attribute named "custom" that you can attach on any [symbol](#Symbol).

## Symbol

Symbols:

- `type`
- `func`
- `var`
- `const`
- function and method arguments

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
- [x] resolving `primitive` types can be during parse

### Not complete

- [ ] each new expression must be put on a new line
- [ ] only resolve sizes for unknown types at the end. Some times can be resolved during parse phase
  (only primitives etc.).
    - how should fields in types be handled?
- [ ] `if` statement
- [ ] `for` loops
- [ ] `switch` statement
- [ ] `cast` between primitives
- [ ] `date` and `time` operations
- [ ] types that inherits from primitives
- [ ] template syntax using `<` and `>`
- [ ] `yield` stream statement in function
- [ ] `async` and `await` suspension statements
    - should be triggered by interrupts from drivers (sockets, io, ...)
- [ ] types in `extern` libraries that use inheritance
    - expose vtable
- [ ] `panic` statements
- [ ] full multi-threaded support
    - [x] multi-thread friendly source code parse
    - [ ] allow for multi-threaded resolve (maybe)
- [ ] code complexity analyser and reporting
    - will be needed anyways for the optimizer to be good

### Consideration / Evaluation

- [ ] exceptions or `error` return value?
    - for example, exceptions should be considered a fatal error so why not panic?
- [ ] consider having separate types for `struct` and `class`.
    - structs will not have a virtual table, so virtual functions of any kind are not allowed and only compile-time
      casting is allowed
    - classes will have virtual table and runtime-based casting is allowed
    - inheritance between classes and struct should not be allowed
- [ ] generate source code based on syntax tree
  - might be useful for macro-troubleshooting 
