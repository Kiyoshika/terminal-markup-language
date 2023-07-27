# TML (Terminal Markup Language)
This is a custom renderer to build (somewhat) interactive terminal applications for the terminal similar to HTML for the web.

See the [TML Specification](tmlspec.md).

## Preview (27 July 2023)
This is a sample login screen with a regular and password input and some dummy buttons



https://github.com/Kiyoshika/tml/assets/49159969/fe083255-9e41-4bb2-b62a-611ee17f105e



Rendered from the following TML:
```text
<tml>
  <text/>
  <text marginLeft=2 fg=cyan bold=true>Login:</text>
  <text/>
  <text/>
  <text marginLeft=2 marginRight=4 bold=true newline=false>Email:</text>
  <input fg=red bg=white/>
  <text/>
  <text marginLeft=2 marginRight=1 bold=true newline=false>Password:</text>
  <input fg=red bg=white password=true/>
  <text/>
  <button marginLeft=2 marginRight=1 fixedWidth=16 newline=false bg=green fg=white>Login</button>
  <button fixedWidth=16 bg=blue fg=white>Register</button>
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

