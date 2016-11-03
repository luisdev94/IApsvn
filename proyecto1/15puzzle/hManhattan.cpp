#include <iostream>
#include <algorithm>
#include <math.h>
#include <limits.h>



#define  MAX_LINE_LENGTH 40 
state_t state;

/*
Recibe un string correspondiente al estado del 15puzzle psvn,
recibe un array de 16 enteros, y transforma los chars a ints.
la letra B correspondiente al Blank lo convierte en 0*/
int charArrayToInt (char * stateStr, int * estado){
    int j = 0;
    int temp = 0;
    for (unsigned i = 0; (i< strlen(stateStr)) ; i++) {
        if ((stateStr[i] == 'B') or (stateStr[i] == 'b')) {
            temp = 0;
        } else if (stateStr[i] == ' '){
            estado[j] = temp;
            temp = 0;
            // std::cout << "Asignado el valor: " << estado[j] << std::endl;
            j++;
        } else {
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
    if ((sprint_state(stateStr,MAX_LINE_LENGTH + 1, &state)) <= 0) {
        std::cout << "No se pudo imprimir el estado del psvn";
        exit(1);
    };

    if (charArrayToInt(stateStr,stateInt)) {
        for (int i = 0; i<16; i++) {
            res += std::abs( (int(i/4)) - int(stateInt[i]/4) );
            res += std::abs( (i%4) - (stateInt[i]%4));
        }
    } else {
        std::cout << "No se pudo transformar de char a int";
        exit(1);
    }

    return res;
}

std::pair<bool, unsigned> f_bounded_dfs_visit (unsigned bound, unsigned g) {
    std::pair<bool, unsigned> res;
    // unsigned f = g + h(state);
    unsigned f = g;
    if (f > bound) {
        res = std::make_pair (false,f);
        return res;
    }
    // if (state.is-goal()) {
        res = std::make_pair (true,g);
        return res;
    // }

    unsigned t = UINT_MAX;
    // para cada hijo del estado.
        // unsigned cost = g + c(state,a); // Costo de ejecutar la accion a desde state
        unsigned cost = g; // Costo de ejecutar la accion a desde state
        // state = f(state,a); // Ejecutar la la accion a en state
        // if (h(state) < UINT_MAX) {
            // path.push-back(a);
            std::pair<bool, unsigned> p = f_bounded_dfs_visit(bound,cost);
            if (p.first) {
                return p;
            }
            t = std::min(t,p.second);
            // path.pop-back(a)
        // }
        // state = f(state, inverse(a));
    res = std::make_pair (false,t);
    return res;
}




int main(int argc, char **argv ) {
    // VARIABLES FOR INPUT
    char str[MAX_LINE_LENGTH + 1];
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    state_t child;
    ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int ruleid; // an iterator returns a number identifying a rule
    int childCount;



/* INICIALIZANDO */
/* STATE = INIT() */
    
    // READ A LINE OF INPUT FROM stdin
    printf("Please enter a state followed by ENTER: ");
    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }

    // CONVERT THE STRING TO A STATE
    nchars = read_state(str, &state);
    if( nchars <= 0 ) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }

    printf("The state you entered is: ");
    print_state(stdout, &state);
    printf("\n");

    std::cout << "La Heuristica dice: " << manhattan(state) << std::endl;

    // char stateStr[MAX_LINE_LENGTH + 1];
    // if ((sprint_state(stateStr,MAX_LINE_LENGTH + 1, &state)) > 0) {
    //     std::cout << stateStr << std::endl;
    // } else {
    //     std::cout << "Imprimio un error";
    // };


    // int estado[16];
    // charArrayToInt(stateStr,estado);
    // for (int i = 0; i<16; i++){
    //     std::cout << estado[i] << " ";
    // }   std::cout << std::endl;



/* INICIALIZANDO */
    unsigned bound = cost_of_cheapest_fwd_rule;
    for (;;) {
        std::pair<bool, unsigned> p = f_bounded_dfs_visit(bound,0);
        if (p.first) return p.second;
        bound = p.second;
    }



    // LOOP THOUGH THE CHILDREN ONE BY ONE
    childCount = 0;
    init_fwd_iter(&iter, &state);  // initialize the child iterator 
    while( (ruleid = next_ruleid(&iter)) >= 0 ) {
        apply_fwd_rule(ruleid, &state, &child);
        ++childCount;
        printf("child %d. ",childCount);
        print_state(stdout, &child);
        printf("  %s (cost %d), goal=%d\n", get_fwd_rule_label(ruleid), get_fwd_rule_cost(ruleid), is_goal(&child));
    }
    if( childCount == 0 )
        printf("Your state has no children.\n");

    return 0;
}

