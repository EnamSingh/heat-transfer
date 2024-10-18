#include "grid.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>


// desc: A conventional copy constructor which creates a duplicate of
//       the assigning grid's data and which sets the constructed
//       instance's dimensions to match the dimensions of the assinging
//       instance.
// pre : None.
// post: None, aside from description.
Grid::Grid(Grid &other)
        : width(other.width)
        , height(other.height)
        , data(new double[width*height])
{
    std::memcpy(data,other.data,sizeof(double)*width*height);
}



// desc: A conventional move constructor which takes ownership of the
//       assigning instance.
// pre : None.
// post: The assigning instance will be given 0x0 dimensions and a null
//       data pointer.
Grid::Grid(Grid &&other)
        : width(other.width)
        , height(other.height)
        , data(other.data)
{
    other.width = 0;
    other.height = 0;
    other.data = nullptr;
}



// desc: A constructor that creates an uninitialized width-by-height grid.
// pre : None.
// post: None, aside from description.
Grid::Grid(size_t width, size_t height)
        : width(width)
        , height(height)
        , data(new double[width*height])
{}



// desc: A constructor that creates an initialized grid by de-serializing
//       a human-readable CSV representation of a grid.
// pre : The input string must represent grids in the same way that the
//       std::string cast operator serializes grids.
// post: None, aside from description.
Grid::Grid(std::string const &text)
{
    width  = 0;
    height = 0;
    std::stringstream ss(text);

    // Ensure dimensions are parsed and correct
    if (!(ss >> width)) {
        throw std::runtime_error("Failed to parse grid width.");
    } else if ( width <=0 ) {
        throw std::runtime_error("Found non-positive grid width.");
    }
    if (!(ss >> height)) {
        throw std::runtime_error("Failed to parse grid height.");
    } else if ( height <=0 ) {
        throw std::runtime_error("Found non-positive grid height.");
    }

    // Parse content of the grid
    data = new double[width*height];
    for (size_t y=0; y<height; y++) {
        for (size_t x=0; x<height; x++) {
            if ( !(ss >> at(x,y)) ) {
                std::stringstream msg_ss;
                msg_ss << "Failed to parse grid value for position "
                       << "("<<x<<","<<y<<").";
                throw std::runtime_error(msg_ss.str());
            }
        }
    }
}




// desc: A conventional destructor that de-allocates the array use to
//       store element data.
// pre : None.
// post: None, aside from description.
Grid::~Grid() {
    delete[] data;
}



// desc: Returns true if and only if the coordinatex (x,y) are contained
//       by the x bounds [0,width) and y bounds [0,height).
// pre : None.
// post: None, aside from description.
bool Grid::contains(size_t x, size_t y)
{
    return ((x>=0) && (x<width) && (y>=0) && (y<height));
}



// desc: Returns a reference to the element at (x,y) in the grid.
// pre : The provided (x,y) coordinates must fit within the grid bounds.
//       If this is not the case, an out-of-range exception will be
//       thrown.
// post: None, aside from description.
double &Grid::at(size_t x, size_t y) {
    if (!contains(x, y)) {
        throw std::out_of_range("Coordinates out of range.");
    }
    return data[width*y+x];
}



// desc: Returns the width of the grid.
// pre : None.
// post: None, aside from description.
size_t Grid::get_width() const
{
    return width;
}




// desc: Returns the height of the grid.
// pre : None.
// post: None, aside from description.
size_t Grid::get_height() const
{
    return height;
}




// desc: Sets the global display width limit.
// pre : None.
// post: None, aside from description.
void Grid::set_max_display_width(size_t new_width)
{
    max_display_width = new_width;
}




// desc: Sets the global display height limit.
// pre : None.
// post: None, aside from description.
void Grid::set_max_display_height(size_t new_height)
{
    max_display_height = new_height;
}



// desc: Serializes the grid's dimensions and data content into a human-
//       readable CSV representation.
// pre : None.
// post: None, aside from description.
Grid::operator std::string ()
{
    std::stringstream ss;
    // The first row is <width>,<height>
    ss << width;
    ss << height;
    // The following rows desribe the grid content in a conventional 2D CSV  // data format
    for (size_t y=0; y<height; y++) {
        for (size_t x=0; x<height; x++) {
            if (x==0) {
                ss << ' ';
            }
            ss << at(x,y);
        }
    }
    return ss.str();
}



// desc: Sets all elements of the grid to zero.
// pre : None.
// post: None, aside from description.
void Grid::clear()
{
    for (size_t y=0; y<height; y++) {
        for (size_t x=0; x<width; x++) {
            at(x,y) = 0;
        }
    }
}




// desc: Displays the grid's content as a grid of greyscale "pixels" via
//       ANSI-escaped strings through stdout.
// pre : Both max_display_width and max_display_height must be positive.
//       If they are not, a runtime exception will be thrown.
// post: None, aside from description.
void Grid::display()
{

    // Determine how much the grid should be scaled down by.
    size_t shrink_factor = 1;
    if ( width > max_display_width ) {
        shrink_factor = (width+(max_display_width-1))/max_display_width;
    }
    if ( (height/shrink_factor) > max_display_height ) {
        shrink_factor = (height+(max_display_height-1))/max_display_height;
    }

    // The width that the grid will be displayed at.
    size_t display_width  = std::min(width,max_display_width);
    size_t display_height = std::min(width,max_display_height);

    std::stringstream ss;
    for (size_t dy=0; dy<display_height; dy++) {
        for (size_t dx=0; dx<display_width; dx++) {
            // To scale down oversized grids, take the average of each
            // element in the real grid to determing the value for the
            // corresponding element in the display grid.
            double total = 0;
            size_t count = 0;

            size_t x_start = dx*shrink_factor;
            size_t x_limit = std::min(x_start+shrink_factor,width);
            size_t y_start = dy*shrink_factor;
            size_t y_limit = std::min(y_start+shrink_factor,height);

            for (size_t y=y_start; y<y_limit; y++) {
                for (size_t x=x_start; x<x_limit; x++) {
                    total += at(x,y);
                    count ++;
                }
            }

            double mean = total / count;

            // Scale one unit of heat to 1/4 brightness. This should avoid
            // most oversaturation cases, since each element has at most
            // four neighbors contributing to its energy content.
            int v = std::min(mean*32,255.d);
            ss << "\033[48;2;"<<v<<";"<<v<<";"<<v<<"m  ";
        }
        ss << "\033[0m\n";
    }
    // Write the entire string in one system call to avoid any prints
    // breaking up the image.
    std::string text = ss.str();
    std::cout.write(text.data(),text.size());
}

size_t Grid::max_display_height=32;
size_t Grid::max_display_width=32;

//
// Created by Enam on 10/16/2024.
//

#ifndef P2PC_GRID_H
#define P2PC_GRID_H

#endif //P2PC_GRID_H
