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
	int id, T, M;
	vector<int> books;
	vector<int> scan;
};

vector<Lib> libs;

bool cmp(int x, int y){
	return b[x] > b[y]; 
}

bool cmpLib(const Lib& x, const Lib& y) {
	return x.T == y.T ? x.M > y.M : x.T < y.T;
}

ll score1(){
	unordered_set<int> booksSet;
	int time =0;
	for(auto &lib : libs){
		time += lib.T;
		int rem = (D-time)*lib.M;
		for(auto book : lib.books){
			if (rem <=0) break;
			booksSet.insert(book);
			rem--;
		}
	}
	ll score=0;
	for(auto book : booksSet){
		score += b[book];
	}
	return score;
}

ll score2(){
	unordered_set<int> booksSet;
	int time =0;
	for(auto &lib : libs){
		lib.scan.clear();
		time += lib.T;
		int rem = (D-time)*lib.M;
		for(auto book : lib.books){
			if (rem <=0) break;
			if(!booksSet.count(book)){
				lib.scan.push_back(book);
				booksSet.insert(book);
				rem--;
			}
		}
	}
	ll score=0;
	for(auto book : booksSet){
		score += b[book];
	}
	return score;
}

unordered_map<int,pll> booksMap;
ll score(){
	booksMap.clear();
	int time =0;
	for(auto &lib : libs){
		time += lib.T;
		// int rem = (D-time)*lib.M;
		int i=0;
		for(auto book : lib.books) {
			// if (i>=rem) break;
			if(!booksMap.count(book) || booksMap[book].first > time+i/lib.M){
				booksMap[book] = {time+i/lib.M, lib.id};
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

void print(string f) {
	ofstream cout(f);
	cout<<L<<"\n";
	for(auto l: libs) {
		cout<<l.id<<" "<<l.books.size()<<"\n";
		for(auto book:l.books){
			if(booksMap[book].second==l.id)
				cout<<book<<" ";
		}
		for(auto book:l.books){
			if(booksMap[book].second!=l.id)
				cout<<book<<" ";
		}
		cout<<"\n";
	}
}

int main(int argc, char **argv){
	ios_base::sync_with_stdio(false);
	cin>>B>>L>>D;
	b.resize(B);
	for(auto &a: b) {
		cin>>a;
	}
	libs.assign(L, Lib());
	int i=0;
	for(auto &l : libs){
		int n;
		l.id = i++;
		cin>>n>>l.T>>l.M;

		l.books.resize(n);
		for(auto &book: l.books){
			cin>>book;
		}
		sort(l.books.begin(), l.books.end(), cmp);
	}
	sort(libs.begin(), libs.end(), cmpLib);
	ll sc = score();

	while(true){
		int x = rand()%(libs.size()/2);
		int y = rand()%(libs.size());
		if(x==y) continue;
		swap(libs[x], libs[y]);
		ll nsc = score();
		if( nsc > sc ){
			cerr<<"score: "<<argv[1]<< " : " <<nsc<<" (+"<<nsc-sc<<")\n";
			sc = nsc;
			print(argv[1]);
		} else {
			swap(libs[x], libs[y]);
		}
	}

	return 0;
}
