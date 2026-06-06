#include <fishnet/CachingMemgraphAdjacency.hpp>
#include <fishnet/MemgraphConnection.hpp>
#include <fishnet/Polygon.hpp>
#include <fishnet/VectorIO.hpp>
#include "../Settlement.hpp"
#include <gdal/ogr_spatialref.h>
#include <fishnet/Task.hpp>
#include <fishnet/GraphFactory.hpp>
#include "../DBSC.hpp"
#include "../DBSCAN.hpp"
#include "../ConvertShapeFile.hpp"
#include "../stats.hpp"

using namespace fishnet;
using ShapeType = fishnet::geometry::Polygon<double>;
using SettlementType = Settlement<ShapeType>;
namespace fs = std::filesystem;


struct ClusteringInput{
    std::vector<SettlementType> settlements;
    OGRSpatialReference ref;
};

static ClusteringInput readInput( CachingMemgraphAdjacency<SettlementType> & adj ,const Shapefile & inputFile)  {
    std::vector<SettlementType> settlements;
    auto layer = fishnet::VectorIO::read<ShapeType>(inputFile);
    if(layer.isEmpty())
        return {settlements, OGRSpatialReference()};
    auto fileRef = adj.getDatabaseConnection().addFileReference(inputFile.getPath());
    if(not fileRef){
        throw std::runtime_error("Could not read file reference for shp file:\n"+inputFile.getPath().string());
    }
    auto optFishnetIdField = layer.getSizeField(Task::FISHNET_ID_FIELD);
    if(not optFishnetIdField) {
        throw std::runtime_error("Could not find FISHNET_ID field in shp file: \n"+inputFile.getPath().string());
    }
    for(auto & feature : layer.getFeatures()) {
        auto optId = feature.getAttribute(optFishnetIdField.value());
        if(not optId){
            throw std::runtime_error("No id exists for feature with geometry:\n"+ feature.getGeometry().toString());
        }
        auto id = optId.value();
        settlements.emplace_back(id,fileRef.value(),std::move(feature.getGeometry()));

    }
    auto ref = layer.getSpatialReference();
    adj.loadNodes(settlements);
    return ClusteringInput{.settlements = std::move(settlements), .ref = ref};
}

int main() {
    fs::path currentFile = __FILE__;
    fs::path PROJECT_ROOT = currentFile.parent_path().parent_path().parent_path();//TODO change this to your project root
    auto adjContainer = CachingMemgraphAdjacency<SettlementType>(MemgraphClient(MemgraphConnection::create("localhost",7687).value_or_throw())); 
    auto pathToShp = PROJECT_ROOT / std::filesystem::path("resources/sda-workflow/Wuerzburg_DE_filtered.shp");
    auto clusteringInput = readInput(adjContainer, Shapefile(pathToShp));
    auto graph = graph::GraphFactory::UndirectedGraph<SettlementType>(std::move(adjContainer));
    std::cout<< "connected" << pathToShp << std::endl;
    // use graph here for clustering
    using GraphType = decltype(graph);
    // Attribute extractor: polygon area
    auto areaExtractor = [](const SettlementType &poly){ return poly.getSize(); };
    // --- Run DBSC ---
    MyWorkflow::DBSC<SettlementType, GraphType> dbsc(2);   // (T1 is set to 1.9), beta is 2
    

    auto start = std::chrono::high_resolution_clock::now();
    auto clusters = dbsc.run(graph, areaExtractor);  // beta=2 (example)
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << fishnet::util::size(graph.getNodes()) << std::endl;
    // calculate the distances of nodes to optimize
    auto nodesView = graph.getNodes();
    std::vector<SettlementType> nodes(nodesView.begin(), nodesView.end());
    std::unordered_map<SettlementType, std::unordered_map<SettlementType, double>> distanceMap;
    int countDist = 0;
    for (auto node : nodes) {
        std::cout<< "node count: " << countDist << std::endl;
        std::unordered_map<SettlementType, double> nbrs_distances;
        int nbrcount = 0;
        for (auto nbr : graph.getNeighbours(node)) {
            double d = node.distance(nbr);
            nbrs_distances[nbr] = d;
            nbrcount++;
        }
        distanceMap[node] = nbrs_distances;
        countDist++;
    }

    //use mean distance as a parameter for DBSCAN
    int minPoints = 1;
    double eps = compute_eps_percentile(graph, distanceMap, minPoints, 50.0);

    MyWorkflow::DBSCAN<SettlementType, GraphType> dbscan(eps, minPoints); 
    // auto start = std::chrono::high_resolution_clock::now();
    // auto clusters = dbscan.run(graph);
    // auto stop = std::chrono::high_resolution_clock::now();
   

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::chrono::duration<double> duration_sec = std::chrono::duration_cast<std::chrono::duration<double>>(duration);

    fs::path projectRoot = currentFile.parent_path().parent_path().parent_path();
    fs::path output_time = projectRoot / "stats" / "final_dbscan_duration.txt";
    std::ofstream out(output_time, std::ios::app);
    out << "Wuerzburg, DE, maxDist=1k, maxN=10, DBSC beta=2\n";
    out << "Computation time:" << duration << ", in seconds: "<<  duration_sec <<  "\n";
 
        
    out.close();

    // --- Export result to shapefile ---
    fs::path output = projectRoot / "resources" / "final_dbsc_Wuerzburg_DE_maxDist1k_maxN10_beta2.shp";
    convertSettlementShapeFile(clusters, output);
    // convertShapeFile(clusters, output);

    // Export compactness values to a file
    fs::path CI_output = projectRoot / "stats"/ "final_dbsc_compactness_Wuerzburg_DE_maxDist1k_beta2.txt";
    compactnessIndex(clusters, CI_output);
     
    fs::path conn_output = projectRoot / "stats"/ "final_dbsc_connectivity_Wuerzburg_DE_maxDist1k_maxN10_beta2.txt";
    get_connectivity<GraphType, SettlementType>(clusters, graph, conn_output);
    return 0;
}