# heist-planner-cpp
Optimization problem combining BFS (graph components) and dynamic programming (subset sum, knapsack
## Problem Description

The goal of this project is to select an optimal subset of vaults to rob in order to maximize total value while staying within a given time constraint.

Each vault represents a non-trivial subproblem:

It contains a locking mechanism modeled as a graph.
Nodes represent pebbles, and edges represent allowed connections.
Some connections are missing, and the vault can only be opened if the pebbles can be split into two equal groups such that:
every pebble on one side is connected to all pebbles on the other side.

Unlocking a vault takes time proportional to the number of moved pebbles.
Moving between vaults also incurs a fixed time cost.

## Graph Processing (BFS)

To determine whether a vault can be opened:
The graph is constructed from the list of missing connections.
Breadth-First Search (BFS) is used to find connected components.
Each connected component represents a group of pebbles that must stay on the same side.

## Subset Selection (Dynamic Programming)
The problem is reduced to selecting a subset of connected components such that:
their total size equals exactly half of all pebbles.
This is solved using dynamic programming (subset sum DP).
If such a subset exists:
the vault is considered unlockable
and a valid unlocking configuration is reconstructed
