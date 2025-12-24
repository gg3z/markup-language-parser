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

 A number of input files are included for testing purposes, as well as their output (e.g., 'valid1.out' is the output of 'valid1.txt')