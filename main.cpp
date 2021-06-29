#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
vector<string> pv_pool = {};
vector<string> nv_pool = {};
map<const string, int> var_pool = {{}};

enum ID
{
	NONE,
	UNARY_OPERATOR,
	BINARY_OPERATOR,
	IMPLIES_OPERATOR,
	DEFINE_OPERATOR,
	UNDEFINE_OPERATOR,
};
class block
{
	public:
	ID typ = NONE;
	vector<string> variables;
	vector<string> r_variables;
	bool operator==(const block b)
	{
		if (this->typ == b.typ)
			if(equal(this->variables.begin(),this->variables.end(),b.variables.begin())) return true;
		return false;
	}


} demo {.typ = DEFINE_OPERATOR, .variables = {"a","b", "a"}}, demo2;
vector<class block> predicates;
vector<class block> _predicates_;

void add_simple(string var_name)
{
	struct block test = {.typ = DEFINE_OPERATOR, .variables = {var_name}};
	predicates.push_back(test);
	//var_pool[var_name] = true;
	pv_pool.push_back(var_name);
	return;
}
void add_complex(struct block blk)
{
	predicates.push_back(blk);
	return;
}
auto find_simple(string var_name)
{
	struct block test = {.typ = DEFINE_OPERATOR, .variables = {var_name}};
	auto it = predicates.begin();
	while(it != predicates.end())
	{
		if (*it == test){
			cout << "found variable " << it->variables[0] << endl;
			return it;
		}
		it++;
	}
	return it;
}
void list_rvars(struct block blk)
{
	if (blk.r_variables.size() < 2)
	{
		for (auto i : blk.r_variables)
		{
			cout << i;
		}
	}
	else
	{
		auto i = blk.r_variables.begin();
		while(i != prev(blk.r_variables.end()))
		{
			cout << *i << ",";
			++i;
		}
		cout << *i;
	}
}
void activate_imps()
{
	struct block test = {.typ = IMPLIES_OPERATOR, .variables = pv_pool};
	auto it = predicates.begin();
	while(it != predicates.end())
	{
		if (*it == test){
			cout << "activating satisfied operator";
			for (auto i : it->r_variables) add_simple(i);
			cout << "(";
			list_rvars(*it);
			cout << ")" << endl;
		}
		it++;
	}	
}
void find_inv_simple(string var_name)
{
}

void list_vars(struct block blk)
{
	if (blk.variables.size() < 2)
	{
		for (auto i : blk.variables)
		{
			cout << i;
		}
		cout << endl;
	}
	else
	{
		auto i = blk.variables.begin();
		while(i != prev(blk.variables.end()))
		{
			cout << *i << ",";
			++i;
		}
		cout << *i << endl;
	}
}
void debug(struct block blk)
{
	if (blk.typ == DEFINE_OPERATOR)
	{
		cout << "testing define operator with variables: ";
		list_vars(blk);
		for (auto i : blk.variables)
		{
			if (find_simple(i) == predicates.end())
			{
				cout << "var doesn't exist, definining " << i << "." <<  endl;
				add_simple(i);
			}
			else cout << "var exists, not defining." << endl;
		}
	}
	else if (blk.typ == IMPLIES_OPERATOR)
	{
		cout << "implies operator with variables: ";
		list_vars(blk);
		add_complex(blk);
		int c = 0;		
		for (auto i : blk.variables){
			if (find_simple(i) == predicates.end()) break;
			c++;
		}
		if (c == blk.variables.size())
		{
			cout << "all conditions satisfied, adding rhs to list ";
			for (auto i : blk.r_variables) add_simple(i);
			cout << "(";
			list_rvars(blk);
			cout << ")" << endl;
			struct block test = {.typ = IMPLIES_OPERATOR, .variables = pv_pool};
		}
		
	}
	else if (blk.typ == UNARY_OPERATOR)
	{
		cout << "increment operator with variables: ";
		list_vars(blk);
	}
	else
	{
		cout << "predicate unknown with variables: ";
		list_vars(blk);
	}
}
class block test_series_1[] = {{
.typ = DEFINE_OPERATOR, .variables = {"a"}},{
.typ = DEFINE_OPERATOR, .variables = {"b"}},{
.typ = IMPLIES_OPERATOR, .variables = {"a","b"}, .r_variables = {"c"}},{
.typ = DEFINE_OPERATOR, .variables = {"c"}}};
int main()
{
	for (auto i : test_series_1)
	debug(i);
	activate_imps();
}
