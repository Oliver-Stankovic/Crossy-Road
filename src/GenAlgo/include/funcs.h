#include <string>
#include <algorithm>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>

struct Candidate {
    std::string value;
    std::size_t fitness = 0;  
};

bool is_valid_sequence(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    if (str.length() != 9) return false;

    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
}

std::vector<Candidate> generate_population(std::size_t count) {
    std::vector<Candidate> result;
    result.reserve(count);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dist(0, 999999999);

    // Reuse the string stream across iterations for better performance
    std::ostringstream oss;

    for (std::size_t i = 0; i < count; ++i) {
        long long num = dist(gen);

        oss.str("");   // Clear the contents
        oss.clear(); // Clear any error flags
        oss << std::setw(9) << std::setfill('0') << num;

        result.push_back(Candidate{oss.str()});
    }
    return result;
}

void score(Candidate& candidate, const std::string& target) {
    candidate.fitness = 0;   // reset first
    for (std::size_t i = 0; i < candidate.value.length(); ++i)
        if (candidate.value[i] == target[i]) ++candidate.fitness;
}

std::vector<Candidate> newGeneration(std::size_t keepcount, std::vector<Candidate> population){
    std::vector<Candidate> next;
    next.reserve(population.size());

    std::size_t actual_keep = std::min(keepcount, population.size());

    for (std::size_t i = 0; i < actual_keep; ++i)
        next.push_back(population[i]);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> pick(0, 8);
    std::uniform_real_distribution<double> mutationchance(0, 1);
    std::uniform_int_distribution<std::size_t> mutationval(0, 3);

    while (next.size() < population.size()){
        const Candidate& p1 = population[next.size()%(population.size()/2)];
        const Candidate& p2 = population[(next.size() + 1)%(population.size()/2)];

        size_t pos = pick(gen);
        Candidate child = {p1.value.substr(0, pos) + p2.value.substr(pos)}; 

        for (size_t i = 0; i<child.value.length(); i++){
            double chance = mutationchance(gen); 
            int current_digit = child.value[i] - '0'; 

            if (chance > 0.975) {
                current_digit = (current_digit + mutationval(gen)) % 10;
                child.value[i] = '0' + current_digit;
            } 
            else if (chance < 0.025) {
                current_digit = (current_digit - mutationval(gen) + 10) % 10;
                child.value[i] = '0' + current_digit;
            }
        }
        next.push_back(child);
    }
    return next;
}
