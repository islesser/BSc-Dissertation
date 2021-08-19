# Generalized Ocean Simulation For Real-Time Applications - BSc Computer Science Dissertation 2020 - 

This project achieved a First-Class Honours.

For a graphical application, a realistically rendered and animated ocean can add a substantial amount of realism and immersion for a user, where it is typically easy to notice errors or unnatural patterns in the simulation, even as a background element.

Typical algorithms for simulating the ocean suffer can only simulate deep/infinite depths, which is unsuitable for the finite/shallow areas such as the coastline, or are complex and non-performant which, for certain applications where the simulation is only a small part of the overall scene, is unsuitable.

This project introduces an algorithm for ocean simulation where by extending the existing Gerstner waves algorithm, and incorporating height fields is able to incorporate shoreline waves and ocean waves in a single render pass suitable for real-time interactive applications viewed at moderate to far distance with a reasonable performance cost completely simulated on the GPU.

<p align="center">
<img width="500" src="https://raw.githubusercontent.com/islesser/BSc-Dissertation/main/Tidal.png" />
</p>

This project uses GLFW and OpenGL for window and GPU access. It also incorporates an implementation of Perlin noise for random terrain generation.

## Building & Running

This project only supports MacOS devices. Run the script **./Build.sh** in your terminal, which will present a menu with the following supported inputs:

| Input | Function    | Details                                                            |
| ----- | ----------- | ------------------------------------------------------------------ |
| 1     | Build & Run | Generate a gmake project and then builds and runs the application.|
| 2     | Build       | Generate a gmake project and then builds the application.|
| 3     | Clean       | Removes platform specific project files                            |
| 4     | Generate    | Generates the gmake project files                                  |
| 5     | Commit      | Will prompt for a commit message and then push to the remote repo.|

If this script will not run, you may need to run the following command in your terminal in the Particle root directory: **sudo chmod +x ./Build.sh**

