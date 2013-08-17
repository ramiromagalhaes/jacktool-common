#ifndef MARKINGS_H
#define MARKINGS_H

#include <string>
#include <vector>
#include <map>
#include "Rectangle.h"
#include "PatchExtractorConfiguration.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>


class Markings
{
public:
    Markings();
    Markings(const std::string base_directory_);
    void changeBaseDirectory(const std::string base_directory_);
    void set(const std::string image, std::vector<Rectangle> * markings);
    const std::vector<Rectangle> * get(const std::string & image) const;
    void remove(const std::string & image);
    bool save();
    void processAll(const PatchExtractorConfiguration & cfg) const;

    bool isDirty();

private:
    bool load();

    template<class Archive>
    void serialize(Archive & ar, const unsigned int);


    friend class boost::serialization::access;

    const std::vector<Rectangle> * empty;


    bool is_dirty;
    std::string base_directory;
    std::map<std::string, std::vector<Rectangle>*> exclusions;
};

#endif // MARKINGS_H
