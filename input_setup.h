#ifndef INPUT_SETUP_H
#define INPUT_SETUP_H

#include <cmath>
#include "input.h"

// A simulation with homogeneous r-value and a one-unit-square
// source of energy at the top-left-most element
class TopLeft : public Input {
public:
    TopLeft(size_t width, size_t height, size_t duration)
            : Input(width, height, duration) {}

    virtual double energy_at(size_t x, size_t y, size_t t) const override {
        if ((x == 0) && (y == 0)) {
            return 10.0;
        } else {
            return 0.0;
        }
    }

    virtual double conductivity_at(size_t x, size_t y, size_t t) const override {
        return 0.2;
    }
};

// A simulation with an energy source that orbits around the center
// of a square region.
class Orbit : public Input {
    size_t size;

public:
    Orbit(size_t size, size_t duration)
            : Input(size, size, duration), size(size) {}

    virtual double energy_at(size_t x, size_t y, size_t t) const override {
        double orbit_x = cos(t * 0.01) * (size / 4) + (size / 2);
        double orbit_y = sin(t * 0.01) * (size / 4) + (size / 2);
        double x_dist = (x - orbit_x) / size;
        double y_dist = (y - orbit_y) / size;

        double dist = sqrt(x_dist * x_dist + y_dist * y_dist);

        if (dist < 0.1) {
            return 0.1;
        } else {
            return 0.0;
        }
    }

    virtual double conductivity_at(size_t x, size_t y, size_t t) const override {
        return 0.8;
    }
};

// A simulation with an energy source placed by a wall of low-conductivity
// material forming a right-angle corner.
class Wall : public Input {
    size_t size;

public:
    Wall(size_t size, size_t duration)
            : Input(size, size, duration), size(size) {}

    virtual double energy_at(size_t x, size_t y, size_t t) const override {
        double center = size / 2;
        double x_dist = (x - center) / size;
        double y_dist = (y - center) / size;

        double dist = sqrt(x_dist * x_dist + y_dist * y_dist);

        if (dist < 0.1) {
            return 0.01;
        } else {
            return 0.0;
        }
    }

    virtual double conductivity_at(size_t x, size_t y, size_t t) const override {
        double center = size / 2;
        double x_dist = (x - center) / size;
        double y_dist = (y - center) / size;

        if ((x_dist > 0.25) || (y_dist > 0.25)) {
            return 0.0001;
        } else {
            return 0.02;
        }
    }
};

// A simulation with an orbiting energy source placed by a wall of low-
// conductivity material forming a right-angle corner.
class OrbitWall : public Input {
    size_t size;

public:
    OrbitWall(size_t size, size_t duration)
            : Input(size, size, duration), size(size) {}

    virtual double energy_at(size_t x, size_t y, size_t t) const override {
        double orbit_x = cos(t * 0.01) * (size / 6) + (size / 3);
        double orbit_y = sin(t * 0.01) * (size / 6) + (size / 3);
        double x_dist = (x - orbit_x) / size;
        double y_dist = (y - orbit_y) / size;

        double dist = sqrt(x_dist * x_dist + y_dist * y_dist);

        if (dist < 0.1) {
            return 0.02;
        } else {
            return 0.0;
        }
    }

    virtual double conductivity_at(size_t x, size_t y, size_t t) const override {
        double center = size / 2;
        double x_dist = (x - center) / size;
        double y_dist = (y - center) / size;

        if ((x_dist > 0.25) || (y_dist > 0.25)) {
            return 0.0001;
        } else {
            return 0.2;
        }
    }
};

#endif // INPUT_SETUP_H
