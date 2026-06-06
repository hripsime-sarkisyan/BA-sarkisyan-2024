# src/examples/CMakeLists.txt
# add executables in the src folder
add_executable(Example1 example1.cpp)
add_executable(Example2 example2.cpp)
add_executable(Example3 example3.cpp)
add_executable(Example4 example4.cpp)
add_executable(Example5 example5.cpp)
add_executable(Example6 example6.cpp)
add_executable(Example7 example7.cpp)

find_package(CGAL REQUIRED)

find_package(GMP REQUIRED)
find_package(MPFR REQUIRED)

# Link all executables to Fishnet
target_link_libraries(Example1 Fishnet::Fishnet Fishnet::Workflow CGAL::CGAL)
target_link_libraries(Example2 Fishnet::Fishnet Fishnet::Workflow CGAL::CGAL)
target_link_libraries(Example3 Fishnet::Fishnet Fishnet::Workflow CGAL::CGAL)
target_link_libraries(Example4 Fishnet::Fishnet Fishnet::Workflow CGAL::CGAL)
target_link_libraries(Example5 Fishnet::Fishnet Fishnet::Workflow CGAL::CGAL)
target_link_libraries(Example6 Fishnet::Fishnet Fishnet::Workflow CGAL::CGAL)


target_include_directories(Example7 PRIVATE
    ${FISHNET_SOURCE_DIR}/lib/workflow/graph-database-adapter/include
)

target_link_libraries(Example7
    PRIVATE
        Fishnet::Fishnet 
        Fishnet::Workflow 
        CGAL::CGAL
        gmp
        gmpxx
        mpfr
)

