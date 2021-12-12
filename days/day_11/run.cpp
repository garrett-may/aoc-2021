#include <array>
#include <experimental/array>
#include <concepts>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <variant>

#include <iostream>

template<std::size_t size>
using grid_t = std::array<std::array<std::size_t, size>, size>;

constexpr std::size_t char_to_int(char c) noexcept{
    return c - '0';
}

template<std::size_t size>
constexpr auto make_grid(std::array<char const*, size> const& input) noexcept{
    grid_t<size> grid{};
    for(std::size_t i = 0; i < size; ++i){
        for(std::size_t j = 0; j < size; ++j){
            grid[i][j] = char_to_int(input[i][j]);
        }
    }
    return grid;
}


constexpr void print(auto const& grid) noexcept{
    std::cout << std::endl;
    for(auto const& row : grid){
        for(auto const& value : row){
            std::cout << (value != 0 ? "\033[0;37m" : "\033[1;37m") << value;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

constexpr void increment(auto& grid) noexcept{
    for(auto& row : grid){
        for(auto& value : row){
            value += 1;
        }
    }
}

template<std::size_t size>
constexpr void spread_recursive(grid_t<size>& grid, std::size_t i, std::size_t j) noexcept{
    if(grid[i][j] <= 9){
        return;
    }
    grid[i][j] += 1;
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            if(
                (x == -1 && i == 0) ||
                (y == -1 && j == 0) ||
                (x == 1 && i == size - 1) ||
                (y == 1 && j == size - 1) ||
                (x == 0 && y == 0)
            ){
                continue;
            }            
            if(grid[i + x][j + y] <= 9){
                grid[i + x][j + y] += 1;                
                spread_recursive(grid, i + x, j + y);
            }
        }
    }
}

template<std::size_t size>
constexpr void spread(grid_t<size>& grid) noexcept{
    for(std::size_t i = 0; i < size; ++i){
        for(std::size_t j = 0; j < size; ++j){
            if(grid[i][j] == 10){
                spread_recursive(grid, i, j);
            }            
        }
    }
}

constexpr std::size_t flash(auto& grid) noexcept{
    std::size_t sum = 0;
    for(auto& row : grid){
        for(auto& value : row){
            if(value > 9){
                value = 0;
                sum += 1;
            }
        }
    }
    return sum;
}

template<std::size_t number_of_days>
constexpr auto simulate_flashes(auto const& input) noexcept{
    auto grid = make_grid(input);
    std::size_t sum = 0;
    for(std::size_t day = 0; day < number_of_days; ++day){        
        increment(grid);
        spread(grid);
        sum += flash(grid);
        //print(grid);        
    }
    return sum;
}

template<std::size_t size>
constexpr std::size_t synchronize_flash(std::array<char const*, size> const& input) noexcept{
    auto grid = make_grid(input);
    for(std::size_t day = 1; true; ++day){        
        increment(grid);
        spread(grid);
        if(flash(grid) == size * size){
            return day;
        }     
    }
    return 0;
}

constexpr static auto input_example = std::experimental::make_array(
    "5483143223",
    "2745854711",
    "5264556173",
    "6141336146",
    "6357385478",
    "4167524645",
    "2176841721",
    "6882881134",
    "4846848554",
    "5283751526"
);

constexpr static auto input = std::experimental::make_array(
    "3322874652",
    "5636588857",
    "7755117548",
    "5854121833",
    "2856682477",
    "3124873812",
    "1541372254",
    "8634383236",
    "2424323348",
    "2265635842"
);

constexpr static std::size_t part_1 = simulate_flashes<100>(input);
constexpr static std::size_t part_2 = synchronize_flash(input);

int main(){
    std::cout << "Answer to part 1: " << part_1 << std::endl;
    std::cout << "Answer to part 2: " << part_2 << std::endl;
    
    return 0;
}
