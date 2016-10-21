#include <cstdio>
#include <cstdlib>
#include <climits>  // for INT_MAX
#include <cassert>
#include <cmath>

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "priority_queue.hpp"

#define  WHITE 0
#define  GRAY 1
#define  BLACK 2
#define  MAX_LINE_LENGTH 40 

class Node {

    private:

        state_t *state;
        Node    *parent;
        int      cost;
        int      color;

    public:

        void make_node(Node *parent_node, state_t *st, int c);
        void make_root_node(state_t *st);

        int get_cost();
        int get_color();
        state_t * get_state();

        void set_cost(int c);
        void set_color(int c);
};

void Node::make_root_node(state_t *st) {
    state  = st;
    parent = NULL;
    cost   = 0;
    color  = GRAY; 
};

void Node::make_node(Node *parent_node, state_t *st, int c) {
    state  = st;
    parent = parent_node;
    cost   = c;
    color  = WHITE;
};

int Node::get_cost() {
    return cost;
};

int Node::get_color() {
    return color;
};

state_t * Node::get_state() { // return a pointer to the state.
    return state;
}

void Node::set_cost(int c) {
    cost = c;
};

void Node::set_color(int c) {
    color = c;
};

/*
Recibe un string correspondiente al estado del 15puzzle psvn,
recibe un array de 16 enteros, y transforma los chars a ints.
la letra B correspondiente al Blank lo convierte en 0*/
int charArrayToInt (char *stateStr, int *state) {

    int j = 0;
    int temp = 0;
    
    for (unsigned int i = 0; (i < strlen(stateStr)); ++i) {
        
        if ((stateStr[i] == 'B') or (stateStr[i] == 'b')) {
            temp = 0;
        } 
        
        else if (stateStr[i] == ' ') {
            state[j] = temp;
            temp = 0;
            // std::cout << "Asignado el valor: " << estado[j] << std::endl;
            ++j;
        } 

        else {
            if (temp == 0) {
                temp = stateStr[i]-48;
            } else {
                temp = temp*10 + stateStr[i]-48; 
            }
        }
    }
    
    return 1;
}

/*Recibe un estado de psvn (15 puzzle), lo transforma a texto,
luego lo transforma a un array de enteros, y finalmente
calcula el costo de llevar cada valor al goal (individualmente)*/
unsigned manhattan (state_t state){
    
    unsigned res = 0;
    int stateInt[16];
    char stateStr[MAX_LINE_LENGTH + 1];
    
    if ((sprint_state(stateStr, MAX_LINE_LENGTH + 1, &state)) <= 0) {
        std::cout << "No se pudo imprimir el estado del psvn" << std::endl;
        exit(1);
    };

    if (charArrayToInt(stateStr, stateInt)) {
        for (int i = 0; i < 16; i++) {
            res += std::abs( (int(i/4)) - int(stateInt[i]/4) );
            res += std::abs( (i%4) - (stateInt[i]%4));
        }
    } 

    else {
        std::cout << "No se pudo transformar de char a int";
        exit(1);
    }

    return res;
}

/*struct GreaterThan {

    bool operator()(const state_t state01, const state_t state02) const
    {
        return ;
    }
};*/

int a_star( state_t state ) {

    // Variables for iterating through state's succesors
    state_t current, child;
    ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int ruleid;             // an iterator returns a number identifying a rule
    int cost, priority;
    int *old_cost;

    // Initialization
    
    //priority_queue<state_t, vector<state_t>, GreaterThan> open;  // cola de c++ STL Library

    PriorityQueue<state_t> open;        // used for the states we have generated but not yet expanded (the OPEN list)
    state_map_t *map = new_state_map(); // contains the cost-to-goal for all states that have been generated

    state_map_add(map, &state, 0);
    open.Add(manhattan(state), manhattan(state), state);

    // Search
    while (!open.Empty()) {

        // Remove top state from priority queue
        current = open.Top();
        open.Pop();

        // Check for goal
        if (is_goal(&current)) { 
            return *state_map_get(map, &current);
        }

        init_fwd_iter( &iter, &current );  // initialize the child iterator 
    
        // Expand Node
        while( ( ruleid = next_ruleid( &iter ) ) >= 0 ) {
            apply_fwd_rule( ruleid, &current, &child );

            if (manhattan(child) ==  INT_MAX) {  // assumes safe heuristic
                continue;
            }

            cost = *state_map_get(map, &current) + get_fwd_rule_cost(ruleid);
            priority = cost + manhattan(current);
              
            //std::cout << cost << std::endl;
            //std::cout << priority << std::endl;

            // check if either this child has not been seen yet or if
            // there is a new cheaper way to get to this child.
            old_cost = state_map_get(map, &child);
            if( (old_cost == NULL) || (*old_cost > cost) ) {
                // add to open with the new distance
                state_map_add(map, &child, cost);
                open.Add(priority, priority, child);
            }
        } 
    }

    return -1;
}

int main( int argc, char **argv ) {
    
    // VARIABLES FOR INPUT
    char str[ MAX_LINE_LENGTH +1 ] ;
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    // READ A LINE OF INPUT FROM stdin
    printf("Please enter a state followed by ENTER: ");
    if ( fgets(str, sizeof str, stdin) == NULL ) {
	printf("Error: empty input line.\n");
	return 0; 
    }

    // CONVERT THE STRING TO A STATE
    nchars = read_state( str, &state );
    if (nchars <= 0) {
	printf("Error: invalid state entered.\n");
	return 0; 
    }

    //int costo = a_star(state);

    int costo = manhattan(state);

    printf("The state you entered is: ");
    print_state( stdout, &state );
    printf("And the cost to reach it is: ");
    printf("%d", costo);
    printf("\n");

    return 0;
}