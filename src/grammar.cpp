#include "grammar.hpp"

using namespace std;

cfg::cfg()
{
	N = set<string>();
	T = set<string>();
	S = "";
	P = rule();
};

void cfg::delete_non_generating()
{
	set<left_part> newly(T);
	newly.insert("");
	bool is_modified = true;

	while(is_modified)
	{
		is_modified = false;
		size_t size_before = newly.size();
		map<left_part, set<right_part> >::iterator i = P.rules.begin();
		for(;i!=P.rules.end();i++)
		{
			set<right_part>::iterator j = i->second.begin();
			for(;j!=i->second.end();j++)
			{
				my_str temp = *j;
				my_str::iterator it = temp.begin();
				bool flag = true;
				for(;it!=temp.end();it++)
				{
					if(find(newly.begin(),newly.end(),*it)==newly.end())
					{
						flag = false;
						break;
					}
				}
				if(flag==true)
				{
					newly.insert(i->first);
				}
			}
		}
		size_t size_after = newly.size();
		if(size_after>size_before)
			is_modified = true;
	}


	map<left_part, set<right_part> >::iterator i = P.rules.begin();
	while(i!=P.rules.end())
	{
		if(find(newly.begin(),newly.end(),i->first)==newly.end())
		{
			map<left_part, set<right_part> >::iterator pos = i;
			i++;
			P.rules.erase(pos);
			continue;
		}
		set<right_part>::iterator j = i->second.begin();
		while(j!=i->second.end())
		{
			bool is_plused = false;
			my_str temp = *j;
			my_str::iterator it = temp.begin();
			for(;it!=temp.end();it++)
			{
				if(find(newly.begin(),newly.end(),*it)==newly.end())
				{
					set<right_part>::iterator pos = j;
					j++;is_plused=true;
					i->second.erase(pos);
					break;
				}
			}
			if(is_plused == false)
				j++;
		}
		i++;
	}

	N.clear();
	newly.erase("");
	set<left_part>::iterator iter = T.begin();
	for(;iter!=T.end();iter++)
		newly.erase(*iter);
	N = newly;
};

void cfg::delete_non_reachable()
{
	set<left_part> newly;
	newly.insert(S);
	bool is_modified = true;

	while(is_modified)
	{
		is_modified = false;
		size_t size_before = newly.size();
		map<left_part, set<right_part> >::iterator i = P.rules.begin();
		for(;i!=P.rules.end();i++)
		{
			if(find(newly.begin(),newly.end(),i->first)!=newly.end())
			{
				set<right_part>::iterator j = i->second.begin();
				for(;j!=i->second.end();j++)
				{
					my_str temp = *j;
					my_str::iterator it = temp.begin();
					for(;it!=temp.end();it++)
						if(find(N.begin(),N.end(),*it)!=N.end())
							newly.insert(*it);
				}
			}
		}
		size_t size_after = newly.size();
		if(size_after>size_before)
			is_modified = true;
	}

	map<left_part, set<right_part> >::iterator i = P.rules.begin();
	while(i!=P.rules.end())
	{
		if(find(newly.begin(),newly.end(),i->first)==newly.end())
		{
			map<left_part, set<right_part> >::iterator pos = i;
			i++;
			P.rules.erase(pos);
			continue;
		}
		set<right_part>::iterator j = i->second.begin();
		while(j!=i->second.end())
		{
			bool is_plused = false;
			my_str temp = *j;
			my_str::iterator it = temp.begin();
			for(;it!=temp.end();it++)
			{
				if(find(newly.begin(),newly.end(),*it)==newly.end()&&find(T.begin(),T.end(),*it)==T.end()&&*it!="")
				{
					set<right_part>::iterator pos = j;
					j++; is_plused = true;
					i->second.erase(pos);
					break;
				}
			}
			if(is_plused == false)
				j++;
		}
		i++;
	}

	N.clear();
	N = newly;
};

ostream& operator << (ostream &cout_, cfg &a)
{
	set<string>::iterator i;

	i=a.N.begin();
	if(a.N.size()==0)
		cout_<<"N = {}"<<endl;
	else
	{
		cout_<<"N = {"<<(*i);
		i++;
		for(;i!=a.N.end();i++)
			cout_<<","<<*i;
		cout_<<"}"<<endl;
	}

	i=a.T.begin();
	if(a.T.size()==0)
		cout_<<"T = {}"<<endl;
	else
	{
		cout_<<"T = {"<<*i;
		i++;
		for(;i!=a.T.end();i++)
			cout_<<","<<*i;
		cout_<<"}"<<endl;
	}

	cout_<<"S = "<<a.S<<endl;

	cout_<<"P = {"<<endl<<a.P<<"}"<<endl;

	return cout_;
};

void cfg::read_file(const char *filename)
{
	filebuf fb;
	ios_base::openmode m = ios::in;
	if(!fb.open(filename,m))
	{
		cerr<<"Can't open file for reading!"<<endl;
		exit(-1);
	}
	istream cin_(&fb);

	string str;
	my_str temp;
	int num_N=0, num_T=0, num_P=0;

	cin_>>num_N;//non-terminals
	for(int i=0;i<num_N;i++)
	{
		cin_>>str;
		N.insert(str);
	}

	cin_>>num_T;//terminals
	for(int i=0;i<num_T;i++)
	{
		cin_>>str;
		T.insert(str);
	}

	cin_>>str;//axiom
	if(find(N.begin(),N.end(),str)==N.end())
	{
		cerr<<"Axiom is not in set of non-terminals!"<<endl;
		exit(-1);
	}
	S = str;

	cin_>>num_P;//rules
	for(int i=0;i<num_P;i++)
	{
		char c=0;
		string l = "";
		string tstr="";

		cin_>>l;
		str="";
		cin_>>c;
		str+=c;
		cin_>>c;
		str+=c;
		if(str!="->")
		{
			cerr<<"Some rule is incorrect!"<<endl;
			exit(-1);
		}
		str="";tstr="";
		temp.clear();

		getline(cin_,str);
		for(size_t i=0;i<str.size();i++)
		{
			if(str[i]==' '||str[i]=='|')
				continue;
			else
			{
				while(i<str.size()&&str[i]!='|'&&str[i]!=' ')
				{
					tstr+=str[i];
					i++;
					if(N.find(tstr)!=N.end()||
							T.find(tstr)!=T.end()||
							tstr=="e")
					{
						if(tstr=="e")
							tstr = "";
						temp.add(tstr);
						tstr = "";
					}
				}
			}
			P.rules[l].insert(temp);
			tstr = "";
			temp.clear();
		}
	}

	map<left_part, set<right_part> >::iterator it = P.rules.begin();
	for(;it!=P.rules.end();it++)
	{
		set<right_part>::iterator it1 = it->second.begin();
		for(;it1!=it->second.end();it1++)
			P.numer.push_back(make_pair(it->first,it1));
	}

	fb.close();
};

void cfg::read_xml(const char *filename)
{
	TiXmlDocument input;
	if(!input.LoadFile(filename))
	{
		cerr<<input.ErrorDesc()<<endl;
		exit(-1);
	}
	TiXmlElement *root = input.FirstChildElement();
	TiXmlElement *nonterminal = root->FirstChildElement("NS");
	while(nonterminal != NULL)
	{
		N.insert(nonterminal->GetText());
		nonterminal = nonterminal->NextSiblingElement("NS");
	}

	TiXmlElement *terminal = root->FirstChildElement("TS");
	terminal = root->FirstChildElement("TS");
	while(terminal != NULL)
	{
		T.insert(terminal->GetText());
		terminal = terminal->NextSiblingElement("TS");
	}

	if(find(N.begin(),N.end(),"S'")!=N.end()||find(T.begin(),T.end(),"S'")!=T.end())
	{
		cerr<<"Sets of terminals and nonterminals shouldn't have \"S'\" element!"<<endl;
		exit(-1);
	}

	set<string>::iterator first1 = N.begin(),
			last1 = N.end(),
			first2 = T.begin(),
			last2 = T.end();
	while(first1!=last1 && first2!=last2)
	{
		 if(*first1<*first2)
			 ++first1;
		 else if (*first2<*first1)
			 ++first2;
		 else
		 {
			 cerr<<"Sets of terminals and nonterminals have non empty intersection!"<<endl;
			 exit(-1);
		 }
	}

	TiXmlElement *rule = root->FirstChildElement("PR");
	TiXmlElement *left_side;
	TiXmlElement *right_side;
	TiXmlElement *text;
	int num = 1;
	while(rule != NULL)
	{
		string left;
		left_side = rule->FirstChildElement("left");
		text = left_side->FirstChildElement("PRS");
		left = text->GetText();

		if(find(N.begin(),N.end(),left)==N.end())
		{
			cerr<<"Left side of rule #"<<num<<" isn't a nonterminal!"<<endl;
			exit(-1);
		}

		P.rules[left];
		right_side = rule->FirstChildElement("right");
		text = right_side->FirstChildElement("PRS");
		my_str right;
		while(text != NULL)
		{
			string symbol = text->GetText();
			if(find(N.begin(),N.end(),symbol)==N.end()&&find(T.begin(),T.end(),symbol)==T.end())
			{
				cerr<<"Right side of rule #"<<num<<" contains symbol \""<<symbol<<"\" that is not from terminal or nonterminal sets!"<<endl;
				exit(-1);
			}
			right.add(symbol);
			text = text->NextSiblingElement("PRS");
		}
		if(right.size()==0)
			right.add("");
		P.rules[left].insert(right);
		rule = rule->NextSiblingElement("PR");
		num++;
	}

	TiXmlElement *axiom = root->FirstChildElement("SS");
	S = axiom->GetText();
	if(find(N.begin(),N.end(),S)==N.end())
	{
		cerr<<"Axiom is not in set of non-terminals!"<<endl;
		exit(-1);
	}

	map<left_part, set<right_part> >::iterator it = P.rules.begin();
	for(;it!=P.rules.end();it++)
	{
		set<right_part>::iterator it1 = it->second.begin();
		for(;it1!=it->second.end();it1++)
			P.numer.push_back(make_pair(it->first,it1));
	}
}

void cfg::cast()
{
	delete_non_generating();
	delete_non_reachable();

	P.numer.clear();
	map<left_part, set<right_part> >::iterator it = P.rules.begin();
	for(;it!=P.rules.end();it++)
	{
		set<right_part>::iterator it1 = it->second.begin();
		for(;it1!=it->second.end();it1++)
			P.numer.push_back(make_pair(it->first,it1));
	}
};

void cfg::augment()
{
	T.insert("$");
	N.insert("S'");
	my_str temp;
	temp.add(S);
	temp.add("$");
	P.rules["S'"].insert(temp);

	P.numer.clear();
	map<left_part, set<right_part> >::iterator it = P.rules.begin();
	for(;it!=P.rules.end();it++)
	{
		set<right_part>::iterator it1 = it->second.begin();
		for(;it1!=it->second.end();it1++)
			P.numer.push_back(make_pair(it->first,it1));
	}

	S = "S'";
};
