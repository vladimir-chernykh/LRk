#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <set>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <math.h>
#include <utility>
#include <list>
#include "grammar.hpp"
#include "my_str.hpp"
#include "tinyxml.h"

using namespace std;

class situation;
class set_of_situation;
class LR;

class situation//LR-situation
{
private:
	string left_part;
	my_str prefix_right_part;
	my_str suffix_right_part;
	my_str avanschain;
	situation()
	{
		left_part = "";
		prefix_right_part = my_str();
		suffix_right_part = my_str();
		avanschain = my_str();
	};
	situation(string lp, my_str pr, my_str su, my_str av)
	{
		left_part = lp;
		prefix_right_part=pr;
		suffix_right_part=su;
		avanschain=av;
	};
public:
	bool operator< (const situation &a) const
	{
		if(left_part<a.left_part)
			return true;
		if(left_part>a.left_part)
			return false;
		if(prefix_right_part<a.prefix_right_part)
			return true;
		if(prefix_right_part>a.prefix_right_part)
			return false;
		if(suffix_right_part<a.suffix_right_part)
			return true;
		if(suffix_right_part>a.suffix_right_part)
			return false;
		if(avanschain<a.avanschain)
			return true;
		return false;
	};
	bool operator== (const situation &a) const
	{
		if(left_part == a.left_part &&
				prefix_right_part == a.prefix_right_part &&
				suffix_right_part == a.suffix_right_part &&
				avanschain == a.avanschain)
			return true;
		return false;
	};
	friend class LR;
	friend class set_of_situation;
};

class set_of_situation
{
private:
	set_of_situation()
	{
		sit = set<situation>();
	};
	set<situation> sit;
	void closure(cfg &grammar, LR &analyzer, int k);
public:
	bool operator< (const set_of_situation &a) const
	{
		if(sit.size()==0)
			return true;
		if(a.sit.size()==0)
			return false;
		set<situation>::iterator i = sit.begin();
		set<situation>::iterator j = a.sit.begin();
		while(i!=sit.end()&&j!=a.sit.end())
		{
			if((!(*i<*j))&&(!(*j<*i)))
			{
				i++;
				j++;
			}
			else
				return *i<*j;
		}
		if(i==sit.end()&&j!=a.sit.end())
			return true;
		else return false;
	};
	bool operator== (const set_of_situation &a) const
	{
		if(sit.size() != a.sit.size())
			return false;
		set<situation>::iterator i = sit.begin();
		set<situation>::iterator j = a.sit.begin();
		while(i!=sit.end()&&j!=a.sit.end())
		{
			if(*i == *j)
			{
				i++;
				j++;
			}
			else
				return false;
		}
		return true;
	};
	friend class LR;
};

class LR
{
private:
	map<string, set<my_str> > first;
	vector<set_of_situation> canonicial;
	map<string, vector<int> > Goto;
	map<my_str, vector<pair<char,int> > > Action;

	set<my_str> plus_k(set<my_str> &L1, set<my_str> &L2, int k);
	set<my_str> summ_k(my_str rule, int k);

	//computes a FIRST_k function for symbols of grammar
	void compute_first_for_symbols(cfg &grammar, int k);
	//computes FIRST_k function for chain of symbols
	set<my_str> compute_first_for_chain(my_str chain, int k);

	//compute where we can go from I set by X
	set_of_situation transition(const set<situation> &I, string x, cfg &grammar, LR &analyzer, int k);
	//compute canonicial system of sets and GOTO table
	void compute_canonicial(cfg &grammar, int k);//grammar should be augment!!

	//compute ACTION table
	void compute_action(cfg &grammar, int k);

	//find first entry of any element from [first2,last2) in [first1,last1)
	vector<string>::iterator my_find_first_of(vector<string>::iterator first1,
			vector<string>::iterator last1,
			set<string>::iterator first2,
			set<string>::iterator last2);

public:
	LR()
	{
		first = map<string, set<my_str> >();
		canonicial = vector<set_of_situation>();
	};

	void build(cfg &grammar, int k);

	string word_analysis(my_str word, cfg &grammar, int k);

	//printing methods
	void print_xml(const char* filename, cfg &grammar, int k);
	void print_first(int k);
	void print_first(ostream& cout_,int k);
	void print_canonicial(int k);
	void print_goto(int k);
	void print_action(int k);

	friend class set_of_situation;
};

