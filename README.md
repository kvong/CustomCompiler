# Custom Compiler

### Description: 
* Compiles UMSL's custom .asm file using a custom high-level language as input(stdin/input.file). Detailed notes are provided below.

### How to compile:
* Go to `./src` directory.
* Run `make`.

### How to run:
1 `cd src/`
2 Run `compile [file]` to translate our language to machine code.
3 Run `vm [outputfile]` and observe the outout of the machine code.

### Location of generated '.asm' file:
* Location of .asm file is where the input file is given.
* STDIN will generate .asm file in current directory.

### To escape from keyboard input:
* CTRL + D

# Components

## 1. Scanner
* Scanner with one look ahead that return a single token and its type.

### Lexical Definitions
* All case sensitive
* Alphabet:
    * All upper- and lower-case English letters, digits, plus the extra characters as shown below, plus WhiteSpace
    * No other characters are allowed and they should generate errors
    * Each scanner error should display "Scanner Error:" followed by details including line number
* Identifiers
    * begin with a lower-case letter (a-z) and
    * continue with one or more letters or digits
        * e.g. i3, aDD8, z920, ifa, are all valid and i, A3, 382, are all invalid identifiers
    * you may assume no identifier is longer than 8 characters
* Keywords (reserved, suggested individual tokens)
    * `begin end repeat if then let void var return scan write program`
Operators and delimiters group
    * `= < > == : & + - * / # . [ ] ,`
* Numbers
    * any sequence of decimal digits (0-9), no sign
    * you may assume no number longer than 8 characters
* Comments start with `%` and end with `%`

### Filters
* Input will be filtered before and after the scanner operation.
* Filter 1:
    * Track line number
    * Map character to FSA Table column number
    * Skip over comments
* Filter 2:
    * If token is an identifyer, then check to see if it's a keyword.

## 2. Parser
* Parser is responsible for creating a tree of the tokens created from the scanner. The structure of the tree is based on the structure of the code block given in the input.

### BNF Grammar
```
<S> -> program <V> <B>
<B> -> begin <V> <Q> end
<V> -> empty | var identifier . <V>
<M> -> <H> + <M> | <H> - <M> | <H> / <M> | <H> * <M> | <H>
<H> -> & <R> | <R>
<R> -> identifier | number
<Q> -> <T> # <Q> | empty
<T> -> <A> , | <W> , | <B> | <I> , | <G> , | <E> ,
<A> -> scan identifier | scan number
<W> -> write<M>
<I> -> if [ <M> <Z> <M> ] <T>
<G> -> repeat [ <M> <Z> <M> ] <T>
<E> -> let identifier : <M>
<Z> -> < | > | : | = | ==

```

## 3. Static Semantics
* Traverse the tree created by the parser to verify that the semantic is correct.

## 4. Code Generation
* After we verify that everything is correct and there are no syntax errors, generate the code.
Grammar Semantics
* Program executes sequentially from beginning to end, one statement at a time
* Delimiters:
    * `. , # [ ]`
* Less than and greater than are standard symbols
    * `< >`
* `&` means absolute value for number
    * Number may be immediate or stored at location indicated by identifier
* The following operators have standard definitions, except no precedence
    * `+ - * /`
    * Operations are applied from left to right
* `let identifier : <M>` assigns the value of M to the given identifier
* `=` and `==` are both treated like standard `==`
    * Assignment of values to variables can only be performed using `[let] identifier : <M>`
* `if [ <M> <Z> <M> ] <T>`
    * Means to do `<T>` if and only if `(<M> <Z> <M>)` is true (always true if `<Z>` is `:`)
* `repeat [ <M> <Z> <M> ] <T>`
    * Means to repeat `<T>` until `(<M> <Z> <M>)` is false
* When scanning an identifier, allocate memory using the identifier as the variable name and set value to zero
* When scanning a number, allocate memory using a temporary variable name and set value to the number

