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
