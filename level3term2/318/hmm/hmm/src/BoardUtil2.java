

import java.util.Random;

public class BoardUtil2 {
    public double[][][][] transitionProbability = new double[9][9][9][9];
    public double[][] initialProbability;

    public int rowGhost = 0;
    public int colGhost = 0;

    Random random = new Random();

    public BoardUtil2(){
        initTransitionProbability();

        rowGhost = random.nextInt(9);

        colGhost = random.nextInt(9);

        initializeInitialProbabilities();
    }

    public double[][] getFinalBeliefState(double[][] partialBeliefState, int sensorColor, int sensorRow, int sensorCol){
        double[][] finalBeliefState = new double[9][9];

        double sum = 0;
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                finalBeliefState[row][col] = partialBeliefState[row][col] * probabilityOfEmissionGivenSquareHasGhost(sensorColor, sensorRow, sensorCol, row, col);
                sum += finalBeliefState[row][col];
            }
        }

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                finalBeliefState[row][col] = finalBeliefState[row][col] / sum;
            }
        }

        return finalBeliefState;
    }

    public double[][] getPartialBeliefState(){
        double[][] partialBeliefState = new double[9][9];
        for(int row = 0; row < 9; row++){
            for(int col = 0; col < 9 ; col++){
                int lowRow = Math.max(0,row-1);
                int highRow = Math.min(row+1,8);
                int lowCol = Math.max(0,col-1);
                int highCol = Math.min(col+1,8);

                for(int i = lowRow; i<=highRow;i++){
                    for(int j = lowCol; j <=highCol; j++){
                        partialBeliefState[row][col]+=(probabilityOfXGivenXprev(row,col,i,j)*initialProbability[i][j]);

                    }
                }

            }
        }

        return partialBeliefState;
    }

    public boolean isGhostHere(int row , int col){
        return this.colGhost==col && this.rowGhost==row;
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

    public void printProbabilities(double[][] initialProbability) {
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
    public int getSensorValue(int row,int col){
        int manhattan = Math.abs(row-rowGhost)+Math.abs(col-colGhost);
        if(manhattan>=Constants.GREEN_LOWER){
            return Constants.GREEN;
        }
        if(manhattan>=Constants.ORANGE_LOWER){
            return Constants.ORANGE;
        }
        return Constants.RED;
    }

    public double probabilityOfEmissionGivenSquareHasGhost(int color, int row, int col, int ghostRow, int ghostCol){
        int manhattan = Math.abs(row-ghostRow)+Math.abs(col-ghostCol);
        if(manhattan>=Constants.GREEN_LOWER && color==Constants.GREEN){
            return 1;
        }
        if(manhattan>=Constants.ORANGE_LOWER && manhattan<Constants.GREEN_LOWER && color==Constants.ORANGE){
            return 1;
        }
        if(manhattan<Constants.ORANGE_LOWER && color==Constants.RED){
            return 1;
        }
        return 0;
    }

    public double probabilityOfXGivenXprev(int newRow, int newCol, int oldRow, int oldCol){
        return transitionProbability[oldRow][oldCol][newRow][newCol];
    }


    public void initTransitionProbability(){
        for(int prevRow = 1; prevRow < 8; prevRow++){
            for(int prevCol = 1 ; prevCol<8; prevCol++){
                transitionProbability[prevRow][prevCol][prevRow+1][prevCol]=0.2;
                transitionProbability[prevRow][prevCol][prevRow-1][prevCol]=0.2;
                transitionProbability[prevRow][prevCol][prevRow][prevCol+1]=0.2;
                transitionProbability[prevRow][prevCol][prevRow][prevCol-1]=0.2;

                transitionProbability[prevRow][prevCol][prevRow+1][prevCol+1]=0.04;
                transitionProbability[prevRow][prevCol][prevRow+1][prevCol-1]=0.04;
                transitionProbability[prevRow][prevCol][prevRow-1][prevCol+1]=0.04;
                transitionProbability[prevRow][prevCol][prevRow-1][prevCol-1]=0.04;

                transitionProbability[prevRow][prevCol][prevRow][prevCol]=0.04;
            }
        }

        for(int k = 1; k< 8; k++){
            transitionProbability[k][0][k+1][0] = 0.28;
            transitionProbability[k][0][k-1][0] = 0.28;
            transitionProbability[k][0][k][1] = 0.28;
            transitionProbability[k][0][k+1][1] = 0.05;
            transitionProbability[k][0][k-1][1] = 0.05;
            transitionProbability[k][0][k][0] = 0.06;

            transitionProbability[k][8][k+1][8] = 0.28;
            transitionProbability[k][8][k-1][8] = 0.28;
            transitionProbability[k][8][k][7] = 0.28;
            transitionProbability[k][8][k+1][7] = 0.05;
            transitionProbability[k][8][k-1][7] = 0.05;
            transitionProbability[k][8][k][8] = 0.06;

            transitionProbability[0][k][0][k+1] = 0.28;
            transitionProbability[0][k][0][k-1] = 0.28;
            transitionProbability[0][k][1][k] = 0.28;
            transitionProbability[0][k][1][k+1] = 0.05;
            transitionProbability[0][k][1][k-1] = 0.05;
            transitionProbability[0][k][0][k] = 0.06;

            transitionProbability[8][k][8][k+1] = 0.28;
            transitionProbability[8][k][8][k-1] = 0.28;
            transitionProbability[8][k][7][k] = 0.28;
            transitionProbability[8][k][7][k+1] = 0.05;
            transitionProbability[8][k][7][k-1] = 0.05;
            transitionProbability[8][k][8][k] = 0.06;
        }

        transitionProbability[0][0][0][1] = 0.4;
        transitionProbability[0][0][1][0] = 0.4;
        transitionProbability[0][0][1][1] = 0.1;
        transitionProbability[0][0][0][0] = 0.1;

        transitionProbability[8][8][7][8] = 0.4;
        transitionProbability[8][8][8][7] = 0.4;
        transitionProbability[8][8][7][7] = 0.1;
        transitionProbability[8][8][8][8] = 0.1;

        transitionProbability[0][8][0][7] = 0.4;
        transitionProbability[0][8][1][8] = 0.4;
        transitionProbability[0][8][1][7] = 0.1;
        transitionProbability[0][8][0][8] = 0.1;

        transitionProbability[8][0][8][1] = 0.4;
        transitionProbability[8][0][7][0] = 0.4;
        transitionProbability[8][0][7][1] = 0.1;
        transitionProbability[8][0][8][0] = 0.1;

    }

    public void moveGhost(){
        int randomValue = random.nextInt(100);

        if(rowGhost!=0 && rowGhost!=8 && colGhost!=0 && colGhost!=8){
            if(randomValue>80){
                rowGhost++;
                return;
            }
            if(randomValue>60){
                colGhost++;
                return;
            }
            if(randomValue>40){
                rowGhost--;
                return;
            }
            if(randomValue>20){
                colGhost--;
                return;
            }
            if(randomValue>16){
                rowGhost++;
                colGhost++;
                return;
            }
            if(randomValue>12){
                rowGhost--;
                colGhost--;
                return;
            }
            if(randomValue>8){
                rowGhost++;
                colGhost--;
                return;
            }
            if(randomValue>4){
                rowGhost--;
                colGhost++;
                return;
            }

            return;
        }

        if(rowGhost==0 && colGhost!=0 && colGhost!=8){
            if(randomValue>72){
                rowGhost++;
                return;
            }
            if(randomValue>44){
                colGhost--;
                return;
            }
            if(randomValue>16){
                colGhost++;
                return;
            }
            if(randomValue>11){
                rowGhost++;
                colGhost--;
                return;
            }
            if(randomValue>6){
                rowGhost++;
                colGhost--;
                return;
            }
            return;
        }

        if(rowGhost==8 && colGhost!=0 && colGhost!=8){
            if(randomValue>72){
                rowGhost--;
                return;
            }
            if(randomValue>44){
                colGhost++;
                return;
            }
            if(randomValue>16){
                colGhost--;
                return;
            }
            if(randomValue>11){
                rowGhost--;
                colGhost++;

                return;
            }
            if(randomValue>6){
                rowGhost--;
                colGhost--;

                return;
            }
            return;
        }

        if(colGhost==0 && rowGhost!=0 && rowGhost!=8){
            if(randomValue>72){
                colGhost++;
                return;
            }
            if(randomValue>44){
                rowGhost++;
                return;
            }
            if(randomValue>16){
                rowGhost--;
                return;
            }
            if(randomValue>11){
                colGhost++;
                rowGhost++;
                return;
            }
            if(randomValue>6){
                colGhost++;
                rowGhost--;
                return;
            }
            return;
        }

        if(colGhost==8 && rowGhost!=0 && rowGhost!=8){
            if(randomValue>72){
                colGhost--;
                return;
            }
            if(randomValue>44){
                rowGhost++;
                return;
            }
            if(randomValue>16){
                rowGhost--;
                return;
            }
            if(randomValue>11){
                colGhost--;
                rowGhost++;
                return;
            }
            if(randomValue>6){
                colGhost--;
                rowGhost--;
                return;
            }
            return;
        }

        if(rowGhost==0 && colGhost==0){
            if(randomValue>60){
                rowGhost++;
                return;
            }
            if(randomValue>20){
                colGhost++;
                return;
            }
            if(randomValue>10){
                rowGhost++;
                colGhost++;
                return;
            }
            return;
        }
        if(rowGhost==0 && colGhost==8){
            if(randomValue>60){
                rowGhost++;
                return;
            }
            if(randomValue>20){
                colGhost--;
                return;
            }
            if(randomValue>10){
                rowGhost++;
                colGhost--;
                return;
            }
            return;
        }
        if(rowGhost==8 && colGhost==0){
            if(randomValue>60){
                rowGhost--;
                return;
            }
            if(randomValue>20){
                colGhost++;
                return;
            }
            if(randomValue>10){
                rowGhost--;
                colGhost++;
                return;
            }
            return;
        }
        if(rowGhost==8 && colGhost==8){
            if(randomValue>60){
                rowGhost--;
                return;
            }
            if(randomValue>20){
                colGhost--;
                return;
            }
            if(randomValue>10){
                rowGhost--;
                colGhost--;
                return;
            }
            return;
        }
    }

    public void printGhostPosition(){
        System.out.println("Ghost is in ("+rowGhost+","+colGhost+") ");
    }

    public String getProbabilityString(){
        String returnString="";
        for(int i = 0 ; i < 9 ; i++){
            for(int j = 0 ; j < 9;j++){
                returnString+=String.format("%.3f_",initialProbability[i][j]);
            }
        }

        returnString = returnString.substring(0,returnString.length()-1);

        return returnString;
    }



}
