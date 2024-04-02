#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Key {
	string strPhoneNum;

	unsigned long long value;
};

struct Data {
	unsigned long ullPhoneNum;

	int id;
};

void countingSort(vector<Data>& Arr, unsigned long long exp) {
	int n = Arr.size();
	vector<int> count(10, 0);
	vector<Data> output(n);
	
	for(int i = 0; i < n; i++) {
		count[(Arr[i].ullPhoneNum / exp ) % 10]++;
	}

	for(int i = 1; i < 10; i++) {
		count[i] += count[i - 1];
	}

	for(int i = n - 1; i >= 0; i--) {
		output[count[(Arr[i].ullPhoneNum / exp) % 10] - 1] = Arr[i];
		count[(Arr[i].ullPhoneNum / exp) % 10]--;
	}

	for(int i = 0; i < n; i++) {
		Arr[i] = output[i];
	}

}

void radixSort(vector<Data>& Arr, unsigned long long max_elem) {
	for(unsigned long long exp = 1; max_elem / exp > 0; exp *= 10) {
		countingSort(Arr, exp);
	}
} 

int main(int argc, char const *argv[]) {
	
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
  	std::cout.tie(nullptr);

	vector<Data> beginArray;
	vector<Key> values;
	
	int count = 0;
	struct Data temp; 
	struct Key tempValue;
	unsigned long long  max_elem = 0;
	while(cin >> tempValue.strPhoneNum >> tempValue.value) {
		temp.ullPhoneNum = 0;
		temp.id = count;
		for(auto c: tempValue.strPhoneNum) {
			if(isdigit(c)) {
				temp.ullPhoneNum = temp.ullPhoneNum * 10 + (c - '0');
			} else {
				continue;
			}
		}

		if(temp.ullPhoneNum > max_elem) {
			max_elem = temp.ullPhoneNum;
		}
		
		
		beginArray.push_back(temp);
		++count;

		values.push_back(tempValue);
	}

	radixSort(beginArray, max_elem);

	for(int i = 0; i < count; ++i) {
		cout << values[beginArray[i].id].strPhoneNum << '\t' << values[beginArray[i].id].value << '\n';
	}
	return 0;
}
