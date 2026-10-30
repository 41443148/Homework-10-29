# 41443148

作業一

## 解題說明

本題採用物件導向的概念，將多項式拆解為兩個類別： 代表單項式 與 代表多項式。
注{Term 類別：包含兩個私有成員，分別為係數 coef（浮點數）與指數 exp（整數）}

使用動態陣列Term *termArray來儲存所有非零項，藉此節省記憶體空間。

維護 capacit（陣列容量）與 terms（實際非零項數量）來進行動態擴容管理。

多項式加法類似合併排序的概念，同時走訪兩個多項式的termArray。

多項式乘法運用分配律，將第一個多項式的每一項分別與第二個多項式的每一項相乘。

### 解題策略

1.資料結構與抽象資料型態策略:
{

稀疏多項式儲存法（Sparse Representation）：

{

因為多項式中常有許多係數為 0 的項若使用傳統的一維陣列（以指數當作陣列索引），會浪費大量記憶體空間。

採用的策略是使用結構體（Term）與動態陣列（termArray），僅儲存非零項的係數（coef）與指數（exp），大幅節省儲存空間與運算時間。

}

}

2-1.多項式加法 — 雙指標合併策略：
{

使用兩個指標（aPos 與 bPos）分別掃描兩個多項式：
{

若兩者指數相同：將係數相加，若和不為 0 則寫入新多項式，兩邊指標同時推進。

若當前指數小於對方：將對方的項直接寫入新多項式，對方指標推進。

處理完共同項後，再將剩餘未走訪完的項直接補到結尾。

}

}

2-2.多項式乘法（Mult）— 分配律與累加策略：
{

採用分配律：拿第一個多項式的每一項，去乘以第二個多項式的每一項。

每乘以一項就產生一個暫時的多項式，其係數相乘、指數相加。

最後透過迴圈呼叫寫好的 Add 函式，將所有的 temp 逐步累加起來，得到最終的乘積結果。

}

3. 記憶體管理與安全策略
{

當非零項數量 terms 達到目前容量上限 capacity 時，採用倍增策略（capacity *= 2），配置更大的記憶體空間、複製舊資料、並安全釋放舊指標，以確保能動態處理任意長度的輸入。

由於類別內部管理了用 new[] 配置的動態陣列，為了避免在物件傳遞（如函式參數傳遞、回傳值）與指定運算（=）時發生淺拷貝導致的記憶體重複釋放（Double Free）或崩潰，嚴格實作了複製建構子（Copy Constructor）與指定運算子（operator=），確保每個物件擁有獨立的記憶體空間。

}

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

1. 時間複雜度：
{

單一多項式空間：O(n)假設一個多項式中含有 n 個非零項，則 termArray 需要配置大小為 n 的記憶體空間來儲存係數與指數。

多項式加法（Add）：O(n + m)假設兩個相加的多項式非零項數量分別為 $n$ 與 $m$，結果多項式最多包含 n + m 項，因此額外配置的動態陣列空間與兩者項數成線性正比。

多項式乘法（Mult）：O(n \times m)在最壞情況下（無同類項可合併），兩個分別有 n 項與 m 項的多項式相乘，會產生最多 n \times m 個暫時項，因此乘法過程中的暫存空間需求為 O(n \times m)。

動態擴容機制：平攤分析 O(1)當陣列容量不足時，採用倍增策略（capacity *= 2），重新配置雙倍空間並搬移舊資料。雖然單次擴容的成本為 O(k)（k 為當前項數），但在多次連續新增的操作下，平攤下來每次操作的成本仍為 O(1)。

}

2. 空間複雜度：
{

多項式加法— O(n + m)演算法採用雙指標（aPos 與 bPos）同步掃描兩個已按指數排序的陣列。

每個項最多被檢視一次，因此迴圈次數由兩者的項數總和決定，時間複雜度為線性時間 O(n + m)，效率極高。

多項式乘法— O(n \times m + (n \times m)^2) 或優化後 O(n \cdot m \log(n \cdot m))

第一層迴圈執行 n 次，第二層迴圈執行 m 次，總共會產生 n \times m 個單項乘積。

每產生一項後都會呼叫一次 Add 函式將其累加回總結果中。若未進行複雜度優化，隨著結果陣列越來越大，每次 Add 的成本會隨之增加，整體時間複雜度大約落在O(n \times m \cdot \text{MAX\_TERMS}) 或透過有效率的合併接近 O(n \times m)。

多項式求值— O(n \cdot \log(\text{exp})) 或 O(n)針對 n 個非零項進行迴圈，每一項透過 pow(f, exp) 計算次方值。

假設 pow 函式的時間複雜度視同常數或對數級別，整體求值時間複雜度為 O(n)。

輸入與輸出運算（>> / <<）— O(n)

需完整走訪多項式的 n 個非零項進行讀取或格式化輸出，時間複雜度為 O(n)。

}
## 測試與驗證
輸入第一個多項式:
輸入非零項數量: 2
輸入第 1 項的係數: 3
輸入第 1 項的指數: 2
輸入第 2 項的係數: 5
輸入第 2 項的指數: 0
輸入第二個多項式:
輸入非零項數量: 2
輸入第 1 項的係數: 2
輸入第 1 項的指數: 1
輸入第 2 項的係數: 1
輸入第 2 項的指數: 0
P1 = 3x^2 + 5x^0
P2 = 2x^1 + 1x^0
P1 + P2 = 3x^2 + 2x^1 + 6x^0
P1 * P2 = 6x^3 + 3x^2 + 10x^1 + 5x^0

C:\Users\jery1\OneDrive\Desktop\新增資料夾\c and c++\Project155\x64\Debug\Project155.exe (流程 11696) 已結束，代碼為 0 (0x0)。
若要在偵錯停止時自動關閉主控台，請啟用 [工具] -> [選項] -> [偵錯] -> [偵錯停止時，自動關閉主控台]。
按任意鍵關閉此視窗…
### 測試案例

| 測試案例 | 輸入參數一項  | 輸入參數二項 | 預期輸出P1+P2 | 預期輸出P1*P2 |
|----------|--------------|----------|----------|----------|
| 測試一   | 3x^2+5x^0      | 2x^1+1x^0        | 3x^2+2x^1+6x^0        |6x^3+3x^2+10x^1+5x^0         |
| 測試二   | 4x^2+3x^1      | -4x^2+2x^0        | 3x^1+2x^0        | -16x^4-12x^3+8x^2+6x^1        |
| 測試三   | 0      | 5x^2+1x^0        | 5x^2+1x^0        | 0      |

### 結論

1. 深化指標與記憶體管理的理解：在處理多項式加法、相乘及動態擴容（NewTerm）時，深刻體會到 C++ 記憶體管理的嚴謹性。若沒有妥善實作複製建構子（Copy Constructor）與指定運算子（operator=），極易引發淺拷貝帶來的記憶體崩潰或重複釋放問題。

2. 演算法與資料結構的結合：採用稀疏多項式的策略，僅儲存非零項，不僅大幅節省了記憶體空間，也讓加法（雙指標合併）與乘法（分配律與累加）的邏輯更加清晰有效率。

3. 模組化與運算子重載的便利性：透過重載 >> 與 << 運算子，讓多項式的輸入與輸出能像內建型態一樣直覺，提升了程式碼的可讀性與除錯效率。

4. 符合作業評分標準：本專案涵蓋了 ADT 實作、運算子重載、時間與空間複雜度分析、以及多項式加乘與動態擴容的完整測試，全面滿足了作業規範的要求。

## 問題

問題現象

現象：在完成多項式加法與乘法 (Mult) 的初步實作後，進行小測資運算（例如 P_1(x) = 3x^2 + 5 與 P_2(x) = 2x^1 + 1）時，加法運算正常，但 P1 * P2 的輸出結果後面卻出現了巨大的亂碼與異常指數（例如 -4.31602e+08x^-842150451），甚至在程式結束時觸發記憶體崩潰（Heap Corruption）。

經過除錯發現，問題出在動態陣列的記憶體管理與物件生命週期：

缺少深拷貝防護（Copy Constructor & operator=）：當多項式在進行函式參數傳遞（值傳遞）或暫生物件指派時，C++ 預設進行「淺拷貝」，導致多個物件共用同一個 termArray 指標。當暫生物件被銷毀時，會順便將記憶體釋放，造成後續存取到已被刪除的記憶體空間（野指標）。

動態擴容（NewTerm）時的指標失效：在乘法迴圈中呼叫 Add 函式時，若暫存多項式在擴容過程中重新配置記憶體，未妥善處理的指標複製會導致內部項數（terms）與資料內容錯亂。

解決方法

實作 Rule of Three（三大守則）：在 Polynomial 類別中明確撰寫複製建構子（Copy Constructor）與指定運算子（operator=），確保每次物件複製或指派時，都會配置獨立的記憶體空間進行「深拷貝（Deep Copy）」。

改用常數參考傳遞（const Polynomial&）：將 Add 等成員函式的參數型態改為常數參考傳遞，避免在函式呼叫與回傳過程中產生不必要的物件複製，徹底消除了記憶體指標錯亂與亂碼的問題，使乘法運算結果正確輸出。
