#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "grammar.hpp"
#include "LR.hpp"

using namespace std;

int main(int argc, char**argv)
{
	if(argc != 3 && argc != 4)
	{
		cerr<<"Wrong usage format!"<<endl<<"Syntax should be like this:"<<endl<<"<k> <source> [<dest>]"<<endl;
		exit(-1);
	}
	string source = "", dest = "";
	int k = 1;

	int i=0;
	while(argv[1][i]!='\0')
	{
		if(!isdigit(argv[1][i]))
		{
			cerr<<"k is not a number!"<<endl;
			exit(-1);
		}
		i++;
	}
	k = atoi(argv[1]);
	source = argv[2];
	if(argc == 4)
		dest = argv[3];
	else
		dest = "res.xml";

	cfg grammar;
	LR analyzer;
	//cout<<"Start!"<<endl;
	//grammar.read_file("in.in");
	grammar.read_xml(source.c_str());
	//cout<<"Reading done!"<<endl;
	grammar.augment();
	//cout<<"Augment done!"<<endl;
	grammar.cast();
	//cout<<"Casting done!"<<endl;
	analyzer.build(grammar,k);
	//cout<<"Analyzer built!"<<endl;
	//analyzer.print_first(k);
	//analyzer.print_canonicial(k);
	//analyzer.print_goto(k);
	//analyzer.print_action(k);
	//my_str word = my_str();
	/*word.plus("id");
	word.plus("*");
	word.plus("id");
	word.plus("+");
	word.plus("id");
	word.plus("*");
	word.plus("id");
	word.plus("$");*/
	/*word.plus("a");
	word.plus("b");
	word.plus("a");
	word.plus("b");
	word.plus("$");*/
	/*word.plus("a");
	word.plus("a");
	word.plus("b");
	word.plus("$");*/
	//string res = analyzer.word_analysis(word,grammar,k);
	//cout<<grammar<<endl<<word<<endl<<res;
	analyzer.print_xml(dest.c_str(),grammar,k);
	return 0;
}
