#include "nano_timer.hh"
#include "config.hh"
#include "fileio.hh"
#include "hill_climb.hh"
#include "point_quadtree/point_quadtree.hh"
#include "tour.hh"

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

int main(int argc, const char** argv)
{
    if (argc == 1) {
        std::cout << "Arguments: config_file_path" << std::endl;
    }
    // Read config file.
    const std::string config_path = (argc == 1) ? "config.txt" : argv[1];
    std::cout << "Reading config file: " << config_path << std::endl;
    Config config(config_path);

    // Read input files.
    const std::optional<std::string> tsp_file_path_string = config.get("tsp_file_path");
    if (not tsp_file_path_string) {
        std::cout << "tsp_file_path not specified.\n";
        return EXIT_FAILURE;
    }
    const std::optional<std::filesystem::path> tsp_file_path(*tsp_file_path_string);
    const auto [x, y] = fileio::read_coordinates(*tsp_file_path_string);
    const auto initial_tour = fileio::initial_tour(x.size(), config.get("tour_file_path"));

    // Initial tour length calculation.
    point_quadtree::Domain domain(x, y);
    std::cout << "domain aspect ratio: " << domain.xdim(0) / domain.ydim(0) << std::endl;
    std::cout << "bounding x, y dim: "
        << domain.xdim(0) << ", " << domain.ydim(0)
        << std::endl;

    Tour tour(&domain, initial_tour);
    const auto initial_tour_length = tour.length();
    std::cout << "Initial tour length: " << initial_tour_length << std::endl;

    // Quad tree.
    NanoTimer timer;
    timer.start();

    std::cout << "\nquadtree stats:\n";
    const auto root {point_quadtree::make_quadtree(x, y, domain)};
    std::cout << "node ratio: "
        << static_cast<double>(point_quadtree::count_nodes(root))
            / point_quadtree::count_points(root)
        << std::endl;
    std::cout << "Finished quadtree in " << timer.stop() / 1e9 << " seconds.\n\n";

    PointSet point_set(root, x, y);

    // hill climb from initial tour.
    HillClimber hill_climber(point_set);
    const auto &kmax = config.get<size_t>("kmax", 3);
    std::cout << "kmax: " << kmax << std::endl;
    auto new_length = hill_climb::hill_climb(hill_climber, tour, kmax);
    std::cout << "non-feasible checks: " << hill_climber.nonfeasible_checks_ << std::endl;

    std::cout << std::endl;
    std::cout << "starting tour length: " << initial_tour_length << std::endl;
    std::cout << "finished tour length: " << new_length << std::endl;

    // Write new tour if better.
    if (new_length < initial_tour_length) {
        const auto &output_path = config.get("output_path");
        if (output_path) {
            fileio::write_ordered_points(tour.order(), *output_path);
            std::cout << "saved new tour to " << *output_path << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
