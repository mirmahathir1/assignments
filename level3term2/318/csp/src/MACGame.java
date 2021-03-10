import java.util.ArrayList;

public class MACGame extends ForwardCheckingGame{

    public MACGame(String fileName) throws Exception {
        super(fileName);
    }

    public boolean MAC(int row, int col, int value, StateFC stateFC) {
        if(stateFC.board[row][col]==0) {

            numberOfConsistencyChecking++;
            if(stateFC.hasCollision(row,col,value)){
                this.numberOfFails++;
                return false;
            }

            stateFC.board[row][col]=value;

            if(!stateFC.removeValueVerticallyHorizontally(row,col,value)){
                this.numberOfFails++;
                return false;
            }

            if(!arcConsistencyCheck(row,col,stateFC)){
                this.numberOfFails++;
                return false;
            }

        }

        int[] nextRowCol = stateFC.getSDF();
//        int[] nextRowCol = stateFC.getNextBlank();
//        int[] nextRowCol = stateFC.getRandomBlank();

        if(nextRowCol[0]==-1){
            this.finishedStateFC = stateFC;
            return true;
        }

//        ArrayList<Integer> domain = stateFC.domains[nextRowCol[0]][nextRowCol[1]];
        ArrayList<Integer> domain = stateFC.domains[nextRowCol[0]][nextRowCol[1]];

        for(Integer i: domain){
            if(MAC(nextRowCol[0],nextRowCol[1],i, new StateFC(stateFC))){
                return true;
            }
        }

        numberOfFails++;
        return false;
    }

    public boolean MACSolve(){
        preprocessDomains();
        return MAC(0,0,0, initialStateFC);
    }

    public boolean arcConsistencyCheck(int row, int col, StateFC stateFC){
        if(!arcConsistencyCheckVertically(col,stateFC)){
            return false;
        }
        if(!arcConsistencyCheckHorizontally(row, stateFC)){
            return false;
        }


        return true;
    }

    public boolean arcConsistencyCheckVertically(int col, StateFC stateFC){
        for(int i = 0 ; i < boardSize; i++){
            if(stateFC.board[i][col]==0 && stateFC.domains[i][col].size()==1){
                for(int j = 0 ; j < boardSize; j++){
                    if(stateFC.board[j][col]==0 && j!=i){
                        int[] domainOfJ = stateFC.domains[j][col].stream().mapToInt(x -> x).toArray();
                        for(int oneOfDomainOfJ: domainOfJ){
                            if(oneOfDomainOfJ==stateFC.domains[i][col].get(0)){
                                stateFC.removeValueFromDomain(j,col,oneOfDomainOfJ);
                                if(stateFC.domains[j][col].size()==0){
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    public boolean arcConsistencyCheckHorizontally(int row, StateFC stateFC){
        for(int i = 0 ; i < boardSize; i++){
            if(stateFC.board[row][i]==0 && stateFC.domains[row][i].size()==1){
                for(int j = 0 ; j < boardSize; j++){
                    if(stateFC.board[row][j]==0 && j!=i){
                        int[] domainOfJ = stateFC.domains[row][j].stream().mapToInt(x -> x).toArray();
                        for(int oneOfDomainOfJ: domainOfJ){
                            if(oneOfDomainOfJ==stateFC.domains[row][i].get(0)){
                                stateFC.removeValueFromDomain(row,j,oneOfDomainOfJ);
                                if(stateFC.domains[row][j].size()==0){
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
    }
}
