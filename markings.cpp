#include "markings.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

#include "extract_patch.h"

Markings::Markings()
{
    empty = new std::vector<Rectangle>();
}

Markings::Markings(const std::string base_directory_)
{
    changeBaseDirectory(base_directory_);
}

void Markings::changeBaseDirectory(const std::string base_directory_)
{
    base_directory = base_directory_;
    is_dirty = false;

    std::map<std::string, std::vector<Rectangle>*>::iterator it = exclusions.begin();
    const std::map<std::string, std::vector<Rectangle>*>::iterator end = exclusions.end();
    for(; it != end; ++it) {
        std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;
        delete v.second;
    }

    exclusions.clear();
    load();
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

    is_dirty = true;
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

    is_dirty = true;
}

bool Markings::save()
{
    boost::filesystem::path archivePath( base_directory );
    archivePath = archivePath / ".jacktool.data";

    boost::filesystem::ofstream outputStream(archivePath);
    if (!outputStream.is_open()) //TODO has write permission?
    {
        return false;
    }

    boost::archive::text_oarchive outputArchive(outputStream);
    outputArchive << *this;
    outputStream.close();

    is_dirty = false;
    return true;
}

void Markings::processAll(const PatchExtractorConfiguration &cfg) const
{
    boost::filesystem::path archivePath( base_directory );

    for(std::map<std::string, std::vector<Rectangle>*>::const_iterator it = exclusions.begin(); it != exclusions.end(); ++it) {
        const std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;

        const boost::filesystem::path file = archivePath / v.first;

        if ( !extract_patches(file, *v.second, cfg) )
        {
            return;
        }
    }
}

bool Markings::isDirty()
{
    return is_dirty;
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
