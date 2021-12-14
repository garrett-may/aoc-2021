#include <array>
#include <experimental/array>
#include <limits>

#include <iostream>

/*

We don't need to maintain the sequence in the polymer template.
All we really care about at the end is the frequency of
single-letter elements.

Solution: we keep track of the number of single-letter elements,
as well as two-letter patterns (so that we know when to replace).

Similar to day 12, we have a (smaller) alphabet as follows:

0 : sentinel value, used to detect no value present
1 - 27 : uppercase letters

*/

constexpr static std::size_t ALPHABET = 1 + 26; // [\0], [A], [B], ..., [Y], [Z]

constexpr std::size_t char_to_int(char c) noexcept{
    return (c - 'A') + 1; // sentinel will be 0, which is the default for ints
}

struct rule_t{
    std::size_t left;
    std::size_t right;
    std::size_t result;
};

struct polymer_t{
    // two-letter patterns have:
    //
    // left * ALPHABET + right
    //
    // as the index to this array
    std::array<std::size_t, ALPHABET * ALPHABET> twos;
    std::array<std::size_t, ALPHABET> ones;
};

constexpr auto parse_polymer(auto const& input) noexcept{
    polymer_t polymer{};
    char const* str = input[0];    
    polymer.ones[char_to_int(str[0])] += 1;
    for(std::size_t size = 1; str[size] != '\0'; ++size){
        auto a = char_to_int(str[size - 1]);
        auto b = char_to_int(str[size]);
        polymer.twos[a * ALPHABET + b] += 1;
        polymer.ones[b] += 1;
    }
    return polymer;
}

constexpr auto parse_rule(char const* str) noexcept{
    rule_t rule{};
    rule.left = char_to_int(str[0]);
    rule.right = char_to_int(str[1]);
    rule.result = char_to_int(str[6]);
    return rule;
}

template<std::size_t size>
constexpr auto parse_rules(std::array<char const*, size> const& input){
    std::array<rule_t, size - 2> rules{};
    for(std::size_t i = 2; i < input.size(); ++i){
        rules[i - 2] = parse_rule(input[i]);
    }
    return rules;
}

constexpr auto search_and_replace(auto const& polymer, auto const& rules) noexcept{
    // a new polymer is required in order to not introduce
    // new patterns i.e. apply rules simultaneously
    auto new_polymer = polymer;
    for(auto const& [left, right, result] : rules){
        auto pattern = left * ALPHABET + right;
        auto value = polymer.twos[pattern];
        if(value > 0){    
            new_polymer.twos[left * ALPHABET + result] += value;
            new_polymer.twos[result * ALPHABET + right] += value;
            new_polymer.twos[pattern] -= value;
            new_polymer.ones[result] += value;
        }
    }
    return new_polymer;
}

constexpr std::size_t calculate(auto const& polymer) noexcept{
    std::size_t max = std::numeric_limits<std::size_t>::min();
    std::size_t min = std::numeric_limits<std::size_t>::max();
    for(auto one : polymer.ones){
        if(one != 0){ // 0 is the sentinel
            max = std::max(max, one);
            min = std::min(min, one);
        }
    }
    return max - min;   
}

template<std::size_t number_of_steps>
constexpr auto run(auto const& input) noexcept{
    auto polymer = parse_polymer(input);
    auto rules = parse_rules(input);
    for(std::size_t step = 0; step < number_of_steps; ++step){
        polymer = search_and_replace(polymer, rules);
    }
    return calculate(polymer);
}

constexpr static auto input_example = std::experimental::make_array(
    "NNCB",
    "",
    "CH -> B",
    "HH -> N",
    "CB -> H",
    "NH -> C",
    "HB -> C",
    "HC -> B",
    "HN -> C",
    "NN -> C",
    "BH -> H",
    "NC -> B",
    "NB -> B",
    "BN -> B",
    "BB -> N",
    "BC -> B",
    "CC -> N",
    "CN -> C"
);

constexpr static auto input = std::experimental::make_array(
    "PPFCHPFNCKOKOSBVCFPP",
    "",
    "VC -> N",
    "SC -> H",
    "CK -> P",
    "OK -> O",
    "KV -> O",
    "HS -> B",
    "OH -> O",
    "VN -> F",
    "FS -> S",
    "ON -> B",
    "OS -> H",
    "PC -> B",
    "BP -> O",
    "OO -> N",
    "BF -> K",
    "CN -> B",
    "FK -> F",
    "NP -> K",
    "KK -> H",
    "CB -> S",
    "CV -> K",
    "VS -> F",
    "SF -> N",
    "KB -> H",
    "KN -> F",
    "CP -> V",
    "BO -> N",
    "SS -> O",
    "HF -> H",
    "NN -> F",
    "PP -> O",
    "VP -> H",
    "BB -> K",
    "VB -> N",
    "OF -> N",
    "SH -> S",
    "PO -> F",
    "OC -> S",
    "NS -> C",
    "FH -> N",
    "FP -> C",
    "SO -> P",
    "VK -> C",
    "HP -> O",
    "PV -> S",
    "HN -> K",
    "NB -> C",
    "NV -> K",
    "NK -> B",
    "FN -> C",
    "VV -> N",
    "BN -> N",
    "BH -> S",
    "FO -> V",
    "PK -> N",
    "PS -> O",
    "CO -> K",
    "NO -> K",
    "SV -> C",
    "KO -> V",
    "HC -> B",
    "BC -> N",
    "PB -> C",
    "SK -> S",
    "FV -> K",
    "HO -> O",
    "CF -> O",
    "HB -> P",
    "SP -> N",
    "VH -> P",
    "NC -> K",
    "KC -> B",
    "OV -> P",
    "BK -> F",
    "FB -> F",
    "FF -> V",
    "CS -> F",
    "CC -> H",
    "SB -> C",
    "VO -> V",
    "VF -> O",
    "KP -> N",
    "HV -> H",
    "PF -> H",
    "KH -> P",
    "KS -> S",
    "BS -> H",
    "PH -> S",
    "SN -> K",
    "HK -> P",
    "FC -> N",
    "PN -> S",
    "HH -> N",
    "OB -> P",
    "BV -> S",
    "KF -> N",
    "OP -> H",
    "NF -> V",
    "CH -> K",
    "NH -> P"
);

constexpr static std::size_t part_1 = run<10>(input);
constexpr static std::size_t part_2 = run<40>(input);

int main(){
    std::cout << "Answer to part 1: " << part_1 << std::endl;
    std::cout << "Answer to part 2: " << part_2 << std::endl;   
    
    return 0;
}
