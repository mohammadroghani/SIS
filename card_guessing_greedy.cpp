#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm>
#include<random>
#include<boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

cpp_int permanent(vector<int> A, vector<int> B, int N){
	vector<pair<int, int> > buckets;
	for(int i = 0; i < A.size(); i++) if(B[i] > 0) buckets.push_back(make_pair(A[i], B[i]));


	int Y = 0;
	for(int i = 0; i < buckets.size(); i++) 
		Y += min(buckets[i].first, buckets[i].second);

	vector<cpp_int> fac;
	cpp_int f = 1;
	fac.push_back(f);
	for(int i = 1; i <= N; i++){
		f *= i;
		fac.push_back(f);
	}
	
	int num = buckets.size();
	vector<vector<cpp_int>> dp(num + 1, vector<cpp_int> (Y + 1, 0));


	dp[0][0] = 1;
	for(int i = 1; i <= num; i++){
		for(int j = 0; j <= Y; j++){
			for(int k = 0; k <= min(j, min(buckets[i - 1].first, buckets[i - 1].second)); k++){
				int x = buckets[i - 1].first;
				int y = buckets[i - 1].second;
				cpp_int mul = (fac[x] * fac[y]) / (fac[y - k] * fac[x - k] * fac[k]);
				dp[i][j] += mul * dp[i - 1][j - k]; 
			}
		}
	}
	
	cpp_int perm = 0;
	for(int i = 0; i <= Y; i++){
		if(i % 2) perm -= fac[N - i] * dp[num][i];
		else perm += fac[N - i] * dp[num][i];
	}

	return perm;
}


void card_guessing(int n, int m){
	int correct = 0;

	vector<int> deck;
	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			deck.push_back(i);

	random_device rd;
	auto rng = default_random_engine { rd() };
	shuffle(deck.begin(), deck.end(), rng);

	vector<int> A(n, m), B(n, 0);
	int index = 0;
	
	int options = 1;	

	for(int row = 0; row < n * m; row++){
		if(deck[row] != index){
			B[index]++;
			continue;
		}
		
		if(index + 1 == options && index + 1 < n) options++;

		correct += 1;
		if(row == n * m - 1) break;

		A[index]--;

		row++;
		vector<cpp_int> dist;
		for(int column = 0; column < options; column++){	
			if(A[column] > 0){
				A[column]--;
				cpp_int matches = permanent(A, B, n * m - correct - 1);
				A[column]++;
				dist.push_back(matches * (cpp_int) A[column]);
			}
			else{
				dist.push_back(0);
			}
		}
		index = max_element(dist.begin(), dist.end()) - dist.begin();
		row--;

	}
	cout << correct << endl;
}

int main(){
	int n, m, samples;
	cin >> n >> m >> samples;
	for(int i = 0; i < samples; i++)
		card_guessing(n, m);
	return 0;
}
