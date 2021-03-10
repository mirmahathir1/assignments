import java.util.ArrayList;
import java.util.Random;

public class StateFC extends State {
//    public ArrayList<Integer>[][] domains;
    public ArrayList<Integer>[][] domains;

    public Random random= new Random();

    public StateFC(int[][] board) {
        super(board);

        domains = new ArrayList[boardSize][];
        for(int i = 0 ; i < boardSize;i++){
            domains[i]= new ArrayList[boardSize];
            for(int j = 0; j < boardSize; j++){
                domains[i][j]= new ArrayList<>();
                for(int k = 1; k <=boardSize; k++){
                    domains[i][j].add(k);
                }
            }
        }


    }
    public StateFC(StateFC stateFC){
        super(stateFC.board);

        domains = new ArrayList[boardSize][];
        for(int i = 0 ; i < boardSize; i++){
            domains[i] = new ArrayList[boardSize];
            for(int j = 0 ; j < boardSize; j++){
                domains[i][j] = new ArrayList<>(stateFC.domains[i][j]);
            }
        }
    }
    public boolean removeValueFromDomain(int row, int col, int value){
        domains[row][col].remove(Integer.valueOf(value));
        if(this.board[row][col]==0 && domains[row][col].size()==0){
            return false;
        }
        return true;
    }

    public void printDomainOf(int row,int col){
        System.out.println("Printing domain of row:"+row+", col:"+col);
        for(Integer i:domains[row][col]){
            System.out.print(i+" ");
        }
        System.out.println();
    }

    public boolean removeValueVerticallyHorizontally(int row, int col,int value){
        for(int i = 0 ; i < boardSize; i++){
            if(!removeValueFromDomain(i,col,value))
                return false;

            if(!removeValueFromDomain(row,i,value)){
                return false;
            }
        }
        return true;
    }

    public int[] getSDF(){
        int[] returnValue = new int[2];
        returnValue[0]=-1;
        returnValue[1]=-1;
        int shortestDomainLength = 9999;
        for(int i = 0; i < boardSize;i++){
            for(int j = 0 ; j < boardSize ; j++){
                if(board[i][j]==0 && this.domains[i][j].size()<shortestDomainLength){
                    returnValue[0]=i;
                    returnValue[1]=j;
                    shortestDomainLength = this.domains[i][j].size();
                }
            }
        }
        return returnValue;
    }

    public int[] getRandomBlank(){
        int[] returnValue = new int[2];
        returnValue[0]=-1;
        returnValue[1]=-1;

        ArrayList<Integer> rowNumbersOfBlanks = new ArrayList<>();
        ArrayList<Integer> colNumbersOfBlanks = new ArrayList<>();

        for(int i = 0; i < boardSize;i++){
            for(int j = 0 ; j < boardSize ; j++){
                if(board[i][j]==0){
                    rowNumbersOfBlanks.add(i);
                    colNumbersOfBlanks.add(j);
                }
            }
        }

        if(rowNumbersOfBlanks.size()==0){
            return returnValue;
        }

        int index = random.nextInt(rowNumbersOfBlanks.size());

        returnValue[0]= rowNumbersOfBlanks.get(index);
        returnValue[1]= colNumbersOfBlanks.get(index);

        return returnValue;

    }

}
