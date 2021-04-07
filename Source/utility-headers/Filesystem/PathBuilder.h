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
        static std::shared_ptr<PathBuilder> GetPathBuilder(PdfPreset preset, bool userOutputFolder, std::shared_ptr<DirectoryChooser> chooser, std::shared_ptr<PreferenceWriter> writer);
        
        virtual boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const = 0;
    protected:
        PathBuilder(){};
    };
    
    
    class ManufacturingPathBuilder : public PathBuilder
    {
    public:
        ManufacturingPathBuilder(std::shared_ptr<DirectoryChooser> chooser, std::shared_ptr<PreferenceWriter> writer) : dirChooser(chooser), prefWriter(writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
        
    private:
        std::shared_ptr<DirectoryChooser> dirChooser;
        std::shared_ptr<PreferenceWriter> prefWriter;
    };
    
    class ProofPathBuilder : public PathBuilder
    {
    public:
        ProofPathBuilder(std::shared_ptr<DirectoryChooser> chooser, std::shared_ptr<PreferenceWriter> writer) : dirChooser(chooser), prefWriter(writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;

    private:
        std::shared_ptr<DirectoryChooser> dirChooser;
        std::shared_ptr<PreferenceWriter> prefWriter;
    };
    
    class MicrProofPathBuilder : public PathBuilder
    {
    public:
        MicrProofPathBuilder(std::shared_ptr<DirectoryChooser> chooser, std::shared_ptr<PreferenceWriter> writer) : dirChooser(chooser), prefWriter(writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
        
    private:
        std::shared_ptr<DirectoryChooser> dirChooser;
        std::shared_ptr<PreferenceWriter> prefWriter;
    };
 
    class UserPathBuilder : public PathBuilder
    {
    public:
        UserPathBuilder(std::shared_ptr<DirectoryChooser> chooser, std::shared_ptr<PreferenceWriter> writer) : dirChooser(chooser), prefWriter(writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
        
    private:
        std::shared_ptr<DirectoryChooser> dirChooser;
        std::shared_ptr<PreferenceWriter> prefWriter;
    };
    
    class TestingPathBuilder : public PathBuilder
    {
    public:
        TestingPathBuilder(std::shared_ptr<DirectoryChooser> chooser, std::shared_ptr<PreferenceWriter> writer) : dirChooser(chooser), prefWriter(writer) {};
        boost::filesystem::path GetPath(const SafeguardFile::PlateNumber pn) const override;
        
    private:
        std::shared_ptr<DirectoryChooser> dirChooser;
        std::shared_ptr<PreferenceWriter> prefWriter;
    };
}
#endif /* defined(__SafeguardTools__PathBuilder__) */

