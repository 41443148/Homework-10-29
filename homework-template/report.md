# 41443148

作業一

## 解題說明

本題採用物件導向（Object-Oriented Programming）的概念，將多項式拆解為兩個類別：Term（代表單項式）與 Polynomial（代表多項式）。

### 解題策略

1.遞迴函式設計：
根據 Ackermann 函數的數學定義進行遞迴拆解：
n+1 if m=0
A(m-1,1) if n=0
A(m-1,A(m,n-1)) otherwise

2.由於 Ackermann 函數遞迴深度極深，容易造成系統堆疊溢位（Stack Overflow），且受限於僅能使用 <iostream> 標頭檔，本程式採用自定義靜態陣列模擬堆疊（Stack）的方式，以 while 迴圈迭代取代直接的函式遞迴。

3. 主程式提供介面讓使用者輸入 $m$ 與 $n$，並分別呼叫遞迴與非遞迴版本進行計算與結果比對。

## 程式實作

以下為主要程式碼：

```cpp
#include<iostream>
using namespace std;
class Polynomial;
ostream& operator<<(ostream& os, const Polynomial& poly);
istream& operator>>(istream& is, Polynomial& poly);
class Term
{
	friend Polynomial;
	friend ostream& operator<<(ostream& os, const Polynomial& poly);
	friend istream& operator>>(istream& is, Polynomial& poly);
private:
	float coef;
	int exp;
};
class Polynomial
{
	Term* termArray;
	int capacity;
	int terms;
public:
	Polynomial() :capacity(2), terms(0)
	{
		termArray = new Term[capacity];
	}
	~Polynomial()
	{
		delete[] termArray;
	}
	Polynomial(const Polynomial& other)
	{
		capacity = other.capacity;
		terms = other.terms;
		termArray = new Term[capacity];
		for (int i = 0; i < terms; i++) {
			termArray[i] = other.termArray[i];
		}
	}
	Polynomial& operator=(const Polynomial& other) {
		if (this == &other) return *this; 

		delete[] termArray; 

		capacity = other.capacity;
		terms = other.terms;
		termArray = new Term[capacity];
		for (int i = 0; i < terms; i++) {
			termArray[i] = other.termArray[i];
		}
		return *this;
	}
	Polynomial add(const Polynomial& poly) const
	{
		Polynomial c;
		int aPos = 0, bPos = 0;
		while ((aPos < terms) && (bPos < poly.terms))
		{
			if (termArray[aPos].exp == poly.termArray[bPos].exp)
			{
				float sum = termArray[aPos].coef + poly.termArray[bPos].coef;
				if (sum != 0)
				{
					c.NewTerm(sum, termArray[aPos].exp);
				}
				aPos++;
				bPos++;
			}
			else if (termArray[aPos].exp < poly.termArray[bPos].exp)
			{
				c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
				bPos++;
			}
			else
			{
				c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
				aPos++;
			}
		}
		for (; aPos < terms; aPos++)
		{
			c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
		}
		for (; bPos < poly.terms; bPos++)
		{
			c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
		}
		return c;
	}
	Polynomial Mult(const Polynomial& poly) const
	{
		Polynomial c;
		for (int i = 0; i < terms; i++)
		{
			Polynomial temp;
			for (int j = 0; j < poly.terms; j++)
			{
				float newCoef = termArray[i].coef * poly.termArray[j].coef;
				int newExp = termArray[i].exp + poly.termArray[j].exp;
				temp.NewTerm(newCoef, newExp);
			}
			c = c.add(temp);
		}
		return c;
	}
	float Eval(float f) 
    {
		float result = 0;
		for (int i = 0; i < terms; i++) {
			result += termArray[i].coef * pow(f, termArray[i].exp);
		}
		return result;
	}
	void NewTerm(float theCoef, int theExp)
	{
		if (terms == capacity)
		{
			capacity *= 2;
			Term* temp = new Term[capacity];
			for (int i = 0; i < terms; i++)
			{
				temp[i] = termArray[i];
			}
			delete[] termArray;
			termArray = temp;
		}
		termArray[terms].coef = theCoef;
		termArray[terms].exp = theExp;
		terms++;
	}
	friend istream& operator>>(istream& is, Polynomial& poly) {
		int num;
		cout << "輸入非零項數量: ";
		is >> num;
		for (int i = 0; i < num; i++) {
			float c;
			int e;
			cout << "輸入第 " << i + 1 << " 項的係數: ";
			is >> c;
			cout << "輸入第 " << i + 1 << " 項的指數: ";
			is >> e;
			poly.NewTerm(c, e);
		}
		return is;
	}
	friend ostream& operator<<(ostream& os, const Polynomial& poly) {
		for (int i = 0; i < poly.terms; i++) {
			os << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
			if (i < poly.terms - 1 && poly.termArray[i + 1].coef > 0) 
			{
				os << " + ";
			}
			else if (i < poly.terms - 1) {
				os << " ";
			}
		}
		return os;
	}
};
int main()
{
	Polynomial p1, p2;
	cout << "輸入第一個多項式: " << endl;
	cin >> p1;
	cout << "輸入第二個多項式: " << endl;
	cin >> p2;
	cout << "P1 = " << p1 << endl;
	cout << "P2 = " << p2 << endl;
	cout << "P1 + P2 = " << p1.add(p2) << endl;
	cout << "P1 * P2 = " << p1.Mult(p2) << endl;
	return 0;
}
```

## 效能分析

1. 時間複雜度：Ackermann 函數的增長速度極快，其時間複雜度會隨著 $m$ 和 $n$ 的增加呈超指数級成長（Hyper-exponential）。
2. 空間複雜度：遞迴版本：空間複雜度主要取決於系統呼叫堆疊（Call Stack），在深度過大時空間開銷極高。非遞迴版本：空間複雜度為 $O(K)$（$K$ 為自定義陣列大小 MAX_STACK），能有效預防系統堆疊崩潰。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $(m,n)$ | 預期輸出 | 遞迴實際輸出 | 非遞迴實際輸出 |
|----------|--------------|----------|----------|----------|
| 測試一   | $m = 0, n = 0$      | 1        | 1        | 1        |
| 測試二   | $m = 1, n = 2$      | 4        | 4        | 4        |
| 測試三   | $m = 2, n = 2$      | 7        | 7        | 7        |
| 測試四   | $m = 3, n = 3$      | 61       | 61       | 61        |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o ackermann ackermann.cpp
$ ./ackermann
請輸入 m 與 n 的值 (例如 2 2): 2 2

--- 計算結果 ---
遞迴版本 A(2, 2) = 7
非遞迴版本 A(2, 2) = 7
```

### 結論

1. 程式成功同時實現了 Ackermann 函數的遞迴與非遞迴兩種計算方式。
2. 非遞迴版本在僅使用 <iostream> 標頭檔的限制下，利用陣列手動模擬 Stack，達到了與遞迴相同的運算結果。
3. 測試案例驗證了兩種實作方式在小數值輸入下的正確性與一致性。

## 申論及開發報告

### 遞迴的結論

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. 遞迴版本的直覺性

遞迴的寫法能夠直接對應數學定義式，邏輯簡單、易於編寫與驗證，特別適合用來理解 Ackermann 函數的結構。

### 非遞迴的結論

非遞迴版本的必要性（突破限制與防範 Overflow）由於 Ackermann 函數的數值與成長速度極快，系統堆疊深度往往無法負荷較大的 $m$ 與 $n$。透過手動維護陣列堆疊來改寫為非遞迴版本，不僅符合作業僅允許使用 <iostream> 的限制，更能避免系統堆疊溢位（Stack Overflow）的問題。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
