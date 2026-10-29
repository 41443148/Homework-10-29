#include<iostream>
using namespace std;
class Polynomial;
class Term
{
	friend Polynomial;
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
		for (int i = 0; i < terms; i++)
		{
			termArray[i] = other.termArray[i];
		}
	}
	Polynomial add(Polynomial poly)
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
				c.NewTerm(poly.termArray[bPos].coef, termArray[aPos].coef);
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
			c.NewTerm(termArray[bPos].coef, termArray[bPos].exp);
		}
		return c;
	}
	Polynomial Mult(Polynomial poly)
	{
		Polynomial c;
		for (int i = 0; i < terms; i++)
		{
			Polynomial temp;
			for (int j = 0; j < poly.terms; j++)
			{
				float newCoef = termArray[i].coef * poly.termArray[j].coef;
				int newExp = termArray[i].exp * poly.termArray[j].exp;
				temp.NewTerm(newCoef, newExp);
			}
			c = c.add(temp);
		}
		return c;
	}
	float Eval(float)
	void NewTerm(float theCoef,int theExp)
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

};
int main()
{

}
