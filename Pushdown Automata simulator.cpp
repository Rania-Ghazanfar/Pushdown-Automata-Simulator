#include <iostream>   //code explanation: arr=CFG, a=New production array, n=lines in CFG, count=lines in New productions array.
#include <string>
#include <cstring>
#include <stack>
using namespace std;

void print(string* arr, int n, string a[], int count) {   //printing the cfg and new production
	for (int i = 0; i < n; i++) {  //for cfg
		if (arr[i] != "\0")
			cout << arr[i] << endl;
	}
	for (int i = 2; i < count; i++) {  //for new production
		if (a[i] != "\0")
			cout << "V" << i << "->" << a[i] << endl;
	}
}

bool string_checker(string* arr, int n, char a, string b)  //checking if string exists in the cfg array and appending it if not
{
	for (int i = 0; i < n; i++)
	{
		if (arr[i][0] == a)
		{
			int q = arr[i].size();
			for (int j = 3; j < q; j++)
			{
				string s;
				while (arr[i][j] != '|' && arr[i][j] != '\0')
				{
					s += arr[i][j];
					j++;
				}
				if (strcmp(s.c_str(), b.c_str()) == 0)  //comparing the two strings
				{
					return true;
				}
			}
		}
	}
	return false;
}
void eliminator(string* arr, int n, char a, int barr[])  //elimianting the symbol which is subtituted by null (a is the character to replace)
{
	for (int i = 0; i < n; i++)
	{
		string s;
		int q = arr[i].size();
		int pp = 0;
		int c = 0, ll = 0;
		for (int j = 3; j < q; j++)
		{
			string s, k;
			int count = 0;
			int y = j;
			while (arr[i][j] != '|' && arr[i][j] != '\0') {  //counting how many char a is present in the string
				if (arr[i][j] == a) {
					c++;
				}
				k += arr[i][j];
				j++;
			}
			int ss = 0;
			if (c > 1) {   //if the string has more than one null character
				for (int m = 0; m < c; m++) {   //replacing the letter one by one and appending them to the starting nonterminal symbol
					s = "";
					int count = 0;
					for (int n = 0; n < k.size(); n++) {  //occurence of a in a string
						if (k[n] == a) {
							if (count == ss)
								s += k[n];
							count++;
						}
						else {
							s += k[n];
						}
					}
					if (string_checker(arr, n, arr[i][0], s) == false)  //check if s exits in the starting nonterminal symbol
					{
						arr[i] += '|';
						arr[i] += s;
					}
					ss++;
				}
				s = "";
				for (int n = 0; n < k.size(); n++) {  //appending the string without the null character
					if (k[n] != a) {
						s += k[n];
					}
				}
				if (string_checker(arr, n, arr[i][0], s) == false)  //check if s exits in the starting nonterminal symbol
				{
					arr[i] += '|';
					arr[i] += s;
				}
			}
			else {    // if string has only one null character
				j = y;
				while (arr[i][j] != '|' && arr[i][j] != '\0') {   //extract string that does not contain null character
					if (arr[i][j] != a) {
						s += arr[i][j];
					}
					j++;
				}
			}
			if (s == "" && (string_checker(arr, n, arr[i][0], "?") == false) && barr[i] == 0) {  //if the string did not have a null in it it will append null in it. the barr keeps check for null placements
				arr[i] += "|?";
				barr[i] == 1;
			}
			else if (string_checker(arr, n, arr[i][0], s) == false)
			{

				arr[i] += '|';
				arr[i] += s;
			}
		}
	}
}


void nulleliminate(string* arr, int n)  //to eliminate the null (?) from the string and call eliminator 
{
	int barr[n] = { 0 };   //checking where ? lies in which production
	for (int i = 0; i < n; i++)
	{
		int q = arr[i].size();
		for (int j = 0; j < q; j++)
		{
			if (arr[i][j] == '?')
			{
				barr[i] = 1;
				break;
			}
		}
	}
	for (int i = 0; i < n; i++) //to find where ? lies and removes it from the starting nonterminal symbol
	{
		int q = arr[i].size();
		for (int j = 0; j < q; j++)
		{
			if (arr[i][j] == '?')
			{
				string y;
				int x = 0;
				if (arr[i][j + 1] != '\0') {   //if ? is not in the end of the starting nonterminal symbol
					while (x != (j)) {
						y += arr[i][x];
						x++;
					}
					x = j + 2;
					while (x != q) {
						y += arr[i][x];
						x++;
					}
				}
				else {   //if ? is in the end
					while (x != (j - 1)) {
						y += arr[i][x];
						x++;
					}
				}
				arr[i] = y;
				eliminator(arr, n, arr[i][0], barr);  //calling the eliminator to substitute the character containing null
				break;
			}
		}
	}
}
bool nullchecker(string* arr, int n)   //checks if the cfg contains a ? in all symbols
{
	for (int i = 0; i < n; i++)
	{
		int q = arr[i].size();
		for (int j = 3; j < q; j++)
		{
			string s;
			while (arr[i][j] != '|' && arr[i][j] != '\0')  //extracting the each separate production from the symbols
			{
				s += arr[i][j];
				j++;
			}
			if (s == "?")
				return true;
		}
	}
	return false;
}
void nullremove(string* arr, int n) //recursive call function to remove all the nulls
{
	while (nullchecker(arr, n))
		nulleliminate(arr, n);
}
void unit_replace(string* arr, int n, char a, char b)  //unit_replace works by substiting the unit character by appending all of unit symbol productions to the starting symbol productions and remove the unit symbol inside the starting nonterminal symbol.
{
	string t, d; //for conversion of char to string so that we can easily compare them
	t += b;
	d += a;
	for (int i = 0; i < n; i++)
	{
		if (arr[i][0] == a)  //to access the starting symbol 
		{
			if (a != b) //when A->A which is useless here and goes to a loop so here we will only append the productions whose symbols are not equal (eg: A->B) it will then append all the B productions inside A.
			{
				for (int j = 0; j < n; j++)  //starting next loop to access the unit production
				{
					if (arr[j][0] == b)
					{
						int f = arr[j].size();
						for (int k = 3; k < f; k++)
						{
							string h;
							while (arr[j][k] != '|' && arr[j][k] != '\0')  //extracting the separate productions of unit symbol
							{
								h += arr[j][k];
								k++;
							}
							if (string_checker(arr, n, arr[i][0], h) == false)  //if the production of unit symbol(B) already exists in starting nonterminal symbol (A) then it will not append in A
							{
								if (h != t || h != d)  // to not append unit symbols again
								{
									arr[i] += '|';
									arr[i] += h;
								}
							}
						}

					}
				}
			}  //here it will remove the unit production. works for equal and not equal symbols
			int q = arr[i].size();
			for (int j = 3; j < q; j++)
			{

				if ((arr[i][j - 1] == '|' && arr[i][j] == b && arr[i][j + 1] == '|') || (arr[i][j - 1] == '>' && arr[i][j] == b && arr[i][j + 1] == '|'))  //checking unit placement
				{
					string y;
					int x = 0;
					if (arr[i][j + 1] != '\0')  //if unit is not in the end of the starting nonterminal symbol
					{
						while (x != (j))
						{
							y += arr[i][x];
							x++;
						}
						x = j + 2;
						while (x != q)
						{
							y += arr[i][x];
							x++;
						}
					}
					else  //if unit is in the end
					{
						while (x != (j - 1))
						{
							y += arr[i][x];
							x++;
						}
					}
					arr[i] = y;
					break;
				}
			}

		}
	}
}
void eliminate_unit(string* arr, int n)  //function to find a unit inside the cfg and calling unit_replace.
{
	for (int i = 0; i < n; i++)
	{
		string s;
		int q = arr[i].size();
		for (int j = 3; j < q; j++)
		{
			string s;
			while (arr[i][j] != '|' && arr[i][j] != '\0')  //extracting separate productions
			{
				s += arr[i][j];
				j++;
			}
			if (s.size() == 1 && (s >= "A" && s <= "Z"))  //if string has a unit symbol which is capital then it calls unit_replace to handle it.
			{
				unit_replace(arr, n, arr[i][0], s[0]);  //sends the starting symbol and the unit character to the function to deal with it
			}
		}
	}
}
bool unit_checker(string* arr, int n)  //to check if the cfg still contains a unit production
{
	for (int i = 0; i < n; i++)
	{
		int q = arr[i].size();
		for (int j = 3; j < q; j++)
		{
			string s;
			while (arr[i][j] != '|' && arr[i][j] != '\0')  //extracting separate productions from the starting symbols
			{
				s += arr[i][j];
				j++;
			}
			if (s.size() == 1 && s >= "A" && s <= "Z")
				return true;
		}
	}
	return false;
}
void unit_productions(string* arr, int n)   //recursive function to remove null in the cfg
{
	while (unit_checker(arr, n))
		eliminate_unit(arr, n);
}

bool hascapital(string s) {  //checks if there is a capital letter in the production. checks for terminals.
	int x = 0;
	while (s[x] != '\0') {
		if (s[x] >= 'A' && s[x] <= 'Z') {
			return true;
		}
		x++;
	}
	return false;
}

bool checkterminal(string* arr, int i) {  //checks if the starting symbol has a terminal or not (will be used in useless production)
	int q = arr[i].size();
	for (int j = 3; j < q; j++)
	{
		string s;
		while (arr[i][j] != '|' && arr[i][j] != '\0') {  //extract production
			s += arr[i][j];
			j++;
		}
		if (!hascapital(s)) {  //checks if it is terminal or not
			return true;
		}
	}
	return false;
}

void checknonterminal(string* arr, int n) {  //removes those string which contains all non terminals not present as a starting symbol in cfg
	for (int i = 0; i < n; i++) { // traverses all non terminals
		string m;
		if (arr[i] != "\0") {  //checks if some non terminal has been removed , if so then dont traverse it
			for (int j = 0; j < 3; j++) { // saves the starting(A->) to a new string
				m += arr[i][j];
			}
			int q = arr[i].size();
			for (int j = 3; j < q; j++)// traverses the production of arr[i] non terminal
			{
				bool check = false;
				string s, z;
				while (arr[i][j] != '|' && arr[i][j] != '\0') { // extract string s
					if (arr[i][j] >= 'A' && arr[i][j] <= 'Z') { //only adds the terminals in a production to string z
						z += arr[i][j];
					}
					s += arr[i][j];      // append the production to s
					j++;
				}
				int x = 0, ch = 1;
				int count = 0;
				while (z[x] != '\0') {    //traverses string z
					check = false;
					for (int k = 0; k < n; k++) {
						if (z[x] == arr[k][0] && arr[k] != "\0") { //checks if the non terminals in z is actually present as a starting symbol in cfg
							check = true;                        //if even one is not present we will remove the string s from that arr[i] 
							break;
						}
					}
					x++;
					if (check == false) {
						ch = 0;
					}
				}
				if (ch != 0) {
					//this indicates that all non terminal are actually present as a starting symbol in cfg
					if (arr[i][j] != '\0') {
						m += s;           // now append the s in string m
						m += '|';
					}
					else {
						m += s;
					}
				}
			}
			arr[i] = m;   // place string m(which contains all strings with all non terminals present as a starting symbol in cfg) in arr[i];
		}
	}
}
bool hasnonterminal(string s, char a) { // checks if the string is a non terminal diff then char a
	int x = 0;
	while (s[x] != '\0') {
		if (s[x] >= 'A' && s[x] <= 'Z') {
			if (s[x] != a) {
				return true;
			}
		}
		x++;
	}
	return false;
}
void uslessproduction(string* arr, int n)
{
	bool check = false;
	int index = -1;
	// if a non terminal does not get called in any transition then remove it
	for (int i = 1; i < n; i++) //doesnot get called for starting symbol
	{
		char c = arr[i][0];
		for (int j = 0; j < n; j++) {   //checks the starting symbol in all the productions in cfg
			int q = arr[j].size();
			for (int x = 3; x < q; x++) {
				if (c == arr[j][x]) {
					check = true;
					break;
				}
			}
			if (check == true) {
				break;
			}
		}
		if (check == false) {
			arr[i] = "\0";
		}
		check = false;
	}
	for (int i = 0; i < n; i++)  // removes infinite productions(A->aA) from cfg
	{
		if (arr[i] != "\0" && checkterminal(arr, i) == false) {  //if there is an infinite production and no terminal present in the starting symbol productions then remove the infinite producton
			string l;
			char a = arr[i][0];
			int q = arr[i].size();
			for (int j = 0; j < 3; j++) {         //append the starting(A->)
				l += arr[i][j];
			}
			for (int j = 3; j < q; j++)        // traverse productions of the starting non terminal
			{
				string s;
				while (arr[i][j] != '|' && arr[i][j] != '\0') {   // extract string
					s += arr[i][j];
					j++;
				}
				int x = 0;
				bool c = false;
				while (s[x] != '\0') {                           //checks if string has starting non terminal of this production and
					if (s[x] == a && !hasnonterminal(s, a)) {   //checks if string has non terminal different from starting non terminal of this production , if it does then donot remove this production
						c = true;                       //if it doesn't then remove it by not appending the string in string l
						break;
					}
					x++;
				}
				if (c == false) {
					if (arr[i][j] != '\0') {
						l += s;
						l += '|';
					}
					else {
						l += s;
					}
				}
				c = false;
			}
			if (arr[i][3] == '\0') {        //if no production exist then eliminate the whole arr[i]
				arr[i] = "\0";
			}
			else {
				arr[i] = l;    //append the non useless strings
			}
		}
	}
	checknonterminal(arr, n);
}

bool assigncheck_V(char c, string a[], int count)  //used for conversion of cfg to GNF where small letters are replaced with a new assigned number in the new production array. it only checks if the char passed exists in the new production arr or not like V2->a. a will be checked here. count indicates the production line.
{
	for (int i = 0; i < count; i++)
	{
		int q = a[i].size();
		if (q == 1 && a[i][0] == c)
		{
			return true;
		}
	}
	return false;
}
int assignindex_V(char c, string a[], int count)   //if the terminal already exists in the new production array then it will return the index.
{
	for (int i = 0; i < count; i++)
	{
		int q = a[i].size();
		if (q == 1 && a[i][0] == c)
		{
			return i;
		}
	}
	return -1;
}
//this function can only be used for GNF form representation
void terminal_replacement(string* arr, int n, string* a, int& count)  //replaces the terminals with a new production value so that we can represent it in GNF form that has only a terminal at the start and non-tarminals afterwards in the productions
{
	for (int i = 0; i < n; i++)  //(it changes the whole line with new symbols attached and separated with brackets for eg: A->abA|baB. will convert to A->a(V2)A|b(V3)B,V2->b,V3->a)
	{
		string s;
		int q = arr[i].size();
		for (int j = 0; j < 4; j++)   //copies the starting nonterminal symbol with the arrows
			s += arr[i][j];
		for (int j = 4; j < q; j++)
		{
			if ((arr[i][j] >= 'A' && arr[i][j] <= 'Z'))  //assigns the nonterminals and does not change them
			{
				s += arr[i][j];
			}
			if (arr[i][j - 1] == '|' && ((arr[i][j] >= 'a' && arr[i][j] <= 'z') || (arr[i][j] >= '0' && arr[i][j] <= '1')))   //it assigns the beginning letter in the production after the separators
			{
				s += arr[i][j];
			}
			if (arr[i][j - 1] != '|' && ((arr[i][j] >= 'a' && arr[i][j] <= 'z') || (arr[i][j] >= '0' && arr[i][j] <= '1'))) //if nonterminals in the center or ending of the production it will assing a nonterminal symbol and add the nonterminal symbol to the new production array
			{
				if (!(assigncheck_V(arr[i][j], a, count))) //checks if the nonterminal has been assigned a new nonterminal symbol if it does exist then it will assign to a new production variable
				{
					s += "(V";    //changes a to (V2) count represents number allocation
					string g = to_string(count);
					s += g;
					s += ')';
					a[count] = arr[i][j];
					count++;
				}
				else  //if the new production variable exists then it will return the index count and assign the nonterminal symbol.
				{
					s += "(V";
					int g = assignindex_V(arr[i][j], a, count);
					if (g != -1)
					{
						string w = to_string(g);
						s += w;
						s += ')';
					}
				}

			}
			if (arr[i][j] == '|')
				s += arr[i][j];
		}
		arr[i] = s;   //replacing the cfg line to the changed line for GNF representation
	}
}
/////////////////////////////////////////
bool cfg_gnf_checker(string* arr, int n)    //now comes the conversion of capital letters at the very beginning of the production in the cfg. this function checks if the cfg has been converted to GNF or not
{
	for (int i = 0; i < n; i++)
	{
		int q = arr[i].size();
		for (int j = 3; j < q; j++)
		{
			if ((arr[i][j - 1] == '|' && arr[i][j] >= 'A' && arr[i][j] <= 'Z') || (arr[i][j - 1] == '>' && arr[i][j] >= 'A' && arr[i][j] <= 'Z'))
			{
				return false;
			}
		}
	}
	return true;
}
bool V_gnf_checker(string a[], int count)  //conversion of capital letters at the very beginning of the production in the New production array. this function checks if the new production has been converted to GNF or not
{
	for (int i = 0; i < count; i++)
	{
		int q = a[i].size();
		for (int j = 0; j < q; j++)
		{
			if (j == 0 && a[i][j] >= 'A' && a[i][j] <= 'Z')  //starting criteria
				return false;
			if ((a[i][j - 1] == '|' && a[i][j] >= 'A' && a[i][j] <= 'Z'))
			{
				return false;
			}
		}
	}
	return true;
}
bool V_string_check(string* a, int count, int ind, string b) //checking if string exists in the new produciton array and appending it if not
{
	for (int i = 0; i < count; i++)
	{
		if (i == ind)  //finds the production line to access
		{
			int q = a[i].size();
			for (int j = 0; j < q; j++)
			{
				string s;
				while (a[i][j] != '|' && j < q)  //extracts productions
				{
					s += a[i][j];
					j++;
				}
				if (s.compare(b) == 0) //compare the strings
				{
					return true;
				}
			}
		}
	}
	return false;
}
void substitute(string* arr, int n, string* a, int count, bool ch, char w1, char w2, int f)  //char w is the subtituting char and int f is for new production index. this function will substitute the capital letter in the beginning of the productions by substituting that letter with its own productions 
{
	if (ch)   //cfg_gnf
	{
		for (int i = 0; i < n; i++)
		{
			if (arr[i][0] == w1)  //finds the starting non terminal in cfg
			{
				int q = arr[i].size();
				for (int j = 3; j < q; j++)
				{
					string s;
					if ((arr[i][j - 1] == '|' && arr[i][j] == w2) || (arr[i][j - 1] == '>' && arr[i][j] == w2))  //finding the production with capital letter at beginning
					{
						while (arr[i][j] != '|' && arr[i][j] != '\0')  //extracting the substituting production
						{
							s += arr[i][j];
							j++;
						}
						for (int ii = 0; ii < s.size(); ii++)   //removing the beginning capital letter
						{
							s[ii] = s[ii + 1];
						}
						for (int jj = 0; jj < n; jj++)  //finding the other symbol so that we can replace the capital letter with its own productions.
						{
							if (arr[jj][0] == w2)
							{
								for (int k = 3; k < arr[jj].size(); k++)   //traversing the other starting symbol to acces the productions
								{
									string h;
									while (arr[jj][k] != '|' && arr[jj][k] != '\0')  //extracting substituting production
									{
										h += arr[jj][k];
										k++;
									}
									h += s;   ///appending it with the w1 starting nonterminal production 
									if (string_checker(arr, n, arr[i][0], h) == false)  //checking if the production already exists in the substituting strating nonterminal symbol's productions
									{
										arr[i] += '|';
										arr[i] += h;
									}
								}

							}
						}

					}
				}
				int ind = 0, v = 0;   //next we remove the productions that had a capital letter at the beginning.
				for (int jj = 0; jj < q; jj++)
				{
					if ((arr[i][jj - 1] == '|' && arr[i][jj] == w2) || (arr[i][jj - 1] == '>' && arr[i][jj] == w2))
					{
						string s;
						ind = jj;
						while (arr[i][jj] != '|' && arr[i][jj] != '\0') //extracting the eliminating production
						{
							s += arr[i][jj];
							jj++;
						}
						v = s.size();
						arr[i].erase(ind, v + 1);  //errasing the production
						jj = 0;
					}
				}
			}
		}

	}
	else if (!ch)  //V_Gnf
	{
		for (int i = 0; i < count; i++)
		{
			if (i == f)  //finds the starting non terminal in new production array by using index
			{
				int q = a[i].size();
				for (int j = 0; j < q; j++)
				{
					string s;
					if ((j == 0 && a[i][j] >= 'A' && a[i][j] <= 'Z') || (a[i][j - 1] == '|' && a[i][j] >= 'A' && a[i][j] <= 'Z'))  //finding the production with capital letter at beginning
					{
						while (a[i][j] != '|' && j < q) //extracting the substituting production
						{

							s += a[i][j];
							j++;
						}
						for (int ii = 0; ii < s.size(); ii++)  //removing the beginning capital letter
						{
							s[ii] = s[ii + 1];
						}
						for (int jj = 0; jj < n; jj++) //finding the other symbol so that we can replace the capital letter with its own productions from the cfg array.
						{
							if (arr[jj][0] == w2)  //capital letter production in cfg to substitute in V array.
							{
								for (int k = 3; k < arr[jj].size(); k++)
								{
									string h;
									while (arr[jj][k] != '|' && arr[jj][k] != '\0') //extracting substituting production
									{
										h += arr[jj][k];
										k++;
									}
									h += s;  ///appending it with the starting nonterminal production 
									if (V_string_check(a, count, i, h) == false)   //checking if the production already exists in the substituting strating nonterminal symbol's productions
									{
										a[i] += '|';
										a[i] += h;
									}
								}
							}
						}
					}
				}
				int ind = 0, v = 0;  //next we remove the productions that had a capital letter at the beginning.
				q = a[i].size();
				for (int jj = 0; jj < q; jj++)   //extracting the eliminating string
				{
					if ((a[i][jj - 1] == '|' && a[i][jj] == w2) || (jj == 0 && a[i][jj] == w2))
					{
						string s;
						ind = jj;
						while (a[i][jj] != '|' && jj < q)
						{
							s += a[i][jj];
							jj++;
						}
						v = s.size();
						a[i].erase(ind, v + 1);  //errasing the production
						jj = 0;
					}
				}
			}
		}
	}
}

/*S->ba|cA|SBB
B->ab|cb|a

S->ba|cA|ba(V2)|cAB(V2)
V2->BB(V2)|BB
B->ab|cb|a
*/
void leftrecursionGNF(string* arr, int n, string* a, int& count, char b)  //to remove the left recursion we convert it to right recursion by using a new production so that the staring letter can be a terminal in the productions
{
	for (int i = 0; i < n; i++)
	{
		if (arr[i][0] == b)  //finds the starting non terminal where left recursion exists
		{
			int q = arr[i].size();
			for (int j = 0; j < q; j++)  //extracting all terminal beginning productiongs and appending them with new production variable in the end for right recursion conversion (deals with all small letters and 0,1 languages).
			{
				string s;
				if ((arr[i][j - 1] == '|' && (arr[i][j] >= 'a' && arr[i][j] <= 'z')) || (arr[i][j - 1] == '>' && (arr[i][j] >= 'a' && arr[i][j] <= 'z')))
				{
					while (arr[i][j] != '|' && arr[i][j] != '\0')
					{
						s += arr[i][j];
						j++;
					}
					s += "(V";    //appending the new production value with productions that have the small letter at the begiining in the cfg starting nonterminal symbol's productions
					string g = to_string(count);
					s += g;
					s += ')';
					if (string_checker(arr, n, arr[i][0], s) == false)  //checks if they dont alreday exists
					{
						arr[i] += '|';
						arr[i] += s;
					}
				}
				else if ((arr[i][j - 1] == '|' && (arr[i][j] >= '0' && arr[i][j] <= '1')) || (arr[i][j - 1] == '>' && (arr[i][j] >= '0' && arr[i][j] <= '1')))  //same functionality for 0 and 1 languages
				{
					while (arr[i][j] != '|' && arr[i][j] != '\0')
					{
						s += arr[i][j];
						j++;
					}
					s += "(V";
					string g = to_string(count);
					s += g;
					s += ')';
					if (string_checker(arr, n, arr[i][0], s) == false)
					{
						arr[i] += '|';
						arr[i] += s;
					}
				}
			}
			string g;   //extract the recursive production to make right recursion in the new production array
			int ind = 0, v = 0;
			for (int j = 0; j < q; j++)
			{
				if ((arr[i][j - 1] == '|' && arr[i][j] >= 'A' && arr[i][j] <= 'Z') || (arr[i][j - 1] == '>' && arr[i][j] >= 'A' && arr[i][j] <= 'Z'))
				{
					ind = j;
					while (arr[i][j] != '|' && arr[i][j] != '\0') //extracting th left recursipn production
					{
						g += arr[i][j];
						j++;
					}
					string h = g;
					v = h.size();
					for (int ii = 0; ii < h.size(); ii++)  //removing the starting capital letter
					{
						h[ii] = h[ii + 1];
					}
					a[count] = h;              //appending it to the new production array with right recursion of its own index variable
					a[count] += '|';
					string u = to_string(count);
					h += "(V";
					h += u;
					h += ')';
					a[count] += h;
				}
			}
			arr[i].erase(ind, v + 1);  //errasing the production from the cfg starting non terminal symbol's production
			count++;
		}
	}
}
void first_alphabet_convert(string* arr, int n, string* a, int& count) //deals with CFG to GNF conversion when a production starts with nonterminal. converts it so that the production begins with a terminal.
{
	{
		if (!cfg_gnf_checker(arr, n))  //if the cfg is not converted to gnf.
		{
			for (int i = 0; i < n; i++)
			{
				int q = arr[i].size();
				for (int j = 0; j < q; j++)
				{
					if ((arr[i][j - 1] == '|' && arr[i][j] >= 'A' && arr[i][j] <= 'Z') || (arr[i][j - 1] == '>' && arr[i][j] >= 'A' && arr[i][j] <= 'Z'))  //checks if the production has capital letter in the beginning
					{
						if (arr[i][0] != arr[i][j])   //sees if the starting non terminal symbol is not equal to the starting capital letter in the production(eg: A->Ba). 
						{
							substitute(arr, n, a, count, true, arr[i][0], arr[i][j], 0);   //it will substitue B productions(arr[i][j]) in starting non terminal symbol(arr[i][0])  true if for usage of substitute function for cfg handling 
						}
						else if (arr[i][0] == arr[i][j])   //sees if the starting non terminal symbol is equal to the starting capital letter in the production(eg: A->Aa). 
						{
							leftrecursionGNF(arr, n, a, count, arr[i][0]);   //function to remove left recursion and send the starting non terminal symbol
						}
					}
				}
			}
		}
		if (!V_gnf_checker(a, count))
		{
			for (int i = 0; i < count; i++)
			{
				int q = a[i].size();
				for (int j = 0; j < q; j++)
				{
					if ((a[i][j - 1] == '|' && a[i][j] >= 'A' && a[i][j] <= 'Z') || (j == 0 && a[i][j] >= 'A' && a[i][j] <= 'Z')) //checks if the production has capital letter in the beginning
					{
						substitute(arr, n, a, count, false, '0', a[i][j], i);  //it will substitue B productions(arr[i][j]) in starting non terminal symbol(arr[i][0])  true if for usage of substitute function for new production array handling and i is for the index where the new production starting symbol lies so that it can access its productions 
					}
				}
			}
		}
	}
}
void gnf(string* arr, int n, string* a, int& count)  //function to convert cfg and new production array into gnf by using check functions
{
	while (!V_gnf_checker(a, count) || !cfg_gnf_checker(arr, n))
	{
		first_alphabet_convert(arr, n, a, count);
	}
}

bool pdaSimulator(string* arr, int n, string a[], int count, string appendstring, string usertest, stack<char> st, char b, int r)
{
	int index = 0;
	if (st.top() == '$' && (appendstring.compare(usertest) == 0))  //if stack is empty and given string is found then end the process
	{
		return true;
	}
	if (st.top() != '$' && (appendstring.compare(usertest) != 0 && appendstring.size() == usertest.size()))  //if stack is not empty and given string  is not equal to appended string but size is same so backtrack
	{
		return false;
	}
	if (appendstring.size() > usertest.size() && st.top() != '$')  // if our found string has size greater than given string than backtrack
	{
		return false;
	}
	if (usertest.substr(0, appendstring.length()) != appendstring)  // if our found string is not equal to given string then backtrack
	{
		return false;
	}
	if (b >= 'A' && b <= 'Z') //checking if b(starting symbol) is indeed a non terminal
	{
		for (int i = 0; i < n; i++)
		{
			if (arr[i][0] == b)   //finding the char b as the starting symbol in our cfg
			{
				int q = arr[i].size();
				for (int j = 3; j < q; j++)   //traverse its productions
				{
					string s;
					while (arr[i][j] != '|' && j < q)  //extract one production at atime
					{
						s += arr[i][j];
						j++;
					}
					if (!hascapital(s))  //checks for ending terminals
					{
						stack<char> sta = st;  //initialise a diff stack and copy the main stack to it
						string o;   // initialise a different appending string
						o += appendstring;   //add the prev appending string to it, and then add the top of the stack character it if it is not $(represents empty)
						while (sta.top() != '$' && ((sta.top() >= 'a' && sta.top() <= 'z') || (sta.top() >= '0' && sta.top() <= '1')))
						{
							o += sta.top();
							sta.pop();
						}
						if (sta.top() == '$' && (o.compare(usertest)) == 0)  //it equals to user string then it is found
							return true;
					}
					// Process each character in the production and push it in stack
					for (int k = (s.size() - 1); k >= 0; k--)
					{
						if (s[k] >= 'A' && s[k] <= 'Z' || s[k] >= 'a' && s[k] <= 'z' || s[k] >= '0' && s[k] <= '1')
						{
							st.push(s[k]);
							index++;               //when anything is pushed in the stack index variable increments and decrements when pop hapens
						}                          //helps in backtracking of the stack
						else if (s[k] == ')')      //if it encounters a (Vcount) sybol then only append the (count) into the stack 
						{                           //where (count >=2) and v sybols represent te new productions after cfg to gnf conversion
							char t2; string t;
							st.push(s[k]);
							t2 = st.top();
							t += t2;
							index++;
							while (s[k] != '(') {
								k--;
								if (s[k] != 'V') {

									st.push(s[k]);
									t2 = st.top();
									t += t2;
									index++;

								}
							}
							st.push(s[k]);
							index++;

						}
					}
					int jj = 0;
					// Now, pop characters from the stack and append them to the appendstring until a capital letter or number is encountered
					while (st.top() != '$' && ((st.top() >= 'a' && st.top() <= 'z') || (st.top() >= '0' && st.top() <= '1')))
					{
						jj++;
						appendstring += st.top();
						st.pop();
						index--;
					}
					char w = st.top();     //now place the top of the stack character in w(it is a non terminal )
					if (w == '$' && (appendstring.compare(usertest) == 0))   //if it turns out to be the $(empty stack) and usr string is equal to appendstring it means the given string is found
						return true;
					else if (w == '$' && (appendstring.compare(usertest) != 0))  //if it turns out to be the $(empty stack) and usr string is not equal to appendstring then backtrack
					{
						return false;
					}
					if (w == '(')    //if w is a ( than find the integer number after it which points to the Vcount(count = number found after bracket) non terminal
					{
						st.pop();
						st.pop();
						index--;
						index--;
						string y;
						y += st.top();
						st.pop();
						index--;
						while (st.top() != ')')   //extract the number one digit at a time and add it to the string
						{
							y += st.top();
							st.pop();
							index--;
						}
						int x = stoi(y);
						st.pop();
						index--;
						if (pdaSimulator(arr, n, a, count, appendstring, usertest, st, ' ', x))   //send the x to pda simulator where it point to Vcount(count=x) productions. 
						{
							return true;   //if the function returns a false due to above conditions than backtrack
						}
						for (int k = 0; k < jj; k++)
						{
							appendstring.pop_back();   //string backtrack
						}
						for (int ii = 0; ii < index; ii++)   //stack backtrack
							st.pop();
					}
					else if (w >= 'A' && w <= 'Z')   //if w is a non terminal then send it to pda simulator function again
					{
						st.pop();
						index--;
						if (pdaSimulator(arr, n, a, count, appendstring, usertest, st, w, -1))    //if the function returns a false due to above conditions than backtrack
						{
							return true;
						}
						for (int k = 0; k < jj; k++)
						{
							appendstring.pop_back();   //string backtrack
						}
						for (int ii = 0; ii < index; ii++)
						{
							st.pop();     //stack backtrack
						}
					}
				}
			}
		}
	}
	else if (b == ' ')   //checking if b(starting symbol) is indeed a space for new production array
	{
		for (int i = 0; i < count; i++)
		{
			if (i == r)
			{
				int q = a[i].size();
				for (int j = 0; j < q; j++) //traverse its productions
				{
					string s;
					while (a[i][j] != '|' && j < q)  //extract one production at atime
					{
						s += a[i][j];
						j++;
					}
					if (!hascapital(s))  //checks for ending terminals
					{
						stack<char> sta = st;  //initialise a diff stack and copy the main stack to it
						string o;              // initialise a different appending string
						o += appendstring;       //add the prev appending string to it, and then add the top of the stack character it if it is not $(represents empty)
						while (sta.top() != '$' && ((sta.top() >= 'a' && sta.top() <= 'z') || (sta.top() >= '0' && sta.top() <= '1')))
						{
							o += sta.top();
							sta.pop();
						}
						if (sta.top() == '$' && (o.compare(usertest)) == 0) //it equals to user string then it is found
							return true;
					}
					// Process each character in the production and push it in stack
					for (int k = (s.size() - 1); k >= 0; k--)
					{
						if (s[k] >= 'A' && s[k] <= 'Z' || s[k] >= 'a' && s[k] <= 'z' || s[k] >= '0' && s[k] <= '1')
						{
							st.push(s[k]);
							index++;    //when anything is pushed in the stack index variable increments and decrements when pop hapens
						}                //helps in backtracking of the stack
						else if (s[k] == ')')    //if it encounters a (Vcount) sybol then only append the (count) into the stack 
						{                         //where (count >=2) and v sybols represent te new productions after cfg to gnf conversion
							st.push(s[k]);
							index++;
							while (s[k] != '(') {
								k--;
								if (s[k] != 'V') {
									st.push(s[k]);
									index++;

								}
							}
							st.push(s[k]);
							index++;
						}
					}
					int jj = 0;
					// Now, pop characters from the stack and append them to the appendstring until a capital letter or number is encountered
					while (st.top() != '$' && ((st.top() >= 'a' && st.top() <= 'z') || (st.top() >= '0' && st.top() <= '1')))
					{
						jj++;
						appendstring += st.top();
						st.pop();
						index--;
					}
					char w = st.top();    //now place the top of the stack character in w(it is a non terminal )
					if (w == '$' && (appendstring.compare(usertest) == 0))  //if it turns out to be the $(empty stack) and usr string is equal to appendstring it means the given string is found
						return true;
					else if (w == '$' && (appendstring.compare(usertest) != 0)) //if it turns out to be the $(empty stack) and usr string is not equal to appendstring then backtrack
					{
						return false;
					}
					if (w == '(')   //if w is a ( than find the integer number after it which points to the Vcount(count = number found after bracket
					{
						st.pop();
						index--;
						string y;
						y += st.top();
						st.pop();
						index--;
						while (st.top() != ')') //extract the number one digit at a time and add it to the string
						{
							y += st.top();
							st.pop();
							index--;
						}
						int x = stoi(y);  //turn the number string to an integer
						st.pop();
						index--;
						if (pdaSimulator(arr, n, a, count, appendstring, usertest, st, ' ', x))  //send the x to pda simulator where it point to Vcount(count=x) productions
						{                                                                            
							return true;      //if the function returns a false due to above conditions than backtrack
						}
						for (int k = 0; k < jj; k++)
						{
							appendstring.pop_back();   //string backtrack
						}
						for (int ii = 0; ii < index; ii++)
							st.pop();    //stack backtrack
					}
					else if (w >= 'A' && w <= 'Z')
					{
						char e = w;
						st.pop();
						index--;
						if (pdaSimulator(arr, n, a, count, appendstring, usertest, st, e, -1))  //if the function returns a false due to above conditions than backtrack
						{
							return true;
						}
						for (int k = 0; k < jj; k++)
						{
							appendstring.pop_back();   //string backtrack
						}
						for (int ii = 0; ii < index; ii++)
							st.pop();    //stack backtrack
					}
				}
			}
		}


	}
	return false;
}

void sort(string* arr, int n) {// places all the ending terminals towards the end of the startng symbol
	for (int i = 0; i < n; i++) {
		int q = arr[i].size();
		string y, h;
		for (int j = 0; j < 3; j++) {
			y += arr[i][j];
		}
		for (int j = 3; j < q; j++) {  //traverses the starting symbols productions
			string s, z;
			int l = j;
			while (arr[i][j] != '|' && arr[i][j] != '\0') { //extract one production at a time in a string
				if (arr[i][j] >= 'A' && arr[i][j] <= 'Z') {  //checking if string has a nonterminal ,if so then append in string z
					z += arr[i][j];
				}
				s += arr[i][j];
				j++;
			}
			if (z[0] == '\0') {  //if z=null then string extracted in s is a terminal
				h += s;          //append it to string h
				h += '|';
			}
			else {
				y += s;      //else if string has a nontermianl and so append in y 
				y += '|';
			}
		}
		arr[i] = y;    //y has all productions woth nonterminals and h has productions with all ending terminals only
		arr[i] += h;          //place y in arr[i] and then append h string to it so all ending terminals are in the end
		int x = arr[i].size() - 1;
		while (arr[i][x] == '|') {
			arr[i].pop_back();
			x--;
		}
	}
}

/*
CFG TEST CASES:

S->aSb|bT|a|b
T->aT|a|b

abaab  //accepted
aaaabbb //accepted
b //accepted

S->bB|bA|SBB
A->a
B->sB|b

babssb  //accepted
ab //not accepted
xbb //not accepted

S->A00|B11|00A|A|01
A->1|0|01
B->010A|1100

010111 //accepted
0111 //not accepted

S->A|C|aaA|aaBA|casABk|b
A->C|aaaAx|ABac
C->A|d|b|g

casdbk //not accepted
aaaaaadxx //accepted

S->aSX|Acx
A->a|b|c
X->xc

aacxxc //accepted
adcxxc //not accepted

S->aaBS|aXCD|aSD
A->zxdB|azSD|b

S->aAb|bb
B->aaA|aaB
A->a|B

S->0S1|1T|0|1
T->0T|0|1

01001 //accepted

*/

int main()
{
	int n, count = 2;
	string a[100] = { "" };
	stack<char> st;
	int k;
	cout << "---------------------SIMULATION OF PUSHDOWN AUTOMATA-----------------------" << endl;
	cout << "Enter The Number Of lines in Context Free Grammar" << endl;
	cin >> n;
	string* arr = new string[n];
	cout << "Enter CFG In This Format (S->A|b|?) {where ? = NULL } :" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> arr[i];
	}
	cout<<"Enter number of Test Strings to Check: ";
	cin>>k;
	cout <<endl<< "Enter a Test Strings to Check: "<<endl;
	string *test = new string[k];
	for (int i = 0; i < k; i++)
	{
		cin >> test[i];
	}
	string d;
	st.push('$');
	nullremove(arr, n);
	unit_productions(arr, n);
	uslessproduction(arr, n);
	gnf(arr, n, a, count);
	sort(arr, n);
	cout << endl << "The GNF form of pda is----------" << endl;
	print(arr, n, a, count);
	cout << endl;
	for (int i = 0; i < k; i++)
	{
		bool f = pdaSimulator(arr, n, a, count, d, test[i], st, 'S', -1);
		if (f == true)
		{
			cout << "string accepted" << endl;
		}
		else if (f == false)
		{
			cout << "string not accepted" << endl;
		}
	}
	cout << "-------------------------------SIMULATION DONE-------------------------------" << endl;
}