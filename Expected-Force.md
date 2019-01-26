# Expected-Force
## 1. Introduction

This C++ package implements the calculation the ***expected Force of Infection***, or ***expected force*** in short. Expected force is one kind of node influential measures. This [Wikipedia page](https://en.wikipedia.org/wiki/Node_influence_metric) provides a review and introduction of major node influence measures.

Expected force, put forward by Glenn Lawyer, is inspired from **Epidemiology**. This [paper](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4345333/) explains the motivation and computation of this expected force metric. This [report](https://arxiv.org/abs/1406.4785) describes the performance of expected force measure.

Glenn Lawyer has already provided an implementation based on [R igraph](https://igraph.org/r/). Igraph is one of the major network analysis packages which can be programmed in R, Python, Mathematica and C/C++. You can check the R source code of expected force [here](). This package offers a complete implementation of expected force in C/C++ to accelerate the calculation in large-scale networks. 

## 2. Installation

### installation and compiling of igraph C package in visual studio 2017

You can download the source code of igraph [here](http://igraph.org/c/#downloads) (igraph-0.7.1-msvc).

1. Open the igraph.sln solution file in igraph-0.7.1-msvc from Visual Studio 2017. Let the Visual Studio convert the solution file for you.
2. Add a compiler option (for both Debug and Release): Open Project menu -> Properties -> C/C++ -> Command Line. In Additional options, add the following option: “/DPRPACK_IGRAPH_SUPPORT=1” (without the quotes)
3. Debug -> Build and Release -> Build

### compile expected force

1. Adding a path to the directory containing ‘igraph.lib’ (for both Debug and Release). Open Project -> Properties -> VC++ Directory -> Library Directory -> Edit. Add new path which points to the Debug/Release directory in your igraph directory (igraph-0.7.1-msvc folder/Debug, for example).
2. Open Project -> Properties -> VC++ Directory -> Include Directory -> Edit. Add new path which points to the include directory in your igraph-0.7.1-msvc folder.
3. Make sure “Project -> Properties -> Linker -> General -> Additional Library Directories” points to your igraph Debug/Release directory.
4. Make sure “Project -> Properties -> Linker -> Input -> Additional Dependencies” includes igraph.lib



