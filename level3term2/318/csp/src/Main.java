import java.io.File;
import java.time.Duration;
import java.time.Instant;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws Exception {
        System.out.println("Initializing...");
        String[] fileNames = {
                "d-10-01.txt.txt",
                "d-10-06.txt.txt",
                "d-10-07.txt.txt",
                "d-10-08.txt.txt",
                "d-10-09.txt.txt",
                "d-15-01.txt.txt"
        };

        for (String fileName : fileNames) {
            System.out.println("----Backtrack in file " + fileName + "-----");
            BacktrackGame g = new BacktrackGame("data/" + fileName);
            g.initialState.print();
            Instant start = Instant.now();
            boolean valid=g.BackTrackSolve();
            Instant end = Instant.now();
            System.out.println("backtrack possible: "+valid);
            g.initialState.print();
            System.out.println("consistency checks: "+g.numberOfConsistencyChecking);
            System.out.println("fails: "+g.numberOfFails);
        }
    }


//    public static void main(String[] args) throws Exception {
//        System.out.println("----Backtrack in file " + "d-10-01.txt.txt" + "-----");
//        BacktrackGame g = new BacktrackGame("data/" + "d-10-01.txt.txt");
//        g.initialState.print();
//        boolean valid = g.BackTrackSolve();
//        System.out.println("backtrack possible: " + valid);
//        g.initialState.print();
//        System.out.println("Number of fails: "+g.numberOfFails);
//        System.out.println("Number of consistency checks: "+g.numberOfConsistencyChecking);
//
//    }
}
