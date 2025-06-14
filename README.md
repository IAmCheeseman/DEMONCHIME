# DEMONCHIME
Currently this is just a 3D renderer, but in the future I want to make it into
an actual game.
## Building
#### Dependencies
- `7z` CLI tool
- LuaJIT shared library
- GLFW shared library
#### Configs
- `debug`: symbols, `-O2` optimizations, and on Linux, will enable asan.
- `release`: no symbols, `-O2` optimizations.
### Compiling
Compile LuaJIT and GLFW as shared libraries, then place them in the root
directory of this project.
#### LuaJIT
On Linux, you need to copy `libluajit.so` to the project directory, and on
Windows, it should be `lua51.dll`, compiled with `msvcbuild`.
#### GLFW
On Linux, you will need to copy `libglfw.so` to the root directory, and on 
Windows, you need to copy `glfw3.dll` to the root directory.

Once you've compiled both LuaJIT and GLFW, you can then compile using `make`.
```bash
$ make config=debug
```
## Project Structure
The project structure is very simple:
- `core`: this is the engine
- `core/opengl`: for any opengl-specific code
- `core/had`: contains scripts and resources that the core engine needs
- `lib`: third-party libraries
- `game`: where the game code goes
- `game/res`: resources, like shaders, textures, audio, etc
## HAD files
HAD file store game data, the main executable will need both `CORE.HAD` and
`DEMONCHIME.HAD` to run.
HAD files are just zip files, but with a different extension so you know that
they're for this engine. You can load additional HAD files by passing them to
the executable in the CLI. HAD stands for "Here's All the Data".
