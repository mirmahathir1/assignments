import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;

public class Draft {
    public static void main(String[] args) throws Exception {
        ForwardCheckingGame g = new ForwardCheckingGame("data/" + "d-10-01.txt.txt");
        g.initialStateFC.print();
        boolean valid=g.ForwardCheckSolve();
        System.out.println("forward check possible: "+valid);
        if(valid){
            g.finishedStateFC.print();
        }



    }
}
