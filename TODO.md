VirtualTerrain TODO
===================

An ordered list of things that need to be accomplished:

- Use some chunking and caching to render the planet color at different resolutions.

- Make scenery baking have a resolution for each axis. Use dot product to reduce quality of bakings.

- I'm not sure whether the terrain detail destruction works properly. Seems like it takes a long time for a child to get deactivated.

- Alter the `SphericalChunk::draw()` to selectively draw only chunks that are within a certain LOD range, or are ocean/land. This allows the use of different shaders, materials and textures for each.

- Compile the spherical chunks into VBOs, i.e. multiple VBOs for different LODs and ocean/land. This probably requires a new class that holds the compiled VBOs, or the `Planet` class could handle that. Having separated LODs and terrain type allows the ocean to be rendered with an ocean shader and the landmasses with differently detailed textures.

- Create a job based multi-threading environment. Create a `JobQueue` that holds a list of `Job`s that need to be executed. Job should have among its properties:
  - Type, e.g. Terrain, Elevation Baking, etc.
  - Priority
  - whether two jobs of the same type are able to run simultaneously
  - Jobs upon whose completion this Job depends.
  - Job types which have to be complete before this. Useful if you have multiple jobs doing some calculations and e.g. a baking job needs the results.
  - whether this job clears all queued jobs of the same type (i.e. an update job supersedes its previous calls)
  
  Maybe jobs should be able to be stalled, i.e. be able to cease execution and add themselves to the queue again (e.g. staged execution).
  
  The job queue is thread safe and lists the queued jobs, where jobs may be added or removed. When a job comes in, the job queue decides whether it is able to run (dependencies, etc.), and increases a sempahore. The `JobWorkerThread`s decrease the semaphore (which blocks and sleeps until a job becomes available) and ask the job queue for the next job to be executed, which in turn returns the one with the highest priority whose dependencies and constraints have been met.

- Move terrain detail update into a separate thread. Accomplish this by creating *TerrainLOD* jobs that perform the calculations.

- Move the VBO compilation code into a separate thread. Accomplish this by creating *TerrainVBO* jobs that clear previous jobs of this type and depend on other terrain jobs to complete, e.g. *TerrainLOD*. This maps an OpenGL buffer, compiles the vertices into it, and unmaps the buffer (Might be impossible due to the thread unsafeness of OpenGL. Maybe move the buffer mapping to the main loop?).

- Calculate atmospheric scattering and use it to draw the atmosphere and fade out distant objects. CryEngine does this as a postprocessing effect based on the depth buffer. Maybe do that the same way?

- Ocean shader. It should be possible to do this through some Perlin noise calculated in the shader, plus time to animate the waves. Maybe even wind directions can be used to decide where the waves move?

- Use the earth coverage map from ? to decide what the general terrain type of a given location is. The original map is a 700MB TIFF, maybe this can either be compressed or the data may be transformed into some sort of indexed image.

- Colorize earth either by using the earth texture from NASA's Blue Marble, or through some noise and coverage map.

- If the camera is close enough to the ground, i.e. the earth's curvature becomes less and less visible, switch to a cartesian chunk system to faciliate adding ground details.

- Once the camera moves very close to the ground, using the `BakedScenery` textures is not sufficient anymore, since the texture will change very frequently. Instead, when dropping to cartesian space, new ways of terrain rendering need to be devised. Maybe chunks grouped by material?

- Alter the elevation provider to refine the terrain in between data points. Maybe this should take place in another class, maybe `SceneryProvider` or the like. At a later stage, roads and land use data will affect the way how terrain is refined (roads require flat and gradual slopes, etc.).

- Stream and cache OpenStreetMap data to display roads, cities and forests.

- Add ground detail, like rocks, grass and fields. Anno 2070 uses some kind of vertex shader and Perlin noise to wiggle the trees in a wave-like motion to simulate slight winds.

- Calculate terrain self-shadowing. One approach would be to cast a ray from each vertex to the camera and check whether it hits the terrain, which would mean that the area is not hit by the sun. Maybe the shadow mapping for objects is good enough also for the terrain?


Future Stuff
------------

- SFML has builtin joystick support. Use that to simulate some simple airplane so we can fly around the terrain a bit to check whether it's convincing when viewed from the close air.
- Add a car to drive around on the ground, especially drive on the roads to see if the terrain is convincing from very close.
- Deferred Shading
- Extend the G-buffer and add a screen space velocity vector for motion blurring.
- Clouds


Resources
---------

- [http://www.naturalearth.com]
- [http://visibleearth.nasa.gov/view_cat.php?categoryID=1484] and [http://visibleearth.nasa.gov/view.php?id=74117]
- [http://earthobservatory.nasa.gov/Features/BlueMarble/BlueMarble_monthlies.php]


Potential Issues
----------------

- SphericalChunks are culled when all of their normals face away from the camera. If the chunk closer to the camera has a deep valley and the chunk behind it has a steep incline it will be culled if it faces away from the camera, even if the incline might be visible due to the valley. This may be fixed by not taking the direction vector from the corner vertices, but moving the vertices towards the center by the highest elevation value in the chunk.