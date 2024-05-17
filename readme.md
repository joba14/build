
![License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=for-the-badge)
![Languages](https://img.shields.io/badge/languages-C-brightgreen.svg?style=for-the-badge)
![Platforms](https://img.shields.io/badge/platforms-Linux-brightgreen.svg?style=for-the-badge)
<br>


# Build
**The make-like build system in c**<br>[Report a bug](https://github.com/joba14/build/issues/new) · [Request a feature](https://github.com/joba14/build/issues/new)


## Table of Contents
- [Getting Started](#getting-started)
	- [From Source](#from-source)
		- [Cloning the Project](#cloning-the-project)
		- [Dependencies](#dependencies)
		- [Building the Project](#building-the-project)
		- [Testing the Project](#testing-the-project)
		- [Generating the Documentation](#generating-the-documentation)
	- [Usage](#usage)
		- [Example Project](#example-project)
		- [Running the Compiler](#running-the-compiler)
- [Contributing](#contributing)
- [License](#license)


## Getting Started

### Cloning the Project
To clone the project repository, use the following command:
```sh
> git clone https://github.com/joba14/build.git <dir-to-clone-to>
```

To use the project, ensure you have the necessary dependencies installed. This project requires the following tools:
- [gcc](https://gcc.gnu.org/)

*Currently, only gcc compiler for unix/linux platforms is supported by the project.*

<div style="text-align: right;"><a href="#build">(to the top)</a></div>

### Usage
1. Copy the [build.h](./build.h) into your project.
2. Create build.c file and, by following examples and templates, define the build targets and rules for your project.
3. Copy the [bootstrap.sh](./bootstrap.sh) script next to the build.c file and run it or call the command `gcc -Wall -Wextra -Werror -o ./build.bin ./build.c`. This creates a build.bin executable that will rebuild itself if any changes will be done in the build.c file. However, if you decide to modify the [build.h](./build.h) file, you will have to bootstrap it once more.
4. Run the build.bin with -h flag to see all the targets you defined for the build system. The output of this command should be similar to the following. *The amount of targets listed in this help message will reflect the targets you defined in the build.c file.*
```sh
usage: ./build.bin [options] <targets...>
options:
    -h, --help
        print this help message.
targets:
    clean
        remove the build directory with all its artefacts.
    prep
        create build directory.
    debug
        build the project in debug build configuration.
    release
        build the project in release build configuration.
    all
        run all targets one after the other.
```
5. To run a specific target or a sequence of them, list the names of the targets separated by spaces in the ./build.bin. Here is an example for the previous step:
```sh
> ./build.bin clean prep debug
# or
> ./build.bin all
```

<div style="text-align: right;"><a href="#build">(to the top)</a></div>

### Examples
See examples in the [examples directory](./examples/).

<div style="text-align: right;"><a href="#build">(to the top)</a></div>


## Contributing
At this time, I am not actively seeking contributions to this project. I appreciate your interest and enthusiasm for contributing to the project.

<div style="text-align: right;"><a href="#build">(to the top)</a></div>


## License
This project is released under the **MIT** license. Users and contributors are required to review and comply with the license terms specified in the [license.md file](./license.md). The license outlines the permitted usage, distribution, and intellectual property rights associated with this project.

Please refer to the [license.md file](./license.md) for more details. By using, modifying, or distributing this project, you agree to be bound by the terms and conditions of the license.

<div style="text-align: right;"><a href="#build">(to the top)</a></div>
