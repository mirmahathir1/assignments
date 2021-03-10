public class BacktrackGame extends Game{
    public final State initialState;

    public BacktrackGame(String fileName) throws Exception {
        super(fileName);
        this.initialState = new State(super.rawBoard);
    }

    public boolean BackTrack(int row, int col, int value){

        if(initialState.board[row][col]==0){

            boolean collision = initialState.hasCollision(row,col,value);
            numberOfConsistencyChecking++;
            if(collision){
                numberOfFails++;
                return false;
            }else{
                initialState.board[row][col] = value;
            }
        }

        int[] nextRowCol = initialState.getNextBlank();

        if(nextRowCol[0]==-1){
            return true;
        }

        for(int possibleCandidate = 1; possibleCandidate <= boardSize; possibleCandidate++){
            if(BackTrack(nextRowCol[0],nextRowCol[1],possibleCandidate)){
                return true;
            }
        }

        //undo
        initialState.board[row][col]=0;
        numberOfFails++;
        return false;
    }



    public boolean BackTrackSolve(){
        return BackTrack(0,0,0);
    }
}
