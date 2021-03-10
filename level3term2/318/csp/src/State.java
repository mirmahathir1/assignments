import java.util.ArrayList;

public class State {
    public final int[][] board;
    public int boardSize;

    public State(int[][] board){
//        this.board = board;
        this.boardSize = board.length;
        this.board = new int[boardSize][];
        for(int i = 0 ; i < boardSize;i++){
            this.board[i]= board[i].clone();
        }


    }
    public void print(){
        for(int i = 0 ; i < boardSize; i++){
            System.out.print("__");
        }
        System.out.println();
        for(int i = 0 ; i < boardSize; i++){
            for(int j = 0 ; j < boardSize ; j++){
                System.out.print(board[i][j]+" ");
            }
            System.out.println();
        }
        for(int i = 0 ; i < boardSize; i++){
            System.out.print("__");
        }
        System.out.println();
    }

    public boolean hasCollision(int row, int col, int value){
        if(value==0) {
            return false;
        }
        for (int i = 0 ; i < boardSize; i++){
            if(board[row][i]==value || board[i][col]==value)
                return true;
        }

        return false;
    }
    public int[] getNextBlank(){
        int[] returnValue = new int[2];
        returnValue[0]=-1;
        returnValue[1]=-1;
        for(int i = 0; i < boardSize;i++){
            for(int j = 0 ; j < boardSize ; j++){
                if(board[i][j]==0){
                    returnValue[0]=i;
                    returnValue[1]=j;
                    return returnValue;
                }
            }
        }
        return returnValue;
    }


}
