# Features

## Working

### Graphics
 * Physically Based Rendering
 * HighDynamicRange Environment maps
 * Image Based Lighting
 * Deferred and Forward rendering
 * Lights rendered as Light Volumes
 * Bloom with texture downsampling
 * Stereoscopic Normal map encoding and decoding for optimal use of G-Buffer
 * Mesh Filter -> Vertex buffer building depending on material needs
 * Basic Statet Managment
 * Render Pipeline Object
 * Frustum culling
 * Sprite font rendering
 * Primitive renderer
 * Shader preprocessing (#include)
 
### Planets
 * Custom triangle CDLOD
 * Triangle tree with frustum culling
 * Icosphere generation
 
### General
 * Scenegraph Entity Hirachy 
 * Entity Component System 
 * Singleton Pattern for things like WINDOW, INPUT, TIME, CAMERA etc 
 * Observer Pattern for input 
 * Factory Pattern for Content Managment 
 * Performance measurment
 * GLSL shader preprocessing with includes and vertex / geo / fragment specializations

 
## Work in progress

### Graphics
 * Shadow Mapping

### Planets
 * Atmospheric Scattering
 
### General
 * Specific atmoic types
 * Custom Maths Library

 
## Backlog

### Graphics
 * Effects
     * SSAO
     * SSR
     * AA for deferred rendering (TAA)
     * Motion Blur
 * Api / Pipeline
     * Render state is pushed right before draw call
     * Allow better Graphics debugging support by tagging
     * Abstract OpenGL for Shader and Framebuffer
     * Texture objects handle their own parameters
     * Vulkan??
     * Sorting objects before rendering
     * batching 
     * Support resizing window
 * UI
     * Sprite rendering
     * Dynamic font file rasterization (any size)
     * UI Space definition and pipeline stage
 * Utility
     * Generate some common shaders from code
     * Basic deferred fragment shader generation
     * Cone Lights/Culling/Primitive
     * Debug renderer
 * Transparent shadows
 * Particle Systems
 * Animation support
 
### Planets
 * World moves around camera
 * Reference frame system
 * Send back vertices for physicalization
 * Clouds / Fog
 * Trees / grass etc
 * Near ground Detail / Detail data
 * Ocean sim
 * Vectorized streets / fields
 * Effects such as Lightning, Aurora Borealis, Atmospheric glow
 * Biome Separation
 * Star data + low res cubemap instead of detailed cubemap
 * Weather particles
 * God rays
 * occlusion culling
 
### General
 * Physics Library (PhysX?)
 * Sounds (FMOD?)
 * Serialize Levels
 * Read engine settings from file
 * Packaging system
 * Binary file types
 * Debug Overlay
     * Debug Overlay as Editor build mode
 * Linux support
 * Screenshot manager
 * SIMD math
 * Multithreading support
 * Loading screen / splash screen