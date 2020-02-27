#include<stdio.h>
#include<vector>
#include<set>
#include<map>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>
#include<numeric>
#include<limits>
using namespace std;

// prints information about progress and computed data
#ifndef DEBUGTRACE
    #define DEBUGTRACE gbl::PRINTDATA
#endif

#ifndef RUNTESTMINTREE
    #define RUNTESTMINTREE false
#endif

#ifndef MISEXT
    #define MISEXT true
#endif

#ifndef MISLIMITK
    #define MISLIMITK true
#endif

// global configuration variable container
class gbl {
public:
    static bool  PRINTDATA,       // prints important computed data
                 CHECKUNIQUE,     // checks whether there is a unique minimal perfect phylogeny
                 ENUMERATE,       // runs KW enumeration algorithm
                 DAGSTATISTICS,   // runs DAG algorithms for statistics
                 ALLOWINTERNAL,   // lets taxon in the dataset be internal nodes in minimal trees
                 NEWICK;          // Prints the initial phylogeny in the Newick tree format
    
    static int OUTGROUPTAXON;     // In the initial phylogeny, this taxon is at the top
    
    static const char *DAGPRETTYF,// output file where the dag will be prettyprinted, or input if doing statistics on existing DAG
                      *DAGDOTOUTF;// output dot file of the dag
                    //*DPDOTOUTF; // output dot file for the tree generated by perf
};

// old debug flags
#define VERBOSE 0
#define VERBOSE1 0
#define VERBOSEDATA 0
#define PRINTREAD 0
#define CLUSTER 0
#define DEBUG 0
#define DEBUG1 0
#define DEBUG2 0

#define DEBUGOLD

enum PrettyPrintFormat {
    PPAF_NONE,  // normal printing
    PPAF_DUMMY, // all negative numbers are printed as "*"
    PPAF_PC,    // zeros are printed as "-", ones printed as their index
};

void prettyprintarray(const vector<int> &array, const char name[], PrettyPrintFormat format = PPAF_NONE);
void prettyprintarray(const vector<vector<int> > &array, const char name[], PrettyPrintFormat format = PPAF_NONE);

// utility functions
bool subset(const vector<int> &v1, const vector<int> &v2);
bool compatible(const vector<int> &v1, const vector<int> &v2);
bool compatible(const vector<int> &Sv1, const vector<int> &Sv2, vector<int> &SvGGprime);
bool isClassUnion(const vector<int> &eqclasses, const vector<int> &pcluster);
void unionStates(vector<int> &root, const vector<int> &sub);

template<class T>
int binary_find(const vector<T> &list, const T &value) {
    if (list.size() == 0)
        return -1;
    
    int index = lower_bound(list.begin(), list.end(), value) - list.begin();
    
    if (index < (int) list.size() && list[index] == value)
        return index;
    
    return -1;
}

// Using QTable is recommended. It has faster construction time and uses less space than the Trie: O(L n) vs O(L n^2)
//#define UseQTable false

#ifndef UseQTable
    #define UseQTable true
#endif

#if UseQTable
    #include "qtable.cpp"
    #define Dict QTable
#else
    #include "trie.cpp"
    #define Dict Trie
#endif

#include "ppDAG.cpp"
#include "maxindset.cpp"

enum ArgError { ERROR = -1, QUIT = 0, NONE = 1 };

ArgError handleArguments(int argc, char* argv[], const char* &infilename);
void readmatrix (istream &inf, vector<vector<int> > &data, int &dataindex, int &n, int &m);
int countstates(const vector<vector<int> > &data, vector<int> &statecount, int n, int m);
int  removeduprows(vector<vector<int> > &data, const vector<int> &statecount, int &n, int m);
void fillmissingdata(vector<vector<int> > &data, vector<int> &statecount, int n, int m);

void findpropclusters(const vector<vector<int> > &data, vector<vector<int> > &propercluster, vector<vector<int> > &Sv, const vector<int> &statecount, int n, int m, int kStates);

void sort(vector<vector<int> > &propercluster, vector<vector<int> > &Sv, vector<int> &newclustersize, Dict &pcdict);

int perf(const vector<vector<int> > &data, const vector<vector<int> > &propercluster, const Dict &pcdict, const vector<vector<int> > &Sv, const vector<vector<int> > &SSvG, const vector<int> &clustersize, const vector<int> &statecount, int properclustercount, int n, int m, int k);

void traceback(const vector<vector<int> > &gooddecomp, const vector<vector<int> > &propercluster, int properclustercount, int n);
void tracebackadj(const vector<vector<int> > &gooddecomp, const vector<vector<int> > &propercluster, int properclustercount, int n);
void taxatraceback(const vector<vector<int> > &gooddecomp, const vector<vector<int> > &propercluster, const vector<int> &clustersize, int properclustercount, int n);
void statetraceback(const vector<vector<int> > &data, const vector<vector<int> > &gooddecomp, const vector<vector<int> > &propercluster, const vector<vector<int> > &Sv, int properclustercount, int n, int m);
void newicktraceback(const vector<vector<int> > &data, const vector<vector<int> > &gooddecomp, const vector<vector<int> > &Sv, int H);
int treesizetraceback(const vector<vector<int> > &gooddecomp, int H);

//long expt(int a, int b);
//long exp2(int b);
unsigned int digits(unsigned int n);

void tracebackDOT(const vector<vector<int> > &gooddecomp, const vector<vector<int> > &propercluster, int properclustercount, int n, const vector<vector<int> > &rootlabels);

void createStateGraph(const vector<vector<int> > &data, const vector<int> &statecount, vector<vector<int> > &prevTaxa, vector<vector<int> > &nextTaxa, vector<vector<int> > &prevStateTaxa);
void findSSvG(vector<vector<int> > &SSvG, const vector<vector<int> > &data, const vector<int>& statecount, const vector<vector<int> > &Sv, const Dict &pcdict, const vector<vector<int> > &propercluster);
bool findDecompSvGG1(const Dict &pcdict, const vector<vector<int> > &SSvG, const vector<int> &good, const vector<vector<int> > &propercluster, vector<int> &tempdecomp, int G, int G1, int n);
int  findEqClassesMulti(const vector<int> &splitter, vector<int> &eqclasses, const vector<vector<int> > &data, const vector<vector<int> > &prevTaxa, const vector<vector<int> > &nextTaxa, const vector<vector<int> > &prevStateTaxa, int n, int m);

bool getCanonicalLabeling(const vector<int> &decomp, const vector<vector<int> > &Sv, vector<int> &clx, int H);

int enumeration(vector<vector<int> > &verygood, const vector<vector<int> > &propercluster, const Dict &pcdict, vector<int> &good, const vector<vector<int> > &Sv, const vector<vector<int> > &SSvG, const vector<int> &clustersize, const vector<vector<int> > &data, const vector<int> &statecount, int k);

void enumMaxIndSets(const vector<int> &partners, const vector<vector<int> > &propercluster, const vector<vector<int> > &Sv, const vector<int>& clustersize, vector<vector<int> > &output, int H, int k);
void enumMaxIndSetsPairs(const vector<pair<int,int> > &partners, const vector<vector<int> > &propercluster, const vector<vector<int> > &Sv, const vector<int>& clustersize, vector<PPDAG::ProductNode> &output, int H, int k);

int constructDAG(vector< vector<vector<int> > > &Conn, vector< vector<vector<int> > > &Vp, const vector<vector<int> > &verygood, const vector<int> &good, const Dict &pcdict, const vector<vector<int> > &propercluster, const vector<vector<int> > &Sv, const vector<vector<int> > &data, const vector<int> &statecount, const vector<int> &clustersize, int properclustercount, int k);

void takeDagStatistics(const PPDAG &dag, const vector<vector<int> > &propercluster, const Dict &pcdict, const vector<vector<int> > &data, const vector<int> &statecount);