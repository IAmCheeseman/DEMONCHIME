# Boomer Shooter
Currently this is just a 3D renderer, but in the future I want to make it into an actual game.
hi

## Building
Make sure to clone the repo with submodules, and ensure LuaJIT is installed.
### On Linux
```bash
$ sh build.sh debug
$ ./bin/debug/bs
```
The `debug` can be replaced with `release`, and that will enable optimizations.
If you want optimizations, but don't want to give up symbols, you can use `releasesym`.
### Windows
Currently not tested, and probably won't work. Will eventually work. Maybe.

## Project Structure
The project structure is very simple:
- `core`: this is the engine
- `core/gfx`: for any rendering code
- `core/wrap`: wraps the engine api into Lua
- `core/math`: linear algebra
- `opengl`: for any opengl-specific code
- `game`: where the game code goes
- `lib`: third-party libraries
- `res`: resources, like shaders, textures, audio, etc
- `build`: build system code
