# MathParser
Vectorized and (hopefully) efficient expression evaluation of C++ strings

Native data structure is QuickArray, a lightweight wrapper around std::vector.

## Syntax guide
### QuickArray creation
**Manually specify contents of QuickArray:**

`x = <1,2,3>` or `x = <1 2 3>`

**TO-DO**

**Create QuickArray from repeated elements:**

----

`x = 10 of <2>` and `x = 10 of 2` are the same as 

`x = <2 2 2 2 2 2 2 2 2 2>`.

----

`x = 3 of <1 2 3>` is the same as 

`<1 2 3 1 2 3 1 2 3>`

### Array operations
All standard operations are automatically performed element-wise and support broadcasting.

----

`x = 5*<2 3 4>` => `<10 15 20>`

----

`b = 5 of <2>`

`e = <1 2 3 4 5>`

`b^e` => `<2 4 8 16 32>`



