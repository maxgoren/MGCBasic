#include <iostream>
#include <vector>
#include <stack>
using namespace std;

template <class T>
struct Stack : public stack<T> {
    T pop() {
        T ret = stack<T>::top();
        stack<T>::pop();
        return ret;
    }
};

template <class T>
struct Bag : public vector<T> {
    Bag& add(T info) {
        vector<T>::push_back(info);
        return *this;
    }
};

class Digraph {
    private:
        int v_;
        Bag<int>* adjlist;
    public:
        Digraph(int nv = 5) {
            v_ = nv;
            adjlist = new Bag<int>[v_];
        }
        Digraph(const Digraph& o) {
            v_ = o.v_;
            adjlist = new Bag<int>[o.v_];
            for (int i = 0; i < o.v_; i++)
                for (int u : o.adjlist[i])
                    addEdge(i, u);
        }
        ~Digraph() {
            delete [] adjlist;
        }
        void addEdge(int v, int w) {
            adjlist[v].add(w);
        }
        Bag<int>& adj(int v) {
            return adjlist[v];
        }
        int V() {
            return v_;
        }
        Digraph operator=(const Digraph& o) {
            v_ = o.v_;
            adjlist = new Bag<int>[o.v_];
            for (int i = 0; i < o.v_; i++)
                for (int u : o.adjlist[i])
                    addEdge(i, u);
            return *this;
        }
};

class DFS {
    private:
        int nv;
        bool *seen;
        void initseen(int v) {
            nv = v;
            seen = new bool[nv];
            for (int i = 0; i < nv; i++)
                seen[i] = false;
        }
        void search(Digraph& G, int v) {
            seen[v] = true;
            for (int u : G.adj(v)) {
                if (!seen[u])
                    search(G, u);
            }
        }
    public:
        DFS(Digraph& G, int s) {
            initseen(G.V());
            search(G, s);
        }
        DFS(Digraph& G, Bag<int>& src) {
            initseen(G.V());
            for (int u : src)
                if (!seen[u])
                    search(G, u);
        }
        DFS(const DFS& o) {
            nv = o.nv;
            seen = new bool[o.nv];
            for (int i = 0; i < o.nv; i++)
                seen[i] = o.seen[i];
        }
        ~DFS() {
            delete [] seen;
        }
        bool marked(int v) {
            return seen[v];
        }
        DFS operator=(const DFS& o) {
            nv = o.nv;
            seen = new bool[o.nv];
            for (int i = 0; i < o.nv; i++)
                seen[i] = o.seen[i];
            return *this;
        }
};

class NFA {
    private:
        Digraph G;
        int m;
        string re;
        void buildEpsilonGraph() {
            Stack<int> ops;
            for (int i = 0; i < m; i++) {
                int lp = i;
                if (re[i] == '(' || re[i] == '|')
                    ops.push(i);
                else if (re[i] == ')') {
                    int ro = ops.pop();
                    if (re[ro] == '|') {
                        lp = ops.pop();
                        G.addEdge(lp, ro+1);
                        G.addEdge(ro, i);
                    } else lp = i;
                }
                if (i < m - 1 && (re[i+1] == '*')) {
                    G.addEdge(lp, i+1);
                    G.addEdge(i+1, lp);
                }
                if (i < m-1 && (re[i+1] == '+'))
                    G.addEdge(i+1, lp);
                if (re[i] == '(' || re[i] == '*' || re[i] == '+' || re[i] == ')')
                    G.addEdge(i, i+1);
            }
            if (!ops.empty()) {
                cout<<"Something went wrong."<<endl;
            }
        }
        void showNFA() {
            for (int i = 0; i < G.V(); i++) {
                if (i < re.size()) {
                    cout<<i<<"("<<re[i]<<"): "<<endl;
                    for (int u : G.adj(i)) {
                        if (u < re.size()) {
                            cout<<"  --> "<<u<<"("<<re[u]<<") "<<endl;
                        }
                    }
                } else {
                    cout<<"$ (Match)."<<endl;
                }
            }
        }
    public:
        NFA(string regexp) {
            re = regexp;
            m = re.length();
            G = Digraph(m+1);
            buildEpsilonGraph();
            showNFA();
        }
        bool recognizes(string text) {
            DFS dfs(G, 0);
            Bag<int> pc;
            for (int v = 0; v < G.V(); v++)
                if (dfs.marked(v))
                    pc.add(v);

            for (int i = 0; i < text.length(); i++) {
                Bag<int> states;
                for (int v : pc)
                    if (v < m)
                        if (re[v] == text[i] || re[v] == '.')
                            states.add(v+1);
                pc.clear();
                dfs = DFS(G, states);
                for (int v = 0; v < G.V(); v++)
                    if (dfs.marked(v))
                        pc.add(v);
                if (pc.empty())
                    return false;
            }
            for (int v : pc) 
                if (v == m) 
                    return true;
            
            return false;
        }
};

int main() {
    Bag<string> strs;
    strs.add("ACD").add("ABD").add("AZD").add("AAAABD").add("AAAAACD").add("CD").add("BD");
    string inner = "(A+C|A+B)D";
    string regex = "(" + inner + ")";
    NFA nfa(regex);
    for (string s : strs) {
        if (nfa.recognizes(s)) {
            cout<<s<<endl;
        }
    }
}