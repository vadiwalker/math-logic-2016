#include <bits/stdc++.h>

using namespace std;

const size_t maxN = 1e6 + 9;

struct operation
{
	virtual string print() = 0;
	virtual ~operation()
	{}
	virtual int get_id() = 0;
	virtual size_t length() = 0;
	unsigned long long hash()
	{
		return get_string_hash(print());
	}
	virtual operation* get_copy(vector <operation*>&) = 0;
	virtual operation* copycopymymy() = 0;

	static const unsigned long long A = 175993;
	static const unsigned long long INF = 1000000007;

	static unsigned long long get_string_hash(string that)
	{
		unsigned long long hash = 0;
		for (size_t i = 0; i < that.size(); i++)
		{
			hash = ((hash * A) % INF + that[i]) % INF;
		}
		return hash;
	}
	static unsigned long long bin_pow(unsigned a, unsigned b)
	{
		if (b == 0)
		{
			return 1;
		}
		unsigned long long res = bin_pow(a, b / 2);
		if (b % 2 == 1)
		{
			return (((res * res) % INF) * a) % INF;
		}
		else
		{
			return (res * res) % INF;
		}
	}
	virtual operation* get_nxt(size_t what) = 0;
};



struct variable;

struct binary_operation: operation
{
	virtual string get_symbol() = 0;
	operation* fst;
	operation* snd;
	binary_operation(operation* _fst, operation* _snd):
		fst(_fst), snd(_snd)
	{}
	virtual string print()
	{
		return "(" + fst->print() + get_symbol() + snd->print() + ")";
	}
	virtual size_t length()
	{
		return 1 + fst->length() + snd->length();
	}
	virtual operation* copycopymymy()
	{
		vector <operation*> for_copy;
		for_copy.push_back(fst->copycopymymy());
		for_copy.push_back(snd->copycopymymy());
		return get_copy(for_copy);
	}
	virtual ~binary_operation()
	{
		delete fst;
		delete snd;
	}
	virtual operation* get_nxt(size_t what)
	{
		if (what == 0)
			return fst;
		if (what == 1)
			return snd;
		return nullptr;
	}
};

struct unary_operation: operation
{
	operation* nxt;
	unary_operation(operation* _nxt):
		nxt(_nxt)
	{}
	virtual size_t length()
	{
		return 1 + nxt->length();
	}
	virtual operation* get_nxt(size_t what)
	{
		if (what == 0)
			return nxt;
		return nullptr;
	}
	virtual operation* copycopymymy()
	{
		vector <operation*> for_copy;
		for_copy.push_back(nxt->copycopymymy());
		return get_copy(for_copy);
	}
};

struct consequence: binary_operation
{
	consequence(operation* _fst, operation* _snd):
		binary_operation(_fst, _snd)
	{}
	string get_symbol()
	{
		return "->";
	}
	size_t length()
	{
		return 2 + fst->length() + snd->length();
	}
	int get_id()
	{
		return 1;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new consequence(ops[0], ops[1]);
	}
};

struct disjunction: binary_operation
{
	disjunction(operation* _fst, operation* _snd):
		binary_operation(_fst, _snd)
	{}
	string get_symbol()
	{
		return "|";
	}
	int get_id()
	{
		return 2;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new disjunction(ops[0], ops[1]);
	}
};

struct conjunction: binary_operation
{
	conjunction(operation* _fst, operation* _snd):
		binary_operation(_fst, _snd)
	{}
	string get_symbol()
	{
		return "&";
	}
	int get_id()
	{
		return 3;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new conjunction(ops[0], ops[1]);
	}
};

struct negation: unary_operation
{
	negation(operation* _nxt):
		unary_operation(_nxt)
	{}
	string print()
	{
		return "!" + nxt->print();
	}
	int get_id()
	{
		return 4;
	}
	/*virtual unsigned long long hash()
	{
		return (('!' * bin_pow(A, nxt->length())) % INF + nxt->hash()) % INF;
	}*/
	operation* get_copy(vector <operation*>& ops)
	{
		return new negation(ops[0]);
	}
};

struct variable: operation
{
	string name;
	variable (const string& _name):
		name(_name)
	{}
	string print()
	{
		return name;
	}
	size_t length()
	{
		return name.size();
	}
	int get_id()
	{
		return 0;
	}
	operation* get_nxt(size_t what)
	{
		return nullptr;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new variable(name);
	}
	operation* copycopymymy()
	{
		return new variable(name);
	}
};

struct substitution: variable
{
	substitution(const string& _name):
		variable(_name)
	{}
	operation* get_copy(vector <operation*>& ops)
	{
		return new substitution(name);
	}
	operation* copycopymymy()
	{
		return new substitution(name);
	}
	int get_id()
	{
		return 13;
	}
};

struct exist: unary_operation
{
	variable* var;
	exist(variable* _var, operation* _nxt):
		unary_operation(_nxt), var(_var)
	{}
	string print()
	{
		return "?" + var->print() + nxt->print();
	}
	int get_id()
	{
		return 5;
	}
	operation* get_nxt(size_t what)
	{
		if (what == 0)
			return var;
		if (what == 1)
			return nxt;
		return nullptr;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		if (ops.size() == 2)
		{
			return new exist(new variable(ops[0]->print()), ops[1]);
		}
		return new exist(var, ops[0]);
	}
};

struct every: unary_operation
{
	variable* var;
	every(variable* _var, operation* _nxt):
		unary_operation(_nxt), var(_var)
	{}
	string print()
	{
		return "@" + var->print() + nxt->print();
	}
	int get_id()
	{
		return 6;
	}
	operation* get_nxt(size_t what)
	{
		if (what == 0)
			return var;
		if (what == 1)
			return nxt;
		return nullptr;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		if (ops.size() == 2)
		{
			return new every(new variable(ops[0]->print()), ops[1]);
		}
		return new every(var, ops[0]);
	}
};

struct summand: binary_operation
{
	summand(operation* _fst, operation* _snd):
		binary_operation(_fst, _snd)
	{}
	string get_symbol()
	{
		return "+";
	}
	int get_id()
	{
		return 7;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new summand(ops[0], ops[1]);
	}
};

struct multiplied: binary_operation
{
	multiplied(operation* _fst, operation* _snd):
		binary_operation(_fst, _snd)
	{}
	string get_symbol()
	{
		return "*";
	}
	int get_id()
	{
		return 8;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new multiplied(ops[0], ops[1]);
	}
};

struct equals: binary_operation
{
	equals(operation* _fst, operation* _snd):
		binary_operation(_fst, _snd)
	{}
	string get_symbol()
	{	
		return "=";
	}
	int get_id()
	{
		return 9;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new equals(ops[0], ops[1]);
	}
};

struct big_null: operation
{
	string print()
	{
		return "0";
	}
	size_t length()
	{
		return 1;
	}
	int get_id()
	{
		return 10;
	}
	operation* get_nxt(size_t what)
	{
		return nullptr;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new big_null();
	}
	operation* copycopymymy()
	{
		return new big_null();
	}
};

struct increment: unary_operation
{
	increment(operation* _nxt):
		unary_operation(_nxt)
	{}
	string print()
	{
		return nxt->print() + "'";
	}
	int get_id()
	{
		return 11;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new increment(ops[0]);
	}
};

struct func_predicate: operation
{
	string name;
	vector <operation*> operations;
	func_predicate(string& _name, vector <operation*>& ops):
		name(_name), operations(ops)
	{}
	virtual string print()
	{
		if (operations.size() == 0)
		{
			return name;
		}
		string now = name + "(";
		for (int i = 0; i < (int)operations.size() - 1; i++)
		{
			now += operations[i]->print() + ",";
		}
		if (operations.size() > 0)
		{
			now += operations[operations.size() - 1]->print();
		}
		now += ")";
		return now;
	}
	size_t length()
	{
		size_t now = name.size() + 2;
		for (size_t i = 0; i < operations.size(); i++)
		{
			now += operations[i]->length();
		}
		return now;
	}
	operation* get_nxt(size_t what)
	{
		if (what < operations.size())
			return operations[what];
		return nullptr;
	}
	operation* copycopymymy()
	{
		vector <operation*> travel;
		for (auto i: operations)
		{
			travel.push_back(i->copycopymymy());
		}
		return get_copy(travel);
	}
};

struct predicate: func_predicate
{
	predicate(string _name, vector <operation*>& that):
		func_predicate(_name, that)
	{}
	int get_id()
	{
		return 11;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new predicate(name, ops);
	}
};

struct func: func_predicate
{
	func(string _name, vector <operation*>& that):
		func_predicate(_name, that)
	{}
	int get_id()
	{
		return 12;
	}
	operation* get_copy(vector <operation*>& ops)
	{
		return new func(name, ops);
	}
};

struct parse_exception
{};

map <size_t, size_t> close_bracket;

size_t nxt = 0;
string to_parse;

operation* get_expression();
operation* get_term();

variable* get_variable()
{
	bool subst = false;
	if (to_parse[nxt] == '_')
	{
		nxt++;
		subst = true;
	}
	string name = "";
	name += to_parse[nxt];
	nxt++;
	for (; nxt < to_parse.size() && '0' <= to_parse[nxt] && to_parse[nxt] <= '9'; nxt++)
	{
		name += to_parse[nxt];
	}
	if (subst)
	{
		return new substitution(name);
	}
	return new variable(name);
}

operation* get_multiplied()
{
	operation* that;
	if (nxt < to_parse.size() && to_parse[nxt] == '0')
	{
		nxt++;
		that = new big_null();
	}
	else if (nxt < to_parse.size() && to_parse[nxt] == '(')
	{
		nxt++;
		that = get_term();
		nxt++;
	}
	else
	{
		string name = "";
		name += to_parse[nxt];
		nxt++;
		for (; nxt < to_parse.size() && '0' <= to_parse[nxt] && to_parse[nxt] <= '9'; nxt++)
		{
			name += to_parse[nxt];
		}
		if (nxt < to_parse.size() && to_parse[nxt] == '(')
		{
			vector <operation*> terms;
			while (to_parse[nxt] != ')')
			{
				nxt++;
				operation* tmp = get_term();
				terms.push_back(tmp);
			}
			nxt++;
			that = new func(name, terms);
		}
		else
		{
			that = new variable(name);
		}
	}
	while (to_parse[nxt] == '\'')
	{	
		that = new increment(that);
		nxt++;
	}
	return that;
}

operation* get_summand()
{
	operation* tmp = get_multiplied();
	while (nxt < to_parse.size() && to_parse[nxt] == '*')
	{
		nxt++;
		operation* snd = get_multiplied();
		tmp = new multiplied(tmp, snd);
	}
	return tmp;
}

operation* get_term()
{
	operation* tmp = get_summand();
	while (nxt < to_parse.size() && to_parse[nxt] == '+')
	{
		nxt++;
		operation* snd = get_summand();
		tmp = new summand(tmp, snd);
	}
	return tmp;
}

operation* get_predicate()
{
	if (nxt < to_parse.size() && 'A' <= to_parse[nxt] && to_parse[nxt] <= 'Z')
	{
		string name = "";
		name += to_parse[nxt];
		nxt++;
		for (; nxt < to_parse.size() && '0' <= to_parse[nxt] && to_parse[nxt] <= '9'; nxt++)
		{
			name += to_parse[nxt];
		}
		vector <operation*> terms;
		if (to_parse[nxt] == '(')
		{
			while (nxt < to_parse.size() && to_parse[nxt] != ')')
			{
				nxt++;
				operation* tmp = get_term();
				terms.push_back(tmp);
			}
			nxt++;
		}
		return new predicate(name, terms);
	}
	operation* tmp1 = get_term();
	nxt++;
	operation* tmp2 = get_term();
	return new equals(tmp1, tmp2);
}

operation* get_unary()
{
	if (nxt < to_parse.size() && to_parse[nxt] == '_')
	{
		nxt++;
		string now = "";
		now += to_parse[nxt];
		nxt++;
		for (; nxt < to_parse.size() && '0' <= to_parse[nxt] && to_parse[nxt] <= '9'; nxt++)
		{
			now += to_parse[nxt];
		}
		return new substitution(now);
	}
	else if (nxt < to_parse.size() && to_parse[nxt] == '!')
	{
		nxt++;
		return new negation(get_unary());
	}
	else if (nxt < to_parse.size() && to_parse[nxt] == '(')
	{
		if (close_bracket[nxt] + 1 == to_parse.size() ||
			to_parse[close_bracket[nxt] + 1] == '-' ||
			to_parse[close_bracket[nxt] + 1] == '|' ||
			to_parse[close_bracket[nxt] + 1] == '&' ||
			to_parse[close_bracket[nxt] + 1] == ')')
		{
			nxt++;
			operation* tmp = get_expression();
			nxt++;
			return tmp;
		}
		else
		{
			return get_predicate();
		}
	}
	else if (nxt < to_parse.size() && to_parse[nxt] == '@')
	{
		nxt++;
		variable* var = get_variable();
		operation* unary = get_unary();
		return new every(var, unary);
	}
	else if (nxt < to_parse.size() && to_parse[nxt] == '?')
	{
		nxt++;
		variable* var = get_variable();
		operation* unary = get_unary();
		return new exist(var, unary);
	}
	else
	{
		return get_predicate();
	}
}

operation* get_conjunction()
{
	operation* tmp = get_unary();
	while (nxt < to_parse.size() && to_parse[nxt] == '&')
	{
		nxt++;
		operation* snd = get_unary();
		tmp = new conjunction(tmp, snd);
	}
	return tmp;
}

operation* get_disjunction()
{
	operation* tmp = get_conjunction();
	while (nxt < to_parse.size() && to_parse[nxt] == '|')
	{
		nxt++;
		operation* snd = get_conjunction();
		tmp = new disjunction(tmp, snd);
	}
	return tmp;
}

operation* get_expression()
{
	operation* tmp = get_disjunction();
	if (nxt + 1 < to_parse.size() && to_parse[nxt] == '-' && to_parse[nxt + 1] == '>')
	{
		nxt += 2;
		return new consequence(tmp, get_expression());
	}
	return tmp;
}

void get_nxt_bracket()
{
	while (nxt < to_parse.size() && (to_parse[nxt] != '(' && to_parse[nxt] != ')'))
	{
		nxt++;
	}
}

void build_nxt()
{
	while (true)
	{
		get_nxt_bracket();
		if (to_parse[nxt] == '(')
		{
			size_t now = nxt;
			nxt++;
			build_nxt();
			close_bracket[now] = nxt;
			nxt++;
		}
		else
		{
			return;
		}
	}
}

operation* get_exp(const string& str)
{
	close_bracket.clear();
	nxt = 0;
	to_parse = str;
	build_nxt();
	nxt = 0;
	return get_expression();
}

operation* get_exp(const string& str, int)
{
	size_t buf = nxt;
	close_bracket.clear();
	nxt = buf;
	build_nxt();
	nxt = buf;
	return get_expression();
}

extern string scheme_axioms[];
extern string axioms[];

enum error_verdict
{
	NOTHING,
	NOT_FREE_TO_SUBSTITUTE,
	FREE_ENTRY,
	FREE_IN_ASSUMPTION
};

error_verdict global_error;

vector <unsigned long long> for_hash;
vector <unsigned long long> for_hash_axiom;
map <unsigned long long, unsigned long long> mp_hash;


set <unsigned long long> assumptions_free_variables;

bool check_on_assumption(operation* for_check, operation* assumption)
{
	return for_check->hash() == assumption->hash();
}

bool dfs(operation* for_check, operation* axiom)
{
	if (axiom->get_id() == 0 || axiom->get_id() == 13)
	{
		for_hash.push_back(for_check->hash());
		for_hash_axiom.push_back(axiom->hash());
	}
	else if (axiom->get_id() != for_check->get_id())
	{
		return false;
	}
	else
	{
		for (size_t i = 0; axiom->get_nxt(i) != nullptr && for_check->get_nxt(i) != nullptr; i++)
		{
			bool res = dfs(for_check->get_nxt(i), axiom->get_nxt(i));
			if (res == false)
				return false;
		}
	}
	return true;
}

bool check_on_coincidence()
{
	for (size_t i = 0; i < for_hash_axiom.size(); i++)
	{
		if (mp_hash.find(for_hash_axiom[i]) == mp_hash.end())
		{
			mp_hash[for_hash_axiom[i]] = for_hash[i];
		}
		else
		{
			if (mp_hash[for_hash_axiom[i]] != for_hash[i])
				return false;
		}
	}
	return true;
}

void clear_everything()
{
	for_hash.clear();
	for_hash_axiom.clear();
	mp_hash.clear();
}

bool check_on_axiom(operation* for_check, operation* axiom)
{
	clear_everything();
	bool res = dfs(for_check, axiom);
	if (!res)
	{
		return false;
	}
	res = check_on_coincidence();
	if (!res)
	{
		return false;
	}
	return true;
}

bool check_on_unfreedom(operation*, vector <operation*>&);

operation* get_something(operation* where, operation* from, unsigned long long that)
{
	if (where->hash() == that)
	{
		return from;
	}
	for (size_t i = 0; where->get_nxt(i) != nullptr; i++)
	{
		operation* res = get_something(where->get_nxt(i), from->get_nxt(i), that);
		if (res != nullptr)
			return res;
	}
	return nullptr;
}

vector <operation*> in_quantifier;
size_t quantifier_pointer;
vector <operation*> res_quantifier;

void dfs3(operation* op, unsigned long long that)
{
	if (op->hash() == that)
	{
		for (; quantifier_pointer < in_quantifier.size(); quantifier_pointer++)
		{
			res_quantifier.push_back(in_quantifier[quantifier_pointer]);
		}
	}
	else if (op->get_id() == 5 || op->get_id() == 6)
	{
		if (op->get_nxt(0)->hash() != that)
		{
			in_quantifier.push_back(op->get_nxt(0));
			dfs3(op->get_nxt(1), that);
			if (quantifier_pointer == in_quantifier.size())
			{
				quantifier_pointer--;
			}
			in_quantifier.pop_back();
		}
	}
	else
	{
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
		{
			dfs3(op->get_nxt(i), that);
		}
	}
}

void take_quantifier_vars(operation* op, unsigned long long that)
{
	in_quantifier.clear();
	quantifier_pointer = 0;
	res_quantifier.clear();
	dfs3(op, that);
}

set <unsigned long long> for_dfs5;

bool dfs5(operation* op)
{
	if (op->get_id() == 0)
	{
		if (for_dfs5.find(op->hash()) != for_dfs5.end())
		{
			return false;
		}
	}
	else
	{
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
		{
			bool res = dfs5(op->get_nxt(i));
			if (!res)
			{
				return false;
			}
		}
	}
	return true;
}

bool check_on_free(operation* op, vector <operation*>& res)
{
	if (res.size() == 0)
	{
		return true;
	}
	for_dfs5.clear();
	for (size_t i = 0; i < res.size(); i++)
	{
		for_dfs5.insert(res[i]->hash());
	}
	return dfs5(op);
}

operation* substitute(operation*, vector <pair <operation*, operation*> >&);

bool check_on_axiom_11_12(operation* for_check, bool which)
{
	if (for_check->get_id() != 1)
	{
		return false;
	}
	else
	{
		operation* left = for_check->get_nxt(0);
		operation* right = for_check->get_nxt(1);
		int id = 6;
		if (which == 1)
		{
			swap(left, right);
			id = 5;
		}
		if (left->get_id() != id)
		{
			return false;
		}
		operation* var = left->get_nxt(0);
		operation* that = left->get_nxt(1);
		clear_everything();
		bool res = dfs(right, that);
		if (!res)
		{
			return false;
		}
		operation* smth = get_something(that, right, var->hash());
		take_quantifier_vars(that, var->hash());
		vector <pair <operation*, operation*> > sub;
		sub.push_back(make_pair(var, smth));
		unique_ptr <operation> check(substitute(that, sub));
		if (check->hash() != right->hash())
		{
			return false;
		}
		if (!check_on_free(smth, res_quantifier))
		{
			global_error = NOT_FREE_TO_SUBSTITUTE;
			return false;
		}
	}
	return true;
}

bool check_on_axiom_a9(operation* op)
{
	if (op->get_id() != 1)
		return false;
	operation* left = op->get_nxt(0);
	operation* right = op->get_nxt(1);
	if (left->get_id() != 3)
		return false;
	operation* left_left = left->get_nxt(0);
	operation* left_right = left->get_nxt(1);
	if (left_right->get_id() != 6)
		return false;
	operation* var = left_right->get_nxt(0);
	operation* tmp = left_right->get_nxt(1);
	if (tmp->get_id() != 1)
		return false;
	operation* left1 = tmp->get_nxt(0);
	operation* left2 = tmp->get_nxt(1);
	if (left1->hash() != right->hash())
		return false;
	vector <pair <operation*, operation*> > sub;
	unique_ptr <operation> null(new big_null());
	unique_ptr <operation> inc(new increment(var));
	sub.push_back(make_pair(var, null.get()));
	if (substitute(right, sub)->hash() != left_left->hash())
		return false;
	sub[0].second = inc.get();
	if (substitute(right, sub)->hash() != left2->hash())
		return false;
	return true;
}

bool check_on_axiom_11(operation* that)
{
	return check_on_axiom_11_12(that, 0);
}

bool check_on_axiom_12(operation* that)
{
	return check_on_axiom_11_12(that, 1);
}

size_t check_all_scheme_axioms(operation* that)
{
	for (size_t i = 0; i < 10; i++)
	{
		if (check_on_axiom(that, get_exp(scheme_axioms[i])))
			return i + 1;
	}
	if (check_on_axiom_11(that))
		return 11;
	if (check_on_axiom_12(that))
		return 12;
	if (check_on_axiom_a9(that))
		return 13;
	return 0;
}

size_t check_all_axioms(operation* that)
{
	for (size_t i = 0; i < 8; i++)
	{
		if (get_exp(axioms[i])->hash() == that->hash())
		{
			return i + 1;
		}
	}
	return 0;
}

pair <operation*, operation*> check_on_modus_ponans(operation* con, operation* op)
{
	operation* left = con->get_nxt(0);
	operation* right = con->get_nxt(1);
	if (left->hash() == op->hash())
		return make_pair(left, right);
	return make_pair <operation*, operation*>(nullptr, nullptr);
}

set <unsigned long long> freedom_hash;
set <unsigned long long> hash_set;

void dfs1(operation* op)
{
	if (op->get_id() == 5 || op->get_id() == 6)
	{
		bool truth = hash_set.find(op->get_nxt(0)->hash()) == hash_set.end();
		if (truth)
		{
			hash_set.insert(op->get_nxt(0)->hash());
		}
		dfs1(op->get_nxt(1));
		if (truth)
		{
			hash_set.erase(op->get_nxt(0)->hash());
		}
	}
	else if (op->get_id() == 0)
	{
		if (hash_set.find(op->hash()) == hash_set.end())
		{
			freedom_hash.insert(op->hash());
		}
	}
	else
	{
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
		{
			dfs1(op->get_nxt(i));
		}
	}
}

bool check_on_unfreedom(operation* for_check, vector <operation*>& var)
{
	freedom_hash.clear();
	hash_set.clear();
	dfs1(for_check);
	
	for (size_t i = 0; i < var.size(); i++)
	{
		if (freedom_hash.find(var[i]->hash()) != freedom_hash.end())
		{
			return false;
		}
	}
	return true;
}

operation* check_on_every(operation* for_check)
{
	if (for_check->get_id() == 1)
	{
		operation* left = for_check->get_nxt(0);
		operation* right = for_check->get_nxt(1);
		if (right->get_id() == 6)
		{
			operation* var = right->get_nxt(0);
			if (assumptions_free_variables.find(var->hash()) != assumptions_free_variables.end())
			{
				global_error = FREE_IN_ASSUMPTION;
				return nullptr;
			}
			operation* nxt = right->get_nxt(1);
			vector <operation*> that;
			that.push_back(var);
			if (check_on_unfreedom(left, that))
			{
				return new consequence(left, nxt);
			}
			else
			{
				global_error = FREE_ENTRY;
			}
		}
	}
	return nullptr;
}

operation* check_on_exist(operation* for_check)
{
	if (for_check->get_id() == 1)
	{
		operation* left = for_check->get_nxt(0);
		operation* right = for_check->get_nxt(1);
		if (left->get_id() == 5)
		{
			operation* var = left->get_nxt(0);
			if (assumptions_free_variables.find(var->hash()) != assumptions_free_variables.end())
			{
				global_error = FREE_IN_ASSUMPTION;
				return nullptr;
			}
			operation* nxt = left->get_nxt(1);
			vector <operation*> that;
			that.push_back(var);
			if (check_on_unfreedom(right, that))
			{
				return new consequence(nxt, right);
			}
			else
			{
				global_error = FREE_ENTRY;
			}
		}
	}
	return nullptr;
}

map <unsigned long long, operation*> sub;

operation* dfs4(operation* op)
{
	if ((op->get_id() == 13 || op->get_id() == 0) && sub.find(op->hash()) != sub.end())
	{
		return sub[op->hash()]->copycopymymy();
	}
	else
	{
		vector <operation*> ops;
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
		{
			ops.push_back(dfs4(op->get_nxt(i)));
		}
		return op->get_copy(ops);
	}
}

operation* substitute(operation* op, vector <pair <operation*, operation*> >& ops)
{
	sub.clear();
	for (size_t i = 0; i < ops.size(); i++)
	{
		sub[ops[i].first->hash()] = ops[i].second;
	}
	return dfs4(op);
}

string scheme_axioms[] = 
{"_a->_b->_a", "(_a->_b)->(_a->_b->_c)->(_a->_c)",
						"_a->_b->_a&_b", "_a&_b->_a", "_a&_b->_b",
						"_a->_a|_b", "_b->_a|_b", "(_a->_c)->(_b->_c)->(_a|_b->_c)",
						"(_a->_b)->(_a->!_b)->!_a", "!!_a->_a"};

string axioms[] = 
{"a=b->a'=b'", "a=b->a=c->b=c", "a'=b'->a=b", "!a'=0", "a+b'=(a+b)'",
				"a+0=a", "a*0=0", "a*b'=a*b+a"};

string theorems_1[] = 
{"_a->_a->_a", "_a->(_a->_a)->_a", "(_a->(_a->_a))->(_a->(_a->_a)->_a)->(_a->_a)",
					"(_a->(_a->_a)->_a)->(_a->_a)", "_a->_a"};

string theorems_2[] = 
{/*"_a->(_b->_c)", "_a->_b", */"(_a->_b)->(_a->(_b->_c))->(_a->_c)",
					"(_a->(_b->_c))->(_a->_c)", "_a->_c"};

enum verdict
{
	MODUS_PONANS,
	EVERY,
	EXIST,
	ASSUMPTION,
	AXIOM_SCHEME,
	AXIOM,
	ERROR
};

string evidence_every[98];
string evidence_exist[106];
pair <verdict, pair <size_t, size_t> > result_every[98];
pair <verdict, pair <size_t, size_t> > result_exist[106];

void delete_objects(vector <pair <operation*, operation*> >& smth)
{
	for (size_t i = 0; i < smth.size(); i++)
	{
		delete smth[i].first;
	}
}

size_t new_position[maxN];

void run_result(vector <pair <verdict, pair <size_t, size_t> > >& result, bool another, size_t ass_number)
{
	size_t length = 98;
	pair <verdict, pair <size_t, size_t> >* pointer = result_every;
	if (another)
	{
		length = 106;
		pointer = result_exist;
	}
	size_t to_add = result.size();
	for (size_t i = 0; i < length; i++)
	{
		if (pointer[i].first == ASSUMPTION)
		{
			result.push_back(result[ass_number]);
		}
		else if (pointer[i].first == MODUS_PONANS)
		{
			result.push_back(make_pair(pointer[i].first, make_pair(pointer[i].second.first + to_add, pointer[i].second.second + to_add)));
		}
		else if (pointer[i].first == EVERY || pointer[i].first == EXIST)
		{
			result.push_back(make_pair(pointer[i].first, make_pair(pointer[i].second.first + to_add, pointer[i].second.second)));
		}
		else
		{
			result.push_back(pointer[i]);
		}
	}
}

void deduction_theorem(vector <operation*>& evidence,
					vector <pair <verdict, pair <size_t, size_t> > >& result,
					vector <operation*>& evidence1,
					vector <pair <verdict, pair <size_t, size_t> > >& result1,
					operation* formula)
{
	//cerr << "Hello!" << endl;
	for (size_t i = 0; i < evidence1.size(); i++)
	{
		delete evidence1[i];
	}
	evidence1.clear();
	result1.clear();
	assert(evidence.size() == result.size());
	for (size_t i = 0; i < evidence.size(); i++)
	{
		if (result[i].first == MODUS_PONANS)
		{
			vector <pair <operation*, operation*> > ops;
			ops.push_back(make_pair(new substitution("a"), formula));
			ops.push_back(make_pair(new substitution("b"), evidence[result[i].second.second]));
			ops.push_back(make_pair(new substitution("c"), evidence[i]));
			for (size_t i = 0; i < 3; i++)
			{
				unique_ptr <operation> res(get_exp(theorems_2[i]));
				evidence1.push_back(substitute(res.get(), ops));
			}
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(1, -1)));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 3, new_position[result[i].second.second])));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 2, new_position[result[i].second.first])));
			delete_objects(ops);
		}
		else if (result[i].first == EVERY)
		{
			run_result(result1, false, new_position[result[i].second.first]);
			vector <pair <operation*, operation*> > ops;
			ops.push_back(make_pair(new substitution("a"), formula));
			ops.push_back(make_pair(new substitution("b"), evidence[i]->get_nxt(0)));
			ops.push_back(make_pair(new variable("d"), evidence[i]->get_nxt(1)->get_nxt(0)));
			ops.push_back(make_pair(new substitution("c"), evidence[i]->get_nxt(1)->get_nxt(1)));
			for (int i = 0; i < 98; i++)
			{
				unique_ptr <operation> op(get_exp(evidence_every[i]));
				evidence1.push_back(substitute(op.get(), ops));
			}
		}
		else if (result[i].first == EXIST)
		{
			run_result(result1, true, new_position[result[i].second.first]);
			//TODO
			vector <pair <operation*, operation*> > ops;
			ops.push_back(make_pair(new substitution("a"), formula));
			ops.push_back(make_pair(new substitution("c"), evidence[i]->get_nxt(1)));
			ops.push_back(make_pair(new variable("d"), evidence[i]->get_nxt(0)->get_nxt(0)));
			ops.push_back(make_pair(new substitution("b"), evidence[i]->get_nxt(0)->get_nxt(1)));
			for (int i = 0; i < 106; i++)
			{
				unique_ptr <operation> op(get_exp(evidence_exist[i]));
				evidence1.push_back(substitute(op.get(), ops));
			}
		}
		else if (result[i].first == ASSUMPTION && evidence[i]->hash() == formula->hash())
		{
			vector <pair <operation*, operation*> > ops;
			ops.push_back(make_pair(new substitution("a"), formula));
			for (size_t i = 0; i < 5; i++)
			{
				unique_ptr <operation> res(get_exp(theorems_1[i]));
				evidence1.push_back(substitute(res.get(), ops));
			}
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(1, -1)));
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(1, -1)));
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(2, -1)));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 3, evidence1.size() - 5)));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 2, evidence1.size() - 4)));
			delete_objects(ops);
		}
		else if (result[i].first == AXIOM_SCHEME || result[i].first == AXIOM || result[i].first == ASSUMPTION)
		{
			
			evidence1.push_back(evidence[i]->copycopymymy());
			unique_ptr <operation> res(get_exp(scheme_axioms[0]));
			vector <pair <operation*, operation*> > ops;
			ops.push_back(make_pair(new substitution("a"), evidence[i]));
			ops.push_back(make_pair(new substitution("b"), formula));
			operation* res1 = substitute(res.get(), ops);
			evidence1.push_back(res1);
			evidence1.push_back(res1->get_nxt(1)->copycopymymy());
			result1.push_back(make_pair(result[i].first, make_pair(result[i].second.first, -1)));
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(1, -1)));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 2, evidence1.size() - 3)));
			delete_objects(ops);
		}
		new_position[i] = evidence1.size() - 1;
	}
}

vector <operation*> evidence;
vector <operation*> evidence1;
vector <operation*> assumption;
vector <pair <verdict, pair <size_t, size_t> > > result;
vector <pair <verdict, pair <size_t, size_t> > > result1;
vector <pair <size_t, size_t> > modus;

vector <operation*> every_evidence;
vector <operation*> exist_evidence;
vector <pair <verdict, pair <size_t, size_t> > > exist_result;


map <unsigned long long, pair <size_t, size_t> > mp_evidence;
map <unsigned long long, pair <size_t, size_t> > wait_to_activate;
map <unsigned long long, size_t> set_evidence;
vector <vector <unsigned long long> > storage;
map <unsigned long long, size_t> mp_assumption;

void check_evidence(vector <operation*>& evidence, vector <operation*>& assumption, vector <pair <verdict, pair <size_t, size_t> > >& result)
{
	mp_evidence.clear();
	wait_to_activate.clear();
	mp_assumption.clear();
	storage.clear();

	for (size_t i = 0; i < assumption.size(); i++)
	{
		mp_assumption[assumption[i]->hash()] = i;
	}
	
	for (size_t i = 0; i < evidence.size(); i++)
	{
		global_error = NOTHING;
		if (mp_assumption.find(evidence[i]->hash()) != mp_assumption.end())
		{
			result.push_back(make_pair(ASSUMPTION, make_pair(mp_assumption[evidence[i]->hash()], -1)));
		}
		else if (check_all_scheme_axioms(evidence[i]))
		{
			result.push_back(make_pair(AXIOM_SCHEME, make_pair(check_all_scheme_axioms(evidence[i]), -1)));
		}
		else if (check_all_axioms(evidence[i]))
		{
			result.push_back(make_pair(AXIOM, make_pair(check_all_axioms(evidence[i]), -1)));
		}
		else if (mp_evidence.find(evidence[i]->hash()) != mp_evidence.end())
		{
			unsigned long long hash = evidence[i]->hash();
			result.push_back(make_pair(MODUS_PONANS, make_pair(mp_evidence[hash].first, mp_evidence[hash].second)));
		}
		else if (check_on_every(evidence[i]) != nullptr && set_evidence.find(check_on_every(evidence[i])->hash()) != set_evidence.end())
		{
			result.push_back(make_pair(EVERY, make_pair(set_evidence[check_on_every(evidence[i])->hash()], -1)));
		}
		else if (check_on_exist(evidence[i]) != nullptr && set_evidence.find(check_on_exist(evidence[i])->hash()) != set_evidence.end())
		{
			result.push_back(make_pair(EXIST, make_pair(set_evidence[check_on_exist(evidence[i])->hash()], -1)));
		}
		else
		{
			result.push_back(make_pair(ERROR, make_pair(global_error, -1)));
		}

		
		if (evidence[i]->get_id() == 1)
		{
			unsigned long long hash = evidence[i]->get_nxt(0)->hash();
			if (set_evidence.find(hash) == set_evidence.end())
			{
				if (wait_to_activate.find(hash) == wait_to_activate.end())
				{
					vector <unsigned long long> st;
					st.push_back(evidence[i]->get_nxt(1)->hash());
					storage.push_back(st);
					wait_to_activate[hash] = make_pair(i, storage.size() - 1);
				}
				else
				{
					pair <size_t, size_t> that = wait_to_activate[hash];
					storage[that.second].push_back(evidence[i]->get_nxt(1)->hash());
				}
			}
			else
			{
				mp_evidence[evidence[i]->get_nxt(1)->hash()] = make_pair(i, set_evidence[hash]);
			}
		}
		unsigned long long hash = evidence[i]->hash();
		if (wait_to_activate.find(hash) != wait_to_activate.end())
		{
			pair <size_t, size_t> which = wait_to_activate[hash];
			for (size_t j = 0; j < storage[which.second].size(); j++)
			{
				mp_evidence[storage[which.second][j]] = make_pair(which.first, i);
			}
		}
		set_evidence[hash] = i;
	}
}

void get_verdict(vector <pair <verdict, pair <size_t, size_t> > >& result)
{
	for (size_t i = 0; i < result.size(); i++)
	{
		switch (result[i].first)
		{
			case MODUS_PONANS:
				cout << "M.P. " << result[i].second.first + 1 << ' ' << result[i].second.second + 1 << endl;
				break;
			case AXIOM:
				cout << "Axiom" << endl;
				break;
			case AXIOM_SCHEME:
				cout << "Axiom scheme " << result[i].second.first << endl;
				break;
			case EVERY:
				cout << "Every " << result[i].second.first + 1 << endl;
				break;
			case EXIST:
				cout << "Exist " << result[i].second.first + 1 << endl;
				break;
			case ERROR:
				cout << "Error" << endl;
				break;
			case ASSUMPTION:
				cout << "Assumption " << result[i].second.first + 1 << endl;
				break;
		}
	}
}

bool check_verdict(vector <pair <verdict, pair <size_t, size_t> > >& result)
{
	for (size_t i = 0; i < result.size(); i++)
	{
		if (result[i].first == ERROR)
		{
			cout << "Вывод некорректен, начиная с формулы " << i + 1 << ' ';
			switch (result[i].second.first)
			{
				case FREE_ENTRY:
					cout << "[Переменная свободно входит в формулу при применении правил для любого и существует]" << endl;
					break;
				case FREE_IN_ASSUMPTION:
					cout << "[Используется правило с квантором по переменной, которая свободно входит в допущение]" << endl;
					break;
				case NOT_FREE_TO_SUBSTITUTE:
					cout << "[Терм не свободен для подстановки вместо переменной]";
					break;
			}
			cout << endl;
			return false;
		}
	}
	return true;
}

void get_every()
{
	ifstream input;
	input.open("for_every");

	for (int i = 0; i < 98; i++)
	{
		input >> evidence_every[i];
	}

	input.close();
	
	ifstream in;
	in.open("comment_for_every");

	for (size_t i = 0; i < 98; i++)
	{
		long long a, b, c;
		in >> a >> b >> c;
		result_every[i] = make_pair((verdict)a, make_pair((size_t)b, (size_t)c));
	}

	in.close();
}

void take_free_variables_from_assumptions()
{
	for (size_t i = 0; i < assumption.size(); i++)
	{
		freedom_hash.clear();
		hash_set.clear();
		dfs1(assumption[i]);

		for (unsigned long long hash: freedom_hash)
		{
			assumptions_free_variables.insert(hash);
		}
	}
}

void get_exist()
{
	ifstream input;
	input.open("for_exist");

	for (int i = 0; i < 106; i++)
	{
		input >> evidence_exist[i];
	}

	input.close();

	ifstream in;
	in.open("comment_for_exist");

	for (size_t i = 0; i < 106; i++)
	{
		long long a, b, c;
		in >> a >> b >> c;
		result_exist[i] = make_pair((verdict)a, make_pair((size_t)b, (size_t)c));
	}

	in.close();
}


int main()
{
	get_every();
	get_exist();

	
	setlocale(LC_ALL, "Russian");


	string s;
	cin >> s;

	string last = "";
	string previous = "";
	int pos = (int)s.size() - 1;
	for (; pos >= 2 && (s[pos - 1] != '-' || s[pos - 2] != '|'); pos--);
	for (int i = 0; i < pos - 2; i++)
	{
		previous += s[i];
	}
	for (size_t i = pos; i < s.size(); i++)
	{
		last += s[i];
	}
	nxt = 0;
	to_parse = previous;
	while (nxt < previous.size())
	{
		operation* op = get_exp(previous, 0);
		assumption.push_back(op);
		if (previous[nxt] == ',')
		{
			nxt++;
		}
	}
	unique_ptr <operation> last_operation(get_exp(last));
	unsigned long long last_hash = last_operation->hash();
	do
	{
		cin >> s;
		evidence.push_back(get_exp(s));
	} while (evidence[evidence.size() - 1]->hash() != last_hash);

	take_free_variables_from_assumptions();



	
	check_evidence(evidence, assumption, result);


	bool what_to_do = check_verdict(result);




	if (what_to_do)
	{
		for (int i = (int)assumption.size() - 1; i >= 0; i--)
		{
			deduction_theorem(evidence, result, evidence1, result1, assumption[i]);
			swap(result, result1);
			swap(evidence, evidence1);
		}
		for (size_t i = 0; i < evidence.size(); i++)
		{
			cout << evidence[i]->print() << endl;
		}
	}


	for (size_t i = 0; i < evidence.size(); i++)
	{
		delete evidence[i];
	}
	for (size_t i = 0; i < assumption.size(); i++)
	{
		delete assumption[i];
	}

	return 0;
}
