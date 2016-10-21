unsigned manhattan (state_t state){
    unsigned res = 0;
    for (int i = 0; i<16; i++) {
        res += std::abs( (int(i/4)) - int(state.vars[i]/4) );
        res += std::abs( (i%4) - (state.vars[i]%4));
    }
    return res;
}


unsigned gap28 (state_t state){
    unsigned res = 0;
    for (int i = 0; i < 27, i++){            
        if (res < abs(state.vars[i] - state.vars[i+1])) { 
            res = abs(state.vars[i] - state.vars[i+1]);
        }
    }
    return res;
}