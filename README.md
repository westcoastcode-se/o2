# o2

[License](LICENSE.txt)

## Features

### Completed

- [x] decide language name
- [x] `primitives`, such as int32, float32
- [x] `type` structs without inheritance
- [x] simple `func` function definitions
- [x] `package`
- [x] `import`
  - [x] partial imports
- [x] types
- [x] recursive symbol querying
  - imports
  - inner types
  - collision detection
- [x] binary and unary operations using basic operators `+`, `-` etc.
- [x] `return` statement
- [x] `package` root is the same as the project root
- [x] `package` names are now figured out from the module name and path
  - modules should be put as children under root, unless they are private
- [x] `import` statements should be the path to the module + package path
- [x] resolving types should not also calculate the type size
- [x] do not require `;`

### Not complete 

- [ ] each new expression must be put on a new line
- [ ] only resolve sizes for unknown types at the end. Some times can be resolved during parse phase 
      (only primitives etc.).
  - how should fields in types be handled?
- [ ] private `modules`
  - should give you the power of having multiple versions of the same requirement
- [ ] resolving `primitive` types can be during parse
- [ ] `if` statement
- [ ] `for` loops
- [ ] `switch` statement
- [ ] `cast` between primitives
- [ ] unicode characters
  - should this be `char` or should a wide character type be added?
- [ ] `date` and `time` operations
- [ ] types that inherits from primitives
- [ ] template syntax using `<` and `>`
- [ ] `macro` functions
- [ ] `macro` statement in functions
- [ ] `yield` stream statement in function
- [ ] `async` and `await` suspension statements
  - should be triggered by interupts from drivers (sockets, io, ...)
- [ ] structs with inheritance
- [ ] compile-time `extern` functions linkage
- [ ] runtime `extern` functions linkage
  - if called and not linked then `panic`
- [ ] types in `extern` libraries that use inheritance
  - expose vtable
- [ ] `panic` statements
- [ ] full multi-threaded support
  - [x] multi-thread friendly source code parse
  - [ ] allow for multi-threaded resolve (maybe)
- [ ] attributes using the `[myattrib]` syntax
- [ ] argument name specification using `call(name_arg = "test")`
- [ ] code complexity analyser and reporting
  - will be needed anyways for the optimizer to be good 
- [ ] add support for custom `include` search directories
- [ ] add project descriptor `qw.proj`
  - compiler version descriptor
  - dependencies (fetched using `git`)
  - pre- and post compile processors
- [ ] add project dependencies  
- [ ] do not allow accessing non-public symbols outside package
- [ ] add `trait` types used as an alternative to template to solve some of the template
      specializations done in C++, for example the `operator<<` for std::ostream

### Consideration / Evaluation

- [ ] exceptions or `error` return value?
  - for example, exceptions should be considered a fatal error so why not panic?

