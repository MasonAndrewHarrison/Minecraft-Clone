# Minecraft-Clone

A voxel-based Minecraft clone written from scratch in C and OpenGL infinite, chunked terrain generation, occlusion-culled meshing, multithreaded chunk streaming, and basic block interaction.

## Screenshots

![Gameplay 1](textures/images/img1.png)
![Gameplay 2](textures/images/img2.png)
![Gameplay 3](textures/images/img3.png)

## Features

- Infinite terrain via deterministic, coordinate-seeded value noise
- Chunked world generation and rendering, with occlusion (hidden-face) culling
- Multithreaded chunk generation and streaming as the player moves
- Backface culling and distance fog
- Texture-atlas–based block rendering
- Block breaking and placing
- Wireframe debug view

## Controls

| Action | Key |
|---|---|
| Move | W A S D |
| Look | Mouse |
| Up / Down | E / Q |
| Break block | Left Click |
| Place block | Right Click |
| Select block | Scroll wheel |
| Toggle infinite world generation | Tab |
| Exit | Esc |

## Building

**Dependencies** (Arch/pacman):
```
sudo pacman -S glfw-x11 glew cglm mesa
```

**Clone**
```
git clone https://github.com/MasonAndrewHarrison/Minecraft-Clone.git
cd Minecraft-Clone
```

**Build & run:**
```
make
./output
```

## Third-party code

This project uses [stb_image.h](https://github.com/nothings/stb) by Sean Barrett for image loading, vendored as a git submodule under `textures/vendor/stb/`. See [THIRD_PARTY_LICENSES.md](THIRD_PARTY_LICENSES.md) for its license.