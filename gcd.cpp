#include <iostream>
#include <random>
#include <limits>
#include <utility>
#include <vector>
#include <algorithm>
#include <chrono>
#include <functional>
#include <array>
#include <iomanip>
using namespace std;

using ll = long long;
using ld = long double;

static int ca = 0, cb = 0;
static int print = 0;

ll safe_gcd(ll u,ll v) {
	++ca;
	if(print)
		cout<<ca<<": "<<u<<","<<v<<endl;
	if( v == 0 ) {
		return u;
	}
	return safe_gcd( v, u % v );
}

ll safe_gcd_it(ll u,ll v) {
	while(v) {
		u %= v;
		v ^= u;
		u ^= v;
		v ^= u;
	}
	return u;
}

ll optimized_gcd(ll u,ll v) {
	++cb;
	if(print)
		std::cout<<cb<<": "<<u<<","<<v<<endl;
	if( v == 0 ) {
		return std::abs(u);
	}
	const ll t = u % v;
	if( t > std::abs(v) / 2 ) {
		if( v > 0 )
			return optimized_gcd( v , std::abs(t - v) );
		return optimized_gcd( v, t + v );
	}
	return optimized_gcd( v , t );
}

ll binary_gcd(ll u,ll v) {
	ll k{ 0 };
	while( u % 2 == 0 && v % 2 == 0 )//Rule n. 1
		u /= 2 , v /= 2, ++k;
	ll t{ u - v };//Rule n. 3
	while( t ) {
		while( t % 2 == 0 )//Rule n. 2
			t /= 2;
		if( t < 0 )//Rule n. 4 (Replacing the larger of the two)
			v = -t;
		else
			u = t;
		t = u - v;//Rule n. 3
	}
	return std::pow(2,k)*u;//From rule n. 1, remember we've reduced u and v by 2^k
}

std::vector<std::pair<ll,ll>> generate_data(const ll amount) {
	std::vector<std::pair<ll,ll>> nums;
	nums.resize(amount);

	random_device rd;
	mt19937 gen(rd());
	const ll big = numeric_limits<long double>::max();
	uniform_int_distribution<ll> dist(1,big);

	for(auto& el:nums) {
		el.first = std::abs(dist(gen));
		el.second = std::abs(dist(gen));
	}

	return nums;
}

ll bench(function<ll(ll,ll)> fnc,
		const vector<pair<ll,ll>>& data) {
	auto beg = chrono::high_resolution_clock::now();
	for(auto& d:data){
		fnc(d.first,d.second);
	}
	auto end = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::milliseconds>(end-beg).count();
}

int main() {
	cout<<"Generating data\n";
	auto nums = generate_data(5000000);

	cout<<"Running benchmarks\n";
	pair<string,ld> exec_time[] = {
		{ "safe_gcd",      bench(safe_gcd,nums) },
		{ "safe_gcd_it",   bench(safe_gcd_it,nums) },
		{ "optimized_gcd", bench(optimized_gcd,nums) },
		{ "binary_gcd",    bench(binary_gcd,nums) },
	};

	cout<<"Results:\n";
	sort(begin(exec_time),end(exec_time),
			[](const pair<string,ld>& a,
				const pair<string,ld>& b) -> bool {
				return a.second < b.second;
				} );

	ld fastes = exec_time[0].second;
	for(auto& res : exec_time) {
		cout<<setw(20)<<left<<res.first<<right<<res.second<<"ms";
		cout<<setw(10)<<right<<res.second / fastes<<endl;
	}
	return 0;
}
