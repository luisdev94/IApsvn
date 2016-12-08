opt_synth : OptSynth.o MiniSat_v1.14/Solver.o
	g++ -O3 OptSynth.o MiniSat_v1.14/Solver.o -o opt_synth

MiniSat_v1.14/Solver.o : MiniSat_v1.14/Solver.C MiniSat_v1.14/Solver.h MiniSat_v1.14/SolverTypes.h MiniSat_v1.14/VarOrder.h
	g++ -O3 -o MiniSat_v1.14/Solver.o -c MiniSat_v1.14/Solver.C

OptSynth.o : OptSynth.C MiniSat_v1.14/Solver.h MiniSat_v1.14/SolverTypes.h MiniSat_v1.14/VarOrder.h
	g++ -O3 -o OptSynth.o -c OptSynth.C
