LKL: Lin-Kernighan-Lee

This is a TSP solver that implements a k-opt hill climber in the style of Lin-Kernighan (LK).

The 2 main advantages over LK and LKH:
1. More efficient. Linearithmic vs. O(N^2)
2. True sequential, non-disconnecting optimality. LK and LKH use fixed neighborhoods (e.g. 5 points) around each point, whereas LKL uses a quadtree to efficiently determine neighborhoods dynamically, so that true sequential non-disconnecting optimality is achieved.

The main disadvantage of the current implementation (not LKL in principle) is that this solver does not currently search for non-sequential nor disconnecting sequential moves. However, this can be easily added, similar to how it is added in LK and LKH.

Explanation of the different types of k-opt moves:

A k-opt move is where a tour is improved by deleting k existing edges and adding k new edges.

There are 2 classes of k-opt moves: non-sequential and sequential.
Non-sequential moves are where new and old edges are disjoint (e.g. double-bridge move).
Sequential moves are where new and old edges are not disjoint, and therefore one could 'walk' the entire move by moving alternate additions and deletions.

Furthermore, there are 2 classes of sequential moves: disconnecting and non-disconnecting.
In a non-disconnecting move, the move can be stopped at any time and form a valid tour. In a disconnecting move, stopping in the middle of it may break the tour into one or more cycles.

LKL currently only finds sequential, non-disconnecting moves. However, unlike LK and LKH, LKL is able to solve for sequential, non-disconnecting optimality, thanks to dynamic search neighborhoods. Fixed neighborhoods (e.g. 'nearest' 5 points) may cause the solver to miss some improving moves.

A little more detail on the advantages:

1. Increase efficiency by using a quadtree to dynamically determine and limit the k-opt search,
    such that candidate edges that violate the LK gain criterion are skipped.
    In other words, all possible improving steps (deletion of one edge and addition of one edge) are considered.
    This approach is novel because fixed search neighborhoods are typically used.
    Original LK limited search neighborhoods to 5 nearest points. LKH limits search to 5 "alpha-nearest" points.
    Alpha-nearness measures the similarity of new edges to those in a minimum spanning tree.
    Computing alpha-nearness is done once before hill-climbing and takes O(n^2) work.
    I estimate the work complexity of each improvement step in the present implementation to be linearithmic,
    perhaps O(n * (log(n) ^ (kmax - 1))).
    For those unfamiliar with LK gain criterion, it is essentially an improvement upon naive k-opt search for sequential moves.
    Sequential moves are moves that can be made by alternating deletion and addition of edges that share a common point.
    LK looks directly for a feature of improving moves, which is that there is at least 1 new edge that shares a common point with a longer deleted edge.
    This prunes out a lot of the k-opt search space.

2. Preserve sequential-move k-optimality. LK / LKH gives up sequential-move k-optimality.
    Note that sequential-move k-optimality is equivalent to full k-optimality for k in [2, 3].
    For k > 3, non-sequential moves are possible, but are not checked in this solver.
    Finding higher-opt moves with this implementation is probably 'slower' than in LKH (disregarding LKH's initial O(n^2) MST creation),
    but there might be moves that are found by this implementation that would be undetected in LKH. Proof:
        Using 3-opt hill-climber on first world record (by LKH) of lrb744710 quickly returns improvements.
        Although it is an old world record, it proves that LKH (and LK) gives up k-optimality by arbitrarily fixing search neighborhoods.
        http://www.math.uwaterloo.ca/tsp/vlsi/lrb744710.log.html

Compilation:
1. Make sure "CXX" in "makefile" is set to the desired compiler.
2. Run "make".

Running:
1. Modify config.txt to point to your tsp instance and (optional) tour file.
2. Run "./lkl.out" for usage details.

Style notes:
1. Namespaces follow directory structure. If an entire namespace is in a single header file, the header file name will be the namespace name.
2. Headers are grouped from most to least specific to this repo (e.g. repo header files will come before standard library headers).
3. Put one line break in between function definitions for convenient vim navigation via ctrl + { and ctrl + }.
4. Filenames are lower case and underscore-separated.
5. Header extension is .hh and source extension is .cc.
6. Reference and pointer symbols are right-associated, e.g. "T &ref" vs. "T& ref".
7. Exceptions are not caught.
8. Compile-time constants are capitalized with underscores.
9. Member variables are indicated as such with trailing underscores.
10. Scope start brace is not new-line separated.
11. Prefer English-word boolean operators: "and", "not".
