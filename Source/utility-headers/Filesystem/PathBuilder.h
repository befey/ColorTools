//
//  PathBuilder.h
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#ifndef __SafeguardTools__PathBuilder__
#define __SafeguardTools__PathBuilder__

#include "PlateNumber.h"
#include "PrintToPdfConstants.h"
#include "DirectoryChooser.hpp"
#include "PreferenceWriter.hpp"
#include <boost/filesystem.hpp>

namespace PrintToPdf
{
    class PathBuilder
    {
    public:
        std::unique_ptr<PathBuilder> GetPathBuilder(PdfPreset preset, bool userOutputFolder, DirectoryChooser& chooser, PreferenceWriter& writer);
        
        virtual boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const = 0;
    protected:
        PathBuilder(DirectoryChooser& chooser, PreferenceWriter& writer) : dirChooser(chooser), prefWriter(writer) {};
        
        DirectoryChooser& dirChooser;
        PreferenceWriter& prefWriter;
    };
    
    
    class ManufacturingPathBuilder : public PathBuilder
    {
    public:
        ManufacturingPathBuilder(DirectoryChooser& chooser, PreferenceWriter& writer) : PathBuilder(chooser, writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
    };
    
    class ProofPathBuilder : public PathBuilder
    {
    public:
        ProofPathBuilder(DirectoryChooser& chooser, PreferenceWriter& writer) : PathBuilder(chooser, writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
    };
    
    class MicrProofPathBuilder : public PathBuilder
    {
    public:
        MicrProofPathBuilder(DirectoryChooser& chooser, PreferenceWriter& writer) : PathBuilder(chooser, writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
    };
 
    class UserPathBuilder : public PathBuilder
    {
    public:
        UserPathBuilder(DirectoryChooser& chooser, PreferenceWriter& writer) : PathBuilder(chooser, writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
    };
    
    class TestingPathBuilder : public PathBuilder
    {
    public:
        TestingPathBuilder(DirectoryChooser& chooser, PreferenceWriter& writer) : PathBuilder(chooser, writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
    };
}
#endif /* defined(__SafeguardTools__PathBuilder__) */

