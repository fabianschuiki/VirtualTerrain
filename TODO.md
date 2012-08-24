An ordered list of things that need to be accomplished:

- Calculate better normals for the chunks.

- With automated activation/deactivation of children, there are T junctions occuring. Looks like removing a child does not always appropriately deactivate the adjacent chunk's edge.

- Also take into account that chunks may disappear behind the planet. These chunks are not caught in frustum culling. Some form of angular culling needs to be implemented.

- Alter the `SphericalChunk::draw()` to selectively draw only chunks that are within a certain LOD range, or are ocean/land.

- Update PerlinElevation to create terrain that is toroidal, i.e. repeats in x and y directions. Maybe use 3D perlin noise? Or some mapping onto 2D noise?

- Compile the spherical chunks into VBOs, i.e. multiple VBOs for different LODs and ocean/land. This probably requires a new class that holds the compiled VBOs, or the `Planet` class could handle that. Having separated LODs and terrain type allows the ocean to be rendered with an ocean shader and the landmasses with differently detailed textures.

- Create a new ElevationProvider that uses the DEM data of earth for rendering. Maybe some caching is required (data > 2GB). On modern hard drives reading continuous data is about 250 MB/s, which would "only" take 8s for the entire earth at max detail.

- Alter the elevation provider to refine the terrain in between data points. Maybe this should take place in another class, maybe `SceneryProvider` or the like. At a later stage, roads and land use data will affect the way how terrain is refined (roads require flat and gradual slopes, etc.).

- Stream and cache OpenStreetMap data to display roads, cities and forests.