import java.util.ArrayList;

public class ForwardCheckingGame extends Game {
    StateFC initialStateFC;
    StateFC finishedStateFC;

    public ForwardCheckingGame(String fileName) throws Exception {
        super(fileName);
        this.initialStateFC = new StateFC(super.rawBoard);
    }

    public boolean ForwardCheck(int row, int col, int value, StateFC stateFC) {
        if (stateFC.board[row][col] == 0) {

            numberOfConsistencyChecking++;
            if (stateFC.hasCollision(row, col, value)) {
                this.numberOfFails++;
                return false;
            }

            stateFC.board[row][col] = value;

            if (!stateFC.removeValueVerticallyHorizontally(row, col, value)) {
                this.numberOfFails++;
                return false;
            }


        }

        //APPLY HEURISTICS HERE
        int[] nextRowCol = stateFC.getSDF();
//        int[] nextRowCol = stateFC.getNextBlank();
//        int[] nextRowCol = stateFC.getRandomBlank();

        if (nextRowCol[0] == -1) {
            this.finishedStateFC = stateFC;
            return true;
        }

        ArrayList<Integer> domain = stateFC.domains[nextRowCol[0]][nextRowCol[1]];

        for (Integer i : domain) {
            if (ForwardCheck(nextRowCol[0], nextRowCol[1], i, new StateFC(stateFC))) {
                return true;
            }
        }

        numberOfFails++;
        return false;
    }

    public boolean preprocessDomains() {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (initialStateFC.board[i][j] != 0) {
                    if (!initialStateFC.removeValueVerticallyHorizontally(i, j, initialStateFC.board[i][j])) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    public boolean ForwardCheckSolve() {
        preprocessDomains();
        return ForwardCheck(0, 0, 0, initialStateFC);
    }


}
