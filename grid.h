#ifndef GRID_H
#define GRID_H

#include <stdexcept>

// A generic 2D container for double-precision floating point data
// which provides facilities for indexing the container, checking
// coordinate validity, determining bounds, greyscale display via
// ANSI-compatible terminal, serialization, and deserialization.
class Grid {

    // The maximum dimensions a grid should be when it is displayed.
    // Grids larger than these bounds will be shrunk to accomodate
    // these bounds.
    static size_t max_display_width;
    static size_t max_display_height;

    // The dimensions of the grid
    size_t width;
    size_t height;

    // A pointer to the grid's data.
    double *data;

public:

    // desc: A conventional copy constructor which creates a duplicate of
    //       the assigning grid's data and which sets the constructed
    //       instance's dimensions to match the dimensions of the assinging
    //       instance.
    // pre : None.
    // post: None, aside from description.
    Grid(Grid &other);

    // desc: A conventional move constructor which takes ownership of the
    //       assigning instance.
    // pre : None.
    // post: The assigning instance will be given 0x0 dimensions and a null
    //       data pointer.
    Grid(Grid &&other);

    // desc: A constructor that creates an uninitialized width-by-height grid.
    // pre : None.
    // post: None, aside from description.
    Grid(size_t width, size_t height);

    // desc: A constructor that creates an initialized grid by de-serializing
    //       a human-readable CSV representation of a grid.
    // pre : The input string must represent grids in the same way that the
    //       std::string cast operator serializes grids.
    // post: None, aside from description.
    Grid(std::string const &text);

    // desc: A conventional destructor that de-allocates the array use to
    //       store element data.
    // pre : None.
    // post: None, aside from description.
    ~Grid();
    // desc: Returns true if and only if the coordinatex (x,y) are contained
    //       by the x bounds [0,width) and y bounds [0,height).
    // pre : None.
    // post: None, aside from description.
    bool contains(size_t x, size_t y);

    // desc: Returns a reference to the element at (x,y) in the grid.
    // pre : The provided (x,y) coordinates must fit within the grid bounds.
    //       If this is not the case, an out-of-range exception will be
    //       thrown.
    // post: None, aside from description.
    double &at(size_t x, size_t y);

    // desc: Returns the width of the grid.
    // pre : None.
    // post: None, aside from description.
    size_t get_width() const;

    // desc: Returns the height of the grid.
    // pre : None.
    // post: None, aside from description.
    size_t get_height() const;

    // desc: Sets the global display width limit.
    // pre : None.
    // post: None, aside from description.
    static void set_max_display_width(size_t new_width);

    // desc: Sets the global display height limit.
    // pre : None.
    // post: None, aside from description.
    static void set_max_display_height(size_t new_height);

    // desc: Serializes the grid's dimensions and data content into a human-
    //       readable CSV representation.
    // pre : None.
    // post: None, aside from description.
    operator std::string();

    // desc: Sets all elements of the grid to zero.
    // pre : None.
    // post: None, aside from description.
    void clear();

    // desc: Display's the grid's content as a grid of greyscale "pixels" via
    //       ANSI-escaped strings through stdout.
    // pre : Both max_display_width and max_display_height must be positive.
    //       If they are not, a runtime exception will be thrown.
    // post: None, aside from description.
    void display();

};


#endif // GRID_H