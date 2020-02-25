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
	ll netScore;
	ll score;
	vector<int> books;
};

vector<Lib> libs;
vector<int> ord;

bool cmp(int x, int y){
	return b[x] > b[y]; 
}

bool cmpSetup(const Lib& x, const Lib& y) {
	return x.T == y.T ? x.M > y.M : x.T < y.T;
}
bool cmpNumBooks(const Lib& x, const Lib& y) {
	return x.books.size() > y.books.size();
}
bool cmpNetScoreOverSetup(const Lib& x, const Lib& y) {
	return x.netScore*y.T > y.netScore*x.T;
}
bool cmpScoreOverSetup(const Lib& x, const Lib& y) {
	return x.score*y.T > y.score*x.T;
}
bool cmpNetScore(const Lib& x, const Lib& y) {
	return x.netScore == y.netScore ? x.T < y.T : x.netScore > y.netScore;
}
inline bool cmpLib(int ix, int iy) {
	return cmpScoreOverSetup(libs[ix], libs[iy]);
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

void sortOrd() {
	vector<int> bestOrd;

	int msc = 0, sc = 0;
	for(int i=0; i<100; ++i){
		random_shuffle(ord.begin(), ord.end());
		sort(ord.begin(), ord.end(), [](int a, int b){ return cmpNetScore(libs[a], libs[b]); }); 
		sc = score();
		if(sc > msc) {msc = sc; bestOrd = ord;}

		random_shuffle(ord.begin(), ord.end());
		sort(ord.begin(), ord.end(), [](int a, int b){ return cmpSetup(libs[a], libs[b]); }); 
		sc = score();
		if(sc > msc) {msc = sc; bestOrd = ord;}

		random_shuffle(ord.begin(), ord.end());
		sort(ord.begin(), ord.end(), [](int a, int b){ return cmpScoreOverSetup(libs[a], libs[b]); }); 
		sc = score();
		if(sc > msc) {msc = sc; bestOrd = ord;}

		random_shuffle(ord.begin(), ord.end());
		sort(ord.begin(), ord.end(), [](int a, int b){ return cmpNetScoreOverSetup(libs[a], libs[b]); }); 
		sc = score();
		if(sc > msc) {msc = sc; bestOrd = ord;}

		random_shuffle(ord.begin(), ord.end());
		sort(ord.begin(), ord.end(), [](int a, int b){ return cmpNumBooks(libs[a], libs[b]); }); 
		sc = score();
		if(sc > msc) {msc = sc; bestOrd = ord;}

		cerr<<"msc = "<<msc<<"\n";
	}
	ord = bestOrd;
}

void readSolution(string f){
	ifstream fin(f);
	if( !fin ){
		cerr<<"Cannot read file!\n";
		return;
	}
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

vector<int> initIdx() {
	cerr<<"initIdx()\n";
	vector<int> v(L);
	for(int i=0; i<L; ++i) v[i] = i;
	return v;
}

int main(int argc, char **argv){
	srand(time(NULL));
	string output = argv[1];
	ios_base::sync_with_stdio(false);
	cin>>B>>L>>D;
	b.resize(B);
	ll maxScore = 0;
	for(auto &a: b) {
		cin>>a;
		maxScore += a;
	}
	cerr<< "Max Score = "<<maxScore<<"\n";
	libs.assign(L, Lib());
	for(auto &l : libs){
		int n;
		cin>>n>>l.T>>l.M;

		l.books.resize(n);
		for(auto &book: l.books){
			cin>>book;
			l.score += b[book];
		}
		sort(l.books.begin(), l.books.end(), cmp);
		for(int i=0; i<(D-l.T)*l.M && i<l.books.size(); ++i ) {
			l.netScore += b[l.books[i]];
		}
	}
	for(int i=0; i<L; ++i) ord.pb(i);

	// sortOrd(); return 0;

	readSolution(output);
	ll sc = score();
	cerr<<output<< ": " <<sc<<" (initial)\n";

	vector<int> idx = initIdx();

	while(true){
		if(idx.size()<2) idx = initIdx();
		random_shuffle(idx.begin(), idx.end());
		int x = idx.back();
		int y = idx[idx.size()-2];
		if( x==y ){
			cerr<<"Panic!!"<<idx.size()<<" :: "<<L<<"\n";
			for(auto i:idx) cerr<<", "<<i;
			cerr<<"\n";
			return 0;
		}
		swap(ord[x], ord[y]);
		// reverse(ord.begin()+x, ord.begin()+y+1);
		ll nsc = score();
		if( nsc > sc ){
			cerr<<output<< ": " <<nsc<<" (+"<<nsc-sc<<")\n";
			sc = nsc;
			print(output);
		} else {
			swap(ord[x], ord[y]);
			// reverse(ord.begin()+x, ord.begin()+y+1);
			if( nsc == sc ) {
				idx.pop_back();
				idx.pop_back();
				// cerr<<"rem eq "<<x<<","<<y<<"\n";
			}
		}
	}

	return 0;
}
