# TML (Terminal Markup Language)
This is a custom renderer to build (somewhat) interactive terminal applications for the terminal similar to HTML for the web.

See the [TML Specification](tmlspec.md).

## Preview (09 July 2023)
Below is a preview of rendering some text with different colours/attributes:

![Capture](https://github.com/Kiyoshika/tml/assets/49159969/8764c835-2ffa-44bd-a9af-db8b69964e25)

Which was rendered with the following tml script:
```text
<tml
  bg=blue
  fg=white>

  <text bold=true>
    I'm some text
  </text>

  <text
    fg=red
    newline=false>
    I'm some other text!!
  </text>

  <text
    bg=black
    fg=yellow
    #nuggets$$&
  </text>

  <text/>
  
  <text newline=false>this</text>
  <space/>
  <text newline=false bold=true>word</text>
  <space/>
  <text>is bold.</text>
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

