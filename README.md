# SPC
A Simple PL/0 Compiler


The PL/0 is similar to but much simpler than the general-purpose 
programming language Pascal, intended as an educational programming language. 


The following is the syntax rules of the model language defined in EBNF:

```
	program = block "." .

	block = [ "const" ident "=" number {"," ident "=" number} ";"]
		    [ "var" ident {"," ident} ";"]
		    { "procedure" ident ";" block ";" }
		    { "function" ident ";" block ";" }
		    statement.

	statement = [ ident ":=" expression
		          | "begin" statement { ";" statement } "end"
		          | "if" condition "then" statement [ "else" statement ]
		          | "do" statement "while" condition
		          | "for" ident ":=" expression "to|downto" expression "do" statement].

	condition = expression ("="|"<>"|"<"|"<="|">"|">=") expression .

	expression = [ "+"|"-"] term { ("+"|"-") term}.

	term = factor {("*"|"/") factor}.

	factor = ident | number | "(" expression ")".
```
