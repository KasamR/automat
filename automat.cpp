#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

//商品リストを読み込むための関数
vector<pair<int, string>> read(vector<pair<int, string>> v) {
	ifstream ifs("items.csv");
	int i = 0;
	string str = "";
	while (ifs >> i) {
		ifs >> str;
		v.push_back(pair<int, string>(i,str));
	}
	return v;
}

//投入された金額がリストにあるかを確認する関数
bool MoneyCheck(int money) {
	vector<int> v = { 10, 50, 100, 500, 1000 };
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == money) return true;
	}
	return false;
}

//買おうとしたときの確認をする関数
int BuyCheck(vector<pair<int, string>> items, int sum, string input) {
	for (int i = 0; i < items.size(); i++) {
		if (items[i].second == input && 0 <= sum - items[i].first) { //ちゃんと買えた時(リストの何番目かを返す)
			return i;
		}
		if (items[i].second == input && sum - items[i].first < 0) { //お金が足りないとき
			return -1;
		}
	}
	return -2; //リストに名前がない
}

int main() {
	string input;
	vector<pair<int, string>> items = read(items);
	int temp = 0; //string to int するための変数
	int sum = 0; //入力された合計金額を保存するための変数
	bool loop = true; //ループ制御用の変数

	do {
		cout << "**商品リストです**" << endl;
		for (int i = 0; i < items.size(); i++) {
			cout << items[i].first << "円: " << items[i].second << endl;
		}
		cout << "投入金額: " << sum << "円" << endl;
		cout << "お金を入れるか，欲しい商品を入力してください(終了は\"f\"): ";
		cin >> input;
		try { //お金が入力された場合はintに変換する
			temp = stoi(input);
			if (MoneyCheck(temp)) sum += temp; //指定された硬貨または紙幣の場合のみお金を足す
			else cout << "不正な入力です" << endl;
		}
		catch (const invalid_argument e) { //お金以外が入力された場合はそのままstringとして使用する
			if (input == "f") loop = false;
			else {
				int bc = BuyCheck(items, sum, input); //確認用の関数の使用
				switch (bc) { //購入できる場合だったかを判断する
				case -1:
					cout << "お金が足りません！" << endl;
					break;
				case -2:
					cout << "リストにないものを買おうとしてます" << endl;
					break;
				default:
					cout << "ゴトン！" << items[bc].second << "を購入しました！" << endl;
					sum = sum - items[bc].first; //残金の計算
					break;
				}
			}
		}
		printf("\n");
	} while (loop);
	cout << "おつりは" << sum << "円です" << endl;
	cout << "ご利用ありがとうございました" << endl;
}