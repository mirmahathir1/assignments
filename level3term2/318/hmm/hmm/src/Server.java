import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Date;

public class Server {
    public static BufferedReader in;
    public static OutputStream outputStream;
    public static PrintWriter pr;
    public static InputStream inputStream;
    public static Socket s;
    public static BoardUtil2 boardUtil;

    public static void main(String[] args) throws IOException {

        ServerSocket serverConnect = new ServerSocket(6789);
        System.out.println("Server started.\nListening for connections on port : 6789 ...\n");

        boardUtil = new BoardUtil2();
        boardUtil.printProbabilities(boardUtil.initialProbability);

        while(true) {
            Socket socket = serverConnect.accept();
            s = socket;
            initiateConnection();
            worker();
            endConnection();
        }
    }
    public static void worker() throws IOException {
        String receivedLine = null;
        receivedLine = in.readLine();

        if (receivedLine == null) return;

        System.out.println("received: " + receivedLine);
        String[] splits = receivedLine.split(" ");
        String[] urlparts = splits[1].split("/");

        String command = urlparts[1];


        if(command.equalsIgnoreCase("sense")){
            String[] numbers = urlparts[2].split("_");
            int sensorRow = Integer.parseInt(numbers[0]);
            int sensorCol = Integer.parseInt(numbers[1]);

            double[][] partialBeliefState=boardUtil.getPartialBeliefState();

            System.out.println("Sensor ("+sensorRow+","+sensorCol+")");

            if (boardUtil.isGhostHere(sensorRow, sensorCol)) {
                System.out.println("Ghost found!");
                sendToClient("found");
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

            sendToClient(boardUtil.getProbabilityString());

            boardUtil.moveGhost();

        }else if(command.equalsIgnoreCase("restart")) {
            boardUtil = new BoardUtil2();

            sendToClient(boardUtil.getProbabilityString());
        }
    }
    public static void initiateConnection() throws IOException {
        outputStream = s.getOutputStream();
        pr = new PrintWriter(outputStream);
        inputStream = s.getInputStream();
        in = new BufferedReader(new InputStreamReader(inputStream));
    }
    public static void endConnection() throws IOException {
        pr.close();
        outputStream.close();
        s.close();
    }

    public static void sendToClient(String text) throws IOException {

        pr.write("HTTP/1.1 200 OK\r\n");
        pr.write("Server: Java HTTP Server: 1.0\r\n");
        pr.write("Access-Control-Allow-Origin: *\r\n");
        pr.write("Date: " + new Date() + "\r\n");
        pr.write("Content-Type: text/html\r\n");
        pr.write("\r\n");
        pr.write(text);
        pr.flush();
    }

}