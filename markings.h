#ifndef MARKINGS_H
#define MARKINGS_H

#include <string>
#include <vector>
#include <map>
#include "Rectangle.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>


class Markings
{
public:
    Markings();
    ~Markings();

    void reset();

    void setBaseDirectory(const std::string base_directory_);
    std::string getBaseDirectory() const;
    bool loadFilenamesFromBaseDirectory();

    std::vector<Rectangle> * set(const std::string image, std::vector<Rectangle> * markings);
    const std::vector<Rectangle> * get(const std::string & image) const;
    void remove(const std::string & image);

    std::map<std::string, std::vector<Rectangle> *>::const_iterator imagesExclusionsMapBegin() const;
    std::map<std::string, std::vector<Rectangle> *>::const_iterator imagesExclusionsMapEnd() const;

    bool save();
    bool save(const std::string &markingsFile);
    bool load(const std::string & markingsFile);

private:
    template<class Archive> void serialize(Archive & ar, const unsigned int);
    friend class boost::serialization::access;

    const std::vector<Rectangle> * empty;

    std::string base_directory;
    std::map<std::string, std::vector<Rectangle>*> exclusions;
};

#endif // MARKINGS_H
