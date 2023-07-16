# TML (Terminal Markup Language)
This is a custom renderer to build (somewhat) interactive terminal applications for the terminal similar to HTML for the web.

See the [TML Specification](tmlspec.md).

## Preview (16 July 2023)
This is a sample preview with a regular & password input box and some basic formatting.

![image](https://github.com/Kiyoshika/tml/assets/49159969/b92aecd5-94ac-42f7-b8dc-f19938a111b0)

Rendered from the following TML:
```text
<tml>
  <text bold=true fg=cyan>Login:</text>

  <text/>

  <text bold=true newline=false>Email:</text><space/>
  <input fg=red bg=white/>

  <text bold=true newline=false>Password:</text><space/>
  <input password=true fg=red bg=white/>
</tml>
```

## Building
This relies on ncurses for all the UI stuff:
```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

The CMake handles the linking/includes.

Now you can clone this repo, enter the root and:
```
mkdir build && cd build
cmake -DDEBUG_MODE=ON -DCOMPILE_TESTS=ON ..
```
* `DEBUG_MODE` is `OFF` by default
* `COMPILE_TESTS` is `ON` by default

An executable called `tmlrender` will be created in `build/src/` directory.

## Usage
TODO:

