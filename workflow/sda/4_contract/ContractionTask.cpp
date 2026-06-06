#include <sstream>
#include "ContractionTask.h"
#include <fishnet/Polygon.hpp>
#include <fishnet/Shapefile.hpp>
#include <CLI/CLI.hpp> //CLI11 2.2 Copyright (c) 2017-2024 University of Cincinnati, developed by Henry Schreiner under NSF AWARD 1414736. All rights reserved.
namespace fs = std::filesystem;

int main(int argc, const char * argv[]){
    
    using GeometryType = fishnet::geometry::Polygon<double>;
    CLI::App app {"ContractionTask"};
    fs::path currentFile = __FILE__;
    fs::path projectRoot = currentFile.parent_path().parent_path().parent_path().parent_path();
    fs::path input = projectRoot / "resources" / "sda-workflow"/ "Corvara_IT_filtered.shp";
    std::cout<< input << std::endl;
    std::vector<std::string> inputFilenames = {{"/mnt/c/Users/sarkisyan/Desktop/fishnet/2024-ba-hripsime-sarkisyan/resources/sda-workflow/Corvara_IT_filtered.shp"}}; 
    std::cout<< inputFilenames[0] << std::endl; 
    // {{"/home/lolo/Documents/fishnet/app/settlement_delineation_pattern_analysis/test/workingDirectory/Punjab_0_1_filtered.shp"}};
    std::vector<ComponentReference> components;// {{{138241},{138240}}};
    std::string configFilename = "/mnt/c/Users/sarkisyan/Desktop/fishnet/2024-ba-hripsime-sarkisyan/workflow/graph-construction-config.json" ;
    //="/home/lolo/Documents/fishnet/app/settlement_delineation_pattern_analysis/cfg/contraction.json";
    
    fs::path output_recources = projectRoot / "resources";
    std::string outputStem = "fishnet_Corvara_IT_contr500" ;//="Test.shp";
    std::string outputDirectory = output_recources.string();
    size_t workflowID = 0;
    app.add_option("-w,--workflowID",workflowID,"Unique workflow id (optional)")->check(CLI::PositiveNumber);
    app.add_option("-i,--inputs",inputFilenames,"Input Shapefiles storing the polygons with id for the contraction")->required()->each([](const std::string & str){
        try{
            auto file = fishnet::Shapefile(str);
            std::cout<< file << std::endl;
            if(not file.exists())
                throw std::invalid_argument("File "+ file.getPath().string() + " does not exist");
        }catch(std::invalid_argument & error){
            throw CLI::ValidationError(error.what());
        }
    });
    app.add_option("-c,--config",configFilename,"Path to configuration file")->required()->check(CLI::ExistingFile);
    app.add_option("--outputStem",outputStem,"Output file path, storing the merged polygons after performing the contraction on all inputs")->required();
    app.add_option("--components",components,"Component ids of connected components to contract")->each([](const std::string & str){
        std::stringstream stringStream {str};
        ComponentReference compRef;
        decltype(compRef.componentId) id = 0;
        if(not stringStream >> id){
            throw CLI::ValidationError("Could not parse \""+str+"\" to a component id");
        }
    });
    app.add_option("--outputDir",outputDirectory,"Output directory, created file at this directory with the filename ${outputStem}.shp")->check(CLI::ExistingDirectory);
    //CLI11_PARSE(app,argc,argv); //TODO Comment me out and use static paths
    if(inputFilenames.size() < 1)
        throw std::runtime_error("No input files provided");
    ContractionConfig config {json::parse(std::ifstream(configFilename))};
    config.params.host = "localhost";
    auto outputPath = std::filesystem::path(outputDirectory) / std::filesystem::path(outputStem+".shp");
    fishnet::Shapefile output {outputPath};
    ContractionTask<GeometryType> task {std::move(config),std::move(components),output,workflowID};
    for(auto && input : inputFilenames) {
        std::cout<< input << std::endl;
        task.addInput({input});
    }
    auto start = std::chrono::high_resolution_clock::now();
    task.run();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::chrono::duration<double> duration_sec = std::chrono::duration_cast<std::chrono::duration<double>>(duration);

    fs::path output_time = projectRoot / "stats" / "final_dbscan_duration.txt";
    std::ofstream out(output_time, std::ios::app);
    out << "Tokio, JP, maxDist=1k, maxN=10, FISHNET contr.distance = 500 \n";
    out << "Computation time:" << duration << ", in seconds: "<<  duration_sec <<  "\n";
 
    return 0;
}