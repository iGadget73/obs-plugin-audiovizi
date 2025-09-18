# OBS Plugin – AudioVizi

## Introduction

**AudioVizi** is a custom OBS Studio plugin that adds a **PCM Audio Visualizer** source.  
It started from the official [OBS Plugin Template](https://github.com/obsproject/obs-plugintemplate) and is maintained by [Günther Gadget](https://igadget.de).

The plugin currently provides:

- A **skeleton GPU source** rendering a scrolling line (test)
- Registration as a proper OBS source
- Build & packaging via GitHub Actions

Planned features:

- Audio tapping from OBS
- Ring buffer & peak-envelope rendering
- Level-dependent coloring

## Author

- **Author**: Günther Gadget
- **Website**: [igadget.de](https://igadget.de)
- **Email**: igadget73@gmail.com

## Supported Build Environments

| Platform      | Tool                                                  |
| ------------- | ----------------------------------------------------- |
| Windows       | Visual Studio 2022                                    |
| macOS         | Xcode 16.0                                            |
| Windows/macOS | CMake ≥ 3.28                                          |
| Ubuntu 24.04  | CMake 3.28+, ninja-build, pkg-config, build-essential |

## Quick Start

1. Clone this repo
2. Run CMake to generate the project
3. Build with your platform toolchain

See the [Plugin Template Wiki](https://github.com/obsproject/obs-plugintemplate/wiki) for a general guide.

## GitHub Actions & CI

This repository includes default GitHub Actions workflows for:

- **push**: run on commits/tags
- **pr-pull**: run on pull requests
- **dispatch**: manual trigger
- **build-project**: build the plugin
- **check-format**: lint CMake and source code

### Retrieving build artifacts

Successful GitHub Actions builds will produce **artifacts** (simple archives).  
For macOS, download the `macOS-arm64` artifact and copy it to:
