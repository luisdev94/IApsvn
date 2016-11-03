#include <vector>
#include "priority_queue.hpp"
struct Node {
  int value;
  Node *parent;

  Node(int val) {
    this->value = val;
  }

  Node(int val, Node *father) {
    this->value = val;
    this->parent = father;
  }
};


int main(int argc, char **argv) {
	/*
	state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
  int d, ruleid;
  ruleid_iterator_t iter;

  PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
  state_map_t *map = new_state_map(); // contains the cost-to-goal for all states that have been generated
  FILE *file; // the final state_map is written to this file if it is provided (command line argument)

	*/

  Node *a = new Node(2);
  Node *b = new Node(3,a);

  printf("%d %d\n", b->value, b->parent->value);

  //const char* str = "b 3 1 5 6 9 10 11 2 8 7 4\n"; // initial state

  //printf("%s", str);

  //read_state(str, &state);

  /*
  open.Add(0, 0, state);

  d = 0;
  while(d < 3) {
  	state = open.Top();
  	open.Pop();

  	
  }
  */
  return 0;
}