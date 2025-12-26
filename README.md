This small c++ project implements a markup language parser defined as follows:
- markup documents contain a set of tags and plain text
- Tags define a label and a scope in the form of <label>scope</label>.
- The symbols <, > and / are only allowed in tag declaration, but not in tag labels or in clear text.
- A tag scope may contain either additional tags or plain text, but not both.
- A tag scope may only end after the scope of all it’s inner tags has ended.
- A document should always start with a root tag labeled doc.
- White spaces are allowed anywhere outside tag declaration.

This project also satisfies the following requirements:
- the executable should accept a file path as a command line argument.
- the input file should be parsed into a tree of tags and plain texts.
- if the input file confirms to the CMML definition, the output should be all the plain texts in BFS traversal order one text per line with no additional messages or white spaces.
- if the input file does not confirm to the CMML definition, the output should be an error message with the line and column at which the document violated the definition in the format Error line # column #.
- the solution is optimized for shortest execution time on a single CPU core (no multi-threading or other hardware specific optimizations).

A number of input files are included for testing purposes in the subdirectory `test`, as well as their output in subdirectory `test/result` (e.g., 'valid_1.out' is the output of 'valid_1.txt')

Description and results of test cases (in ./test)

 valid_1.txt: basic pair of labels with no plain text

 valid_2.txt: basic pair of labels with one line of plain text on separate lines

 valid_3.txt: basic pair of labels with one line of text, all on the same line

 valid_4.txt: two pairs of labels with no plain text, all on the same line

 valid_5.txt: basic pair of labels with two lines of plain text, all on separate lines

 valid_6.txt: basic pair of labels with four lines of plain text, all on separate lines

 valid_7.txt: three pairs of labels with plain text, all on the same line

 valid_8.txt: three pairs of nested labels with plain text, all on the same line


 invalid_1.txt: invalid character in line 1 col 5 (should be '>')

 invalid_2.txt: missing character in line 1 col 5 (should be '>')

 invalid_3.txt: invalid whitespace in line 1, col 4

 invalid_4.txt: whitespace in line 1 col 1 (ok), invalid character in col 2 (should be '<')

 invalid_5.txt: invalid character in line 2, col 5 in plain text  ('/')

 invalid_6.txt: invalid label in line 3 col 3 (should be 'l' given the previous characters)

 invalid_7.txt: missing last label (</doc>)

 invalid_8.txt: invalid label in line 1 col 41 (should be '2' given the previous characters)
 