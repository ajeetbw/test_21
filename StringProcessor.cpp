/**
 * @file StringProcessor.cpp
 * @brief A high-performance string processing utility for the mock SVN repository.
 * 
 * Demonstrates C++17 features, memory safety, standard library algorithms,
 * and object-oriented design principles.
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace utils {

    /**
     * @class StringAnalyzer
     * @brief Analyzes and transforms text data efficiently.
     */
    class StringAnalyzer {
    private:
        std::string raw_text;
        std::vector<std::string> tokens;
        std::unordered_map<std::string, int> word_frequencies;

        // Helper to lowercase a string in place
        void toLowerCase(std::string& str) {
            std::transform(str.begin(), str.end(), str.begin(),
                 { return std::tolower(c); }
            );
        }

        // Helper to remove punctuation
        void stripPunctuation(std::string& str) {
            str.erase(std::remove_if(str.begin(), str.end(),
                 { return std::ispunct(c); }),
                str.end());
        }

    public:
        /**
         * @brief Construct a new String Analyzer object
         * @param text The input text to analyze
         */
        explicit StringAnalyzer(const std::string& text) : raw_text(text) {}

        /**
         * @brief Tokenizes the raw text into words.
         */
        void tokenize() {
            tokens.clear();
            std::string processed_text = raw_text;
            
            stripPunctuation(processed_text);
            toLowerCase(processed_text);

            std::istringstream stream(processed_text);
            std::string word;
            
            while (stream >> word) {
                if (!word.empty()) {
                    tokens.push_back(word);
                }
            }
        }

        /**
         * @brief Calculates frequencies of each word.
         */
        void calculateFrequencies() {
            if (tokens.empty()) {
                throw std::logic_error("Tokens list is empty. Call tokenize() first.");
            }
            
            word_frequencies.clear();
            for (const auto& token : tokens) {
                word_frequencies[token]++;
            }
        }

        /**
         * @brief Prints the top N most frequent words.
         * @param n Number of top words to display
         */
        void printTopWords(size_t n) const {
            if (word_frequencies.empty()) {
                std::cerr << "Warning: No frequency data available.\n";
                return;
            }

            // Copy to vector to sort by value
            std::vector<std::pair<std::string, int>> sorted_freq(
                word_frequencies.begin(), word_frequencies.end()
            );

            // Sort descending by count, then alphabetically
            std::sort(sorted_freq.begin(), sorted_freq.end(),
                 {
                    if (a.second != b.second) return a.second > b.second;
                    return a.first < b.first;
                }
            );

            std::cout << "\n=== Top " << n << " Most Frequent Words ===\n";
            size_t limit = std::min(n, sorted_freq.size());
            for (size_t i = 0; i < limit; ++i) {
                std::cout << i + 1 << ". '" << sorted_freq[i].first 
                          << "' : " << sorted_freq[i].second << " occurrences\n";
            }
            std::cout << "====================================\n";
        }

        size_t getWordCount() const { return tokens.size(); }
    };
} // namespace utils

int main() {
    try {
        std::string sample_data = 
            "The svn-to-git migration agent is a crucial tool. "
            "Git is decentralized, whereas SVN is centralized. "
            "Migration from SVN to Git requires tracking history accurately. "
            "The tool must process SVN commits, SVN branches, and SVN tags into Git equivalents. "
            "Developers love Git because Git is fast, and Git branching is superior.";

        auto analyzer = std::make_unique<utils::StringAnalyzer>(sample_data);
        analyzer->tokenize();
        analyzer->calculateFrequencies();
        analyzer->printTopWords(5);

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}