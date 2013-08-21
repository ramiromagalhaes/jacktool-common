#include "markings.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

Markings::Markings() : empty(new std::vector<Rectangle>())
{
}

Markings::~Markings()
{
    reset();
}

void Markings::reset()
{
    std::map<std::string, std::vector<Rectangle>*>::iterator it = exclusions.begin();
    const std::map<std::string, std::vector<Rectangle>*>::iterator end = exclusions.end();
    for(; it != end; ++it) {
        std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;
        delete v.second;
    }

    exclusions.clear();
}

void Markings::setBaseDirectory(const std::string base_directory_)
{
    base_directory = base_directory_;
}

std::string Markings::getBaseDirectory() const
{
    return base_directory;
}

bool Markings::loadFilenamesFromBaseDirectory()
{
    if ( !boost::filesystem::exists( base_directory ) ) return false;

    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
    for ( boost::filesystem::directory_iterator itr( base_directory ); itr != end_itr; ++itr )
    {
        if ( boost::filesystem::is_regular_file(itr->status()) )
        {
            const boost::filesystem::path p = itr->path();
            if ( p.extension() == ".jpg"
                 || p.extension() == ".png"
                 || p.extension() == ".bmp"
                 || p.extension() == ".pgm"
                 || p.extension() == ".JPG"
                 || p.extension() == ".PNG"
                 || p.extension() == ".BMP"
                 || p.extension() == ".PGM" )
            {
                std::vector<Rectangle> * nothingHere = new std::vector<Rectangle>();
                set(p.filename().string(), nothingHere);
            }
        }
    }

    return false;
}

std::vector<Rectangle> * Markings::set(const std::string image, std::vector<Rectangle> * markings)
{
    std::map<std::string, std::vector<Rectangle> *>::iterator it = exclusions.find(image);

    if ( it != exclusions.end() )
    {
        std::vector<Rectangle> * returnMe = exclusions[image];
        exclusions[image] = markings;
        return returnMe;
    }

    exclusions.insert(std::map<std::string, std::vector<Rectangle> *>::value_type(image, markings));
    return 0;
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
    boost::filesystem::path defaultSave( base_directory );
    defaultSave = defaultSave / ".jacktool.data";
    const std::string s = defaultSave.native();
    return save(s);
}

bool Markings::save(const std::string & markingsFile)
{
    boost::filesystem::path baseDir( base_directory );

    {//removes images that do not exist in the base_directory
        std::map<std::string, std::vector<Rectangle> *>::iterator it = exclusions.begin();
        const std::map<std::string, std::vector<Rectangle> *>::iterator end = exclusions.end();
        for(; it != end;)
        {
            const std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;
            const boost::filesystem::path imagePath = baseDir / v.first;

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

    boost::filesystem::ofstream outputStream(markingsFile, std::ios::out | std::ios::trunc);
    if (!outputStream.is_open())
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

bool Markings::load(const std::string & markingsFile)
{
    boost::filesystem::path archive( markingsFile );
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
