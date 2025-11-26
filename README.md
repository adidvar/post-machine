# Emil Post Machine Simulation Program

![GitHub license](https://img.shields.io/github/license/adidvar/post-machine?style=flat)
![GitHub stars](https://img.shields.io/github/stars/adidvar/post-machine?style=flat)
![GitHub forks](https://img.shields.io/github/forks/adidvar/post-machine?style=flat)
![GitHub issues](https://img.shields.io/github/issues/adidvar/post-machine?style=flat)
[![CI](https://github.com/adidvar/post-machine/actions/workflows/build.yml/badge.svg)](https://github.com/your-username/your-repo/actions)

## Overview

This project is a Post Machine simulation program written in C++ using the Qt library. The program is designed to simulate the operation of a Post Machine, a theoretical computational model proposed by Emil Post in the 1930s. It is distributed under the terms of the GNU General Public License, version 3 (GPL-3.0).

## Available Packages

Prebuilt packages are available for the following operating systems:

- ✅ **Ubuntu**: Available as a `.deb` package.
- ✅ **Fedora**: Available as an `.rpm` package.
- ✅ **Arch Linux**: PKGBUILD is available in the repository directory.
- ✅ **Windows**:
  - Portable `.zip` package.
  - Windows Installer (`.exe`).
- ✅ **macOS**: Available as a `.dmg` file.

## Features

- Graphical user interface (GUI) built with Qt 6.
- Simulation of a Post Machine with step-by-step execution.
- Tape visualization and manipulation.
- Loading and saving of machine instructions.
- Debugging tools for instruction execution.
- Cross-platform support.

## Screenshots

![Screenshot 1](assets/Screenshot_1.png "Screenshot 1")

## Getting Started

### Prerequisites

Before you can build and run the Post Machine simulation program, you will need to have the following prerequisites installed on your system:

- C++ compiler that supports C++17 or later.
- Qt development libraries (Qt 6 or later).
- CMake (version 3.30 or later).

### Building (Linux)

To build the project using CMake:

```sh
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

After the build completes, the executable will be available in the `build` directory.

## Contributing

Contributions are welcome! If you find a bug, have an idea for an improvement, or want to add a new feature, feel free to open an issue or submit a pull request. Your contributions help make this project better!

## License

This project is licensed under the GNU General Public License, version 3 (GPL-3.0). See the [LICENSE](LICENSE) file for details.
