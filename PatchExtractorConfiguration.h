#ifndef PATCHEXTRACTORCONFIGURATION_H
#define PATCHEXTRACTORCONFIGURATION_H

#include <boost/filesystem.hpp>

class PatchExtractorConfiguration {
public:
    bool rotate90,rotate180,rotate270;
    int patchHeight, patchWidth;
    boost::filesystem::path destinationFolder;

    PatchExtractorConfiguration() {
        rotate90 = rotate180 = rotate270 = false;
        patchHeight = patchWidth = 20; //Pavani et al; 2010 defaults
                                       //Viola, Jones; 2004 is 24x24
    }

    PatchExtractorConfiguration(boost::filesystem::path & destinationFolder_) {
        PatchExtractorConfiguration();
        destinationFolder = destinationFolder_;
    }
};

#endif // PATCHEXTRACTORCONFIGURATION_H
