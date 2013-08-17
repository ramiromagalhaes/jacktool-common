#ifndef EXCLUSION_H
#define EXCLUSION_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Rectangle {
public:
    int x, y, width, height;

    Rectangle() { }

    bool contains(int x, int y) {
        return this->x <= x && x <= this->x + width &&
                this->y <= y && y <= this->y + height;
    }

    int bottom() {
        return y + height;
    }

    int right() {
        return x + width;
    }

private:
    friend class boost::serialization::access;

    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
        ar & x;
        ar & y;
        ar & width;
        ar & height;
    }

};

#endif // EXCLUSION_H
