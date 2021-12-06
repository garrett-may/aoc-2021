#include <array>
#include <concepts>

#include <iostream>

template<std::integral T>
constexpr T str_to_int(char const* str, char const** endptr) noexcept{
    T i = 0;
    for(; *str != '\0'; ++str){
        char c = *str;
        if('0' <= c && c <= '9'){
            i = i * 10 + (c - '0');
        } else{
            if(endptr){
                *endptr = str;
            }
            return i;
        }
    }
    if(endptr){
        *endptr = str;
    }
    return i;
}

template<std::size_t no_of_days>
constexpr auto simulate(char const* str) noexcept{
    std::array<std::size_t, 7> lanternfish{};   // base school of fish stored in a ring
    std::array<std::size_t, 2> newlanternfish{}; // new school of fish stored in a queue
    
    // parsing (assumes no new fish, but could
    // be modified to handle this if required)
    while(*str != '\0'){
        auto value = str_to_int<std::size_t>(str, &str);
        lanternfish[value] += 1;        
        if(*str == ','){
            ++str;
        }
    }
    
    std::size_t offset = 0;
    for(std::size_t day = 0; day < no_of_days; ++day){
        auto value = newlanternfish[0];          // store fish of age=7
        newlanternfish[0] = newlanternfish[1];   // fish of age=8 are now age=7
        newlanternfish[1] = lanternfish[offset]; // fish of age=0 spawn fish of age=8  
        lanternfish[offset] += value;            // fish of age=0 will next turn be fish
                                                 // of age=6, so add previous fish of age=7
        offset = (offset + 1) % 7;               // rotate around the school of fish ages
    }
    
    // summation
    std::size_t sum = 0;
    for(auto value : lanternfish){
        sum += value;
    }
    for(auto value : newlanternfish){
        sum += value;
    }    
    return sum;
}

constexpr static char const* input_example = "3,4,3,1,2";

constexpr static char const* input = "1,1,3,1,3,2,1,3,1,1,3,1,1,2,1,3,1,1,3,5,1,1,1,3,1,2,1,1,1,1,4,4,1,2,1,2,1,1,1,5,3,2,1,5,2,5,3,3,2,2,5,4,1,1,4,4,1,1,1,1,1,1,5,1,2,4,3,2,2,2,2,1,4,1,1,5,1,3,4,4,1,1,3,3,5,5,3,1,3,3,3,1,4,2,2,1,3,4,1,4,3,3,2,3,1,1,1,5,3,1,4,2,2,3,1,3,1,2,3,3,1,4,2,2,4,1,3,1,1,1,1,1,2,1,3,3,1,2,1,1,3,4,1,1,1,1,5,1,1,5,1,1,1,4,1,5,3,1,1,3,2,1,1,3,1,1,1,5,4,3,3,5,1,3,4,3,3,1,4,4,1,2,1,1,2,1,1,1,2,1,1,1,1,1,5,1,1,2,1,5,2,1,1,2,3,2,3,1,3,1,1,1,5,1,1,2,1,1,1,1,3,4,5,3,1,4,1,1,4,1,4,1,1,1,4,5,1,1,1,4,1,3,2,2,1,1,2,3,1,4,3,5,1,5,1,1,4,5,5,1,1,3,3,1,1,1,1,5,5,3,3,2,4,1,1,1,1,1,5,1,1,2,5,5,4,2,4,4,1,1,3,3,1,5,1,1,1,1,1,1";

constexpr static std::size_t part_1 = simulate<80>(input);
constexpr static std::size_t part_2 = simulate<256>(input);

int main(){
    std::cout << "Answer to part 1: " << part_1 << std::endl;
    std::cout << "Answer to part 2: " << part_2 << std::endl;
    
    return 0;
}
