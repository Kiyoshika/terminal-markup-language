# TSL Specification (Terminal Scripting Language)
Version 0.0.1 - 29 July 2023

To see older oversion of the spec, see the [TSL Spec Archive](#tslspec-archive/)

TSL is embedded in a `.tml` file within the `<script>` tags.

It's a statically-typed scripting language to interact with the elements on the user interface.

Code is checked at "render time" (when `./tmlrender [file]` is used) for any errors; if any errors are present, the renderer will not be invoked and instead an error will be shown to the user.

# API Reference

### Introduction
* [Reserved Keywords](#reserved-keywords)
* [Basic Types and Variables](#basic-types-and-variables)
* [Global and Local Scopes](#global-and-local-scopes)
* [Variable References](#variable-references)
* [Null Values](#null-values)
* [Arrays](#arrays)
* [Functions](#functions)
* [Control Flow](#control-flow)
* [Arithmetic](#arithmetic)
* [Loops](#loops)
### Types
* [String Type](#string-type)
* [Int Type](#int-type)
* [Float Type](#float-type)
* [Bool Type](#bool-type)
* [Static Arrays](#static-arrays)
* [Dynamic Arrays](#dynamic-arrays)
### Objects
* [Text Object](#text-object)
* [Input Object](#input-object)
* [Button Object](#button-object)
### Misc
* [Future Plans](#future-plans)

## Introduction
### Reserved Keywords
This is a list of keywords that cannot be used for variable or function names:
* `int`
* `string`
* `float`
* `void`
* `null`
* `bool`

### Basic Types and Variables
There are a few funtamental basic types:
* `string`
* `int`
* `float`
* `bool`
* `void` (only used in functions)

Creating variables is as follows:
```
<script>
  string hello = "hello";
  int value = 12;
  float value2 = 3.14159;
</script>
```

Variable names must start with an ASCII letter, but can contain both letters and numbers. Variable names cannot used a [reserved keyword](#reserved-keywords).

In TSL, there is no implicit casting. Conversions would have to be done explicitly:
```
<script>
  string value = "12";
  int? value2 = value; // <-- error
  int? value2 = value.toInt(); // proper
</script>
```

NOTE: don't worry much about the `?` after the type right now, it's explained [later](#null-values).

Variables are initialized to default values if not done explicitly:
```
<script>
  string strval; // defaults to ""
  int intval; // defaults to 0
  float floatval; // defaults to 0.0
</script>
```

Literals will always be interpreted as the non-nullable type. E.g:
```
123 -> int
"hey" -> string
3.14159 -> float

<script>
  void myFunc(int? a) {
    // ...
  }

  void myCallback() {
    // valid since 10 -> int which is assignable to int?
    myFunc(10);
  }
</script>
```

It's illegal to take references of literals:
```
<script>
  void myFunc(string a) {
    // ...
  }

  void otherFunc(string& a) {
    // ...
  }

  void myCallback() {
    myFunc("hello"); // ok, literal is allocated and transfered to 'a'
    otherFunc("hello"); // error, cannot take reference of literal

    int& x = 5; // error, cannot take reference of literal
  }
</script>
```

### Global and Local Scopes
Any variables created outside of a function exists in `global` scope and can be accessed anywhere:
```
<script>
  int value = 12;

  void myFunc() {
    value = 13; // ok because value is global
  }
</script>
```

If there are name collisions between a global variable and a local variable/parameter, the `global` keyword must be used to disambiguate:
```
<script>
  int value = 12;

  // disambiguate with parameter
  void myFunc(int value) {
    value = 13; // applies to parameter
    global value = 13; // applies to global
  }

  // disambiguate with local
  void myFunc2() {
    int value = 13; // local variable, separate from global
    global value = 13; // applies to global
  }
</script>
```

It's a good practice to use some type of prefix such as `g` for globals to avoid these types of collisions:

```
<script>
  int gValue = 12;

  void myFunc(int value) {
    value = 13;
    gValue = 13;
  }
</script>
```

Name collisions between parameters and local variables will be an error:
```
<script>
  void myFunc(int value) {
    int value = 12; // <-- error, same name as parameter
  }
</script>
```

### Variable References
Just like C++, we can pass variable references to functions. Although, the syntax is slightly different.

```
<script>
  void setValue(int& value) {
    value = 10;
  }

  void myCallback() {
    int myLocal = 0;
    setValue(&myLocal);
    // myLocal is now 10
  }
</script>
```

In TSL, use the `&` operator to turn a variable into a reference. This is required to pass variables to a function that expects a reference which will have a signature of `[type]&` such as `int&` or `string&` etc.

Unlike C++, TSL is more strict on the placement of `&`. `int& variable` is valid while `int &variable` is not valid.

References could also be treated like variables:

```
<script>
  void myCallback() {
    int value = 10;
    int& refValue = &value;
    refValue = 20;
    // 'value' is now also 20 since we modified its reference
  }
</script>
```

Assigning a non-reference to a reference will copy the value pointed to by the reference:
```
<script>
  void myCallback() {
    int value = 10;
    int& refValue = &value;

    int valueCopy = refValue; // refValue gets "dereferenced" and copied
    
    refValue = 20;
    // value is now 20
    // valueCopy is still 10
  }
</script>
```

### Null Values
The value `null` represents an empty value, different from `0`. It is typically used in the API as a failure condition, e.g., failed to parse a string to int.

Null values can only be used on a type with the `?` operator:

```
<script>
  int value = null; // <-- error, value not marked as nullable
  int? value2 = null; // ok
</script>
```

Reassignment cannot be done on from a nullable to non-nullable type, but is allowed the other way around:
```
<script>
  int value = 12;
  int? value2 = 20;
  int? value3 = 30;

  value = value2; // <-- error, can't assign a nullable type to a non-nullable type

  value2 = value; // ok
  value3 = value2; // ok

</script>
```

Passing a `null` literal will only be valid to nullable types and will error if trying to assign to a non-nullable type:
```
<script>
  void myFunc(float? a) {
    // ...
  }

  void otherFunc(float a) {
    // ...
  }

  void myCallback() {
    myFunc(null); // ok, float? is nullable
    otherFunc(null); // error, float is not nullable
  }
</script>
```

### Arrays
Arrays hold a contiguous block of memory of the same type. Unlike `string`s, stack and heap arrays are supported.

Stack arrays cannot be resized, and must be sized upfront.

The size becomes a part of their type. In this case, the type is `int[5]` which is different than `int`.

```
<script>
  int[5] myArr; // defaults to { 0, 0, 0, 0, 0 }
  int[5] myArr2 = { 1, 2, 3, 4, 5 };
</script>
```

Dynamic arrays can be resized and must be specified with no size.

These will have type `int[]` with no specified size, differentiating them from stack arrays which have a pre-defined size.

```
<script>
  int[5] stackArray;

  int[] dynamicArray; // defaults to {}, empty array
  dynamicArray.append(3); // { 3 }

  int[] dynamicArray2 = { 1, 2 };
  dynamicArray2.append(3); // { 1, 2, 3}
</script>
```

Access elements with the `[]` operator:
```
<script>
  int[5] arr = { 1, 2, 3, 4, 5};
  arr[0] = 2;
</script>
```

References to arrays can also be taken:
```
<script>
  int[5] stackArray = { 1, 2, 3, 4, 5 };
  int[5]& ref = &stackArray;

  ref[0] = 10; // also modifies original stackArray
</script>
```

### Functions
The general structure of a function is:
```
return-type name(type arg1, type arg2, ...) { }
```

For example, a function that takes two integers and returns an integer
```
<script>
  int myFunc(int a, int b) {
    return a + b;
  }
</script>
```

You can also accept/return optional types:
```
  int? myFunc(int? a) {
    return a + 1;
  }
```

As mentioned in [null values](#null-values) section, a function that as a `int` return type cannot return a `int?` type, but the other way around is fine:

```
  // ok
  int? myFunc(int a) {
    return a;
  }

  // error, cannot convert a nullable type to a non-nullable type
  int myFunc(int? a) {
    return a;
  }
```

References can be used in parameters but cannot be returned:
```
  // ok
  int myFunc(int& a) {
    return a; // returns a copy of the value pointed to by a
  }

  // error, cannot return references from functions
  int& myFunc(int& a) {
    return a;
  }
```

Whenever variables are passed as parameters, a copy is made (unless passing by reference):

```
<script>
  // a is copied
  // b is copied (to another heap buffer)
  // c is passed by address and not copied
  // d is passed by address and not copied
  void myFunc(int a, string b, int& c, string& d) {
    a = 20; // no effect on original a
    b.toUpper(); // no effect on original b
    
    c = 20; // affects original a
    d.toUpper(); // affects original b
  }

 void start() {
    int a = 10;
    string b = "hello";
    myFunc(a, b, &a, &b);
  } 
</script>
```

`void` type functions can return early with an empty `return`. This is useful for guard clases:
```
<script>
  void myFunc(int a) {
    if (a < 50) { return; }

    // continue...
  }
</script>
```

### Control Flow
You can branch your logic with typical `if` statements:
```
if (expression) { // i.e., if expression is true
  // ...
} else if (expression) {
  // ...
} else if (expression) {
  // ...
} else { // <-- optional
  // ...
}
```

These will be evaluated linearly; if the first expression isn't met, it will test the second, then the third, etc.

You can use logical OR `||` and logical AND `&&` for more complicated expressions.

```
if (expression1 || (expression2 && expression3)) {
  // ...
}
```

You can use logical NOT `!` to invert expressions (true to false and vice versa)

```
if (!expression) { // i.e., if expression is false
  // ...
}
```

You can test equality with `==`. For integers, it's a straightforward comparison. For floats, it will do an espsilon check (e.g., check if `|a - b| < eps`) and for strings it will compare each character.

Different types cannot be used in equality comparison

```
float a = 3.15;
float b = 3.15;
string c = "hey";

if (a == b) { // perform epsilon comparison
  // ...
}

// error, cannot compare float with string
if (a == c) { 
  // ...
}
```

Comparing a reference to a reference will compare the addresses:
```
int value = 10;
int value2 = 20;
int& r1 = &value;
int& r2 = &value;
int& r3 = &value2;

r1 == r2; // true
r1 == r3; // false
```

Static and dynamic arrays can also be compared. First the size will be checked, and if sizes are equal, elements will be compared.

This allows to compare static arrays with dynamic arrays.

```
int[5] a1 = { 1, 2, 3, 4, 5 };
int[5] a2 = { 1, 2, 3, 4, 5 };
int[5] a3 = { 2, 4, 5, 3, 2 };
int[4] a4 = { 1, 2, 3, 4 };
int[] a5 = { 1, 2, 3, 4, 5 };

a1 == a2; // true
a1 == a3; // false
a1 == a4; // false
a1 == a5; // true
```

The same rules apply to all other logical operators: `<`, `<=`, `>`, `>=`, `!=`


Comparing a reference to a value will dereference before comparison:
```
int value = 10;
int& r1 = &value;

if (r1 == value) { // true since r1 gets dereferenced before comparison
  // ...
}
```

### Arithmetic
All basic arithmetic operators are supported: `+`, `-`, `/`, `*`

Arithmetic must be performed on the same base type, e.g.,:
* `int` to `int`
* `int&` to `int`
* `int` to `int&`
* Likewise for the other types

Arithmetic from reference to reference (e.g., `int&` to `int&`) is not allowed and will error.

If performing arithemtic with a reference and value type, the reference will be dereferenced before the operation:
```
int value = 10;
int value2 = 20;
int& rValue2 = &value2;

// newValue = 30 since rValue2 gets dereferenced before addition
int newValue = rValue2 + value; 
```

Arithmetic on static arrays of same size will perform element-wise operations:

```
int[5] a1 = { 1, 2, 3, 4, 5};
int[5] a2 = { 1, 2, 3, 4, 5};
int[5] a3 = a1 + a2; // { 2, 4, 6, 8, 10 };
```

Arithmetic on dynamic arrays is not supported and will error.

### Loops
TODO: finish

## Types
### String Type
**Creation:**
```
string variableName = "value";
```

**Reassignment:**
Reassignment will create a copy of the buffer and pointed to by the new string:
```
string a = "hello";
string b = a; // copy of a is made and pointed to by b
```

To avoid copies, use references:
```
string a = "hello";
string& b = &a; // b and a now point to same string
b.toUpper(); // also modifies 'a'
```

**Methods:**
* `int? toInt()` - attempts to convert buffer to int, returns `null` on failure
* `float? toFloat()` - attempts to convert buffer to float, returns `null` on failure
* `void toUpper()` - sets all lowercase ASCII characters to uppercase; modifies string inplace
* `void toLower()` - sets all uppercase ASCII characters to lowercase; modifies string inplace
* `string[] split(string delim)` - split a string on a delimiter which returns a dynamic string array of all tokens. If no delimiter is found, returns a dynamic array with the first element being the string itself.

### Int Type
The `int` type is a signed 32-bit integer.

**Creation:**
```
int variableName = 10;
```

**Methods:**
* `string toString()` - converts the integer into string representation
* `float toFloat()` - converts the integer into float representation

### Float Type
**Creation:**
```
float variableName = 1.234;
```

**Methods:**
* `string toString()` - converts the float into string representation
* `int toInt()` - converts the float into int representation

### Static Arrays
TODO: finish

### Dynamic Arrays
**Creation**:
```
type[] variableName = { ... };

// e.g.,
int[] = intArray = { 1, 2, 3 };
```

**Reassignment:**
Reassignment will deallocate the original array and create a new copy to be pointed to by the new array.
```
int[] a = { 1, 2, 3 };
int[] b = { 1, 2, 3, 4, 5 };

b = a; // original b array is deallocated and a new copy of { 1, 2, 3 } is created
```

To avoid copies, use references:
```
int[] a = { 1, 2, 3 };
int[]& b = &a; // b and a point to the same array

b[0] = 10; // also modifies a[0]
```

**Methods:**
* `int length()` - get the length of the array
* `void append(type value)` - append value with type `type` at the end of array, dynamically growing in size
* `void clear()` - clear the contents of the array
* `int find(type value)` - searches the array for the first occurrence of a value with type `type`. If found, returns the index where it was found, otherwise returns `-1`.
* `int[]? findAll(type value)` - searches the array for all occurrences of a value with type `type`. If none were found, returns `null`, otherwise returns a dynamic int array containing all indices of the value.

## Objects
TODO: finish this section

## Misc
### Future Plans
Some general items planned for the future that are not really documented here yet:
* math functions (`log10`, `ln`, `exp`, `pow`, etc.)
* classes (i.e., user-defined types)
