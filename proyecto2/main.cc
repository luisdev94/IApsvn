// Game of Othello -- Example of main
// Universidad Simon Bolivar, 2012.
// Author: Blai Bonet
// Last Revision: 1/11/16
// Modified by: 

#include <iostream>
#include <limits>
#include <climits>
#include "othello_cut.h" // won't work correctly until .h is fixed!
#include "utils.h"

#include <unistd.h>

#include <unordered_map>

#define BLACK    true
#define WHITE    false

using namespace std;

unsigned expanded = 0;
unsigned generated = 0;
int tt_threshold = 32; // threshold to save entries in TT

// Transposition table
struct stored_info_t {
    int value_;
    int type_;
    enum { EXACT, LOWER, UPPER };
    stored_info_t(int value = -100, int type = LOWER) : value_(value), type_(type) { }
};

struct hash_function_t {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public unordered_map<state_t, stored_info_t, hash_function_t> {
};

hash_table_t TTable[2];

int maxmin(state_t state, int depth, bool player, bool use_tt);

// int maxmin(state_t state, int depth, bool use_tt);
int minmax(state_t state, int depth, bool player, bool use_tt = false){
    if ((depth == 0) || (state.terminal())) {
        return state.value();
    }
    int score = INT_MAX;
    std::vector<int> valid_moves = state.get_moves(player);
    if (valid_moves[0] == (-1)) { // No possible move for this player.
        score = min(score, maxmin(state, depth, ((player+1) % 2), use_tt));
    } else {
        for (unsigned int c = 0; c < valid_moves.size(); c++) {
            // cout << endl << "posibles movimientos: " << movemove[c] << endl;   
            score = min(score, maxmin(state.move(player,valid_moves[c]), depth - 1, ((player+1) % 2), use_tt));
        }
    }
    return score;
};

int maxmin(state_t state, int depth, bool player, bool use_tt = false) {
    if ((depth == 0) || (state.terminal()))
        return state.value();
    int score = INT_MIN;
    std::vector<int> valid_moves = state.get_moves(player);

    if (valid_moves[0] == (-1)) { // No possible move for this player.
        score = max(score, minmax(state, depth, ((player+1) % 2), use_tt));
    } else {
        for (unsigned int c = 0; c < valid_moves.size(); c++) {
            // cout << endl << "posibles movimientos: " << movemove[c] << endl;
            score = max(score, minmax(state.move(player,valid_moves[c]), depth - 1, ((player+1) % 2), use_tt));
        }
    }
    return score;
};

int negamax(state_t state, int depth, int color, bool use_tt = false) {

	if ((depth == 0) || (state.terminal())) {
        //cout << "terminal " << color * state.value() << endl;
        return color * state.value();
    }

    int alpha = INT_MIN;
    bool player = (color == 1) ? BLACK : WHITE;
    std::vector<int> valid_moves = state.get_moves2(player);
    //cout << valid_moves.size()<< "\n" << endl;
    
    if (valid_moves.empty()) {   // No possible move for this player.
        alpha = max(alpha, (-1) * negamax(state, depth, (-1) * color));
    } 

    else {
        
        for (unsigned int i = 0; i < valid_moves.size(); ++i) {
            //cout << valid_moves[i] << endl;
            ++generated;
            alpha = max(alpha, (-1) * negamax(state.move(player, valid_moves[i]), depth - 1, (-1) * color, use_tt));
        }
        ++expanded;
    }
    
    //cout << "alpha " << alpha << endl;
    return alpha;

}
 
int negamax(state_t state, int depth, int alpha, int beta, int color, bool use_tt = false) {

	if ((depth == 0) || (state.terminal())) {
        //cout << "terminal " << color * state.value() << endl;
        return color * state.value();
    }

    int score = INT_MIN;
    bool player = (color == 1) ? BLACK : WHITE;
    std::vector<int> valid_moves = state.get_moves2(player);

    if (valid_moves.empty()) {   // No possible move for this player.
        
        int aux = (-1) * negamax(state, depth - 1, (-1) * beta, (-1) * alpha, (-1) * color, use_tt);
        score = max(score, aux);
        alpha = max(alpha, aux);
    }

    else {
        
        for (unsigned int i = 0; i < valid_moves.size(); ++i) {
            //cout << valid_moves[i] << endl;
            ++generated;

            int aux = (-1) * negamax(state.move(player, valid_moves[i]), depth - 1, (-1) * beta, (-1) * alpha, (-1) * color, use_tt);
            score = max(score, aux);
            alpha = max(alpha, aux);

            if (alpha >= beta) {   // cut-off
                break;
            }
        }
        ++expanded;
    }

    return score;

}

int scout(state_t state, int depth, int color, bool use_tt = false) {
    if (depth == 0 || state.terminal()) { 
        return state.value();
    }
    int score = 0;
    return score;
}
int negascout(state_t state, int depth, int alpha, int beta, int color, bool use_tt = false);

int main(int argc, const char **argv) {
    state_t pv[128];
    int npv = 0;
    for( int i = 0; PV[i] != -1; ++i ) ++npv;

    int algorithm = 0;
    if( argc > 1 ) algorithm = atoi(argv[1]);
    bool use_tt = argc > 2;

    // Extract principal variation of the game
    state_t state;
    cout << "Extracting principal variation (PV) with " << npv << " plays ... " << flush;
    for( int i = 0; PV[i] != -1; ++i ) {
        bool player = i % 2 == 0; // black moves first!        
        int pos = PV[i];
        pv[npv - i] = state;
        //cout << endl << "representacion? " << pos <<endl;
        std::vector<int> movemove = state.get_moves(player);
        /*for (unsigned int c = 0; c < movemove.size(); c++) {
            cout << endl << "posibles movimientos: " << movemove[c] << endl;   
        }*/

        state = state.move(player, pos);
        
        //int moves = movemove[0];
        //Print the board after each move.
        //cout << endl << "moviendo a: " << pos << endl;
        //cout << endl << state << endl;
        // sleep(1);
    }
    pv[0] = state;
    cout << "done!" << endl;

#if 0
    // print principal variation
    for( int i = 0; i <= npv; ++i )
        cout << pv[npv - i];
#endif

    // Print name of algorithm
    cout << "Algorithm: ";
    if( algorithm == 0 ) {
        cout << "Minmax-Maxmin";
    } else if( algorithm == 1 ) {
        cout << "Negamax (minmax version)";
    } else if( algorithm == 2 ) {
        cout << "Negamax (alpha-beta version)";
    } else if( algorithm == 3 ) {
        cout << "Scout";
    } else if( algorithm == 4 ) {
        cout << "Negascout";
    }
    cout << (use_tt ? " w/ transposition table" : "") << endl;

    // Run algorithm along PV (bacwards)
    cout << "Moving along PV:" << endl;
    for( int i = 0; i <= npv; ++i ) {
        //cout << pv[i];
        int value = 0;
        TTable[0].clear();
        TTable[1].clear();
        float start_time = Utils::read_time_in_seconds();
        expanded = 0;
        generated = 0;
        int color = i % 2 == 1 ? 1 : -1;

        try {
            if( algorithm == 0 ) {
                value = color * (color == 1 ? maxmin(pv[i], i, color, use_tt) : minmax(pv[i], i, color, use_tt));
            } else if( algorithm == 1 ) {
                value = negamax(pv[i], i, color, use_tt);
            } else if( algorithm == 2 ) {
                value = negamax(pv[i], i, -200, 200, color, use_tt);
            } else if( algorithm == 3 ) {
                value = scout(pv[i], npv-i, color, use_tt);
            } else if( algorithm == 4 ) {
                //value = negascout(pv[i], 0, -200, 200, color, use_tt);
            }
        } catch( const bad_alloc &e ) {
            cout << "size TT[0]: size=" << TTable[0].size() << ", #buckets=" << TTable[0].bucket_count() << endl;
            cout << "size TT[1]: size=" << TTable[1].size() << ", #buckets=" << TTable[1].bucket_count() << endl;
            use_tt = false;
        }

        float elapsed_time = Utils::read_time_in_seconds() - start_time;

        cout << npv + 1 - i << ". " << (color == 1 ? "Black" : "White") << " moves: "
             << "value=" << color * value
             << ", #expanded=" << expanded
             << ", #generated=" << generated
             << ", seconds=" << elapsed_time
             << ", #generated/second=" << generated/elapsed_time
             << endl;
    }

    return 0;
}
