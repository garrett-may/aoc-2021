#include <array>
#include <experimental/array>

#include <iostream>

/*

Whilst using strings is user-friendly, it would require the use of a map.

Instead we convert each string to a "cave code". This lets us make a fake
map and a fake queue, by using arrays.

The cave code alphabet is as follows:

0       : sentinel value, used to detect no value present
1       : the start cave
2       : the end cave
3       : sentinel value, used for handling strings of length=1
4 - 30  : lowercase letters
31 - 56 : uppercase letters

*/

constexpr static std::size_t START_CODE = 1;
constexpr static std::size_t END_CODE = 2;

constexpr static std::size_t ALPHABET = 4 + 26 + 26; // [\0], [start], [end], [ ], [a], [b], ..., [y], [z], [A], [B], ..., [Y], [Z]

// converts a string representing a cave into a "cave code" (an integer) that also represents the cave
constexpr std::size_t str_to_cave_code(char const* str, char const** endptr) noexcept{
    std::size_t size = 0;
    std::size_t code = 0;
    for(; str[size] != '\0' && str[size] != '-'; ++size){
        // handling lowercase and uppercase letters
        std::size_t value = ('a' <= str[size] && str[size] <= 'z') ? str[size] - 'a' + 4 : (str[size] - 'A') + 4 + 26;
        code = code * ALPHABET + value;
    }
    if(endptr){
        *endptr = str + size;
    }
    if(size == 1){
        // handling single-letter caves
        code = code * ALPHABET + 3;
    }
    // handling start and end caves
    return (size == 5) ? START_CODE : (size == 3) ? END_CODE : code;
}

// checks if this cave is a big cave or not
//
// start and end are not considered big caves, despite also
// not following the rules for small caves
constexpr bool is_big(std::size_t code) noexcept{
    constexpr std::size_t elem = str_to_cave_code("A", nullptr);
    return code >= elem;
}

// a type to represent the cave map
struct cave_map_t{
    constexpr void insert(std::size_t key, std::size_t value) noexcept{
        for(auto& elem : array[key]){
            if(elem == 0){
                elem = value;
                break;
            }
        }
    }
    
    // this is "good enough" for cave strings of length<=2
    std::array<std::array<std::size_t, ALPHABET * ALPHABET>, ALPHABET * ALPHABET> array;
};

// a fixed-size queue
struct queue_t{
    constexpr void push_back(std::size_t value) noexcept{
        array[size++] = value;
    }
    constexpr bool contains(std::size_t value) noexcept{
        for(std::size_t i = 0; i < size; ++i){
            if(array[i] == value){
                return true;
            }
        }
        return false;
    }
    
    // capacity=100 is "good enough" for the problems
    std::array<std::size_t, 100> array;
    std::size_t size;
};

// parse the input as a cave map
// each path is represented twice in the map; for X-Y we have:
// - X -> Y
// - Y -> X
constexpr cave_map_t parse_cave_map(auto const& input) noexcept{
    cave_map_t cave_map{};
    for(auto const* str : input){
        auto from = str_to_cave_code(str, &str);
        ++str; // '-'
        auto to = str_to_cave_code(str, &str);
        cave_map.insert(from, to);
        cave_map.insert(to, from);
    }
    return cave_map;
}

// traverse through the caves to find the number
// of paths, by making use of recursion
//
// note that std::accumulate would have made
// things easier, but we prefer to be able
// to break out earlier for compile-time
// reasons
constexpr std::size_t traverse(std::size_t key, auto const& map, auto queue, std::size_t small_count = 1) noexcept{
    if(key == END_CODE){
        // end cave
        return 1;
    } else if(is_big(key)){
        // big cave
        std::size_t sum = 0;
        for(auto value : map.array[key]){
            if(value == 0){
                break;
            }
            sum += traverse(value, map, queue, small_count);
        }
        return sum;
    } else if(!queue.contains(key)){
        // small cave, not seen yet
        auto new_queue = queue;
        new_queue.push_back(key);
        std::size_t sum = 0;
        for(auto value : map.array[key]){
            if(value == 0){
                break;
            }
            sum += traverse(value, map, new_queue, small_count);
        }
        return sum;
    } else if(small_count == 1 && key != START_CODE){
        // small cave, seen, but can see again, and not the start cave
        std::size_t sum = 0;
        for(auto value : map.array[key]){
            if(value == 0){
                break;
            }
            sum += traverse(value, map, queue, small_count - 1);
        }
        return sum;
    } else{
        return 0;
    }
}

template<std::size_t small_count>
constexpr std::size_t how_many_paths(auto const& input) noexcept{
    auto cave_map = parse_cave_map(input);
    return traverse(START_CODE, cave_map, queue_t{}, small_count);    
}

constexpr static auto input_example_0 = std::experimental::make_array(
    "start-A",
    "start-b",
    "A-c",
    "A-b",
    "b-d",
    "A-end",
    "b-end"
);

constexpr static auto input_example_1 = std::experimental::make_array(
    "dc-end",
    "HN-start",
    "start-kj",
    "dc-start",
    "dc-HN",
    "LN-dc",
    "HN-end",
    "kj-sa",
    "kj-HN",
    "kj-dc"
);

constexpr static auto input_example_2 = std::experimental::make_array(
    "fs-end",
    "he-DX",
    "fs-he",
    "start-DX",
    "pj-DX",
    "end-zg",
    "zg-sl",
    "zg-pj",
    "pj-he",
    "RW-he",
    "fs-DX",
    "pj-RW",
    "zg-RW",
    "start-pj",
    "he-WI",
    "zg-he",
    "pj-fs",
    "start-RW"
);

constexpr static auto input = std::experimental::make_array(
    "GC-zi",
    "end-zv",
    "lk-ca",
    "lk-zi",
    "GC-ky",
    "zi-ca",
    "end-FU",
    "iv-FU",
    "lk-iv",
    "lk-FU",
    "GC-end",
    "ca-zv",
    "lk-GC",
    "GC-zv",
    "start-iv",
    "zv-QQ",
    "ca-GC",
    "ca-FU",
    "iv-ca",
    "start-lk",
    "zv-FU",
    "start-zi"
);

constexpr static std::size_t part_1 = how_many_paths<0>(input);
constexpr static std::size_t part_2 = how_many_paths<1>(input);

int main(){
    std::cout << "Answer to part 1: " << part_1 << std::endl;
    std::cout << "Answer to part 2: " << part_2 << std::endl;
    
    return 0;
}
