#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "my_str.hpp"
#include "tinyxml.h"

using namespace std;

typedef string left_part;
typedef my_str right_part;

class rule
{
public:
	map<left_part, set<right_part> > rules;
	vector< pair<left_part, set<right_part>::iterator> > numer;
	rule()
	{
		rules = map<left_part,set<right_part> >();
		numer = vector< pair<left_part, set<right_part>::iterator> >();
	};
	friend ostream& operator << (ostream &cout_, rule &a)
	{
		for(size_t i=0;i!=a.numer.size();i++)
		{
			cout_<<i<<" : "<<(a.numer[i].first)<<" -> "<<*(a.numer[i].second);
			cout_<<endl;
		}
		return cout_;
	};
};

//in grammar module "e" is an empty word

class cfg
{
private:
	set<string> N;//non-terminals
	set<string> T;//terminals
	string S;//axiom
	rule P;//rules

	void delete_non_generating();
	void delete_non_reachable();
public:
	cfg();
	friend ostream& operator<< (ostream &cout_, cfg &a);
	void read_file(const char *filename);
	void read_xml(const char *filename);
	void augment();
	void cast();
	friend class set_of_situation;
	friend class LR;
};
