//---------------------------------------------------------------------------------------------------
//
// Suggested for inputs: 
//
//    opt_sat 0xABBA 5    (UNSAT)
//    opt_sat 0xABBA 6    (SAT)
//    opt_sat 0xDEAD 7    (UNSAT)
//    opt_sat 0xDEAD 8    (SAT)
//
//---------------------------------------------------------------------------------------------------

#include <map>
#include <vector>
#include "MiniSat_v1.14/Solver.h"

//#define REDUNDANT_CONSTRAINTS   // Uncomment this to add redundant constraints to speed up SAT


using namespace std;

inline int max_(int x, int y) { return (x > y) ? x : y; }


//=================================================================================================
// SAT stuff:

struct StrCmp {
    bool operator()(char const *a, char const *b) { return strcmp(a, b) < 0; }
};


map<char*, Lit, StrCmp> varmap;
vector<char*>           revmap;
Solver                  S;


Lit v(cchar* name, int i = INT_MAX, int j = INT_MAX)
{
    char* text = (char*)malloc(strlen(name) + 32);
    if      (i == INT_MAX) strcpy(text, name);
    else if (j == INT_MAX) sprintf(text, "%s[%d]", name, i);
    else                   sprintf(text, "%s[%d][%d]", name, i, j);

    Lit lit = varmap[text];
    if (lit == lit_Undef){
        lit = Lit(S.newVar());
        varmap[text] = lit;
        while (revmap.size() <= var(lit))
            revmap.push_back(NULL);
        revmap[var(lit)] = text;
    }else
        free(text);

    return lit;
}


inline void addClause(vec<Lit>& ps) { S.addClause(ps); }
inline bool solve() { S.verbosity = 0; return S.solve(); }

inline int model(Lit p)
{
    int x = var(p);
    if (x < 0 || x >= S.model.size()) return -1;
    if (S.model[x] == l_True ) return 1-sign(p);
    if (S.model[x] == l_False) return sign(p);
    return -1;
}

inline void addClause(Lit p0) { vec<Lit> tmp(1); tmp[0] = p0; addClause(tmp); }
inline void addClause(Lit p0, Lit p1) { vec<Lit> tmp(2); tmp[0] = p0; tmp[1] = p1; addClause(tmp); }
inline void addClause(Lit p0, Lit p1, Lit p2) { vec<Lit> tmp(3); tmp[0] = p0; tmp[1] = p1; tmp[2] = p2; addClause(tmp); }
inline void addClause(Lit p0, Lit p1, Lit p2, Lit p3) { vec<Lit> tmp(4); tmp[0] = p0; tmp[1] = p1; tmp[2] = p2; tmp[3] = p3; addClause(tmp); }


void addOneHot(vec<Lit>& ps)
{
    addClause(ps);
    for (int i = 1; i < ps.size(); i++)
        for (int j = 0; j < i; j++)
            addClause(~ps[i], ~ps[j]);
}


//=================================================================================================
// Solver for AIG wiring:


uint F;     // 16-bit function table
int  N;     // AIG size


#define Out(n, b)    v("Out"   , n, b)
#define InL(n, b)    v("InL"   , n, b)
#define InR(n, b)    v("InR"   , n, b)
#define ChildL(n, m) v("ChildL", n, m)
#define ChildR(n, m) v("ChildR", n, m)
#define SignL(n)     v("SignL" , n)
#define SignR(n)     v("SignR" , n)

void saveModel(int n);


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


void solveSynth()
{
    //
    // =====  FORMULATE PROBLEM AS SAT  =====
    //

    // *****  PIs  *****
    for (int n = 0; n <  4; n++)
    for (int b = 0; b < 16; b++)
        addClause( ((b >> n) & 1)  ?  Out(~n,b) : ~Out(~n,b) );

    // *****  PO  *****
    for (int b = 0; b < 16; b++)
        addClause( ((F >> b) & 1)  ?  Out(N-1, b) : ~Out(N-1, b));

    // *****  ANDs (gate logic)  *****
    for (int n =  0; n <  N; n++)      // For all nodes
    for (int b =  0; b < 16; b++)      // For all bits of FTB
    {
        addClause( ~Out(n,b),  InL(n,b) );
        addClause( ~Out(n,b),  InR(n,b) );
        addClause(  Out(n,b), ~InL(n,b), ~InR(n,b) );
    }

    // *****  ANDs (left/right children)  *****
    for (int n =  0; n <  N; n++)      // For all nodes
    for (int b =  0; b < 16; b++)      // For all bits of FTB
    for (int m = -4; m <  n; m++)      // For all nodes and inputs
    {
        // If 'm' is left child of 'n', then: InL(n) = Out(m) ^ SignL(n)
        addClause( ~ChildL(n,m),  SignL(n), ~Out(m,b),  InL(n,b) );
        addClause( ~ChildL(n,m),  SignL(n),  Out(m,b), ~InL(n,b) );
        addClause( ~ChildL(n,m), ~SignL(n), ~Out(m,b), ~InL(n,b) );
        addClause( ~ChildL(n,m), ~SignL(n),  Out(m,b),  InL(n,b) );

        // Ditto for right child of 'n'
        addClause( ~ChildR(n,m),  SignR(n), ~Out(m,b),  InR(n,b) );
        addClause( ~ChildR(n,m),  SignR(n),  Out(m,b), ~InR(n,b) );
        addClause( ~ChildR(n,m), ~SignR(n), ~Out(m,b), ~InR(n,b) );
        addClause( ~ChildR(n,m), ~SignR(n),  Out(m,b),  InR(n,b) );
    }

    // *****  CHILDREN ONE HOT  *****
    for (int n = 0; n < N; n++){
        // Left child:
        vec<Lit> tmp;
        for (int m = -4; m < n; m++)
            tmp.push(ChildL(n, m));
        addOneHot(tmp);

        // Right child:
        tmp.clear();
        for (int m = -4; m < n; m++)
            tmp.push(ChildR(n, m));
        addOneHot(tmp);
    }

#ifdef REDUNDANT_CONSTRAINTS
    for (int n = 0; n < N; n++){
        // Right < Left child
        for (int m = -4; m < n; m++)
        for (int k = m; k < n; k++)
            addClause( ~ChildL(n,m), ~ChildR(n,k) );
    }

    // *****  ORDER LEFT CHILDREN  *****
    for (int n =  0; n < N-1; n++)
    for (int m = -4; m < n  ; m++)
    for (int k = -4; k < m  ; k++)
        addClause( ~ChildL(n,m), ~ChildL(n+1,k) );

    for (int n =  0; n < N-1; n++)
    for (int m = -4; m < n  ; m++)
        addClause( ~ChildL(n,m), ~ChildL(n+1,m), ~SignL(n), SignL(n+1) );

    // *****  USE ALL NODES  *****
    for (int m = -4; m < N-1; m++){      // (only valid for proper 4-input functions)
        vec<Lit> tmp;
        for (int n = max_(0,m+1); n < N; n++){
            tmp.push(ChildL(n,m));
            tmp.push(ChildR(n,m));
        }
        addClause(tmp);
    }

    // *****  TWO ADJACENT NODES HAVE UNIQUE CHILDREN  *****
    for (int n =  1; n < N; n++)
    for (int m =  0; m < n; m++)
    for (int a = -4; a < m; a++)
    {
        addClause(~ChildL(n,m), ~ChildR(n,a), ~ChildL(m,a));
        addClause(~ChildL(n,m), ~ChildR(n,a), ~ChildR(m,a));
        addClause(~ChildR(n,m), ~ChildL(n,a), ~ChildL(m,a));
        addClause(~ChildR(n,m), ~ChildL(n,a), ~ChildR(m,a));
    }
#endif


    //
    // =====  FIND ALL SOLUTIONS  =====
    //

    int    n  = 0;
    while (solve()){
        // Print progress:
        n++;
        printf("Model %d  (%.2f s)\n", n, cpuTime());
        saveModel(n);

        // Ban this model:
        vec<Lit> tmp;
        for (int n =  0; n < N; n++)
        for (int m = -4; m < n; m++)
        {
            if (model(ChildL(n,m)))
                tmp.push(~ChildL(n,m)),
                tmp.push(model(SignL(n)) ? ~SignL(n) : SignL(n));
            if (model(ChildR(n,m)))
                tmp.push(~ChildR(n,m)),
                tmp.push(model(SignR(n)) ? ~SignR(n) : SignR(n));
        }
        S.addClause(tmp);

    }
    printf("No more models  (%.2f s)\n", cpuTime());
}


//=================================================================================================
// Main:


int main(int argc, char** argv)
{
    if (argc != 3 || strncmp(argv[1], "0x", 2) != 0)
        fprintf(stderr, "USAGE: opt_synth 0x#### [<nodes>]\n"), exit(1);

    sscanf(argv[1], "0x%x", &F);
    N = atoi(argv[2]);

    solveSynth();

    return 0;
}


//=================================================================================================
// Debug:


void saveModel(int n)
{
    char filename[128];
    sprintf(filename, "model_%.3d.dot", n);

    FILE* out = fopen(filename, "wb");
    fprintf(out, "digraph Circuit {\n");
    fprintf(out, "  edge [fontcolor=red labeldistance=1.75 arrowsize=0.7 arrowhead=none arrowtail=normal]\n\n");

    for (int n = 0; n < N; n++)
        fprintf(out, "  n%d [shape=ellipse label=\"n%d\"]\n", n+4, n);
    for (int n = -4; n < 0; n++)
        fprintf(out, "  n%d [shape=plaintext fontcolor=\"#0000CC\" label=\"x%d\"]\n", n+4, ~n);

    for (int n = 0; n < N; n++){
        for (int m = -4; m < n; m++){
            if (model(ChildL(n,m)))
                fprintf(out, "  n%d->n%d [taillabel=\"L\"%s]\n", n+4, m+4, model(SignL(n))?" arrowtail=dotnormal color=\"#00AA00\"":"");
            if (model(ChildR(n,m)))
                fprintf(out, "  n%d->n%d [taillabel=\"R\"%s]\n", n+4, m+4, model(SignR(n))?" arrowtail=dotnormal color=\"#00AA00\"":"");
        }
    }

    fprintf(out, "}\n");
    fclose(out);
}
