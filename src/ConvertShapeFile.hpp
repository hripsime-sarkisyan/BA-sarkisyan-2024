#pragma once
#include <fishnet/Fishnet.hpp>
#include <vector>
#include <unordered_map>
#include <ogr_spatialref.h>
#include "Settlement.hpp"

namespace fs = std::filesystem;


//VectorLayerWriter<ShapefileWriter<G>, G, F>

template <typename GeometryT>
int convertShapeFile(const std::unordered_map<int, std::vector<GeometryT>>& clusters, const std::string& shapefilePath) {

    if (!std::filesystem::exists(std::filesystem::path(shapefilePath).parent_path())) {
        std::cerr << "Directory does not exist: " << std::filesystem::path(shapefilePath).parent_path() << "\n";
        return 1;
    }

    using G = GeometryT;
    using F = fishnet::Shapefile;
    using LayerWriter = fishnet::ShapefileWriter<G>;


    OGRSpatialReference spatialRef;
    spatialRef.importFromEPSG(3857); //  Web Mercator — good for 2D visualization
    auto resultLayer = fishnet::VectorIO::empty<G>(spatialRef);
    auto clusterFieldExpected = resultLayer.addSizeField("cluster_id");
    if (not clusterFieldExpected) {
        return 1;
    }
    auto clusterField = clusterFieldExpected.value();

    for (const auto& [clusterID, cluster] : clusters) {
        for (const auto& geom : cluster) {
            fishnet::Feature<G> feature {geom};
            feature.addAttribute(clusterField, static_cast<std::size_t>( clusterID));
            resultLayer.addFeature(std::move(feature));
        }
    }


    fishnet::Shapefile output(shapefilePath);
    LayerWriter writer;

    // Call the overwrite function
    fishnet::VectorIO::overwrite<G, F>(writer, resultLayer, output);
    return 0;
}

template <typename PolygonT>
int convertSettlementShapeFile(
    const std::unordered_map<int, std::vector<Settlement<PolygonT>>>& clusters,
    const std::string& shapefilePath)
{
    if (!std::filesystem::exists(std::filesystem::path(shapefilePath).parent_path())) {
        std::cerr << "Directory does not exist: " << std::filesystem::path(shapefilePath).parent_path() << "\n";
        return 1;
    }

    using G = PolygonT; // We will write the internal polygon
    using F = fishnet::Shapefile;
    using LayerWriter = fishnet::ShapefileWriter<G>;

    // Create an empty vector layer for polygons
    OGRSpatialReference spatialRef;
    spatialRef.importFromEPSG(3857);
    auto resultLayer = fishnet::VectorIO::empty<G>(spatialRef);

    auto clusterFieldExpected = resultLayer.addSizeField("cluster_id");
    if (!clusterFieldExpected) return 1;
    auto clusterField = clusterFieldExpected.value();


    for (const auto& [cluster_id, cluster] : clusters) {
        for (const auto& settlement : cluster) {
            const PolygonT& poly = settlement.geometry();
            fishnet::Feature<G> feature{poly};

            // Optional: you can also store settlement ID
            feature.addAttribute(clusterField, static_cast<std::size_t>(cluster_id));

            resultLayer.addFeature(std::move(feature));
        }
    }

    fishnet::Shapefile output(shapefilePath);
    LayerWriter writer;
    fishnet::VectorIO::overwrite<G, F>(writer, resultLayer, output);

    return 0;
}
