![Main view](/images/title.png)

Course project for ECE 565: Performance Optimization & Parallelism.

Shared memory parallel programming with threads.

Uses C++ (with threads library), and performs time step simulation, simulating what happens across
the 2D landscape in each time step as raindrops fall onto the points, trickle to lower elevation
points, and get absorbed into the ground.

Two versions: sequential and parallel.

# Output
![Main view](/images/output.png)

# Speedups on multi-threads
![Main view](/images/measure.png)

# Distribution of work
My part: 
1. Implemented first traversal (sequential)
2. Parallized second traversal