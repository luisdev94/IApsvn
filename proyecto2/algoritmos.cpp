minimax(state_t node, unsigned depth)
	if ((depth == 0) || (node.terminal())) {
		return h(node);
	}
	score := INT_MAX;
	foreach child of node
		score := min(score, maximin(child, depth - 1))
	return score

	
maximin(state_t node, unsigned depth)
	if depth == 0 || node is terminal
		return h(node)
	score := INT_MIN;
	foreach child of node
		score := max(score, minimax(child, depth - 1))
	return score

