#include "my_str.hpp"

my_str::size_type my_str::size() const
{
	return str.size();
};

void my_str::clear()
{
	str.resize(0);
};

my_str my_str::substr(size_t pos, size_t len)
{
	my_str result;
	if(pos >= str.size())
	{
		result.add("");
		return result;
	}
	else if(len == my_str::npos||pos+len>=str.size())
	{
		vector<string>::iterator i = str.begin() + pos;
		for(;i!=str.end();i++)
			result.add(*i);
	}
	else
	{
		vector<string>::iterator i = str.begin()+pos;
		for(;i!=str.begin()+pos+len;i++)
			result.add(*i);
	}
	return result;
};

my_str::iterator my_str::begin()
{
	return str.begin();
};

my_str::iterator my_str::end()
{
	return str.end();
};

void my_str::erase(my_str::iterator pos)
{
	str.erase(pos);
};

void my_str::erase(my_str::iterator first, my_str::iterator last)
{
	str.erase(first,last);
};

string& my_str::operator[](int index)
{
	return str[index];
};

string const& my_str::operator[](int index) const
{
	return str[index];
};

bool my_str::operator< (const my_str &s) const
{
	if(s.size()==0)
		return false;
	if(this->size()==0)
		return true;
	string thi="", ss="";
	for(my_str::size_type i = 0; i < s.str.size(); i++)
		ss+=s[i];
	for(my_str::size_type i = 0; i < this->str.size(); i++)
		thi+=(*this)[i];
	return thi<ss;
};

bool my_str::operator> (const my_str &s) const
{
	if(this->size()==0)
		return false;
	if(s.size()==0)
		return true;
	string thi="", ss="";
	for(my_str::size_type i = 0; i < s.str.size(); i++)
		ss+=s[i];
	for(my_str::size_type i = 0; i < this->str.size(); i++)
		thi+=(*this)[i];
	return thi>ss;
};

bool my_str::operator== (const my_str &s) const
{
	if(*this>s)
		return false;
	if(*this<s)
		return false;
	return true;
};

bool my_str::operator!= (const my_str &s) const
{
	if(*this == s)
		return false;
	else
		return true;
};

void my_str::del_empty()
{
	vector<string>::iterator new_end = std::remove(str.begin(),str.end(),"");
	if(new_end!=str.end())
		str.resize(size_t(new_end-str.begin()));
};

void my_str::plus(const char *s)
{
	str.push_back("");
	size_t i = 0;
	while(s[i]!='\0')
	{
		str[str.size()-1]+=s[i];
		i++;
	}
	this->del_empty();
	if(str.size()==0)
		str.push_back("");
};

void my_str::plus(const string &s)
{
	str.push_back(s);
	this->del_empty();
	if(str.size()==0)
		str.push_back("");
};

void my_str::plus(const my_str &s)
{
	my_str::size_type i = 0;
	for(;i!=s.size();i++)
		this->plus(s[i]);
};

void my_str::add(const char *s)
{
	str.push_back("");
	size_t i = 0;
	while(s[i]!='\0')
	{
		str[str.size()-1]+=s[i];
		i++;
	}
};

void my_str::add(const string &s)
{
	str.push_back(s);
};

void my_str::add(const my_str &s)
{
	my_str::size_type i = 0;
	for(;i!=s.size();i++)
		this->add(s[i]);
};
