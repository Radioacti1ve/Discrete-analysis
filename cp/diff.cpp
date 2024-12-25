#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

class SequenceAnalyzer {
public:
    vector<string> findLCS(const vector<string>& seq1, const vector<string>& seq2) {
        if (seq1.empty() || seq2.empty())
            return {};

        if (seq1.size() == 1) {
            auto it = find(seq2.begin(), seq2.end(), seq1[0]);
            if(it != seq2.end()) {
                return vector<string>{seq1[0]};
            } else {
                return vector<string>{};
            }
        }

        size_t middle = seq1.size() / 2;

        vector<string> left(seq1.begin(), seq1.begin() + middle);
        vector<string> right(seq1.begin() + middle, seq1.end());

        vector<int> prefixScores = calculatePrefixScores(left, seq2);
        vector<int> suffixScores = calculateSuffixScores(right, seq2);

        size_t optimalSplit = 0;
        int maxScore = prefixScores[0] + suffixScores[0];

        for (size_t j = 1; j <= seq2.size(); ++j) {
            if (prefixScores[j] + suffixScores[j] > maxScore) {
                maxScore = prefixScores[j] + suffixScores[j];
                optimalSplit = j;
            }
        }

        vector<string> leftSubsequence = findLCS(left, vector<string>(seq2.begin(), seq2.begin() + optimalSplit));
        vector<string> rightSubsequence = findLCS(right, vector<string>(seq2.begin() + optimalSplit, seq2.end()));

        leftSubsequence.insert(leftSubsequence.end(), rightSubsequence.begin(), rightSubsequence.end());
        return leftSubsequence;
    }

private:
    vector<int> calculateSuffixScores(const vector<string>& suffix1, const vector<string>& suffix2) {
        size_t n = suffix1.size();
        size_t m = suffix2.size();
        vector<int> previousScores(m + 1, 0);

        for (int i = n - 1; i >= 0; --i) {
            vector<int> currentScores(m + 1, 0);

            for (int j = m - 1; j >= 0; --j) {
                if (suffix1[i] == suffix2[j]) {
                    currentScores[j] = previousScores[j + 1] + 1;
                } else {
                    currentScores[j] = max(currentScores[j + 1], previousScores[j]);
                }
            }
            previousScores = currentScores;
        }

        return previousScores;
    }

    vector<int> calculatePrefixScores(const vector<string>& prefix1, const vector<string>& prefix2) {
        size_t n = prefix1.size();
        size_t m = prefix2.size();
        vector<int> previousScores(m + 1, 0);

        for (size_t i = 0; i < n; ++i) {
            vector<int> currentScores(m + 1, 0);

            for (size_t j = 0; j < m; ++j) {
                if (prefix1[i] == prefix2[j]) {
                    currentScores[j + 1] = previousScores[j] + 1;
                } else {
                    currentScores[j + 1] = max(currentScores[j], previousScores[j + 1]);
                }
            }
            previousScores = currentScores;
        }

        return previousScores;
    }
};

int main() {
    string input1, input2;
    getline(cin, input1);
    getline(cin, input2);

    stringstream stream1(input1), stream2(input2);
    vector<string> sequence1, sequence2;
    string word;

    while (stream1 >> word) {
        sequence1.push_back(word);
    }

    while (stream2 >> word) {
        sequence2.push_back(word);
    }

    SequenceAnalyzer analyzer;
    vector<string> result = analyzer.findLCS(sequence1, sequence2);

    cout << result.size() << "\n";

    for (const string& str : result) {
        cout << str << ' ';
    }

    cout << endl;

    return 0;
}
