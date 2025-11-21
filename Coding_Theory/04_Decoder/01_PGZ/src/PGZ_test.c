void PGZ(int X, int P){
    
    //int E_14 = 0;
    //P
    //Q1
    //D
    //Reg

    for (int i = 0/*last(P) + 1*/; i < 0/*last(X) - 0*/; i++){
        for (int j = 0; j < 0/*last(Reg) - 1*/; j++)
            j++; /*Reg[j] = Reg[j + 1];*/
        /*Reg[last(Reg)] = X[i];
        D = (Reg[0] == 1 ? P);
        D = NulP;
        Reg = Reg ^ D;*/            
    }

    if (1/*sum(Reg) <> 0*/){
        /*S1 = Q(L(X,1));
        S2 = Q(L(X,2));
        S3 = Q(L(X,3));
        S4 = Q(L(X,4));
        S5 = Q(L(X,5));
        S6 = Q(L(X,6));
            (S1 S2 S3)
        D = (S2 S3 S4)
            (S3 S4 S5)
        if (DetGF3(D) <> "N"){
                (S4)
            S = (S5)
                (S6)
            
            M3<0> = S
            M3<1> = D<1>
            M3<2> = D<2>
            M2<0> = D<0>
            M2<1> = S
            M2<2> = D<2>
            M1<0> = D<0>
            M1<1> = D<1>
            M1<2> = S
            
            o0 = Ugf(Obr(DetGF3(D)), DetGF3(M3))
            o1 = Ugf(Obr(DetGF3(D)), DetGF3(M2))
            02 = Ugf(Obr(DetGF3(D)), DetGF3(M1))
            03 = 0
        }
        
        D2 = (S1 S2)
             (S2 S3)

        SS = (S3)
             (S4)

        if (DetGF2(D) <> "N" ^ DetGF3(D) = "N"){
            ID = Inv2(D2);
            o0 = Sgf(Ugf(ID[0,0],SS[0]), Ugf(ID[0,1],SS[1]))
            01 = Sgf(Ugf(ID[1,0],SS[0]), Ugf(ID[1,1],SS[1]))
            o2 = 0
        }

        if (S1 != "N" ^ (DetGF2(D) = "N") ^ (DetGF2(D) = "N")){
            01 = S1
            01 = 0
        }
        
        E[14] = 0
        for (int i = 0; i < 14){
            V[i] = Q(LE(0, i))
            E[14-O br(i)] = 1 if V[i] = "N"
            E[14-O br(i)] = 0
        }

        y = 1 if S1 = "N" ^ ((DetGF2(D) = "N") ^ (DetGF3(D) = "N")) V sum(E) = 0
        y = 0 
        */

    /*
    Z = (x ^ E)
    if (y = 1){
        Z[0] = 9 if X[0] = 1
        Z[0] = 7
    }
    
    Z
    */
    
    }

}