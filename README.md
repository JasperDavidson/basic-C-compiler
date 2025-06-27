Building a basic C compiler to get familiar with the entire process, from lexing and parsing to optimized code generation. After this I hope to build something unique, maybe to do with
JIT compilation!

Currently, this compiler supports only constant operations. Arithmetic, unary operations like negation, and other binary operations like comparisons (==, <=, >=, !=, etc.) and logical
operators (like &&, ||, etc.).

Next I'm adding a few more operators (like bitshifting, modulo, etc.) and then I'll move onto local variables!

Currently this compiler is *literal*. It will take the C code and translate it directly to ARM 64-bit assembly. Obviously, for all these constant expressions a modern compiler would
implement an optimization like constant folding. These optimizations are what I'm excited to tackle once I get a bit further into the project.
