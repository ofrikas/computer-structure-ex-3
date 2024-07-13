Assignment - Second Year Computer Science

This project was completed during our second year in the Computer Science degree program. The assignment consists of three main parts:

1. Cache Simulation (cache folder)
2. Optimization (optimization folder)
3. Patching (patch folder)

-------------------
1. Cache Simulation
In this part of the assignment, we simulate how a cache operates. The goal is to define two structs to represent cache sets and the cache itself. These are fundamental to understanding cache behavior and efficiency.


---------------
2. Optimization
This section tackles the problem of finding a minimal Vertex Cover, which is known to be NP-Hard. Given a graph G = (V, E) presented as an adjacency matrix A ∈ {0, 1}^{|V| × |V|}, the task is to determine the minimal number of vertices needed to touch every edge in the graph.

You will find an unoptimized implementation in vertex-cover.c, which includes:
- isVertexCover: Checks if a proposed cover is a valid set cover.
- generateCombinations: Recursively generates possible combinations of covers.
- findMinVertexCover: Main function that prints the minimum set cover.


-----------
3. Patching
For this section, we were given a binary file named patchwork. The task was to patch this binary to make it print a secret flag. We used reverse engineering tools, specifically Cutter, to accomplish this.

Steps Taken:
- Tool Selection: Used Cutter for reverse engineering.
- Initial Analysis: Loaded the binary into Cutter and performed an initial analysis.
- Locating the Flag Check: Identified the code section responsible for the flag check.
- Understanding the Condition: Found a conditional jump instruction critical to the control flow.
- Binary Patching: Modified the value moved into the register checked by the jump condition, ensuring the program would print the flag.
- Testing the Patch: Saved and executed the patched binary to verify the correct output.

The detailed steps and reasoning are documented in the file cheating_is_bad.txt.


----------
Conclusion

This project showcases our skills in cache simulation, optimization problems, and binary patching. Each section builds on the principles and techniques learned throughout the course.

the full assiment is on the pdf 
    [text](homework3.pdf)

