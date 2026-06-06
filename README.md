# A Hybrid, Density-based Clustering Approach for Spatial Settlement Graphs

## Overview

This project implements a hybrid density-based clustering approach for the analysis of spatial settlement graphs. The work extends the FISHNET framework by addressing limitations of its original graph-based clustering strategy, which may produce unrealistic settlement chains and fragmented urban structures.

The proposed methodology adapts the Density-Based Spatial Clustering algorithm considering both Spatial proximity and attribute similarity (DBSC) to graph-based polygon settlement data derived from the World Settlement Footprint (WSF). Unlike traditional density-based approaches operating on point datasets, this implementation considers both spatial proximity and polygon attributes, enabling the generation of more semantically coherent urban clusters.

The primary objective is to improve urban settlement delineation by reducing chaining effects and increasing cluster compactness and connectivity while preserving meaningful spatial relationships.

## Features

* Adaptation of the DBSC algorithm to graph-based polygon data
* Integration into the FISHNET settlement analysis framework
* Clustering based on spatial proximity and settlement polygon similarity
* Support for point, polygon, and multi-polygon datasets
* Generation of GIS-compatible Shapefile outputs
* Evaluation using compactness and connectivity metrics
* Comparison with DBSCAN and baseline FISHNET clustering

## Methodology

The workflow consists of the following stages:

1. **Data Preprocessing**

   * Filtering noisy settlement pixels
   * Polygonization of WSF raster data
   * Attribute augmentation
   * Distance matrix computation
   * Graph construction

2. **Graph-Based Clustering**

   * Construction of settlement graphs
   * Application of global and local distance constraints
   * Density indicator computation
   * Cluster expansion using adapted DBSC

3. **Evaluation**

   * Runtime analysis
   * Connectivity analysis
   * Compactness analysis
   * Visual inspection of clustering results

## Dataset

The implementation uses data derived from the **World Settlement Footprint (WSF)** dataset.

Evaluation was performed using settlement datasets representing different urban morphologies:

* Corvara, Italy
* Würzburg, Germany
* Tokyo, Japan

## Requirements

### Software

* Ubuntu
* GCC 13.3.0
* CMake 3.11+
* Docker

### Libraries

* GDAL
* CGAL
* GMP
* MPFR
* FISHNET Framework

## Installation

Install the required dependencies:

```bash
sudo apt update

sudo apt install gcc-13 g++-13 cmake make -y

sudo apt install -y \
    gdal-bin \
    libgdal-dev \
    libcgal-dev \
    libcgal-demo \
    libgmp-dev \
    libmpfr-dev
```

## Build

Create a build directory and compile the project:

```bash
mkdir build
cd build

cmake ..
make
```

The project requires C++23 support and linkage with the FISHNET framework.

## Usage

### Step 1: Generate the Settlement Graph

Before clustering, construct the settlement graph from the filtered settlement polygons.

```bash
python3 SDAGraphConstruction.py \
    -i <filtered_input.shp> \
    -c graph-construction-config.json
```

The graph construction configuration may include parameters such as:

* `maxNeighbours`
* `maxDistance`
* contraction thresholds

These parameters influence graph connectivity and clustering behavior.

### Step 2: Execute Clustering

After compilation, clustering experiments can be executed using the example programs located in:

```text
src/examples/
```

The implementation supports:

* `Vec2DStd` (point datasets)
* `SimplePolygon` (polygon datasets)
* `MultiPolygon` (multi-polygon datasets)

The adapted DBSC algorithm operates directly on the generated graph representation of settlement polygons.

### Step 3: Analyze Results

The clustering process generates GIS-compatible Shapefiles containing cluster assignments.

Output attributes include:

* Cluster ID
* Polygon geometry
* Settlement attributes

Noise objects are assigned:

```text
cluster_id = -1
```

The resulting files can be visualized and analyzed using GIS software.

## Evaluation Metrics

The clustering quality is evaluated using:

### Connectivity

Connectivity measures how strongly connected settlement polygons are within a cluster. Higher connectivity indicates more stable and coherent settlement structures.

### Compactness

Compactness evaluates how tightly grouped settlement polygons are spatially. Higher compactness values correspond to more realistic urban settlement clusters.

### Computational Performance

Runtime measurements are used to compare algorithm efficiency and scalability.

## Results

Experimental results demonstrate a trade-off between computational performance and spatial quality.

Compared with DBSCAN and the baseline FISHNET clustering approach, the adapted DBSC implementation:

* Produces more compact clusters
* Improves cluster connectivity
* Reduces settlement chaining effects
* Better handles heterogeneous urban density patterns

These improvements come at the cost of increased computational effort.

## Thesis

This repository contains the implementation developed for the Bachelor's Thesis:

**A Hybrid, Density-based Clustering Approach for Spatial Settlement Graphs**

Hripsime Sarkisyan
Department of Computer Science
University of Würzburg

## Future Work

Potential extensions include:

* Integration of demographic attributes
* Incorporation of socio-economic indicators
* Support for additional density-based clustering methods
* Enhanced semantic settlement analysis
* Large-scale optimization for metropolitan datasets
