#include "LR.hpp"
#include <typeinfo>

using namespace std;

void set_of_situation::closure(cfg &grammar, LR &analyzer, int k)//closure of situation set
{
	bool is_modified = true;
	do
	{
		is_modified = false;//is set modified on this step
		set<situation>::size_type size_before = sit.size();

		set<situation>::iterator i = sit.begin();
		for(;i!=sit.end();i++)
		//over all situations A->x.By,a in this set
		{
			if(i->suffix_right_part.size()!=0&&grammar.N.find(i->suffix_right_part[0]) != grammar.N.end())
			//if By isn't empty and B is a nonterminal
			{
				//getting suf = ya from the situation
				my_str suf;
				if(i->suffix_right_part.size()>1)
				{
					my_str temp = i->suffix_right_part;
					suf = temp.substr(1,my_str::npos);
				}
				suf.plus(i->avanschain);

				set<my_str> _first = analyzer.compute_first_for_chain(suf,max(k,1));
				set<my_str>::iterator j = grammar.P.rules[i->suffix_right_part[0]].begin();
				for(;j!=grammar.P.rules[i->suffix_right_part[0]].end();j++)
				//over all rules, which has B in left side
				{
					set<my_str>::iterator p = _first.begin();
					for(;p!=_first.end();p++)
					//over all terminals from First(ya)=First(suf)
					{
						//adding new situation to the set
						my_str in1 = *j;
						situation temp = situation(i->suffix_right_part[0],my_str(""),in1,*p);
						sit.insert(temp);
					}
				}
			}
		}

		set<situation>::size_type size_after = sit.size();
		if(size_after>size_before)//if anything was added, set was modified
			is_modified = true;
	}
	//if set was changed on this step, then we do another one
	while(is_modified);
};

set<my_str> LR::plus_k(set<my_str> &L1, set<my_str> &L2, int k)//result = L1 +k L2
{
	//computation by definition of +k
	set<my_str> result = set<my_str>();
	set<my_str>::iterator i = L1.begin();
	for(;i!=L1.end();i++)
	{
		if(i->size()>=size_t(k)&&*i!=my_str(""))
		{
			my_str temp = *i;
			result.insert(temp.substr(0,k));
		}
		else
		{
			set<my_str>::iterator j = L2.begin();
			for(;j!=L2.end();j++)
			{
				my_str temp = *i;
				temp.plus(*j);
				result.insert(temp.substr(0,k));
			}
		}
	}
	return result;
};

set<my_str> LR::summ_k(my_str rule,int k)
{
	//result = first([X[0]) +k first(X[1]) +k ... +k first(X[n-1])
	my_str::iterator i = rule.begin();
	set<my_str> result = first[*i];
	i++;
	for(;i!=rule.end();i++)
		result = plus_k(result,first[*i],k);
	return result;
};

void LR::compute_first_for_symbols(cfg &grammar, int k)
{
	set<string>::iterator i;

	//init for terminals
	for(i=grammar.T.begin();i!=grammar.T.end();i++)
		first[*i].insert(my_str(*i));
	//end of init for terminals

	//init for nonterminals
	for(i=grammar.N.begin();i!=grammar.N.end();i++)
	//over all nonterminals
	{
		first[*i] = set<my_str>();
		set<my_str>::iterator j = grammar.P.rules[*i].begin();
		for(;j!=grammar.P.rules[*i].end();j++)
		//over all rules for this nonterminal
		{
			my_str temp = *j;
			vector<string>::iterator p = my_find_first_of(temp.begin(),temp.end(),
					grammar.N.begin(),grammar.N.end());//first entry of any nonterminal
			if(p == temp.end() || p - temp.begin() >= k)
			//if there is no nonterminals, or the first of them is righter than k
					first[*i].insert(temp.substr(0,k));
		}
	}
	//end of init for nonterminals

	string eps = "";
	first[eps].insert(my_str(""));

	bool is_modified = true;
	do
	{
		is_modified = false;//is "first" set modified on this step
		map<string,set<my_str> >::iterator j = grammar.P.rules.begin();
		for(;j!=grammar.P.rules.end();j++)
		//over all nonterminals, that are present in left sides of the rules
		{
			set<my_str>::iterator z = j->second.begin();
			for(;z!=j->second.end();z++)
			//over all rules for this nonterminal
			{
				//A->X1X2...Xn
				size_t size_before =first[j->first].size();
				set<my_str> to_insert = summ_k(*z,k);//first(X1) +k first(X2) +k ... +k first(Xn)
				first[j->first].insert(to_insert.begin(),to_insert.end());
				size_t size_after =first[j->first].size();
				if(size_after > size_before)//if anything was added
					is_modified = true;
			}
		}
	}
	//if sets changed on this step, then we do another one
	while(is_modified);
};

set<my_str> LR::compute_first_for_chain(my_str chain, int k)
{
	if(chain == my_str(""))
		return set<my_str>();
	else
		return summ_k(chain,k);//by theorem
};

set_of_situation LR::transition(const set<situation> &I, string x, cfg &grammar, LR &analyzer, int k)
//compute the set of situation where we move from I by x
{
	set_of_situation result = set_of_situation();
	set<situation>::iterator i = I.begin();
	for(;i!=I.end();i++)
	//through all situation in I
	{
		if(i->suffix_right_part != my_str("") && i->suffix_right_part[0] == x)
		//if situation has x right after cursor
		{
			//add this situation with cursor moved one position righter
			my_str prefix = i->prefix_right_part;
			prefix.plus(i->suffix_right_part[0]);
			my_str temp = i->suffix_right_part;
			my_str suffix = temp.substr(1,string::npos);
			result.sit.insert(situation(i->left_part, prefix, suffix, i->avanschain));
		}
	}
	result.closure(grammar, analyzer, k);//make a closure of new set
	return result;
};

void LR::compute_canonicial(cfg &grammar, int k)
//compute canonicial system of sets and GOTO table
{
	//initialization of GOTO table
	set<string>::iterator r = grammar.N.begin();
	for(;r!=grammar.N.end();r++)
		Goto[*r] = vector<int>();
	r = grammar.T.begin();
	for(;r!=grammar.T.end();r++)
		Goto[*r] = vector<int>();

	//grammar should be augment
	my_str S;
	S = (*(grammar.P.rules["S'"].begin()))[0];
	set_of_situation temp = set_of_situation();

	//start from the rule S' -> S$
	temp.sit.insert(situation("S'",my_str(""),S,my_str("$")));
	temp.closure(grammar,*this,k);
	canonicial.push_back(temp);

	bool is_modified = true;
	do
	{
		is_modified = false;//if any set was added on this step
		vector<set_of_situation>::size_type size_before = canonicial.size();

		size_t i = 0;
		for(;i<canonicial.size();i++)
		//over all sets in system
		{
			set<string>::iterator j = grammar.N.begin();
			for(;j!=grammar.N.end();j++)
			//over all nonterminals
			{
				//we add all sets where we can move from current by nonterminal
				//and which is not empty.
				//We compute GOTO table at the same time.
				temp = transition(canonicial[i].sit,*j,grammar,*this,k);
				if(temp.sit.size()!=0)
				{
					//looking for new derived set in already presented sets
					vector<set_of_situation>::iterator pos;
					pos = find(canonicial.begin(),canonicial.end(),temp);

					//if GOTO has less fields than we need we add new
					if(i+1>Goto[*j].size())
					{
						r = grammar.N.begin();
						for(;r!=grammar.N.end();r++)
							Goto[*r].resize(i+1,-1);
					}
					//GOTO from "i" set by char "j" leads to pos set
					Goto[*j][i] = pos - canonicial.begin();

					//if there is no set like new then we add it to our system
					if(pos == canonicial.end())
						canonicial.push_back(temp);
				}
			}

			j = grammar.T.begin();
			for(;j!=grammar.T.end();j++)
			//over all terminals
			{
				//we add all sets where we can move from current by terminal
				//and which is not empty
				//We compute GOTO table at the same time.
				temp = transition(canonicial[i].sit,*j,grammar,*this,k);
				if(temp.sit.size()!=0)
				{
					//looking for new derived set in already presented sets
					vector<set_of_situation>::iterator pos;
					pos = find(canonicial.begin(),canonicial.end(),temp);

					//if GOTO has less fields than we need we add new
					if(i+1>Goto[*j].size())
					{
						r = grammar.T.begin();
						for(;r!=grammar.T.end();r++)
							Goto[*r].resize(i+1,-1);
					}
					//GOTO from "i" set by char "j" leads to pos set
					Goto[*j][i] = pos - canonicial.begin();

					//if there is no set like new then we add it to our system
					if(pos == canonicial.end())
						canonicial.push_back(temp);
				}
			}
		}


		vector<set_of_situation>::size_type size_after = canonicial.size();
		if(size_after>size_before)//some sets were added
			is_modified = true;
	}
	//if canonicial system of sets was changed on this step,then we do another one
	while(is_modified);

	//extend GOTO table to full, filling remaining fields with empty sets
	r = grammar.N.begin();
	for(;r!=grammar.N.end();r++)
		Goto[*r].resize(canonicial.size(),-1);
	r = grammar.T.begin();
	for(;r!=grammar.T.end();r++)
		Goto[*r].resize(canonicial.size(),-1);
};

void LR::compute_action(cfg &grammar, int k)
{
	vector<set_of_situation>::size_type size = canonicial.size();
	if(k==0)
		Action[""] = vector<pair<char,int> >(size,make_pair(char(0),-1));

	vector<set_of_situation>::size_type i = 0;

	my_str S;
	S = (*(grammar.P.rules["S'"].begin()))[0];

	for(;i<canonicial.size();i++)
	//over all sets in canonicial system
	{
		set<situation>::iterator j = canonicial[i].sit.begin();
		for(;j!=canonicial[i].sit.end();j++)
		//over all situations in set
		{
			if(j->suffix_right_part != "" &&
					find(grammar.T.begin(),grammar.T.end(),j->suffix_right_part[0])!=grammar.T.end()&&
					Goto[j->suffix_right_part[0]][i]!=-1)
			//if situation is like [A -> B.C,u] , C is terminal
			//and GOTO[C[0],"i" set] is not empty
			{
				if(k==0)
				{
					if(Action[""][i] != make_pair('S',-1) &&
							Action[""][i] != make_pair(char(0),-1))
					{
						cerr<<"Grammar is not LR("<<k<<")-grammar"<<endl;
						exit(1);
					}
					Action[""][i] = make_pair('S',-1);
					continue;
				}
				//we compute first_k for chain Cu
				my_str str = j->suffix_right_part;
				str.plus(j->avanschain);
				set<my_str> temp = compute_first_for_chain(str,max(k,1));
				set<my_str>::iterator z = temp.begin();
				for(;z!=temp.end();z++)
				//over all strings "z" in first_k(Cu)
				//ACTION[z,"i" set] is shift and go to GOTO[z[0],"i" set] set
				{
					Action[*z];
					if(Action[*z].size()==0)
						Action[*z] = vector<pair<char,int> >(size,make_pair(char(0),-1));
					if(Action[*z][i] != make_pair(char(0),-1)&&
							Action[*z][i] != make_pair('S',Goto[(*z)[0]][i]))
					{
						cerr<<"Grammar is not LR("<<k<<")-grammar"<<endl;
						exit(1);
					}
					Action[*z][i] = make_pair('S',Goto[(*z)[0]][i]);
				}
			}

			else if(j->suffix_right_part == my_str("") &&
					j->left_part != "S'")
			//if situation is like [A -> B.,u] and A != S'
			{
				my_str avans = j->avanschain;
				if(k==0)
					avans = my_str("");
				//ACTION[u, "i" set] is reduce by the rule A -> B
				if(Action[avans].size()==0)
					Action[avans] = vector<pair<char,int> >(size,make_pair(char(0),-1));

				my_str rp = j->prefix_right_part;
				rp.plus(j->suffix_right_part);

				//computing the number of the rule
				int num = -1;
				for(size_t l=0;l!=grammar.P.numer.size();l++)
				{
					if(grammar.P.numer[l].first==j->left_part &&
							*grammar.P.numer[l].second==rp)
					{
						num = l;
						break;
					}
				}

				if(Action[avans][i] != make_pair(char(0),-1)&&
						Action[avans][i] != make_pair('R',num))
				{
					cerr<<"Grammar is not LR("<<k<<")-grammar"<<endl;
					exit(1);
				}
				Action[avans][i] = make_pair('R',num);
			}

			else if(*j == situation("S'",S,my_str(""),my_str("$")))
			//if situation is like [S' -> S.,$]
			{
				//ACTION[$,"i" set] is ACCEPT
				my_str symb;
				if(k==0)
					symb = my_str("");
				else
					symb = my_str("$");
				Action[symb];
				if(Action[symb].size()==0)
					Action[symb] = vector<pair<char,int> >(size,make_pair(char(0),-1));

				if(Action[symb][i] != make_pair(char(0),-1)&&
						Action[symb][i] != make_pair('A',0))
				{
					cerr<<"Grammar is not LR("<<k<<")-grammar"<<endl;
					exit(1);
				}

				Action[symb][i] = make_pair('A',0);
			}
			//in all other situation result is ERROR = -1
		}
	}
};

vector<string>::iterator LR::my_find_first_of(vector<string>::iterator first1,
			vector<string>::iterator last1,
			set<string>::iterator first2,
			set<string>::iterator last2)
{
	vector<string>::iterator i = first1;
	for(;i!=last1;i++)
	{
		set<string>::iterator j = first2;
		for(;j!=last2;j++)
			if(*i==*j)
				return i;
	}
	return last1;
};

void LR::build(cfg &grammar, int k)
{
	compute_first_for_symbols(grammar,k);
	compute_canonicial(grammar,k);
	compute_action(grammar,k);
};

int strtoint(string a)
{
	int res=a[0]-'0';
	for(size_t i=1;i<a.size();i++)
	{
		res *= 10;
		res += a[i]-'0';
	}
	return res;
};

string inttostr(int a)
{
	if(a==0)
		return "0";
	string res = "";
	while(a!=0)
	{
		res += a%10 + '0';
		a /= 10;
	}
	reverse(res.begin(),res.end());
	return res;
};

string LR::word_analysis(my_str word, cfg &grammar, int k)
{
	string derivation = "";
	list<string> stack = list<string>();
	stack.push_back("0");
	my_str preview;
	if(k==0)
		preview = my_str("");
	else
		preview = word.substr(0,k);
	int i=0;
	pair<char, int> act = pair<char, int>();
	while(1)
	{
		act = Action[preview][strtoint(stack.back())];

		if(act.first == char(0))
		{
			return "-1";
		}

		else if(act.first == 'A')
		{
			break;
		}

		else if(act.first == 'S')
		{
			string symb,last;
			if(k!=0)
				symb = preview[0];
			else
				symb = word.substr(i,1)[0];
			last = stack.back();
			stack.push_back(symb);
			if(k!=0)
				stack.push_back(inttostr(act.second));
			else
				stack.push_back(inttostr(Goto[symb][strtoint(last)]));
			i++;
			if(k!=0)
				preview = word.substr(i,k);
		}

		else
		{
			int len = 0;
			if(*grammar.P.numer[act.second].second != my_str(""))
				len = grammar.P.numer[act.second].second->size();
			stack.resize(stack.size()-2*len);
			string last = stack.back();
			stack.push_back(grammar.P.numer[act.second].first);
			stack.push_back(inttostr(Goto[stack.back()][strtoint(last)]));
			derivation += inttostr(act.second);
		}
	}
	reverse(derivation.begin(),derivation.end());
	return derivation;
};

void LR::print_xml(const char* filename, cfg &grammar, int k)
{
	TiXmlDocument output;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "Windows-1251", "" );
	output.LinkEndChild(decl);

	TiXmlStyle *style = new TiXmlStyle("text/xsl","lrk.xsl");
	output.LinkEndChild(style);

	TiXmlElement *lrk = new TiXmlElement("lrk");
	lrk->SetAttribute("xmlns","http://umeta.ru/namespaces/trpl");
	lrk->SetAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
	lrk->SetAttribute("xsi:schemaLocation","http://umeta.ru/namespaces/trpl lrk.xsd");
	output.LinkEndChild(lrk);

	TiXmlElement *gr = new TiXmlElement("grammar");
	lrk->LinkEndChild(gr);


	set<string>::iterator it1 = grammar.N.begin();
	TiXmlElement *nonterminal;
	for(;it1!=grammar.N.end();it1++)
	{
		nonterminal = new TiXmlElement("NS");
		nonterminal->LinkEndChild(new TiXmlText(it1->c_str()));
		gr->LinkEndChild(nonterminal);
	}

	it1 = grammar.T.begin();
	TiXmlElement *terminal;
	for(;it1!=grammar.T.end();it1++)
	{
		terminal = new TiXmlElement("TS");
		terminal->LinkEndChild(new TiXmlText(it1->c_str()));
		gr->LinkEndChild(terminal);
	}

	TiXmlElement *rul;
	for(size_t i=0;i!=grammar.P.numer.size();i++)
	{
		rul = new TiXmlElement("PR");
		rul->SetAttribute("order",i+1);
		TiXmlElement *left = new TiXmlElement("left");
		TiXmlElement *prs = new TiXmlElement("PRS");
		prs->SetAttribute("order",1);
		prs->LinkEndChild(new TiXmlText(grammar.P.numer[i].first.c_str()));
		left->LinkEndChild(prs);
		rul->LinkEndChild(left);

		TiXmlElement *right = new TiXmlElement("right");
		for(my_str::size_type j=0;j!=grammar.P.numer[i].second->size();j++)
		{
			prs = new TiXmlElement("PRS");
			prs->SetAttribute("order",j+1);
			prs->LinkEndChild(new TiXmlText((*(grammar.P.numer[i].second))[j].c_str()));
			right->LinkEndChild(prs);
		}
		rul->LinkEndChild(right);

		gr->LinkEndChild(rul);
	}

	TiXmlElement *s = new TiXmlElement("SS");
	s->LinkEndChild(new TiXmlText(grammar.S.c_str()));
	gr->LinkEndChild(s);

	TiXmlElement *anal = new TiXmlElement("analyzer");
	anal->SetAttribute("k",k);
	anal->SetAttribute("startState","I0");
	lrk->LinkEndChild(anal);

	size_t size = canonicial.size();

	for(size_t i=0;i!=size;i++)
	{
		TiXmlElement *state = new TiXmlElement("state");
		string name = "I" + inttostr(i);
		state->SetAttribute("name",name.c_str());
		map<my_str, vector<pair<char,int> > >::iterator j = Action.begin();

		for(;j!=Action.end();j++)
		{
			if(j->second[i].first==char(0))
				continue;
			TiXmlElement *act = new TiXmlElement("action");
			if(j->second[i].first=='S')
			{
				act->SetAttribute("value","shift");
				TiXmlElement *str = new TiXmlElement("string");
				for(my_str::size_type z = 0;z != j->first.size();z++)
				{
					TiXmlElement *symb = new TiXmlElement("symbol");
					symb->SetAttribute("order",z+1);
					symb->LinkEndChild(new TiXmlText(j->first[z].c_str()));
					str->LinkEndChild(symb);
				}
				act->LinkEndChild(str);
			}
			else if(j->second[i].first=='R')
			{
				act->SetAttribute("value","reduce");
				TiXmlElement *str = new TiXmlElement("string");
				for(my_str::size_type z = 0;z != j->first.size();z++)
				{
					TiXmlElement *symb = new TiXmlElement("symbol");
					symb->SetAttribute("order",z+1);
					symb->LinkEndChild(new TiXmlText(j->first[z].c_str()));
					str->LinkEndChild(symb);
				}
				act->LinkEndChild(str);
				TiXmlElement *ru = new TiXmlElement("rule");
				ru->LinkEndChild(new TiXmlText(inttostr(j->second[i].second + 1).c_str()));
				act->LinkEndChild(ru);
			}
			else
			{
				act->SetAttribute("value","accept");
				TiXmlElement *str = new TiXmlElement("string");
				for(my_str::size_type z = 0;z != j->first.size();z++)
				{
					TiXmlElement *symb = new TiXmlElement("symbol");
					symb->SetAttribute("order",z+1);
					symb->LinkEndChild(new TiXmlText(j->first[z].c_str()));
					str->LinkEndChild(symb);
				}
				act->LinkEndChild(str);
			}

			state->LinkEndChild(act);
		}

		map<string, vector<int> >::iterator t = Goto.begin();

		for(;t!=Goto.end();t++)
		{
			if(t->second[i] == -1)
				continue;

			TiXmlElement *go = new TiXmlElement("goto");

			TiXmlElement *symb = new TiXmlElement("symbol");
			symb->LinkEndChild(new TiXmlText(t->first.c_str()));
			go->LinkEndChild(symb);

			TiXmlElement *val = new TiXmlElement("value");
			string num = "I" + inttostr(t->second[i]);
			val->LinkEndChild(new TiXmlText(num.c_str()));
			go->LinkEndChild(val);

			state->LinkEndChild(go);
		}

		anal->LinkEndChild(state);
	}

	if(!output.SaveFile(filename))
	{
		cerr<<"While saving to \"dest\" file an error was occured:"<<endl<<output.ErrorDesc()<<endl;
		if(!output.SaveFile("res.xml"))
		{
			cerr<<"While saving to \"res.xml\" file an error was occured:"<<endl<<output.ErrorDesc()<<endl;
			cerr<<"Unable to save the xml document!"<<endl;
			exit(-1);
		}
	}
};

void LR::print_first(int k)
{
	LR::print_first(cout,k);
};

void LR::print_first(ostream& cout_,int k)
{
	if(k==0)
	{
		cout<<"What is First_0???"<<endl;
		return;
	}
	my_str empty;
	empty.add("");
	map<string,set<my_str> >::iterator i=first.begin();
	for(;i!=first.end();i++)
	{
		cout_<<"First("<<i->first<<") = {";
		set<my_str>::iterator j = i->second.begin();
		if(j!=i->second.end())
		{
			cout_<<*j;
			j++;
			for(;j!=i->second.end();j++)
				cout_<<","<<*j;
		}
		cout_<<"}"<<endl;
	}
};

void LR::print_canonicial(int k)
{
	vector<set_of_situation>::iterator i = canonicial.begin();
	for(;i!=canonicial.end();i++)
	{
		set<situation>::iterator j = (i->sit).begin();
		for(;j!=(i->sit).end();j++)
		{
			cout<<j->left_part<<" -> ";
			cout<<j->prefix_right_part;
			cout<<".";
			cout<<j->suffix_right_part;
			if(k==0)
			{
				if(j->left_part == "S'")
					cout<<","<<"$"<<endl;
				else
					cout<<","<<endl;
			}
			else
				cout<<","<<j->avanschain<<endl;
		}
		cout<<endl;
	}
};

void LR::print_goto(int k)
{
	map<string, vector<int> >::iterator i = Goto.begin();
	cout<<" ";
	for(size_t j=0;j!=canonicial.size();j++)
		cout<<"\t"<<j;
	cout<<endl;
	for(;i!=Goto.end();i++)
	{
		cout<<i->first;
		for(size_t j=0;j!=canonicial.size();j++)
			cout<<"\t"<<i->second[j];
		cout<<endl;
	}
};

void LR::print_action(int k)
{
	cout<<endl<<endl;

	map<my_str, vector<pair<char,int> > >::iterator i = Action.begin();
	cout<<" ";
	for(size_t j=0;j!=canonicial.size();j++)
		cout<<"\t"<<j;
	cout<<endl;
	for(;i!=Action.end();i++)
	{
		cout<<i->first;
		for(size_t j=0;j!=canonicial.size();j++)
		{
			cout<<"\t";
			if(i->second[j].first == 0)
				cout<<"";
			else
				cout<<i->second[j].first;
			cout<<i->second[j].second;
		}
		cout<<endl;
	}
};
