#ifndef P2PC_INPUT_H
#define P2PC_INPUT_H

#include <cstddef>

// Represents a input into a heat transfer simulation, providing
// (x,y) spatial bounds, time (t) bounds, as well as the energy input
// and r value for integer (x,y,t) coordinates in the simulation.
class Input
{
protected:

    size_t width;
    size_t height;
    size_t duration;

public:

    // desc: Standard constructor that initializes width, height, duration.
    // pre : None.
    // post: None, aside from description.
    Input(size_t width, size_t height, size_t duration);

    // desc: Returns the width of the Input.
    // pre : None.
    // post: None, aside from description.
    size_t get_width() const;

    // desc: Returns the height of the Input.
    // pre : None.
    // post: None, aside from description.
    size_t get_height() const;

    // desc: Returns the duration of the Input.
    // pre : None.
    // post: None, aside from description.
    size_t get_duration() const;

    // desc: Returns the energy that is added into the material at a
    //       given (x,y,t) coordinate.
    // pre : None.
    // post: None, aside from description.
    virtual double energy_at(size_t x, size_t y, size_t t) const = 0;

    // desc: Returns the R-value of the material at a given (x,y,t)
    //       coordinate.
    // pre : None.
    // post: None, aside from description.
    virtual double conductivity_at(size_t x, size_t y, size_t t) const = 0;

};

//
// Created by Enam on 10/16/2024.
//







#endif //P2PC_INPUT_H
