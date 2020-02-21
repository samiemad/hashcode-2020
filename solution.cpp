#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
#define mp(x, y) make_pair(x, y)
#define pb(x) push_back(x)

int B,L,D;
vector<int> b;
struct Lib{
	int T, M;
	vector<int> books;
};

vector<Lib> libs;
vector<int> ord;

bool cmp(int x, int y){
	return b[x] > b[y]; 
}

bool cmpLib(int ix, int iy) {
	auto& x = libs[ix];
	auto& y = libs[iy];
	return x.T == y.T ? x.M > y.M : x.T < y.T;
}

unordered_map<int,pll> booksMap;
ll score(){
	booksMap.clear();
	int time =0;
	for(auto id: ord){
		auto& lib = libs[id];
		time += lib.T;
		int i=0;
		for(auto book : lib.books) {
			if(!booksMap.count(book) || booksMap[book].first > time+i/lib.M){
				booksMap[book] = {time+i/lib.M, id};
				i++;
			}
		}
	}
	ll score=0;
	for(auto book : booksMap){
		if (book.second.first >= D) continue;
		score += b[book.first];
	}
	return score;
}

void readSolution(string f){
	ifstream fin(f);
	int n, m, x;
	fin>>n;
	for(int i=0; i<n; ++i){
		fin>>ord[i] >> m;
		for(int j=0; j<m; ++j) fin>>x;
	}
}

void print(string f) {
	ofstream cout(f);
	cout<<L<<"\n";
	for(auto id: ord) {
		auto& l = libs[id];
		cout<<id<<" "<<l.books.size()<<"\n";
		for(auto book:l.books){
			if(booksMap[book].second==id)
				cout<<book<<" ";
		}
		for(auto book:l.books){
			if(booksMap[book].second!=id)
				cout<<book<<" ";
		}
		cout<<"\n";
	}
}

int main(int argc, char **argv){
	string output = argv[1];
	ios_base::sync_with_stdio(false);
	cin>>B>>L>>D;
	b.resize(B);
	for(auto &a: b) {
		cin>>a;
	}
	libs.assign(L, Lib());
	for(auto &l : libs){
		int n;
		cin>>n>>l.T>>l.M;

		l.books.resize(n);
		for(auto &book: l.books){
			cin>>book;
		}
		sort(l.books.begin(), l.books.end(), cmp);
	}
	for(int i=0; i<L; ++i) ord.pb(i);

	readSolution(output);
	// sort(ord.begin(), ord.end(), cmpLib); print(output);

	ll sc = score();
	cerr<<output<< ": " <<sc<<" (initial)\n";

	while(true){
		int x = rand()%(libs.size()/2);
		int y = rand()%(libs.size());
		if(x==y) y=(y+1)%libs.size();
		swap(ord[x], ord[y]);
		ll nsc = score();
		if( nsc > sc ){
			cerr<<output<< ": " <<nsc<<" (+"<<nsc-sc<<")\n";
			sc = nsc;
			print(output);
		} else {
			swap(ord[x], ord[y]);
		}
	}

	return 0;
}
