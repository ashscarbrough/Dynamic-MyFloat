/*
Author  :  Ash Scarbrough
Date    :  4/28/2015
Homework:  10
Source  :  MyFloatD.cpp
Compiler:  Visual Studio 2013 Professional

ACTION:     Create a dynamically implemented version of the MyFloat class. 
			The original abstract data type named "MyFloat" serves as the base
			for this dynamically allocated class.  The MyFloat data type serves
			specialized needs for small floats between 0 and 1 that can have as many
			digits as is permitted by system memory.  This class allows calculations
			that are more accurate than the standard float class. Class will make
			conservative use of memory, utilizing characters instead of integers. MyFloats
			are held in a character array with dynamic memory allocation, having default
			size of 10 number.  The array ignores subscript 0 when storing digits,
			so the intialized array will be Number[DEFAULT_SIZE+1], this provides
			a scaling effect, leaving (array) Number[0] empty, as shown:

	char Number[DEFAULT_SIZE]

	  0   1   2   3   4   5   6   7   8   9  10  
	 -------------------------------------------
	| X |Ch1|Ch2|Ch3|Ch4|Ch5|Ch6|Ch7| 0 | 0 | 0 |
	 -------------------------------------------

		Array Number[] holds 7 digits, ignoring memory slot Number[0]. Remaining 	
		memory slots are unfilled intially.

		Class will contain the following member functions:

		Digits		Returns the number of digits in a MyFloat. If there is an error reading
					a float or if the float exceeds the parameters of the class "MyFloat2"
					then 0 digits will be displayed.

		MaxDigits	Returns the maximum number of possible digits in a MyFloat.


		Deconstructor		Deconstructs data & reallocates memory to be used elsewhere

		Constructor 		Default constructor sets NumberofDigits to 0.
		
		Copy Constructor	Makes Deep Copy of MyFloat

		Max Length Constructor	Constructor that creates any length of MyFloat


		>>		Reads a MyFloat from standard input. If an error is detected in reading
				MyFloat, then NumberofDigits will be changed to 0, alerting program to
				the error.

		<<		Writes leading 0 and decimal point "0." followed by digits in MyFloat to
				standard output. If MyFloat has 0 digits (error), output will be '0.?'


		+		Adds two MyFloat variables X and Y together and stores results in
				MyFloat Z. Standard addition rules apply. Carry over into ones field is
				ignored for this assignment.

		==		Returns non-zero if two MyFloats are equal, 0 otherwise.

		>		Greater than comparison operator, two MyFloats can be compared so that
				the larger of the two values is determined.  (Ex: MyFloat1 > MyFloat2)
				If MyFloat1 is larger, returns true.  If Myfloat2 is larger, returns
				false.


		=		Converts a string representing a float between 0 and 1 to a MyFloat.
				Use of assignment operator must return a value of MyFloat so as to
				permit cascading assignment operators: i.e. X=Y=Z.

		=		Allows deep copy of X = Y

*/


#include <iostream>
#include <ctype.h>
#include <new>
using namespace std;

class MyFloat						//Dynamically allocated MyFloat Class
{
private:
	enum { DEFAULT_SIZE = 10 };			//Default MyFloat length
	char* Number;					//Pointer to create dynamic char
							//char Number[DEFAULT_SIZE + 1];
	int NumberofDigits;		//Number of valid chars in Number array
	int MaximumDigits;			//Max possible length of MyFloat
							//Set to 0 if memory allocation error
public:
	int Digits() const			//Provides access to NumberofDigits
	{
		return NumberofDigits;
	}

	int MaxDigits() const		//Provides access to MaximumDigits
	{
		return MaximumDigits;
	}

	inline ~MyFloat();		//Decsonstructor: de-allocates memory

	inline MyFloat(const MyFloat &S);	//Copy constructor, deep copy
	inline MyFloat();				//Default length constructor for MyFloat
	inline MyFloat(int NumberofDigits);		//Constructor, creates any length MyFloats

	inline MyFloat& operator= (char Rhs[]);	//Allows assignment operator - X = "0.12345"
	inline MyFloat& operator= (const MyFloat &Rhs);	//Allows deep copy X = Y

	inline friend ostream& operator<< (ostream &MyFloatOut, const MyFloat &X);  //Outputs MyFloat
	inline friend istream& operator>> (istream &In, MyFloat &X);		//Reads MyFloat

	inline MyFloat operator+ (const MyFloat &Rhs);		
	inline int operator== (const MyFloat &Rhs);
	inline int operator> (const MyFloat& Rhs);

};



/*================================= ~MyFloat ====================================

	The class deconstructor is called automatically when an instance of the MyFloat
	class goes out of scope.  The deconstructor is called to return dynamically allocated 
	storage when the program exits a function.  Memory is deallocated in:
		1) local variables
		2) parameters passed by value

==================================================================================*/

MyFloat::~MyFloat()
{
	delete Number;		//Return dynamic storage associated with Number
}



/*========================== MyFloat (copy constructor) =============================

	The copy constructor is called when:
	
	1) When an instance of the String is passed by value to a function, the constructor
	   makes a copy of the MyFloat

	2) When a MyFloat is initialized in a declaration, the copy constructor is called
	   to make a copy of 

	3) When MyFloat is returned as a function value or from a MyFloat operator

==================================================================================*/

MyFloat::MyFloat(const MyFloat &S)
{
	MaximumDigits = S.MaximumDigits;
	NumberofDigits = S.NumberofDigits;

	Number = new(nothrow) char[MaximumDigits+1];


	if (Number != NULL)
		for (int k = 0; k < NumberofDigits; ++k)
			Number[k] = S.Number[k];
	else
		MaximumDigits = 0;
}



/*======================= MyFloat (default constructor) ===========================

	Default constructor that supports the delcarations of "MyFloat S", creating a
	MyFloat with NumberofDigits set to 0 and max value set to DEFAULT_SIZE.

	This constructor will be called automatically if an array of MyFloat is declared.

==================================================================================*/

MyFloat::MyFloat()
{
	MaximumDigits = DEFAULT_SIZE;
	NumberofDigits = 0;
	Number = new(nothrow) char[DEFAULT_SIZE+1];
	if (Number == NULL)					//If memory allocation failure
		NumberofDigits = 0;			//NumberofDigits set to error alert value
}



/*========================== MyFloat (constructor) ===============================

	Constructor allows declarations of MyFloat in the form of "MyFloat S[50]", creating
	a MyFloat with a MaxiumDigits number of 50, initialized to "".

==================================================================================*/

MyFloat::MyFloat(int Digits)
{
	Number = new(nothrow) char[Digits+1];
	NumberofDigits = 0;
	if (Number == NULL)
		MaximumDigits = 0;
	else
		MaximumDigits = NumberofDigits;
}



/*=================================(operator=)=====================================

	This function gives user the ability to assign a value between 0 and 1 to a
	to a variable of datatype MyFloat.  It returns MyFloat *this, which is attained
	by copying a MyFloat on the Right hand side of the assignment operator.

	Because it provides a return value it permits the cascading of assignment
	operators, allowing for the following: (MyFloat) X= (MyFloat) Y = (MyFloat) Z

==================================================================================*/

MyFloat& MyFloat::operator= (char Rhs[])
{
	int k, NumberofDigitsRhs = 0;

	while (Rhs[NumberofDigitsRhs])
		++NumberofDigitsRhs;

	if (NumberofDigitsRhs > MaximumDigits)
	{
		delete[] Number;
		Number = new(nothrow) char[NumberofDigitsRhs];
		if (Number == NULL)
			{
				NumberofDigits = 0;
				return *this;
			}
		MaximumDigits = NumberofDigitsRhs;
	}

	for (k = 0; k < NumberofDigitsRhs; ++k)
		Number[k] = Rhs[k];

	NumberofDigits = NumberofDigitsRhs;
	return *this;
}



/*===========================operator=(MyFloat)================================

This function gives user the ability to assign a MyFloat value to a new MyFloat.  
It returns MyFloat *this, which is attained by copying a MyFloat on the Right hand 
side of the assignment operator.

Because it provides a return value it permits the cascading of assignment
operators, allowing for the following: (MyFloat) X= (MyFloat) Y = (MyFloat) Z

==================================================================================*/

MyFloat& MyFloat::operator= (const MyFloat &Rhs)
{

	int k = 0;

	if (this == &Rhs)
		return *this;

	if (MaximumDigits < Rhs.NumberofDigits)
	{
		delete[] Number;

		Number = new(nothrow) char[Rhs.NumberofDigits+1];
		if (Number == NULL)
		{
			MaximumDigits = 0;
			return *this;
		}
	}

	NumberofDigits = Rhs.NumberofDigits;

	for (k = 0; k < (Rhs.NumberofDigits); ++k)
		Number[k] = (char)Rhs.Number[k];

	return *this;
}



/*================================= operator>> ====================================

Overloads input operator (>>) so that it can read and hold a MyFloat.  It replaces
the Read() function of MyFloat class.  It accepts MyFloat from standard input.

This function maintains standard conventions for input using >>. It:
skips white space
reads and stores a continuous non-blank value
stops when whitespace is read or when error occurs.
Extra functions - It:
Reads only up to MAX_DIGIT
Only accepts digit input

Function will also return character to input buffer after completion of reading
MyFloat to clear the istream.

If an error is detected in reading MyFloat, then NumberofDigits will be changed to 0.

==================================================================================*/

istream& operator>> (istream& In, MyFloat& Y)
{
	char Ch;

	Y.NumberofDigits = 0;		//Ensures that NumberofDigits is reset to 0

	cin >> Ch;					//Cin ignores leading white spaces

	if (!cin.good() || (Ch != '0' && Ch != '.'))
		return In;                         //Bad input, NumberofDigits = 0, return In

	while (Ch == '0' || isspace(Ch))
		Ch = cin.get();                  // skip leading zero's and spaces

	if (Ch != '.' || !cin.good())
		return In;                      // if no decimal, bad input, or EOF, return

	Ch = cin.get();                 //get character after decimal

	int i = 0;

	while (cin.good() && isdigit(Ch) && i < Y.MaximumDigits)
	{
		Y.Number[++i] = Ch - '0';         // convert char to int and store
		Ch = cin.get();                 // get next character
	}

	cin.putback(Ch);     // leave terminating char in input stream
	Y.NumberofDigits = i;
	
	return In;

}



/*================================= operator<< ===================================

Function overloads the << operator to give output of MyFloat class.  This function
replaces the Write() function of MyFloat.

Writes leading 0 and decimal point, "0.", followed by digits in MyFloat to
standard output. If MyFloat has 0 digits (error), output will be '0.?'

==================================================================================*/

ostream& operator<< (ostream& MyFloatOut, const MyFloat& Y)
{

	cout << "0.";		// "0." is included with or without error  ***was MyFloatOut not cout

	if (Y.NumberofDigits == 0)		//If NumberofDigits is 0, display error message
		cout << "?";
	else
	{
		for (int i = 1; i <= Y.NumberofDigits; i++)
		{
			cout << int(Y.Number[i]);  //typecast character into integer		}
	}
	return MyFloatOut;
}



/*==================================(operator+)===================================

Allows two MyFloats to be added together, using standard rules of addition, with
carrying over of sums greater than 9.  Carry-over into the one's place will be
ignored.

Returns (Value): MyFloat Z, which is the resulting sum of added MyFloats

==================================================================================*/

MyFloat MyFloat::operator+ (const MyFloat &Rhs)
{
	MyFloat Z;

	int i;

	int ColumnSum, CarryOver = 0;

	if (NumberofDigits > Rhs.NumberofDigits)   //Determines the largest number of Digits
	{						   //and transfers that number to sum MyFloat
		Z.NumberofDigits = NumberofDigits;	

		for (i = NumberofDigits; i >= Rhs.NumberofDigits; --i)
		{
			Z.Number[i] = Number[i];
		}
	}
	else if (NumberofDigits < Rhs.NumberofDigits)
	{
		Z.NumberofDigits = Rhs.NumberofDigits;

		for (i = Rhs.NumberofDigits+1; i >= NumberofDigits; --i)
		{
			Z.Number[i] = Rhs.Number[i];
		}
	}

	for ( i; i >= 1; i--)		//For each column place
	{
		ColumnSum = Number[i] + Rhs.Number[i] + CarryOver;
						//Sum of two MyFloat columns and carry over value found
		if (ColumnSum >= 10)
		{
			Z.Number[i] = ColumnSum - 10;	   //Column keeps one's digits and carries
			CarryOver = 1;				//over values greater than 10
		}
		else
		{
			Z.Number[i] = ColumnSum;
			CarryOver = 0;
		}
	}

	return Z;						//Returns MyFloat Z

}


/*=================================(operator==)===================================

Function compares two MyFloats and checks for equality.  Returns true (non-zero)
if two MyFloats are equal, otherwise returns false (0).

Returns bool value: True or False

Precondition: MyFloats being compared need to have trailing 0s to be seen as equal

==================================================================================*/

int MyFloat::operator== (const MyFloat &Rhs)
{
	int GreatestNumDigits;      //Determines which myfloat has greatest number of Digits
	if (NumberofDigits >= Rhs.NumberofDigits)
		GreatestNumDigits = NumberofDigits;
	else
		GreatestNumDigits = Rhs.NumberofDigits;

	for (int i = 1; i <= GreatestNumDigits; i++)  //Each column of MyFloat is compared.
	{
		if (Number[i] != Rhs.Number[i])
			return 0;
	}

	return 1;
}



/*=================================(operator>)====================================

Function compares two MyFloats and checks if 1st MyFloat is greater than 2nd MyFloat.
Returns true (non-zero) if 1st MyFloat is greater, otherwise returns false (0).

Function accounts for the NumberofDigits to determine how many numbers need to be
compared.  This conserves memory.

Returns bool value: true or false.

Precondition: MyFloats being compared need to have trailing 0s.

==================================================================================*/

int MyFloat::operator> (const MyFloat &Rhs)
{
	int i;    //Determines which myfloat has greatest number of Digits

	while (i < (NumberofDigits+1) && i < (Rhs.NumberofDigits+1))  //Each column of MyFloat is compared
	{						  //from greatest number place to least
		if (Number[i] > Rhs.Number[i])	//If value of 1st MyFloat is greater, then true
			return 1;
		else
			++i;
	}

	return 0;

}

