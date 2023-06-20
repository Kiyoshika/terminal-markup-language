# TML (Terminal Markup Language)
This is a custom renderer to build (somewhat) interactive terminal applications for the terminal similar to HTML for the web.

See the [TML Specification](tmlspec.md).

## Building
This relies on ncurses for all the UI stuff:
```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

The CMake handles the linking/includes.

Now you can clone this repo, enter the root and:
```
mkdir build && cd build
cmake -DDEBUG_MODE=ON .. # NOTE: DEBUG_MODE is OFF by default
```

An executable called `tml` will be created in `build/src/` directory.

## Usage
TODO:

