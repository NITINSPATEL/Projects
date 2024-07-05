#include <bits/stdc++.h>

using namespace std;

template<typename T>
class Graph {
private:
    int V;  // Number of vertices
    unordered_map<T, vector<pair<T, int>>> adj;  // Adjacency list
    bool isDirected;

public:
    // Constructor
    Graph(int V, bool isDirected) : V(V), isDirected(isDirected) {}

    // Function to add an edge
    void addEdge(T u, T v, int w) {
        adj[u].emplace_back(v, w);
        if (!isDirected) {
            adj[v].emplace_back(u, w);
        }
    }

    // BFS
    T bfs(T start, T target) {
        unordered_map<T, bool> visited;
        queue<T> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            T node = q.front();
            q.pop();
            if (node == target) return node;

            for (const auto& neighbor : adj[node]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        return T();  // Node not found, returning default-constructed value
    }

    // DFS
    T dfs(T start, T target) {
        unordered_map<T, bool> visited;
        stack<T> s;
        s.push(start);

        while (!s.empty()) {
            T node = s.top();
            s.pop();
            if (node == target) return node;

            if (!visited[node]) {
                visited[node] = true;
                for (const auto& neighbor : adj[node]) {
                    if (!visited[neighbor.first]) {
                        s.push(neighbor.first);
                    }
                }
            }
        }
        return T();  // Node not found, returning default-constructed value
    }

    // Bellman-Ford for shortest path handling negative edges
    int shortestPath(T start, T end) {
        unordered_map<T, int> dist;
        for (const auto& kv : adj) {
            dist[kv.first] = numeric_limits<int>::max();
        }
        dist[start] = 0;

        for (int i = 1; i <= V - 1; ++i) {
            for (const auto& kv : adj) {
                T u = kv.first;
                for (const auto& edge : kv.second) {
                    T v = edge.first;
                    int weight = edge.second;
                    if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                    }
                }
            }
        }

        // Check for negative weight cycles
        for (const auto& kv : adj) {
            T u = kv.first;
            for (const auto& edge : kv.second) {
                T v = edge.first;
                int weight = edge.second;
                if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                    // Formulate the exception message with the nodes involved in the cycle
                    string message = "Negative weight cycle found: ";
                    throw runtime_error(message);
                }
            }
        }

        return dist[end] == numeric_limits<int>::max() ? -1 : dist[end];
    }

    // Cycle detection in directed graph using DFS
    bool cycleUtilDirected(T v, unordered_map<T, bool>& visited, unordered_map<T, bool>& recStack) {
        if (!visited[v]) {
            visited[v] = true;
            recStack[v] = true;

            for (const auto& neighbor : adj[v]) {
                if (!visited[neighbor.first] && cycleUtilDirected(neighbor.first, visited, recStack))
                    return true;
                else if (recStack[neighbor.first])
                    return true;
            }
        }
        recStack[v] = false;
        return false;
    }

    bool isCyclicDirected() {
        unordered_map<T, bool> visited;
        unordered_map<T, bool> recStack;

        for (const auto& kv : adj)
            if (cycleUtilDirected(kv.first, visited, recStack))
                return true;

        return false;
    }

    // Cycle detection in undirected graph using DFS
    bool cycleUtilUndirected(T v, unordered_map<T, bool>& visited, T parent) {
        visited[v] = true;

        for (const auto& neighbor : adj[v]) {
            if (!visited[neighbor.first]) {
                if (cycleUtilUndirected(neighbor.first, visited, v))
                    return true;
            }
            else if (neighbor.first != parent)
                return true;
        }
        return false;
    }

    bool isCyclicUndirected() {
        unordered_map<T, bool> visited;

        for (const auto& kv : adj)
            if (!visited[kv.first] && cycleUtilUndirected(kv.first, visited, T()))
                return true;

        return false;
    }

    // Cycle detection wrapper
    bool isCyclic() {
        if (isDirected) {
            return isCyclicDirected();
        } else {
            return isCyclicUndirected();
        }
    }
};


class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
public:
    Trie() : root(new TrieNode()) {}

    void insert(const std::string &word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndOfWord = true;
    }

    bool search(const std::string &word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                return false;
            }
            node = node->children[c];
        }
        return node->isEndOfWord;
    }

private:
    TrieNode* root;
};

class KMP {
public:
    std::vector<int> computePrefixFunction(const std::string &pattern) {
        int m = pattern.size();
        std::vector<int> prefix(m, 0);
        int k = 0;
        for (int q = 1; q < m; ++q) {
            while (k > 0 && pattern[k] != pattern[q]) {
                k = prefix[k - 1];
            }
            if (pattern[k] == pattern[q]) {
                ++k;
            }
            prefix[q] = k;
        }
        return prefix;
    }

    std::vector<int> findOccurrences(const std::string &text, const std::string &pattern) {
        int n = text.size();
        int m = pattern.size();
        std::vector<int> prefix = computePrefixFunction(pattern);
        std::vector<int> result;
        int q = 0;
        for (int i = 0; i < n; ++i) {
            while (q > 0 && pattern[q] != text[i]) {
                q = prefix[q - 1];
            }
            if (pattern[q] == text[i]) {
                ++q;
            }
            if (q == m) {
                result.push_back(i - m + 1);
                q = prefix[q - 1];
            }
        }
        return result;
    }
};


// BY DEFAULT , class BigInt number is initialized with 0 , digits.size()==1 and digits[0]=0 , can do arithmetic operations with normal numbers as well
class BigInt{
	string digits;    // digits contain the big number in reverse order , e.g. 124 will stored as 421 in digits string variable 
public:

	//Constructors:
	BigInt(unsigned long long n = 0);
	BigInt(string &);
	BigInt(const char *);
	BigInt(BigInt &);

	//Helper Functions:
	friend void divide_by_2(BigInt &a);
	friend bool Null(const BigInt &);
	friend int Length(const BigInt &);
	int operator[](const int)const;

			/* * * * Operator Overloading * * * */

	//Direct assignment
	BigInt &operator=(const BigInt &);

	//Post/Pre - Incrementation
	BigInt &operator++();
	BigInt operator++(int temp);
	BigInt &operator--();
	BigInt operator--(int temp);

	//Addition and Subtraction
	friend BigInt &operator+=(BigInt &, const BigInt &);
	friend BigInt operator+(const BigInt &, const BigInt &);
	friend BigInt operator-(const BigInt &, const BigInt &);
	friend BigInt &operator-=(BigInt &, const BigInt &);

	//Comparison operators
	friend bool operator==(const BigInt &, const BigInt &);
	friend bool operator!=(const BigInt &, const BigInt &);

	friend bool operator>(const BigInt &, const BigInt &);
	friend bool operator>=(const BigInt &, const BigInt &);
	friend bool operator<(const BigInt &, const BigInt &);
	friend bool operator<=(const BigInt &, const BigInt &);

	//Multiplication and Division
	friend BigInt &operator*=(BigInt &, const BigInt &);
	friend BigInt operator*(const BigInt &, const BigInt &);
	friend BigInt &operator/=(BigInt &, const BigInt &);
	friend BigInt operator/(const BigInt &, const BigInt &);

	//Modulo
	friend BigInt operator%(const BigInt &, const BigInt &);
	friend BigInt &operator%=(BigInt &, const BigInt &);

	//Power Function
	friend BigInt &operator^=(BigInt &,const BigInt &);
	friend BigInt operator^(BigInt &, const BigInt &);
	
	//Square Root Function
	friend BigInt sqrt(BigInt &a);

	//Read and Write
	friend ostream &operator<<(ostream &,const BigInt &);
	friend istream &operator>>(istream &, BigInt &);

	//Others
	friend BigInt NthCatalan(int n);
	friend BigInt NthFibonacci(int n);
	friend BigInt Factorial(int n);
};

// storing reverse of number in digits member variable of BigInt Class
BigInt::BigInt(string & s){
	digits = "";
	int n = s.size();
	for (int i = n - 1; i >= 0;i--){
		if(!isdigit(s[i]))
			throw("ERROR");
		digits.push_back(s[i] - '0');
	}
}
BigInt::BigInt(unsigned long long nr){
	do{
		digits.push_back(nr % 10);
		nr /= 10;
	} while (nr);
}
BigInt::BigInt(const char *s){
	digits = "";
	for (int i = strlen(s) - 1; i >= 0;i--)
	{
		if(!isdigit(s[i]))
			throw("ERROR");
		digits.push_back(s[i] - '0');
	}
}
BigInt::BigInt(BigInt & a){
	digits = a.digits;
}

// returns true iff digits=="0"
bool Null(const BigInt& a){
	if(a.digits.size() == 1 && a.digits[0] == 0)
		return true;
	return false;
}

// returns size of string 'digits'
int Length(const BigInt & a){
	return a.digits.size();
}

	/* * * * Operator Overloading * * * */

// returns digits[index]
int BigInt::operator[](const int index)const{
	if(digits.size() <= index || index < 0)
		throw("ERROR");
	return digits[index];
}

// returns true IFF bigint type variables/numbers  a and b are EQUAL
bool operator==(const BigInt &a, const BigInt &b){
	return a.digits == b.digits;
}

// returns true IFF bigint type variables/numbers  a and b are NOT EQUAL
bool operator!=(const BigInt & a,const BigInt &b){
	return !(a == b);
}

// returns true IFF first bigint number 'a' is strictly less than number 'b'
bool operator<(const BigInt&a,const BigInt&b){
	int n = Length(a), m = Length(b);
	if(n != m)
		return n < m;
	while(n--)
		if(a.digits[n] != b.digits[n])
			return a.digits[n] < b.digits[n];
	return false;
}

// returns true IFF first bigint number 'a' is strictly greater than number 'b'
bool operator>(const BigInt&a,const BigInt&b){
	return b < a;
}

// returns true IFF first bigint number 'a' is  greater than or equal to number 'b'
bool operator>=(const BigInt&a,const BigInt&b){
	return !(a < b);
}

// returns true IFF first bigint number 'a' is  less than or equal to number 'b'
bool operator<=(const BigInt&a,const BigInt&b){
	return !(a > b);
}

// assigns current class variable to bigint 'a' , usage x=a , x gets assigned with digits of 'a'
BigInt &BigInt::operator=(const BigInt &a){
	digits = a.digits;
	return *this;
}

// increments the current bigint number 
BigInt &BigInt::operator++(){
	int i, n = digits.size();
	for (i = 0; i < n && digits[i] == 9;i++)
		digits[i] = 0;
	if(i == n)
		digits.push_back(1);
	else
		digits[i]++;
	return *this;
}

// current bigint is equalised with temp , temp is incremented , usage ->  a = ++temp ;
BigInt BigInt::operator++(int temp){
	BigInt aux;
	aux = *this;
	++(*this);
	return aux;
}

// decrements the current bigint number , pops off the last last digit if it becomes zero (MSD of original number == rightmost/biggest digit of string 'digits' of current bigint class number) 
BigInt &BigInt::operator--(){
	if(digits[0] == 0 && digits.size() == 1)
		throw("UNDERFLOW");
	int i, n = digits.size();
	for (i = 0; digits[i] == 0 && i < n;i++)
		digits[i] = 9;
	digits[i]--;
	if(n > 1 && digits[n - 1] == 0)
		digits.pop_back();
	return *this;
}

// current bigint is equalised with temp , temp is decremented , usage ->  a = --temp ;
BigInt BigInt::operator--(int temp){
	BigInt aux;
	aux = *this;
	--(*this);
	return aux;
}

// usage , a+=b , increments number 'a' by 'b' units
BigInt &operator+=(BigInt &a,const BigInt& b){
	int t = 0, s, i;		// t is carry digit in summation , s is current position digit of final summation
	int n = Length(a), m = Length(b);
	if(m > n)
		a.digits.append(m - n, 0);
	n = Length(a);
	for (i = 0; i < n;i++){
		if(i < m)
			s = (a.digits[i] + b.digits[i]) + t;
		else
			s = a.digits[i] + t;
		t = s / 10;
		a.digits[i] = (s % 10);
	}
	if(t)
		a.digits.push_back(t);
	return a;
}

// returns value of a+b , which can be assigned to another bigint variable
BigInt operator+(const BigInt &a, const BigInt &b){
	BigInt temp;
	temp = a;
	temp += b;
	return temp;
}

// usage , a-=b , decrements number 'a' by 'b' units
BigInt &operator-=(BigInt&a,const BigInt &b){
	if(a < b)
		throw("UNDERFLOW");
	int n = Length(a), m = Length(b);
	int i, t = 0, s;
	for (i = 0; i < n;i++){
		if(i < m)
			s = a.digits[i] - b.digits[i]+ t;
		else
			s = a.digits[i]+ t;
		if(s < 0)
			s += 10,
			t = -1;
		else
			t = 0;
		a.digits[i] = s;
	}
	while(n > 1 && a.digits[n - 1] == 0)
		a.digits.pop_back(),
		n--;
	return a;
}

// returns value of a-b , which can be assigned to another bigint variable
BigInt operator-(const BigInt& a,const BigInt&b){
	BigInt temp;
	temp = a;
	temp -= b;
	return temp;
}

// usage , a*=b , multiplies number 'a' by 'b' and stores in a , school method of multiplication
BigInt &operator*=(BigInt &a, const BigInt &b)
{
	if(Null(a) || Null(b)){
		a = BigInt();
		return a;
	}
	int n = a.digits.size(), m = b.digits.size();
	vector<int> v(n + m, 0);
	for (int i = 0; i < n;i++)
		for (int j = 0; j < m;j++){
			v[i + j] += (a.digits[i] ) * (b.digits[j]);
		}
	n += m;
	a.digits.resize(v.size());
	for (int s, i = 0, t = 0; i < n; i++)
	{
		s = t + v[i];
		v[i] = s % 10;
		t = s / 10;
		a.digits[i] = v[i] ;
	}
	for (int i = n - 1; i >= 1 && !v[i];i--)
			a.digits.pop_back();
	return a;
}

// returns value of a*b , which can be assigned to another bigint variable
BigInt operator*(const BigInt&a,const BigInt&b){
	BigInt temp;
	temp = a;
	temp *= b;
	return temp;
}

/*
usage , a/=b , divides number 'a' by 'b' and stores in a , school method of division , long division algorithm
lgcat is used to track the length of the quotient
cc is used to store the current digit of the quotient
cat is a vector to store the digits of the quotient in correct order , 123 is stored as 123 only
*/
BigInt &operator/=(BigInt& a,const BigInt &b){
	if(Null(b))
		throw("Arithmetic Error: Division By 0");
	if(a < b){
		a = BigInt();
		return a;
	}
	if(a == b){
		a = BigInt(1);
		return a;
	}
	int i, lgcat = 0, cc;
	int n = Length(a), m = Length(b);
	vector<int> cat(n, 0);
	BigInt t;

	/* This loop initializes t with enough digits from a to ensure t is greater than or equal to b */
	for (i = n - 1; t * 10 + a.digits[i] < b;i--){
		t *= 10;
		t += a.digits[i] ;
	}

	/* This loop performs the long division algorithm:
	It multiplies t by 10 and adds the next digit of a.
	It finds the largest digit cc such that cc * b is less than or equal to t.
	It subtracts cc * b from t to update the remainder.
	It stores the digit cc in the cat vector. */
	for (; i >= 0; i--){
		t = t * 10 + a.digits[i];
		for (cc = 9; cc * b > t;cc--);
		t -= cc * b;
		cat[lgcat++] = cc;
	}

	a.digits.resize(cat.size());
	for (i = 0; i < lgcat;i++)
		a.digits[i] = cat[lgcat - i - 1];
	a.digits.resize(lgcat);
	return a;
}

// returns value of a/b , which can be assigned to another bigint variable
BigInt operator/(const BigInt &a,const BigInt &b){
	BigInt temp;
	temp = a;
	temp /= b;
	return temp;
}

// usage : a%=b , long form of division , remainder assigned to a
BigInt &operator%=(BigInt& a,const BigInt &b){
	if(Null(b))
		throw("Arithmetic Error: Division By 0");
	if(a < b){
		return a;
	}
	if(a == b){
		a = BigInt();
		return a;
	}
	int i, lgcat = 0, cc;
	int n = Length(a), m = Length(b);
	vector<int> cat(n, 0);
	BigInt t;
	for (i = n - 1; t * 10 + a.digits[i] < b;i--){
		t *= 10;
		t += a.digits[i];
	}
	for (; i >= 0; i--){
		t = t * 10 + a.digits[i];
		for (cc = 9; cc * b > t;cc--);
		t -= cc * b;
		cat[lgcat++] = cc;
	}
	a = t;
	return a;
}

// returns value of a%b , which can be assigned to another bigint variable
BigInt operator%(const BigInt &a,const BigInt &b){
	BigInt temp;
	temp = a;
	temp %= b;
	return temp;
}

// usage :  a^=b , binary exponentiation
BigInt &operator^=(BigInt & a,const BigInt & b){
	BigInt Exponent, Base(a);
	Exponent = b;
	a = 1;
	while(!Null(Exponent)){
		if(Exponent[0] & 1)
			a *= Base;
		Base *= Base;
		divide_by_2(Exponent);
	}
	return a;
}

// returns value of a^b , which can be assigned to another bigint class variable
BigInt operator^(BigInt & a,BigInt & b){
	BigInt temp(a);
	temp ^= b;
	return temp;
}

// divides bigint class variable a by 2 , usage : a/=2
void divide_by_2(BigInt & a){
	int add = 0;		// carry from more significant digit towards less significant digit
	for (int i = a.digits.size() - 1; i >= 0;i--){
		int digit = (a.digits[i] >> 1) + add;
		add = ((a.digits[i] & 1) * 5);	// The bitwise AND operator (& 1) checks if the current digit is odd. If it is odd, add is set to 5 (because we are effectively carrying over 0.5, which translates to 5 when working with integers). If it is even, add is set to 0.
		a.digits[i] = digit;
	}
	while(a.digits.size() > 1 && !a.digits.back())
		a.digits.pop_back();
}

// binary search for finding square root
BigInt sqrt(BigInt & a){
	BigInt left(1), right(a), v(1), mid, prod;
	divide_by_2(right);
	while(left <= right){
		mid += left;
		mid += right;
		divide_by_2(mid);
		prod = (mid * mid);
		if(prod <= a){
			v = mid;
			++mid;
			left = mid;
		}
		else{
			--mid;
			right = mid;
		}
		mid = BigInt();
	}
	return v;
}

// nth catalan number is  ( 2*n choose n ) / (n+1)
BigInt NthCatalan(int n){
	BigInt a(1),b;
	for (int i = 2; i <= n;i++)
		a *= i;
	b = a;
	for (int i = n + 1; i <= 2 * n;i++)
		b *= i;
	a *= a;
	a *= (n + 1);
	b /= a;
	return b;
}

// nth fibonacci number , general way : curr = c , prev = a , pprev = b
BigInt NthFibonacci(int n){
	BigInt b(1), a(1), c;
	if(!n)
		return c;
	n--;
	while(n--){
		c = a + b;
		b = a;
		a = c;
	}
	return b;
}

// n factorial simple implementation
BigInt Factorial(int n){
	BigInt f(1);
	for (int i = 2; i <= n;i++)
		f *= i;
	return f;
}

// input stream , cin operator (>>) overloading , returned value is stored in bigint class variable
istream &operator>>(istream &in,BigInt&a){
	string s;
	in >> s;
	int n = s.size();
	 a.digits.resize(n); 
	for (int i = n - 1; i >= 0;i--){
		if(!isdigit(s[i]))
			throw("INVALID NUMBER");
		a.digits[n - i - 1] = s[i]-'0';
	}
	return in;
}

// output stream , cout operator (<<) overloading , returned value is printed on terminal
ostream &operator<<(ostream &out,const BigInt &a){
	for (int i = a.digits.size() - 1; i >= 0;i--)
		cout << (short)a.digits[i]; // The cast to short ensures the digit is correctly interpreted as a number rather than a character
	return cout;
}



// TEST CASES TO UNDERSTAND ABOVE CLASSES IMPLEMENTATION //
int main()
{
	// Test code with some examples to understand bigint

    cout << "---------------------------------------------------------- BIG INT IMPLEMENTATION -----------------------------------------------------------------------" << endl;

	BigInt first("12345");
	cout << "The number of digits"
		<< " in first big integer = "
		<< Length(first) << '\n';
	BigInt second(12345);
	if (first == second) {
		cout << "first and second are equal!\n";
	}
	else
		cout << "Not equal!\n";
	BigInt third("10000");
	BigInt fourth("100000");
	if (third < fourth) {
		cout << "third is smaller than fourth!\n";
	}
	BigInt fifth("10000000");
	if (fifth > fourth) {
		cout << "fifth is larger than fourth!\n";
	}
	// Printing all the numbers
	cout << "first = " << first << '\n';
	cout << "second = " << second << '\n';
	cout << "third = " << third << '\n';
	cout << "fourth = " << fourth<< '\n';
	cout << "fifth = " << fifth<< '\n';

	// Incrementing the value of first
	first++;
	cout << "After incrementing the"
		<< " value of first is : ";
	cout << first << '\n';
	BigInt sum;
	sum = (fourth + fifth);
	cout << "Sum of fourth and fifth = "
		<< sum << '\n';
	BigInt product;
	product = second * third;
	cout << "Product of second and third = "
		<< product << '\n';

	// Print the fibonacci and catalan number from 91 to 100
	cout << "-------------------------Fibonacci"
		<< "------------------------------\n";
	for (int i = 91; i <= 100; i++) {
		BigInt Fib;
		Fib = NthFibonacci(i);
		cout << "Fibonacci " << i << " = " << Fib<<'\n';
	}
	cout << "-------------------------Catalan"
		<< "------------------------------\n";
	for (int i = 91; i <= 100; i++) {
		BigInt Cat;
		Cat = NthCatalan(i);
		cout << "Catalan " << i << " = " << Cat<<'\n';
	}

	// Calculating factorial of from 91 to 100
	cout << "-------------------------Factorial"
		<< "------------------------------\n";
	for (int i = 91; i <= 100; i++) {
		BigInt fact;
		fact = Factorial(i);
		cout << "Factorial of "
			<< i << " = ";
		cout << fact << '\n';
	}

	BigInt input ;
	cout<<"ENTER INPUT NUMBER : ";
	cin>>input;
	cout<<"INPUT NUMBER IS : " <<input<<'\n';

// ---------------------------------------------------------------------------------------------------------------------------------- //

cout<<" -------------------------------------------------------------------------------------------------------------------------------------------------------- "<<'\n';
cout<<" ---------------------------------------------------------- GRAPHS IMPLEMENTATION ----------------------------------------------------------------------- "<<'\n';

// Example usage for GRAPHS CLASS
        Graph<string> g(5, false);  // Create a directed graph with 5 vertices , false==undirected graph , true==directed graph 
        g.addEdge("A", "B", 1);     // A=starting node , B = end node , 1 = weight of edge
        g.addEdge("B", "C", 1);
        g.addEdge("C", "D", 1);
        g.addEdge("D", "E", 1);
        g.addEdge("E", "B", -6);  // Introduce a negative weight cycle

        
        // Two vertex search starting from some source node (1st argument) to another destination node (2nd argument)
        cout << "BFS from A to D: " << g.bfs("A", "D") << endl; 
        cout << "DFS from A to D: " << g.dfs("A", "D") << endl;  
        
        // NOTE that SHORTEST PATH we study is actually SHORTEST WALK , all algorithms of shortest path give answer for something what is by definition called SHORTEST WALK
        try {
        cout << "Shortest path from A to D: " << g.shortestPath("A", "D") << endl;  // return shortest path if there is some path from 'A' to 'D' , otherwise return -1
        } catch (const exception& e) {
        cerr << "Exception caught: " << e.what() << endl;
        }
        // IF NEGATIVE WEIGHT CYCLE IS DETECTED , it throws exception , because in that case , shortest path can go to -infinity 
        
        // checks for any cycle in the graph
        cout << "Cycle detection: " << (g.isCyclic() ? "Cycle detected" : "No cycle detected") << endl; 

// ---------------------------------------------------------------------------------------------------------------------------------- //

cout<<" --------------------------------------------------------------------------------------------------------------------------------------------------------------- "<<endl;
cout<<" ---------------------------------------------------------- TRIES and KMP IMPLEMENTATION ----------------------------------------------------------------------- "<<endl;



// example usage for tries and KMP algorithm
   Trie trie;
    KMP kmp;

    // Test cases for Trie
    trie.insert("hello");
    trie.insert("world");

    std::cout << std::boolalpha;
    std::cout << "Search 'hello': " << trie.search("hello") << std::endl;  // Output: true
    std::cout << "Search 'world': " << trie.search("world") << std::endl;  // Output: true
    std::cout << "Search 'helloo': " << trie.search("helloo") << std::endl;  // Output: false
    std::cout << "Search 'worl': " << trie.search("worl") << std::endl;  // Output: false

    // Test cases for KMP
    std::string text = "nspabcnsabnspnsp";
    std::string pattern = "nsp";

    std::vector<int> occurrences = kmp.findOccurrences(text, pattern);   // find occurences of variable pattern in variable text

    std::cout << "Occurrences of '" << pattern << "' in '" << text << "': ";
    for (int index : occurrences) {
        std::cout << index << " ";
    }
    std::cout << std::endl;  // Output: Occurrences of 'nsp' in 'nspabcnsabnspnsp': 0 10 13     

    return 0;


}
