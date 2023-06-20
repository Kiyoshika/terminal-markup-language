# TML Spec (Terminal Markup Language)
Version: 0.0.1 - 17 June 2023

The TML file will be rendered in the terminal similar to how HTML is rendered in a web page. Any navigatable items (e.g., buttons, user input, etc.) can be navigated with using default vim motions (h, j, k, l) and space/enter to interact.

## Colours
Acceptable colours used in attributes such as `fg`, `bg`, etc.:
* `white`
* `black`
* `red`
* `blue`
* `yellow`
* `cyan`
* `green`
* `magenta`

# Tag Reference

* [Root](#root)
* [Script](#script)
* [Text](#text)
* [Input](#input)

TODO: button, radio buttons, check box, grid system, stack layout (kind of like material UI)

## Root
**Tag Name:** `<tml>`

**Description:** The root node that contains all other tags. Allows user to configure background colour of entire terminal.

**Attributes:**
* (optional) `bg` - background; the background colour of the terminal
  * default value: `black`

**Examples:**
This creates a blank black terminal
```
<tml></tml>
```

This creates a blank red terminal
```
<tml bg=red></tml>
```

## Script
**Tag Name:** `<script>`

**Description:** A code block to enter functions used to handle callbacks. See the [TSL Documentation](tsldoc.md)

## Text
**Tag Name:** `<text>`

**Description:** Write a line of text (will automatically append a newline)

**Attributes:**
* (optional) `fg` - foreground; the colour of the text itself
  * default value: `white`
* (optional) `bg` - background; the background colour of the text
  * default value: `black`
* (optional) `bold` - if present, make text bold
  * default value: off
* (optional) `id` - An ID that can be referenced in callback functions

**Examples:**
```
<text>white on black</text>
<text bold>bold white on black</text>
<text fg=red bg=white>red on white</text>
```

## Input
**Tag Name:** `<input>`

**Description:** Get input from a user and pass it to a callback. Blocks input (making the renderer wait until user submits.) If text box is highlighted, user can press space to start typing and enter to submit.

**Attributes:**
* (required) `callback` - function name to call when user submits. callback function must take a single `string` argument
* (optional) `minLength` - min length of the buffer; prevents user from submitting until this length is reached
  * default value: `1`
* (optional) `maxLength` - max length of the buffer; prevents user from inputting more characters
  * default value: `25`

**Examples:**
```
<tml>
  <text>First Name:</text>
  <text id=firstNameId></text>

  <newline>

  <text>Last Name:</text>
  <text id=lastNameId></text>

  <input callback=setFirstName>Enter First Name:</input>
  <input callback=setLastName>Enter Last Name:</input>

  <script>
    function setFirstName(string firstName) {
      setText(firstNameId, firstName);
    }

    function setLastName(string lastName) {
      setText(lastNameId, lastName);
    }
  </script>
</tml>
```

