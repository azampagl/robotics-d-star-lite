D* Lite (D Star Lite)
====================


> D\* Lite is an incremental heuristic search algorithm by Sven Koenig and Maxim Likhachev that builds on LPA\*, an incremental heuristic search algorithm that combines ideas of A\* and Dynamic SWSF-FP.
> ### Wikipedia


Structure
---------------------



     bin/

Contains pre-built binary files to run on both Windows and Linux machines.

     ide/

Contains the project files for Visual Studio 2010 (Windows) and Eclipse (Linux).

     src/

Contains the source files of the project.  The actual implementation of the D\* Lite algorithm can be found in **planner.h** and **planner.cpp**.


Usage
---------------------



![alt text](https://github.com/azampagl/robotics-d-star-lite/raw/basic/img/screenshot.jpg "Screenshot")

### Maps

The "maps" used for this project are grayscale bitmaps.  The darker the pixel the more it costs to traverse it.  Black pixels (RGB 0 0 0) are considered "unwalkable" while white pixels (RGB 255 255 255) cost "1".  Pixel colors in between vary in value.

### Execution

     d-star-lite.exe "Simulator 1" "../maps/map-03-real.bmp" "../maps/map-03-robot.bmp" 0 0 249 249 30

To run the program, the following arguments must be passed via command line.

+ *[char\*]* Name of the window.
+ *[char\*]* Location of the real map.
+ *[char\*]* Location of the robot map.
+ *[int]* Start x-coordinate.
+ *[int]* Start y-coordinate.
+ *[int]* Goal x-coordinate.
+ *[int]* Goal y-coordinate.
+ *[int]* Scanner radius.

References
---------------------



*Koenig, S and Likhachev, M. Improved Fast Replanning for Robot Navigation in Unknown Terrain. Atlanta : Georgia Institute of Technology, 2002.*
