# KnightsOnBoard

KnightsOnBoard is a small SDL3 framebuffer demo that draws a growing pattern on a large canvas.
It ships with C and C++ versions of the renderer and saves snapshots of the result as images.

## Build it yourself
1. Install SDL3 and `pkg-config`.
2. Clone this repo and open the project folder.
3. Build the C++ version with `make build`.
4. Or build the C version with `make C`.
5. Run the program with `make run` or `./framebuffer`.
6. When it exits, the latest rendered image is written to `Output/`.
7. Sample images are stored in `Images/` for quick preview.

## Images

![Sample 1](Images/Out_1.jpg)
![Sample 2](Images/Out_5.jpg)
