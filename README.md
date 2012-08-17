VirtualTerrain
==============

An OpenGL renderer for terrains. Plans to use OpenStreetMap and the NASA Digital Elevation Map to generate the planet. Resolution shall be enhanced artificially by applying procedural noise in between data points. The focus is on plausibility, not accuracy.


Implemented Things
------------------

- Basic HDR rendering with bloom and exposure tone mapping.


Planned Things
--------------

- Atmospheric scattering (Rayleigh + Mie)
- Depth-based turbidity (similar to above)
- HDR rendering
- Lens flares
- Automated, delayed exposure adjustment to simulate the human eye.


How To Give It A Try
--------------------

- Create a `build` directory and switch into it
- Run `$ cmake ..`
- Resolve any errors (installing SFML and the like)
- Build the program through `$ make`
- Make sure there is a symbolic link to `data` in the program's working directory. If not create one, e.g. `$ ln -s ../data`
- Run the program `$ ./VirtualTerrain`

### On Mac OS X

- Create a `build` directory and switch into it
- Run `$ cmake -G Xcode ..`
- Resolve any errors (installing SFML and the like)
- Open `VirtualTerrain.xcodeproj`
- *Build* the project
- Switch into the `build/Debug` folder and create a symbolic link to the `data` folder, e.g. `$ ln -s ../../data`
- In Xcode, *run* the program