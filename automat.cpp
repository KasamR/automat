#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#define NO_MONEY -1
#define NOT_FOUND -2
using namespace std;

class Drink { //飲み物クラス
private:
	string name = "";
	int money = 0;

public:
	Drink(int m, string n);
	string getName() { return name; }
	int getMoney() { return money; }
	int calcChange(int sum); //残金の計算
};

Drink::Drink(int m, string n) {
	money = m;
	name = n;
}

int Drink::calcChange(int sum) {
	return sum - money; //引数から自分の分の代金を引く
}

//csvから商品のお金と名前を読み取ってvectorを返す関数
vector<Drink> read() {
	ifstream ifs("items.csv");
	int i = 0;
	string str = "";
	vector<Drink> v;
	while (ifs >> i) {
		ifs >> str;
		v.push_back(Drink(i, str));
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
int BuyCheck(vector<Drink> items, int sum, string input) {
	for (int i = 0; i < items.size(); i++) {
		if (items[i].getName() == input && 0 <= sum - items[i].getMoney()) { //ちゃんと買えた時
			return i; //リストの何番目かを返す
		}
		if (items[i].getName() == input && sum - items[i].getMoney() < 0) { //お金が足りないとき
			return NO_MONEY; //-1を返す
		}
	}
	return NOT_FOUND; //リストに名前がない(-2を返す)
}

//おつりの計算と表示をする関数
void changeMoney(int sum) {
	vector<int> m = { 1000, 500, 100, 50, 10 };
	if (sum == 0) { //引数が0の時はちょうどの時
		cout << "釣銭はありません" << endl;
	} else { //そうでないときは計算をする
		cout << "おつりは" << sum << "円で，釣銭は ";
		for (int i = 0; i < m.size(); i++) {
			if (0 < sum / m[i]) { //"~円が0枚"という表示を避けるための処理
				cout << m[i] << "円が" << sum / m[i] << "枚 ";
				sum = sum - (sum / m[i])*m[i];
			}
		}
		cout << "です" << endl;
	}
}

int main() {
	string input;
	vector<Drink> items = read();
	int temp = 0; //string to int するための変数
	int sum = 0; //入力された合計金額を保存するための変数
	bool loop = true; //ループ制御用の変数

	do {
		cout << "**商品リストです**" << endl;
		for (int i = 0; i < items.size(); i++) {
			cout << items[i].getMoney() << "円: " << items[i].getName() << endl;
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
				case NO_MONEY:
					cout << "お金が足りません！" << endl;
					break;
				case NOT_FOUND:
					cout << "リストにないものを買おうとしてます" << endl;
					break;
				default:
					cout << "ゴトン！" << items[bc].getName() << "を購入しました！" << endl;
					sum = items[bc].calcChange(sum); //残金の計算
					break;
				}
			}
		}
		printf("\n");
	} while (loop);
	changeMoney(sum); //おつりの表示と計算
	cout << "ご利用ありがとうございました" << endl;
}
