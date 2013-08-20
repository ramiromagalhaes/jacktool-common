#include "markings.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

Markings::Markings()
{
    empty = new std::vector<Rectangle>();
}

Markings::Markings(const std::string base_directory_)
{
    empty = new std::vector<Rectangle>();
    setBaseDirectory(base_directory_);
}

void Markings::setBaseDirectory(const std::string base_directory_)
{
    base_directory = base_directory_;

    std::map<std::string, std::vector<Rectangle>*>::iterator it = exclusions.begin();
    const std::map<std::string, std::vector<Rectangle>*>::iterator end = exclusions.end();
    for(; it != end; ++it) {
        std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;
        delete v.second;
    }

    exclusions.clear();
    load();
}

std::string Markings::getBaseDirectory() const
{
    return base_directory;
}

void Markings::set(const std::string image, std::vector<Rectangle> *markings)
{
    std::map<std::string, std::vector<Rectangle> *>::iterator it = exclusions.find(image);

    if ( it == exclusions.end() )
    {
        exclusions.insert(std::map<std::string, std::vector<Rectangle> *>::value_type(image, markings));
    }
    else
    {
        exclusions[image] = markings;
    }
}

const std::vector<Rectangle> * Markings::get(const std::string &image) const
{
    std::map<std::string, std::vector<Rectangle> *>::const_iterator it = exclusions.find(image);
    if ( it != exclusions.end() )
    {
        const std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;
        return v.second;
    }

    return empty;
}

void Markings::remove(const std::string & image)
{
    std::map<std::string, std::vector<Rectangle> *>::iterator it = exclusions.find(image);
    if ( it != exclusions.end() )
    {
        exclusions.erase(it);
    }
}

bool Markings::save()
{
    boost::filesystem::path archivePath( base_directory );

    {
        std::map<std::string, std::vector<Rectangle> *>::iterator it = exclusions.begin();
        const std::map<std::string, std::vector<Rectangle> *>::iterator end = exclusions.end();
        for(; it != end; ++it)
        {
            const std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;
            const boost::filesystem::path imagePath = archivePath / v.first;

            if ( boost::filesystem::exists(imagePath) )
            {
                ++it;
            }
            else
            {
                exclusions.erase(it++);
            }
        }
    }

    archivePath = archivePath / ".jacktool.data";

    boost::filesystem::ofstream outputStream(archivePath, std::ios::out | std::ios::trunc);
    if (!outputStream.is_open()) //TODO has write permission?
    {
        return false;
    }

    boost::archive::text_oarchive outputArchive(outputStream);
    outputArchive << *this;
    outputStream.close();

    return true;
}

std::map<std::string, std::vector<Rectangle> *>::const_iterator Markings::imagesExclusionsMapBegin() const
{
    return exclusions.begin();
}

std::map<std::string, std::vector<Rectangle> *>::const_iterator Markings::imagesExclusionsMapEnd() const
{
    return exclusions.end();
}

bool Markings::load()
{
    boost::filesystem::path archive( base_directory );
    archive = archive / ".jacktool.data";

    if ( !boost::filesystem::exists(archive) )
    {
        return false;
    }

    boost::filesystem::ifstream ifs(archive);
    boost::archive::text_iarchive ia(ifs);
    ia >> *this;
    ifs.close();

    return true;
}

template<class Archive>
void Markings::serialize(Archive &ar, const unsigned int)
{
    ar & base_directory;
    ar & exclusions;
}
