# TML Specification (Terminal Markup Language)
Version: 0.0.8 - 24 July 2023

To see older versions of the spec, see the [TML Spec Archive](tmlspec-archive/)

The TML file will be rendered in the terminal similar to how HTML is rendered in a web page. 

Interactable items (input, buttons, etc.) can be clicked on with the mouse.

## Overview
This is a markup language similar to HTML to create elements to be rendered onto a terminal screen.

Press the `ESC` key to properly exit the renderer (`Ctrl+C` is disabled as of `v0.0.7`)

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
* `tagname` is the name of the tag. It must only contain (lowercase or camelCase) ASCII letters with no spaces or other special characters (e.g., hyphens)
* `attr1`/`attr2` is the name of an attribute. It must only contain lowercase ASCII letters with no spaces or other special characters (e.g., hypens)
* `value1`/`value2` is the value of an attribute. It must only contain lowercase ASCII letters with no spaces or other special characters (e.g., hyphens)

Every tag must have a corresponding closing tag `</tag>`. This determines which nodes to nest into other nodes.

Tags that do not require a body can use the shorthand notation, `<tag/>` which is equivalent to `<tag></tag>`. One example of this is the [space tag](#space).

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
* [Script](#script)
* [Text](#text)
* [Space](#space)
* [Input](#input)
* [Button](#button)
* [Planned Future Tags](#planned-future-tags)
* [Planned Future Attributes](#planned-future-attributes)


## Root
**Tag Name:** `<tml>`

**Description:** The root node that contains all other tags. Allows user to configure background colour of entire terminal.

**Attributes:**
* `bg` - background; the background colour of the terminal
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

This creates a white terminal where all nodes will have a red foreground by default (whichever ones are applicable):
```
<tml bg=white fg=red>
  <text>I'm red</text>
  <text fg=green>I'm green</text>
</tml>
```

## Script
**Tag Name:** `<script>`

**Description:** A code block to enter functions used to handle callbacks. See the [TSL Documentation](tsldoc.md)

## Text
**Tag Name:** `<text>`

**Description:** Write a line of text. Note that any whitespace in the tag body is consumed (ignored). If you wish to add a space between text nodes on the same line, use the [space tag](#space).

**Attributes:**
* `fg` - foreground; the colour of the text itself
  * default value: same as parent node
* `bg` - background; the background colour of the text
  * default value: same as parent node
* `bold` - if present, make text bold
  * default value: `false `
* `newline` - determine whether or not to add a newline after the text is written
  * default value: `true`
* `id` - An ID that can be referenced in callback functions
* `marginLeft` - apply padding to the left of the element
  * default value: `0`
* `marginRight` - apply padding to the right of the element
  * default value: `0`

NOTE: you can use this tag as a newline by using the shorthand `<text/>` which only prints a newline

**Examples:**
```
<text>white on black</text> (assuming parent node is black)
<text bold=true>bold white on black</text> (assuming parent node is black)
<text fg=red bg=white>red on white</text>
```

Write multiple text on the same line by disabling `newline` attribute:
```
<tml>
  <text newline=false>hello</text>
  <space/>
  <text>there</text>

  <text>how are you</text>
</tml>

RENDERED AS:
hello there
how are you
```

Or you can use margins to avoid an extra tag:
```
<tml>
  <text newline=false marginRight=1>hello</text>
  <text>there</text>
</tml>
```

Which will be rendered the same as above.

## Space
**Tag Name:** `<space>`

**Description:** Inserts a single space character on the current line, commonly used with the [text tag](#text). Note that you can use the short-hand notation with this tag: `<space/>` is equivalent to `<space></space>` since it does not require a body.

This tag was implemented prior to `v0.0.7` which introduced `marginLeft` and `marginRight` but still keeping for compatability reasons.

**Attributes:** None

**Examples:**
```text
<text newline=false>hello</text>
<space/>
<text>there</text>

RENDERED AS:
hello there
```

## Input
**Tag Name:** `<input>`

**Description:** Get input from a user and pass it to a callback. Click anywhere in the input box to start typing.

The input box is notated with square brackets `[]` with the text appearing between such as `[hello there]`.

Users can click anywhere in the text box (except the first opening bracket) and text will appear where the cursor is when typing.

Users can also use the left/right arrow keys while focused in the input box to move the cursor left/right.

Clicking outside the text box will disable focus and prevent any more text from being written until refocused.

Pressing `Ctrl + R` will clear ("reset") the input box, deleting all text inside it. 

**Attributes:**
* `callback` - function name to call when user submits. callback function must take a single `string` argument
* `fg` - the foreground colour of the input box and text inside it
  * default value: same as parent node
* `bg` - the background colour of the input box
  * default value: same as parent node
* `fixedWidth` - set the width of the input box. users cannot enter more characters past this width unless `grow=true`.
  * default value: `25`
* `grow` - determine whether to allow users to enter more characters than specified by `fixedWidth` which expands (grows) the input box's size.
  * default value: `false`
* `password` - mask the input with `*` used for password or secret inputs
  * default value: `false`
* `newline` - determine whether to add a newline after the input box
  * default value: `true`
* `marginLeft` - apply padding to the left of the element
  * default value: `0`
* `marginRight` - apply padding to the right of the element
  * default value: `0`

**Examples:**

NOTE: as of the version of this spec, the scripting language is not yet designed; below is only a rough prototype of what it *might* look like.
```
<tml>
  <text>First Name:</text>
  <text id=firstNameId></text>

  <text/>

  <text>Last Name:</text>
  <text id=lastNameId></text>

  <text/>

  <text marginRight=1 newline=false>Enter First Name:</text>
  <input callback=setFirstName/>

  <text marginRight=1 newline=false>Enter Last Name:</text>
  <input callback=setLastName/>

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

Would be rendered as (after typing in inputs and pressing enter on each one):

```text
First Name:
Zach

Last Name:
Weaver

Enter First Name: [Zach]
Enter Last Name: [Weaver]
```

## Button 
**Tag Name:** `<button>`

**Description:** A clickable button to trigger a callback function. 

The body of the button (button's name) will be wrapped around parenthesis. So `<button>Submit</button>` will be rendered as `(Submit)`. It's recommended to set different fg/bg colours to make it more clear to the user that it's a button and not general text. 

When the button is pressed, the foreground/background colours are inverted on mouse down, and changed back on mouse up to visually indicate to the user that the button was pressed.

**Attributes:**
* `callback` - function name to call when user submits. callback function must take a single `string` argument
* `fg` - the foreground colour of the input box and text inside it
  * default value: same as parent node
* `bg` - the background colour of the input box
  * default value: same as parent node
* `fixedWidth` - the specified width of the button
  * default value: length of the body + 2 (to account for the parenthesis)
* `newline` - determine whether to add a newline after the input box
  * default value: `true`
* `marginLeft` - apply padding to the left of the element
  * default value: `0`
* `marginRight` - apply padding to the right of the element
  * default value: `0`

**Examples:**

NOTE: as of the version of this spec, the scripting language is not yet designed; below is only a rough prototype of what it *might* look like.
```
<tml>
  <text marginLeft=3 marginRight=1 newline=false>Number:</text>
  <input id=num1/>

  <text marginLeft=3 marginRight=1 newline=false>Number:</text>
  <input id=num2/>

  <button marginLeft=6 bg=white fg=black callback=add(num1.getTextAsInt(), num2.getTextAsInt())>Add</button>

  <text marginLeft=3 marginRight=1 newline=false>Result:</text>
  <text id=result/>

  <script>
    // this is assuming no conversion problems from getTextAsInt()
    function add(int num1, int num2) {
      int res = num1 + num2;
      string resultStr = res.toString();
      result.setText(resultStr);
    }
  </script>
</tml>
```

Would be rendered as (after typing in inputs and clicking the `(Add)` button):

```text
   Number: [12]
   Number: [25]
      (Add)
   Result: 37
```

## Planned Future Tags
Some tags that are planned in future iterations, these are not yet documented/fully scoped out yet. Note that this is a very tentative list and some may get scrapped/added.
* `<radiobutton>`
* `<checkbox>`
* `<grid>`
* `<stack>`
* `<center>`
* `<group>` (kind of like html's div, can group child nodes together to apply same formatting/styles)
* `<table>`
* `<dropdown>`

## Planned Future Attributes
Some future attributes that are planned:
* `hidden` - toggle whether to display the current tag or not
