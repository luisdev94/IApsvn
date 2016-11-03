#include <iostream>
#include <math.h>
#include <vector>
#include "priority_queue.hpp"
#include <sstream>
#define MAX_LINE_LENGTH 999
using namespace std;

struct Node {
	const state_t *state;
	Node *parent;
	const char* action;
	unsigned g;

	Node(const state_t *value) {
		this->state = value;
	}

	Node(const state_t *value, Node *father, const char* label, unsigned cost) {
		this->state = value;
		this->parent = father;
		this->action = label;
		this->g = cost;
	}
};

void generateChildren(Node *node, int depth, int *cs, int *h) {
	if (depth > 0) {
		int childCount = 0;
		int children = 0;
		int ruleid;
		int child_hist;
		state_t child;
		ruleid_iterator_t iter;
		init_fwd_iter(&iter, node->state);  // initialize the child iterator 
	  while( (ruleid = next_ruleid(&iter)) >= 0 ) {
	  	if (!fwd_rule_valid_for_history(*h, ruleid)) {
	  		continue;
	  	}
	  	child_hist = next_fwd_history(*h, ruleid);
	    apply_fwd_rule(ruleid, node->state, &child);
	    ++childCount;
	    Node *leaf = new Node(&child, node, get_fwd_rule_label(ruleid), get_fwd_rule_cost(ruleid));
	    generateChildren(leaf, depth - 1, cs, &child_hist);
	  }
	  cs[depth-1] += childCount;
	  if( childCount == 0 )
	    printf("Your state has no children.\n");
	}
}

int main(int argc, char **argv) {
	
	state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
  int d, ruleid, hist;
  ruleid_iterator_t iter;

  //const char* str = "b 3 1 5 6 9 10 11 2 8 7 4"; // initial state

  char string [MAX_LINE_LENGTH + 1];

  printf("Please enter a state followed by ENTER: ");
  if( fgets(string, sizeof string, stdin) == NULL ) {
      printf("Error: empty input line.\n");
      return 0; 
  }

	read_state(string, &state);

	Node *root = new Node(&state);

	printf("Please enter depth followed by ENTER: ");
  if( fgets(string, sizeof string, stdin) == NULL ) {
      printf("Error: empty input line.\n");
      return 0; 
  }

  d = 0;

  stringstream(string) >> d;

  //stringstream convert(string);

	//convert>>d;

	int kids[d];

	for (int i = 0; i < d; i ++) {
		kids[i] = 0;
	}

	hist = init_history;

	generateChildren(root, d, kids, &hist);
	printf("1 nodo hijo en la profunidad 0\n");
	for (int i = 0; i < d; i ++ ) {
		printf("%d nodos hijos en la profundidad %d\n",kids[d-i-1],i+1);
	}
  return 0;
}