# LearnOpenGL

A C++ OpenGL renderer built while following [learnopengl.com](https://learnopengl.com).

## Dependencies

- [GLFW](https://github.com/glfw/glfw) — windowing and input
- [GLM](https://github.com/g-truc/glm) — math
- [glad](https://glad.dav1d.de/) — OpenGL loader
- [Dear ImGui](https://github.com/ocornut/imgui) — debug UI

All dependencies except glad are managed as git submodules. glad is committed directly as pre-generated source.

## Requirements

- CMake 3.20+
- Visual Studio 2022 (or any C++17-compliant compiler)

## Building

Clone the repo with submodules:
```bash
git clone --recurse-submodules https://github.com/dhdylan/LearnOpenGL.git
```

If you already cloned without `--recurse-submodules`:
```bash
git submodule update --init --recursive
```

Configure and build:
```bash
cmake -B build
cmake --build build --config Debug
```

The executable will be at `build/Debug/LearnOpenGL.exe`. Shaders and assets are copied there automatically as part of the build.

Alternatively, open the repo folder directly in Visual Studio 2022 — it will detect the CMakeLists.txt automatically.
