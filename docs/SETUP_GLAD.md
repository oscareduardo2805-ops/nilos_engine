# GLAD Setup Instructions

GLAD is an OpenGL function loader. It's required to use modern OpenGL on most platforms.

## Automatic Setup (Recommended)

### Option 1: Use the GLAD web service

1. Go to https://glad.dav1d.de/
2. Configure:
   - Language: **C/C++**
   - Specification: **OpenGL**
   - API gl: **Version 3.3** (or higher)
   - Profile: **Core**
   - Check: **Generate a loader**
3. Click **Generate**
4. Download the zip file
5. Extract and copy:
   - `include/glad/` → `external/glad/include/glad/`
   - `include/KHR/` → `external/glad/include/KHR/`
   - `src/glad.c` → `external/glad/src/glad.c`

### Option 2: Use pre-generated files (if provided)

If you cloned this repository and GLAD files are already present in `external/glad/`, you don't need to do anything.

## Manual Setup (Alternative)

If you prefer to generate GLAD yourself:

```bash
# Install glad generator
pip install glad

# Generate GLAD files
python -m glad --generator c --spec gl --out-path external/glad --api="gl=3.3"
```

## Verify Setup

After setup, your directory structure should look like:

```
external/
└── glad/
    ├── include/
    │   ├── glad/
    │   │   └── glad.h
    │   └── KHR/
    │       └── khrplatform.h
    └── src/
        └── glad.c
```

## Troubleshooting

**Error: "Cannot find glad/glad.h"**
- Make sure the `external/glad/include` directory exists and contains the GLAD headers
- Check that CMakeLists.txt has the correct include path

**Error: "Undefined reference to gladLoadGLLoader"**
- Make sure `external/glad/src/glad.c` is being compiled
- Check that it's added to the CMakeLists.txt

**Alternative: Use system GLAD**
Some Linux distributions provide GLAD packages:
```bash
# Arch Linux
sudo pacman -S glad

# Ubuntu/Debian (may not be available)
# Install from source instead
```

## Why GLAD?

OpenGL on Windows and Linux doesn't provide function pointers directly. GLAD:
- Loads OpenGL function pointers at runtime
- Handles OpenGL extensions
- Supports multiple OpenGL versions and profiles
- Is lightweight and customizable

## Alternatives

Other OpenGL loaders (if you prefer):
- **GLEW** (older, more common but larger)
- **gl3w** (minimalist alternative)
- **glbinding** (modern C++ approach)

To switch loaders, modify CMakeLists.txt and update includes in Window.cpp.

