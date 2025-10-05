-	Paul Heyden (ph2715)

-	Homework 1

	LEXICAL SCANNER

	1. My lexical scanner implementation is defined in `lexer.cpp`.
        It scans the input (here, from arbitrary files), identifies
        tokens, and determines which line and column the token starts
        at and whether the token is a keyword, operator, identifier,
        integer, delimiter, assignment operator, comment (which it
        skips), or end-of-file. It stores each token in the lexer's
        `tokens` vector, and returns a `LexerException` if the token
        is not among the types listed above.
	2. The main challenges I faced at the beginning of the
        assignment included coordination between reading in
        an assortment of different tokens. Once I realized that the
        main work of the lexer's task could be accomplished with
        a switch statement, the task was simplified; a key initial
        insight was that the program must continually read from
        stdin until a new token, whitespace, or EOF is encountered.

        Another stumbling block was the implementation of the
        maximal munch rule. Here it was important to implement
        a helper function which could look at the next character
        without assigning it to the lexeme yet. In this way,
        it became possible to see where a specific token type
        stops while still following the maximal munch rule:
        we must simply keep assigning more characters to the
        given token until whitespace is encountered or a
        character which does not fit into the token type is read.

        Additionally, the error tests were initially confusing for me.
        One stumbling block here was the double equals sign, but I 
        eventually realized it can be treated in a similar way as the
        assignment and inequality operators, except unlike the
        assignment operator (in which `:` is a delimiter if not followed
        be an equals sign), if the token's first character is `=`,
        simply look at the next character and if it is not also `=`, we
        throw an error.
	3. In this assignment, I learned the rudiments of how any
        program is read into a compiler. This is done by reading 
        input, identify tokens, and parse errors, thus 
        enabling program text analysis. From testing, I learned
        how error recognition (such as identifying illegal
        characters) works in basic forms.
    4. To test equals sign logic, I first tested
        the code by implementing checks, such as having
        three equals signs, equals signs after illegal operators,
        and illegal double delimiters. I then focused on edge cases
        with the comments, such as having several comments in a row,
        comments mid-line, comments at the end of lines, and comments on
        the last line. I also tested extensively for maximal munch
        cases, such as checking whether my lexer picked up that
        `function` is actually an identifier, not the keyword `func`,
        etc. I also tested for code with no whitespace, to see if new
        delimiters were picked up as new tokens.