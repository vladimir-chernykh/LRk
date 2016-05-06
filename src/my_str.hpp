#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class my_str
{
private:
	vector<string> str;
public:
	typedef vector<string>::size_type size_type;//size_type
	typedef vector<string>::iterator iterator;//iterator

	static const size_t npos = string::npos;//end of my_str

	my_str()//constructor 1
	{
		str = vector<string>(0);
	};
	my_str(const string &s)//constructor 2
	{
		str = vector<string>(1,s);
	};
	my_str(const char *s)//constructor 3
	{
		str = vector<string>(1,"");
		size_t i = 0;
		while(s[i]!='\0')
		{
			str[0]+=s[i];
			i++;
		}
	};

	my_str::size_type size() const;//size of my_str
	void clear();//delete all elements
	my_str substr(size_t pos = 0, size_t len = my_str::npos);
	//return a substring of my_str starting at pos and size of len
	//if len==npos then to the end of my_str
	my_str::iterator begin();//iterator to the first element
	my_str::iterator end();//iterator to past-the-end element
	void erase(my_str::iterator pos);
	void erase(my_str::iterator first, my_str::iterator last);


	string& operator[](int index);//reference to the element at position index
	string const& operator[](int index) const;//const reference to the element at position index
	bool operator< (const my_str &s) const;
	bool operator> (const my_str &s) const;
	bool operator== (const my_str &s) const;
	bool operator!= (const my_str &s) const;
	friend ostream& operator<< (ostream &cout_, my_str const& s)
	{
		for(my_str::size_type i = 0; i < s.str.size(); i++)
			cout_<<s[i];
		return cout_;
	};


	void del_empty();//delete all symbols like ""(empty string)
	void plus(const char *s);//add element to my_str like to string
	void plus(const string &s);
	void plus(const my_str &s);
	void add(const char *s);//add element to my_str like to vector
	void add(const string &s);
	void add(const my_str &s);
};
