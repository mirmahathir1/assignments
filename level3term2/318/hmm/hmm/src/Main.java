import java.util.Scanner;

public class Main {
    public static void main(String[] args){
        BoardUtil2 boardUtil = new BoardUtil2();

        Scanner scanner = new Scanner(System.in);

        boardUtil.printProbabilities(boardUtil.initialProbability);


        while(true) {

            double[][] partialBeliefState=boardUtil.getPartialBeliefState();


            int sensorRow, sensorCol;

            System.out.println("Enter row of Sensor:");
            sensorRow = scanner.nextInt();
            System.out.println("Enter col of Sensor:");
            sensorCol = scanner.nextInt();

            if (boardUtil.isGhostHere(sensorRow, sensorCol)) {
                System.out.println("Ghost found!");
                return;
            }

            System.out.println("Ghost not found");

            int sensorColor = boardUtil.getSensorValue(sensorRow, sensorCol);

            System.out.println("Sensor color is: " + Constants.COLORS[sensorColor]);

            boardUtil.printGhostPosition();

            //System.out.println(boardUtil.getProbabilityString());

            double[][] finalBeliefState = boardUtil.getFinalBeliefState(partialBeliefState, sensorColor, sensorRow, sensorCol);


            System.out.println("final belief State: ");
            boardUtil.printProbabilities(finalBeliefState);

            boardUtil.initialProbability = finalBeliefState;



            boardUtil.moveGhost();

        }


    }


}
