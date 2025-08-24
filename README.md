
# Sim-Espace-Qt-C++

3D simulation of celestial bodies and gravity using Qt and C++.
![Sim-Espace Simulation](https://i.imgur.com/oatzS1A.png)

## Description

Sim-Espace is an interactive **3D** simulation modeling the motion of celestial bodies under **Newtonian gravity**, using **real planetary data** for masses, distances, and sizes.

## Key Features

- 3D simulation of celestial bodies with Newtonian gravity.  
- Navigate through space using the arrow keys.  
- Camera controls:
  - **Pitch**: drag the mouse left-right.  
  - **Roll**: drag the mouse up-down.  
  - Rotate the camera on itself: `Q` and `D` keys.  
- Add or remove celestial bodies manually via the menu.  
- Accelerate simulation time (configurable; default: 1 real minute = 1 simulated year).  
- Follow a specific celestial body.  
- Resize a celestial body if it appears too small or too large.  
- Pause the simulation at any time.  

## Technologies Used

- **Language**: C++  
- **Framework**: Qt (Qt Widgets / Qt GUI)  
- OpenGL for 3D rendering.  

## Installation & Compilation & Run
0. You need to have Qt libraries and indicate where your include folder in the Makefile by changing the **ABS_PATH** variable
1. Clone the repository:  
```bash
git clone https://github.com/RiriZermi/sim-espace-qt-c--
```
2. Compile & Run 
 ```bash
make
```





