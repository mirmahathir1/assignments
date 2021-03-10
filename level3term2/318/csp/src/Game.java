import java.io.File;
import java.util.Scanner;

public abstract class Game {
    public int boardSize;
    public final int[][] rawBoard;

    public long numberOfFails = 0;
    public long numberOfConsistencyChecking = 0;

    public Game(String fileName) throws Exception {
        rawBoard = loadFile(fileName);
    }

    public int[][] loadFile(String fileName) throws Exception{
        Scanner scanner = new Scanner(new File(fileName));

        String boardSizeString = scanner.nextLine();
        int boardSize = Integer.parseInt(boardSizeString.replaceAll("[N=;]", ""));

        int[][] board = new int[boardSize][];
        for(int i = 0 ; i < boardSize; i++){
            board[i]= new int[boardSize];
        }
        scanner.nextLine();
        scanner.nextLine();
        for(int i = 0; i < boardSize;i++){
            String[] numberStrings = scanner.nextLine().replaceAll("[ |;\\]]","").split(",");
            for(int j = 0 ; j < boardSize; j++){
                board[i][j] = Integer.parseInt(numberStrings[j]);
            }
        }

        scanner.close();

        this.boardSize = boardSize;

        return board;
    }




}
