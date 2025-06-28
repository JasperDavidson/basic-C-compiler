Building a basic C compiler to get familiar with the entire process, from lexing and parsing to optimized code generation. After this I hope to build something unique!

Currently, this compiler supports only constant operations. Arithmetic, unary operations like negation, and other binary operations like comparisons (==, <=, >=, !=, etc.) and logical
operators (like &&, ||, etc.).

Now that a fair amount of basic operations have been added in, I'm currently working on supporting local variables!

Currently this compiler is *literal*. It will take the C code and translate it directly to ARM 64-bit assembly. Obviously, for all these constant expressions a modern compiler would
implement an optimization like constant folding. These optimizations are what I'm excited to tackle once I get a bit further into the project.
