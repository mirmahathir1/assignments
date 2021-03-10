import java.util.Random;

public class BoardUtil {
    private double[][][] probabilityOfRGivenX;

    private double[][] initialProbability;

    private int ghostRow = 0;
    private int ghostCol = 0;

    Random random = new Random();

    public BoardUtil() {
        //int[][][] sensorCountValues = initializeSensorCountValues();

        //populateAllProbableSensorValueCounts(sensorCountValues);

        //this.probabilityOfRGivenX = populateProbabilityOfRGivenX(sensorCountValues);

        initializeInitialProbabilities();

        ghostRow = random.nextInt(9);

        ghostCol = random.nextInt(9);
    }

    public void moveGhost() {
        int option = random.nextInt(100);

        if(ghostCol!=0 && ghostRow!=0){
        if (option > 80) {
            System.out.println("move up");
            ghostRow = checkBoardWall(ghostRow-1);
            return;
        }

        if(option > 60){
            System.out.println("move down");
            ghostRow = checkBoardWall(ghostRow+1);
            return;
        }

        if(option > 40){
            System.out.println("move left");
            ghostCol = checkBoardWall(ghostCol-1);
            return;
        }

        if(option > 20){
            System.out.println("move right");
            ghostCol = checkBoardWall(ghostCol+1);
            return;
        }

        if(option > 16){
            System.out.println("move up right");
            ghostRow = checkBoardWall(ghostRow-1);
            ghostCol = checkBoardWall(ghostCol+1);
            return;
        }

        if(option > 12){
            System.out.println("move up left");
            ghostRow = checkBoardWall(ghostRow-1);
            ghostCol = checkBoardWall(ghostCol-1);
            return;
        }

        if(option > 8){
            System.out.println("move down right");
            ghostRow = checkBoardWall(ghostRow+1);
            ghostCol = checkBoardWall(ghostCol+1);
            return;
        }

        if(option > 4){
            System.out.println("move down left");
            ghostRow = checkBoardWall(ghostRow+1);
            ghostCol = checkBoardWall(ghostCol-1);
            return;
        }

        System.out.println("move none");
        }

    }

    public boolean isGhostHere(int row , int col){
        return this.ghostCol==col && this.ghostRow==row;
    }

    public int getSensorValue(int row, int col) {
        return getSensorValue(this.ghostRow, this.ghostCol, row, col);
    }

    public double getProbabilityOfX(int row, int col) {
        return initialProbability[row][col];
    }

    public void setProbabilityOfX(int row, int col, double probability) {
        initialProbability[row][col] = probability;
    }

    private void initializeInitialProbabilities() {
        initialProbability = new double[9][];
        for (int i = 0; i < 9; i++) {
            initialProbability[i] = new double[9];
            for (int j = 0; j < 9; j++) {
                initialProbability[i][j] = 1 / 81.0;
            }
        }
    }

    public void printGhostPosition(){
        System.out.println("Ghost is in ("+ghostRow+","+ghostCol+") ");
    }


    public double getProbabilityOfRGivenX(int row, int col, int color) {
        return this.probabilityOfRGivenX[row][col][color];
    }

    public double getProbabilityOfXGivenPreviousX(int newRow, int newCol, int oldRow, int oldCol) {
        int colDif = Math.abs(newCol - oldCol);
        int rowDif = Math.abs(newRow - oldRow);
        if ((colDif == 1 && rowDif == 0) || (colDif == 0 && rowDif == 1)) {
            return 0.2;
        }

        if ((colDif == 1 && rowDif == 1) || (colDif == 0 && rowDif == 0)) {
            return 0.04;
        }

        return 0;

    }

    private static int getSensorValue(int rowGhost, int colGhost, int rowSensor, int colSensor) {
        if (Math.abs(rowGhost - rowSensor) <= 2 && Math.abs(colGhost - colSensor) <= 2) {
            return Constants.RED;
        }

        if (Math.abs(rowGhost - rowSensor) == 3 && Math.abs(colGhost - colSensor) <= 3
                || Math.abs(rowGhost - rowSensor) <= 3 && Math.abs(colGhost - colSensor) == 3) {
            return Constants.ORANGE;
        }

        return Constants.GREEN;
    }

    private static int[][] getAllSensorValues(int ghostRow, int ghostCol) {
        int[][] board = new int[9][];
        for (int i = 0; i < 9; i++) {
            board[i] = new int[9];
        }

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                board[row][col] = getSensorValue(ghostRow, ghostCol, row, col);
            }
        }

        return board;

    }

    public static void printAllSensorValues(int[][] board) {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                System.out.print(Constants.COLORS[board[row][col]] + " ");
            }
            System.out.println();
        }
    }

    public static void printAllSensorCountValues(int[][][] allSensorCounts) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                for (int k = 0; k < 3; k++) {
                    System.out.print(allSensorCounts[i][j][k] + ",");
                }
                System.out.print(" ");
            }
            System.out.println();
        }
    }

    private static int[][][] initializeSensorCountValues() {
        int[][][] allSensorCounts = new int[9][][];
        for (int i = 0; i < 9; i++) {
            allSensorCounts[i] = new int[9][];
            for (int j = 0; j < 9; j++) {
                allSensorCounts[i][j] = new int[3];
            }
        }
        return allSensorCounts;
    }

    private static void populateAllProbableSensorValueCounts(int[][][] allSensorCounts) {
        for (int ghostRow = 0; ghostRow < 9; ghostRow++) {
            for (int ghostCol = 0; ghostCol < 9; ghostCol++) {
                int[][] allSensorValues = BoardUtil.getAllSensorValues(ghostRow, ghostCol);
                for (int row = 0; row < 9; row++) {
                    for (int col = 0; col < 9; col++) {
                        allSensorCounts[row][col][allSensorValues[row][col]]++;
                    }
                }
            }
        }
    }

    private static double[][][] initializeProbabilities() {
        double[][][] allSensorCounts = new double[9][][];
        for (int i = 0; i < 9; i++) {
            allSensorCounts[i] = new double[9][];
            for (int j = 0; j < 9; j++) {
                allSensorCounts[i][j] = new double[3];
            }
        }
        return allSensorCounts;
    }

    private static double[][][] populateProbabilityOfRGivenX(int[][][] allSensorCounts) {
        double[][][] probabilitiesOfSensor = initializeProbabilities();

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                double sum = allSensorCounts[row][col][0] + allSensorCounts[row][col][1] + allSensorCounts[row][col][2];
                for (int color = 0; color < 3; color++) {
                    probabilitiesOfSensor[row][col][color] = allSensorCounts[row][col][color] / sum;
                }
            }
        }
        return probabilitiesOfSensor;
    }

    public void printProbabilityOfRGivenX() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                for (int k = 0; k < 3; k++) {
                    System.out.print(probabilityOfRGivenX[i][j][k] + ",");
                }
                System.out.print(" ");
            }
            System.out.println();
        }
    }

    public void printProbabilities() {
        System.out.print("     ");
        for(int col = 0 ; col < 9; col++){
            System.out.print("col"+col+"  ");
        }
        System.out.println();

        for (int row = 0; row < 9; row++) {
            System.out.print("row"+row+" ");
            for (int col = 0; col < 9; col++) {
                //System.out.print(initialProbability[row][col]+" ");
                System.out.printf("%.3f ", initialProbability[row][col]);
            }
            System.out.println();
        }
    }

    public int checkBoardWall(int value){
        return Math.min(Math.max(0,value),8);
    }

    public void updateProbabilities(int row,int col, int value){
        System.out.println("Updating probabilities");

    }
}
