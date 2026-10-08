# Wolfenstein-style raycasting in C++

A small rendering demo using only the C++ standard library. It casts rays through a 2D grid and draws walls as vertical strips. This is raycasting, not a full ray tracer.

## Run

```sh
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o raycaster
./raycaster
```

The program writes `view.ppm`. Open it with an image viewer that supports PPM. Use `w` and `s` to move, `a` and `d` to turn, and `q` to quit. Press Enter after a command, then reload the image to see the new view.

The map uses `S` for a wall and a space for empty ground. Change it directly in `main.cpp`.

A minimal C++ adaptation of the raycasting part of [wolfenstein-rl](https://github.com/su-ndey/wolfenstein-rl). The Basic map and 320-by-200 resolution come from `src/base.py`; the starting position and vertical/horizontal grid intersection method follow `rl/env/basic.py`. Coordinates are measured in map squares rather than pixels.

This version keeps wall projection and movement only. Walls are plain colors, and output is an image file rather than a real-time window. PPO, training, textures, enemies, and weapons remain in the original Python project.
