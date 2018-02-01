# Rough Object Hierarchy

## `Grid`

Top level object. Represents a 3 dimensional hexagon tile based map

## `Stack`

Maps are divided into sections called `Stacks`

## `Slice`

Stacks are made of 2d layers known as `Slices`

## `Hex`

Slices are composed of nodes named `Hexes`, These are the base units of a hex grid.
`Hex` subclasses `Abstract Hex` but adds object storage, and a relationship to a greater collection.

## `Abstract Hex`

And `Abstract Hex` is a container for addressing and manipulating hexagonal coordinates. 