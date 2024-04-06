import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

# Example grid and parameters
grid_size = 9
population_grid = np.random.randint(0, 10, (grid_size, grid_size))
initial_budget = 12
station_cost = 4
deputy_cost = 1

# Deputy radius mapping
deputy_radius = {0: 1, 1: 2, 5: 3, 13: 4}

def calculate_coverage(grid, station_pos, radius, visual_grid):
    """
    Calculate the number of families covered for a given station and radius.
    Update the visual grid for covered areas.
    """
    covered_families = 0
    for i in range(max(0, station_pos[0] - radius), min(grid_size, station_pos[0] + radius + 1)):
        for j in range(max(0, station_pos[1] - radius), min(grid_size, station_pos[1] + radius + 1)):
            if visual_grid[i][j] == 0:  # Mark as covered
                visual_grid[i][j] = 2
            covered_families += grid[i][j]
    return covered_families, visual_grid

def depth_limited_dfs(grid, budget, depth, current_coverage, visual_grid, max_depth):
    """
    Perform a depth-limited DFS to place stations and deputies optimally.
    """
    if depth == max_depth or budget <= 0:
        return current_coverage, visual_grid

    max_coverage = current_coverage
    best_visual_grid = np.copy(visual_grid)

    # Try placing a police station at each position
    for x in range(grid_size):
        for y in range(grid_size):
            local_visual_grid = np.copy(visual_grid)
            if budget >= station_cost:
                # Mark the station
                local_visual_grid[x][y] = 3
                new_coverage, updated_visual_grid = calculate_coverage(grid, (x, y), 1, local_visual_grid)
                total_coverage, visual_grid_at_depth = depth_limited_dfs(grid, budget - station_cost, depth + 1, current_coverage + new_coverage, updated_visual_grid, max_depth)
                if total_coverage > max_coverage:
                    max_coverage = total_coverage
                    best_visual_grid = np.copy(visual_grid_at_depth)

            # Try placing deputies for each station configuration
            for deputies, radius in deputy_radius.items():
                local_visual_grid = np.copy(visual_grid)
                total_deputy_cost = station_cost + deputies * deputy_cost
                if budget >= total_deputy_cost:
                    # Mark the station with deputies
                    local_visual_grid[x][y] = 4
                    new_coverage, updated_visual_grid = calculate_coverage(grid, (x, y), radius, local_visual_grid)
                    total_coverage, visual_grid_at_depth = depth_limited_dfs(grid, budget - total_deputy_cost, depth + 1, current_coverage + new_coverage, updated_visual_grid, max_depth)
                    if total_coverage > max_coverage:
                        max_coverage = total_coverage
                        best_visual_grid = np.copy(visual_grid_at_depth)

    return max_coverage, best_visual_grid

def plot_grid(visual_grid):
    """
    Plot the grid showing stations, deputies, and coverage.
    """
    fig, ax = plt.subplots()
    ax.imshow(visual_grid, cmap='Wistia', interpolation='nearest')

    # Adding gridlines
    ax.set_xticks(np.arange(-.5, grid_size, 1), minor=True)
    ax.set_yticks(np.arange(-.5, grid_size, 1), minor=True)
    ax.grid(which='minor', color='black', linestyle='-', linewidth=2)

    # Adding annotations
    for i in range(grid_size):
        for j in range(grid_size):
            if visual_grid[i][j] == 3:
                ax.text(j, i, 'S', ha='center', va='center', color='black')
            elif visual_grid[i][j] == 4:
                ax.text(j, i, 'D', ha='center', va='center', color='black')

    plt.show()

# Initialize a visual grid to track placements and coverage
visual_grid = np.zeros((grid_size, grid_size))

# Run the depth-limited DFS and get the visualization grid
max_depth = initial_budget // min(station_cost, deputy_cost)
max_covered_families, final_visual_grid = depth_limited_dfs(population_grid, initial_budget, 0, 0, visual_grid, max_depth)

print(f"Maximum families covered: {max_covered_families}")

# Plot the final state
plot_grid(final_visual_grid)
