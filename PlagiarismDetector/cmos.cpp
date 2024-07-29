 /* 
    @author: Mark Tetteh 
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <iomanip>

using namespace std;

// Function to read tokenized submissions from the file
vector<vector<string>> readTokenizedSubmissions(const string& filepath) {
    vector<vector<string>> submissions;
    ifstream file(filepath);
    string line;
    string trash;
    while (getline(file, line)) {
        vector<string> tokens;
        istringstream iss(line);
        string token;

        while (iss >> token) {
        // Remove spaces from the token
        token.erase(remove(token.begin(), token.end(), ' '), token.end());
        tokens.push_back(token);
        }
        submissions.push_back(tokens);
        getline(file, trash);
    }
    file.close();
    return submissions;
}

// Generate k-grams from a single tokenized submission
vector<string> generateKgrams(const vector<string>& tokens, int k) {
    vector<string> kgrams;
    for (size_t i = 0; i + k <= tokens.size(); i++) {
        string kgram;
        for (int j = 0; j < k; j++) {
            kgram += tokens[i + j];
        }
        kgrams.push_back(kgram);
    }
    return kgrams;
}

// Using Winnowing algorithm to select fingerprints from hashed k-grams
vector<size_t> winnow(const vector<size_t>& hashes, int w) {
    vector<size_t> fingerprints;
    if (hashes.size() < w) return fingerprints;

    for (size_t i = 0; i <= hashes.size() - w; i++) {
        auto windowBegin = hashes.begin() + i;
        auto minElementIt = min_element(windowBegin, windowBegin + w);
        fingerprints.push_back(*minElementIt);
    }
    return fingerprints;
}

// Count matching fingerprints between two sets
int countMatchingFingerprints(const vector<size_t>& fp1, const vector<size_t>& fp2) {
    unordered_set<size_t> set1(fp1.begin(), fp1.end());
    int matchCount = 0;
    for (auto hash : fp2) {
        if (set1.find(hash) != set1.end()) {
            ++matchCount;
        }
    }
    return matchCount;
}

int main() {
    const string filepath = "tokens.txt";
    const int k = 14; // Length of k-grams
    const int w = 28; // Window size for winnowing

    // Read tokenized submissions from file
    vector<vector<string>> submissions = readTokenizedSubmissions(filepath);
    vector<vector<size_t>> allFingerprints;

    // Process each submission
    for (const auto& tokens : submissions) {
        vector<string> kgrams = generateKgrams(tokens, k);
        vector<size_t> hashes;
        for (const auto& kgram : kgrams) {

            // Get the hash of the k-gram
            size_t hash = std::hash<std::string>{}(kgram);
            hashes.push_back(hash);
        }
        auto fingerprints = winnow(hashes, w);
        allFingerprints.push_back(fingerprints);
    }

    // Store pair percentages and indices
    vector<pair<double, pair<size_t, size_t>>> pairPercentages;

    // Compare fingerprints between all pairs of submissions
    for (size_t i = 0; i < allFingerprints.size(); ++i) {
        for (size_t j = i + 1; j < allFingerprints.size(); ++j) {
            int shared = countMatchingFingerprints(allFingerprints[i], allFingerprints[j]);
            double percentage = static_cast<double>(shared) / allFingerprints[i].size();
            pairPercentages.push_back({percentage, {i+1, j+1}});
        }
    }

    // Sort pairs based on percentage in descending order
    sort(pairPercentages.begin(), pairPercentages.end(), greater<pair<double, pair<size_t, size_t>>>());

    // Print the pairs in order of high percentage
    for (const auto& pairPercentage : pairPercentages) {
        cout << "Pair (" << pairPercentage.second.first << ", " << pairPercentage.second.second << "): Shared Fingerprints = " << pairPercentage.first * 100 << "%\n";
    }

    return 0;
}
