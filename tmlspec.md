# TML Specification (Terminal Markup Language)
Version: 0.0.2 - 25 June 2023

To see older versions of the spec, see the [TML Spec Archive](tmlspec-archive/)

The TML file will be rendered in the terminal similar to how HTML is rendered in a web page. Any navigatable items (e.g., buttons, user input, etc.) can be navigated with using default vim motions (h, j, k, l) and space/enter to interact.

## Overview
This is a markup language similar to HTML to create elements to be rendered onto a terminal screen.

A bit of terminology: tags (`<tag>`) are sometimes referred to as nodes, especialy in error messages. Node/tag are interchangeable.

The extension for a tml file is `.tml` and must contain exactly one root node (tag) `<tml>`:

```text
<tml>

</tml>
```

The general structure of a tag is:

```text
<tagname attr1=value1 attr2=value2>body</tagname>
```

Where
* `tagname` is the name of the tag. It must only contain lowercase ASCII letters with no spaces or other special characters (e.g., hyphens)
* `attr1`/`attr2` is the name of an attribute. It must only contain lowercase ASCII letters with no spaces or other special characters (e.g., hypens)
* `value1`/`value2` is the value of an attribute. It must only contain lowercase ASCII letters with no spaces or other special characters (e.g., hyphens)

Every tag must have a corresponding closing tag `</tag>`. This determines which nodes to nest into other nodes.

Attributes and values are tied together with an `=` AND MUST NOT CONTAIN SPACES. For example, this is an illegal attribute definition:
```text
ILLEGAL:
<tagname attr = value></tagname>

LEGAL:
<tagname attr=value></tagname>
```

Every tag/node has a specified list of allowable child nodes. For example, anything can be a child of the root node `<tml>` but you cannot nest a `<text>` insde of another `<text>`.
```text
ILLEGAL:
<tml>
  <text>
    <text>hey</text>
  </text>
</tml>

LEGAL:
<tml>
  <text>hey</text>
  <text>there</text>
</tml>
```

For more detailed documentation on all the tags, see the [tag reference](#tag-reference)

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
* [New Line](#new-line)
* [Script](#script)
* [Text](#text)
* [Input](#input)
* [Planned Future Tags](#planned-future-tags)


## Root
**Tag Name:** `<tml>`

**Description:** The root node that contains all other tags. Allows user to configure background colour of entire terminal.

**Attributes:**
* (optional) `bg` - background; the background colour of the terminal
  * default value: black

**Examples:**
This creates a blank black terminal
```
<tml></tml>
```

This creates a blank red terminal
```
<tml bg=red></tml>
```

## New Line
**Tag Name:** `<newline>`

**Description:** Adds a new line during rendering.

**Attributes:** None

**Examples:**
This writes some text separated by an extra new line. Note that `<text>` by default already creates a new line.
```
<tml>
  <text>hello</text>
  <newline>
  <text>there</text>
</tml>

RENDERED AS:
hello

there
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
  * default value: same as parent node
* (optional) `bold` - if present, make text bold
  * default value: false 
* (optional) `id` - An ID that can be referenced in callback functions

**Examples:**
```
<text>white on black</text> (assuming parent node is black)
<text bold=true>bold white on black</text> (assuming parent node is black)
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

## Planned Future Tags
Some tags that are planned in future iterations, these are not yet documented/fully scoped out yet. Note that this is a very tentative list and some may get scrapped/added.
* `<button>`
* `<radiobutton>`
* `<checkbox>`
* `<grid>`
* `<stack>`
* `<center>`
