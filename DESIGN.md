# Rough Object Hierarchy

## `Grid`

Top level object. Represents a 3 dimensional hexagon tile based map

## `Stack`

Maps are divided into sections called `Stacks`

The split between `Grids` and `Stacks` is meant to match a split in the underlying memory.
This is to allow loading and unloading of `Stacks` independently of one another for use in extremely large or even "infinite" maps.

## `Slice`

Stacks are made of 2d layers known as `Slices`

`Slices` are implemented as "smart" wrappers for `Stacks` that filter by a particular z height.
These are mostly here for the covenience of the programmer.
## `Hex`

Slices are composed of nodes named `Hexes`, These are the base units of a hex grid.
`Hex` subclasses ` Abstract Hex` but adds object storage, and a relationship to a greater collection.

## `Abstract Hex`

And `Abstract Hex` is a container for addressing and manipulating hexagonal coordinates. 