#include <iostream>

#include "dstar_lite/dstar.h"

int main(int argc, char **argv) {
  // Check that there are 3 arguments, aka argc == 4.
  if (argc != 4) {
    std::cerr << "Terminating. Incorrect number of arguments."
              << "Expected 3." << std::endl;
    return EXIT_FAILURE;
  }

  // Parse grid.
  std::string flat_grid = argv[3];

  // Construct a grid.
  const uint grid_size = (uint)std::sqrt(flat_grid.size());
  std::vector<std::vector<int>>
      occupancy_grid (grid_size, vector<int>(grid_size));

  // Parse input
  // Retrieve start position
  const uint start = std::atoi(argv[1]);
  std::array<uint, 2> start_indices;
  start_indices[0] = std::floor(start / grid_size); //x index
  start_indices[1] = start - start_indices[0] * grid_size; //y index

  // Retrieve goal position
  const uint goal = std::atoi(argv[2]);
  std::array<uint, 2> goal_indices;
  goal_indices[0] = std::floor(goal / grid_size); //x index
  goal_indices[1] = goal - goal_indices[0] * grid_size; //y index

  // Reshape input to a grid with x, y coordinates
  for (uint i = 0, j = 0, k = 0; k < flat_grid.size(); k++) {
    j = k % grid_size;

    // Check that we are not out of bounds.
    if ( i < grid_size && j < grid_size) {
      const int a = (int)flat_grid.at(k) - 48;
      occupancy_grid[i][j] = a;
    } else {
      std::cerr << "Index out of bounds, check that"
                   " input grid is squared." << std::endl;
      return EXIT_FAILURE;
    }

    if (j == 0) {
      if (first) {
        first = false;
      } else {
        i++;
      }
    }
  }

  Dstar* dstar = new Dstar();
  dstar->init(start_indices[0], start_indices[1], goal_indices[0], goal_indices[1]);
  for (uint i = 0; i < occupancy_grid.size(); i++) {
    for (uint j = 0; j < occupancy_grid.at(i).size(); j++) {
      std::cout << "Occ grid vals: " << occupancy_grid[i][j] << '\n';
      if (occupancy_grid.at(i).at(j) == 1) {
        dstar->updateCell(i+1, j+1, -1);
      }
    }
  }

  if (!dstar->replan()) {
    std::cerr << "No found path to goal" << std::endl;
    return EXIT_FAILURE;
  }

  list<state> path = dstar->getPath();
  for(const state& waypoint: path) {
    std::cout << "Waypoint: "
              << waypoint.x << ", "
              << waypoint.y << std::endl;
  }

  return EXIT_SUCCESS;
}
