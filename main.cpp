#include <iostream>
#include <sstream>
#include <regex>
#include <cstdio>
#include <cmath>
#include <cctype>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
#include <iomanip>
#include  <cassert>
#include <fstream>
#include <codecvt>

using namespace std;
using ll = long long int;
#define rep(i,n) for(int i = 0; i < n; i++)
#define FOR(i, a, b)  for(int i = (a); i < (b) ; i++)
#define pb push_back
#define SORT(v,n) sort(v, v+n)
#define ALL(x) (x).begin(),(x).end()
#define debug(x) cerr << #x << ": " << x << '\n'
#define elif else if
#define int ll
const int INF = (1LL << 32);
const int MOD = (int)1e9 + 7;
const double EPS = 1e-9;
int dx[8] = { 1, 0, -1, 0, 1, -1, -1, 1 };
int dy[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
typedef vector<vector<int>> vvi;
typedef vector<vector<vector<int>>> vvvi;
void Load();
void print();
static const int NUM_MENU = 100;
static const int COST_MAX = 1001;
static const int DIAGONAL = 1;
static const int TOP = 0;

struct Menu {
	wstring name; //商品名。
	int cal;    //カロリー。
	int cost;    //お値段。
};

Menu menus[NUM_MENU + 1];    // メニューのデータ。
int C[NUM_MENU + 1][COST_MAX + 1],    // i番目までのメニューまでを考慮してj円まで使える時の摂取可能カロリーの最大値を記録しておく二次元配列。
G[NUM_MENU + 1][COST_MAX + 1];   // メニューの選択状況を記録しておくための二次元配列。
static const int nMenu = 77;    //メニューの数
static const int mMoney = 1000; //お財布事情
								//スライドのDPとはiに関するループの向きが逆になっている
void compute(int& maxValue, vector<int>& selection) {
	rep(i, mMoney + 1) {
		C[0][i] = 0;
		G[0][i] = DIAGONAL;
	}
	FOR(i, 1, nMenu + 1) C[i][0] = 0;
	FOR(i, 1, nMenu + 1) {
		FOR(w, 1, mMoney + 1) {
			//品物iを選ばない
			C[i][w] = C[i - 1][w];
			G[i][w] = TOP;
			//選べない・・
			if (menus[i].cost > w) continue;
			//メニューiを選ぶ
			if (menus[i].cal + C[i - 1][w - menus[i].cost] > C[i - 1][w]) {
				C[i][w] = menus[i].cal + C[i - 1][w - menus[i].cost];
				G[i][w] = DIAGONAL;
			}
		}
	}
	maxValue = C[nMenu][mMoney];
	selection.clear();
	for (int i = nMenu, w = mMoney; i >= 1; i--) {
		if (G[i][w] == DIAGONAL) {
			selection.pb(i);
			w -= menus[i].cost;
		}
	}
	reverse(ALL(selection));
}
signed main() {
	ios::sync_with_stdio(true);
	int maxCal = 0;
	vector<int> selection;
	Load();
	compute(maxCal, selection);
	print(maxCal, selection);
	return 0;
}
void print(int maxCal, vector<int> selection) {
	int totalCost = 0;
	cout << "1000円使って最大カロリー摂れるメニューの組み合わせ\n\n";
	//選んだメニューの情報を表示
	for (auto i : selection) {
		wcout << L"・" << menus[i].name << ' ';
		cout << menus[i].cal << "kcal " << menus[i].cost << "円\n";
		totalCost += menus[i].cost;
	}
	cout << endl;
	//結果
	cout << "合計 " << maxCal << "kcal " << totalCost << "円\n";
}

//メニューの名前とカロリーをロードする(データが条件悪いとまともに動かないのでやばE）
void Load() {
	setlocale(LC_ALL, "");
	wifstream ifs("menu.txt");
	ifs.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
	wstring str((istreambuf_iterator<wchar_t>(ifs)), istreambuf_iterator<wchar_t>());
	wcout << str << endl;
	cout << "Data loading completed....\n\n";
	int n = 0;
	vector<wstring> vs;
	rep(i, nMenu) {
		wstring m;
		while (str.at(n) != '\n') {
			m += str.at(n);
			n++;
		}
		n++;
		vs.pb(m);
	}
	rep(i, nMenu) {
		bool inname = false;
		bool incal = false;
		wstring nm;
		wstring cal;
		wstring cost;
		rep(j, vs[i].size()) {
			if (!iswdigit(vs[i][j])) {
				if (inname) continue;
				if (vs[i][j] == 'k') {
					incal = true;
					inname = true;
					continue;
				}
				nm += vs[i][j];
			}
			if (iswdigit(vs[i][j])) {
				if (incal) {
					cost += vs[i][j];
				}
				else {
					cal += vs[i][j];
				}
			}
		}
		menus[i + 1].name = nm;
		menus[i + 1].cal = stoi(cal);
		menus[i + 1].cost = stoi(cost);
	}
}