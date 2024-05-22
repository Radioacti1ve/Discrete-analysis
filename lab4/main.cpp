#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
  
using namespace std;

struct Data {
	unsigned int value;
	int stringNum;
	int wordNum;
};

vector<int> LinearZFunction(const vector<Data>& message) {
	unsigned int n = message.size();
	vector<int> result(n);
	int l = 0, r = 0;
	for(int i = 0; i < n; ++i) {
		result[i] = max(0, min(r - i, result[i - l]));
		while(i + result[i] < n && message[result[i]].value == message[i + result[i]].value) {
			result[i]++;
		}
		if(i + result[i] > r) {
			l = i;
			r = i + result[i];
		}
	}
	return result;
}

int main(int argc, char const *argv[]) {
	ios_base::sync_with_stdio(false);
    cin.tie(nullptr); 
	cout.tie(nullptr);

	vector<unsigned int> pattern;
	string strPattern;
	getline(cin, strPattern);

	vector<Data> message;
	stringstream patternStream(strPattern);
	unsigned int word;

	while(patternStream >> word) {
		message.push_back({word, 0});
	}

	const int Sentinel = 9999;

	unsigned int size = message.size();
	message.push_back({Sentinel, 0});
	
	string text;
	int num = 1;
	while(!cin.eof()) {
		getline(cin, text);
		stringstream textStream(text);
		int wordNum = 1;
		while(textStream >> word) {
			message.push_back({word, num, wordNum});
			wordNum++;
		}

		num++;
	}

	vector<int> Z = LinearZFunction(message);

	for(int i = size + 1; i < Z.size(); ++i) {
		if(Z[i] == size) {
			cout << message[i].stringNum << ", " << message[i].wordNum << endl;
		}
	}
	
	return 0;
}
 