# Compiler-and-lexer
Building a lexer, the first element of any compiler


`make test`
This will run your code against the inputs present under test/input and generate results under
test/result.
Below, I am attaching an example of a sample input and corresponding output:
Input:
var y := 20;
Output:
Keyword('var') at 1:1
Identifier('y') at 1:5
AssignOp(':=') at 1:7
Integer('20') at 1:10
Delimiter(';') at 1:12
EOF('') at 1:12
