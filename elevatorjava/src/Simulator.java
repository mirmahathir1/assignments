import javax.swing.plaf.synth.SynthLookAndFeel;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;
import java.util.Scanner;
import java.util.concurrent.ThreadLocalRandom;

public class Simulator {
    public final int TOTAL_MAX_CUSTOMERS = 1000000;
    public final int TOTAL_ELEVATORS = 4;
    public final int TOTAL_FLOORS = 12;
    public final int MAX_OCCUPANCY = 12;
    public final int MAX_BETWEEN_TIME = 25;
    public final int TIME_DELAY_TO_WAIT_FOR_PASSENGER = 15;
    public final int TIME_TO_TRAVEL_BETWEEN_FLOORS = 5;
    public final int TIME_FOR_A_PASSENGER_TO_DISEMBARK = 3;
    public final int TIME_FOR_A_PASSENGER_TO_EMBARK = 3;
    public final int OPEN_TIME_OF_DOOR = 3;
    public final int CLOSE_TIME_OF_DOOR = 3;
    public final int TOTAL_TIME_OF_SIMULATION = 10000;
    public final double MEAN_INTERARRIVAL_TIME = 1.5;
    public final int MAX_BATCH_SIZE = 6;


    public double DELTIME = 0;
    public double ELEVTIME = 0;
    public double MAXDEL = 0;
    public int MAXELEV = 0;
    public int QUELEN = 0;
    public double SUM_OF_QUELEN = 0;
    public double QUE_LENGTH_COUNTER = 0;
    public int QUETIME = 0;
    public double MAXQUE = 0;
    public double SUM_OF_QUETIME = 0;
    public double COUNT_OF_QUETIME = 0;
    public int quetotal = 0;
    public int remain = 0;

    // these three are counted from floor 1
    public double[] load_count = new double[TOTAL_ELEVATORS+1];
    public double[] max_load = new double[TOTAL_ELEVATORS+1];
    public double[] total_load = new double[TOTAL_ELEVATORS+1];

    public double[] between;
    public int[] floor;
    public double[] delivery;
    public int[] first;
    public int[] occup;
    public int[][] selvec;
    public int[][] flrvec;
    public int[] elevator;
    public int[] eldel;
    public double[] arrive;

    public int i = 0;
    public int j = 0;
    public int k = 0;

    public double TIME;
    public double[] return_to_ground;
    public int[] stop;
    public int[] operate;
    public double[] wait;

    public int limit = 0;

    public int quecust = 0;
    public double startque = 0;
    public int queue = 0;
    public int R;


    //variables outside algorithm
    public Method method;
    public boolean jumpedFrom30 = false;
    public Random rand = new Random();
    public String methodString = "";

    public static int randInt(int min, int max) {
        return ThreadLocalRandom.current().nextInt(min, max + 1);
    }

    public Simulator() {
        // step 1
        between = new double[TOTAL_MAX_CUSTOMERS + 1];
        floor = new int[TOTAL_MAX_CUSTOMERS + 1];
        delivery = new double[TOTAL_MAX_CUSTOMERS + 1];

        int batch_size = 0;
        for (int c = 0; c < TOTAL_MAX_CUSTOMERS + 1; c++) {
            if(batch_size!=0){
                between[c] = 0;
            }else{
                between[c] = getNext()*60;
            }

            if(batch_size==0){
                batch_size = randInt(1,MAX_BATCH_SIZE);
            }

            floor[c] = randInt(2, TOTAL_FLOORS);
            delivery[c] = 0;
            batch_size--;
        }

        first = new int[TOTAL_ELEVATORS + 1];
        occup = new int[TOTAL_ELEVATORS + 1];
        selvec = new int[TOTAL_ELEVATORS + 1][];
        for (int c = 0; c < TOTAL_ELEVATORS + 1; c++) {
            selvec[c] = new int[TOTAL_FLOORS + 1];
        }
        flrvec = new int[TOTAL_ELEVATORS + 1][];
        for (int c = 0; c < TOTAL_ELEVATORS + 1; c++) {
            flrvec[c] = new int[TOTAL_FLOORS + 1];
        }
        elevator = new int[TOTAL_MAX_CUSTOMERS + 1];
        eldel = new int[TOTAL_ELEVATORS + 1];
        arrive = new double[TOTAL_MAX_CUSTOMERS + 1];

        // step 2
        i = 1;
        delivery[i] = TIME_DELAY_TO_WAIT_FOR_PASSENGER;

        // step 3
        TIME = between[i];
        return_to_ground = new double[TOTAL_ELEVATORS + 1];
        for (int c = 0; c < TOTAL_ELEVATORS + 1; c++) {
            return_to_ground[c] = TIME;
        }
        stop = new int[TOTAL_ELEVATORS + 1];
        operate = new int[TOTAL_ELEVATORS + 1];
        wait = new double[TOTAL_MAX_CUSTOMERS + 1];
        j = 0;


    }

    public double getNext() {
        return - MEAN_INTERARRIVAL_TIME * Math.log(rand.nextDouble());
    }

    public void handleFirstAvailableCustomer() {
        // step 5
        j = 0;
        if (TIME >= return_to_ground[1]) {
            j = 1;
        } else if (TIME >= return_to_ground[2]) {
            j = 2;
        } else if (TIME >= return_to_ground[3]) {
            j = 3;
        } else if (TIME >= return_to_ground[4]) {
            j = 4;
        }

        if (j == 0) {
            goTo("initializeQueue");
        } else {
            goTo("initializeElevatorOccupancy");
        }
    }

    public void initializeElevatorOccupancy() {

        // step 6
        first[j] = i;
        occup[j] = 0;
        for (int k = 1; k <= TOTAL_ELEVATORS; k++) {
            selvec[j][k] = 0;
            flrvec[j][k] = 0;
        }

        goTo("loadCurrentCustomerOnElevator");
    }

    public void loadCurrentCustomerOnElevator() {
        // step 7
        selvec[j][floor[i]] = 1;
        flrvec[j][floor[i]] = flrvec[j][floor[i]] + 1;
        occup[j] = occup[j] + 1;
        goTo("getNextCustomer");
    }

    public void getNextCustomer() {

        // step 8

        i = i + 1;
        TIME = TIME + between[i];
        delivery[i] = TIME_DELAY_TO_WAIT_FOR_PASSENGER;

        // step 9
        for (int k = 1; k <= TOTAL_ELEVATORS; k++) {
            if (TIME >= return_to_ground[k]) {
                return_to_ground[k] = TIME;
            }
        }

        if (between[i] <= TIME_DELAY_TO_WAIT_FOR_PASSENGER && occup[j] < MAX_OCCUPANCY) {
            for (int k = first[j]; k <= i - 1; k++) {
                delivery[k] = delivery[k] + between[i];
            }
            goTo("loadCurrentCustomerOnElevator");
        } else {
            limit = i - 1;

            goTo("calculateTimeSpentOnElevator");
        }

    }
    public void check_load(){
        total_load[j] += occup[j];
        if(occup[j] ==  MAX_OCCUPANCY){
            max_load[j] ++;
        }
        load_count[j]++;
    }

    public void calculateTimeSpentOnElevator() {
        // step 11
        //check load
        check_load();

        k = first[j];
        jumpedFrom30 = false;
        goTo("getTimeOfOneCustomerOnElevator");
    }

    public void getTimeOfOneCustomerOnElevator() {
        // step 12
        int N = floor[k] - 1;

        int sumFlrVec = 0;
        for (int m = 1; m <= N; m++) {
            sumFlrVec += flrvec[j][m];
        }
        int sumSelVec = 0;
        for (int m = 1; m <= N; m++) {
            sumSelVec += selvec[j][m];
        }
        elevator[k] = TIME_TO_TRAVEL_BETWEEN_FLOORS * N + TIME_FOR_A_PASSENGER_TO_DISEMBARK * sumFlrVec + TIME_FOR_A_PASSENGER_TO_DISEMBARK + (OPEN_TIME_OF_DOOR + CLOSE_TIME_OF_DOOR) * sumSelVec + OPEN_TIME_OF_DOOR;

        // stem 13
        delivery[k] = delivery[k] + elevator[k];

        // step 14
        DELTIME = DELTIME + delivery[k];

        // step 15
        if (delivery[k] > MAXDEL) {
            MAXDEL = delivery[k];
        }
        // step 16
        if (elevator[k] > MAXELEV) {
            MAXELEV = elevator[k];
        }

        k++;
        if (k <= limit) {
            goTo("getTimeOfOneCustomerOnElevator");
        } else {
            goTo("calculateStopCountAndTimeToReturn");
        }
    }

    public void calculateStopCountAndTimeToReturn() {
        // step 17
        int selvecSum = 0;
        for (int m = 1; m <= TOTAL_FLOORS; m++) {
            selvecSum += selvec[j][m];
        }
        stop[j] = stop[j] + selvecSum;

        int Max = -1;
        int maxValue = -99999;

        for (int m = 1; m <= TOTAL_FLOORS; m++) {
            if (selvec[j][m] > maxValue) {
                maxValue = selvec[j][m];
                Max = m;
            }
        }

        int flrvecSum = 0;
        for (int m = 1; m <= TOTAL_FLOORS; m++) {
            flrvecSum += flrvec[j][m];
        }

        eldel[j] = TIME_TO_TRAVEL_BETWEEN_FLOORS * 2 * (Max - 1) + TIME_FOR_A_PASSENGER_TO_DISEMBARK * flrvecSum + (OPEN_TIME_OF_DOOR + CLOSE_TIME_OF_DOOR) * selvecSum;

        return_to_ground[j] = TIME + eldel[j];
        operate[j] = operate[j] + eldel[j];

        if (jumpedFrom30) {
            goTo("updateQueueLength");
            return;
        }
        goTo("repeatLoopToGetFirstAvailableElevator");

    }

    public void repeatLoopToGetFirstAvailableElevator() {
        //step 18
        goTo("handleFirstAvailableCustomer");
    }

    public void initializeQueue() {
        // step 19
        quecust = i;
        startque = TIME;
        queue = 1;
        arrive[i] = TIME;
        goTo("loadCustomerIntoAvailableElevator");
    }

    public void loadCustomerIntoAvailableElevator() {
        while (true) {
            // step 20
            i = i + 1;
            TIME = TIME + between[i];
            ;
            arrive[i] = TIME;
            queue = queue + 1;
            // step 21
            if (TIME >= return_to_ground[1]) {
                j = 1;
                break;
            } else if (TIME >= return_to_ground[2]) {
                j = 2;
                break;
            } else if (TIME >= return_to_ground[3]) {
                j = 3;
                break;
            } else if (TIME >= return_to_ground[4]) {
                j = 4;
                break;
            }
        }

        // step 22
        for (int k = 1; k <= TOTAL_FLOORS; k++) {
            selvec[j][k] = 0;
            flrvec[j][k] = 0;
        }
        remain = queue - MAX_OCCUPANCY;
        // step 23

        if (remain <= 0) {
            R = i;
            occup[j] = queue;

        } else {
            R = quecust + (MAX_OCCUPANCY - 1);
            occup[j] = MAX_OCCUPANCY;
        }

        check_load();

        // step 24
        for (int k = quecust; k <= R; k++) {
            selvec[j][floor[k]] = 1;
            flrvec[j][floor[k]] = flrvec[j][floor[k]] + 1;
        }
        // step 25
        if (queue >= QUELEN) {
            QUELEN = queue;
        }
        SUM_OF_QUELEN += queue;
        QUE_LENGTH_COUNTER++;

        // step 26
        int sumOfTimeSubArrive = 0;
        for (int m = quecust; m <= R; m++) {
            sumOfTimeSubArrive += (TIME - arrive[m]);
        }
        quetotal = quetotal + occup[j];
        QUETIME = QUETIME + sumOfTimeSubArrive;

        // step 27
        SUM_OF_QUETIME += (TIME - startque );
        COUNT_OF_QUETIME++;
        if (TIME - startque >= MAXQUE) {
            MAXQUE = TIME - startque;
        }
        // step 28
        first[j] = quecust;
        // step 29
        for (int k = first[j]; k <= R; k++) {
            delivery[k] = TIME_DELAY_TO_WAIT_FOR_PASSENGER + (TIME - arrive[k]);
            wait[k] = TIME - arrive[k];
        }
        // step 30
        if (remain <= 0) {
            queue = 0;
            goTo("getNextCustomer");
        } else {
            limit = R;
            jumpedFrom30 = true;
        }
    }

    public void updateQueueLength() {
        // step 31
        queue = remain;
        quecust = R + 1;
        startque = arrive[R + 1];
        // step 32
        goTo("loadCustomerIntoAvailableElevator");
    }

    public void goTo(String methodName) {
        try {
            method = Simulator.class.getMethod(methodName);
            methodString = methodName;
        } catch (NoSuchMethodException e) {
            System.out.println("METHOD NOT FOUND");
        }
    }

    public void log() {
        // step 33
        float N = (i - queue);
        System.out.println("The total number of customers served: " + N);
        System.out.println("Average delivery time: " + (DELTIME / N));
        System.out.println("Maximum delivery time of a customer: " + MAXDEL);
        // step 34
        ELEVTIME = 0;
        for (int m = 1; m <= limit; m++) {
            ELEVTIME += (elevator[m] * 1.0 / limit);
        }
        System.out.println("The average time spent in an elevator: " + ELEVTIME);
        System.out.println("The maximum time spent in an elevator: " + MAXELEV);

        // step 35
        System.out.println("The number of customers waiting in the longest queue: " + QUELEN);
        System.out.println("Average number of customers in the queue: "+ SUM_OF_QUELEN / QUE_LENGTH_COUNTER);
        System.out.println("The average time a customer who waits in line spends in a queue: " + (QUETIME * 1.0 / quetotal));
        System.out.println("The longest time spent in a queue: " + MAXQUE);
        System.out.println("Average time spent in a queue: "+(SUM_OF_QUETIME/COUNT_OF_QUETIME));

        // step 36
        System.out.println("The total number of stops for each elevator and the percentage time each elevator is in transport: ");
        for (int k = 1; k <= 4; k++) {
            double operationPortion = ((operate[k] / (TOTAL_TIME_OF_SIMULATION * 1.0)) * 100);
            System.out.println("k=" + k + ": stop count: " + stop[k] + ", in transport portion: " + operationPortion + "%");
            System.out.println("available time: "+(100-operationPortion)+" %");
            System.out.println("number of times max load occured: "+ max_load[k]);
            System.out.println("number of times elevator left floor 1: " + load_count[k]);
            System.out.println("number of total load: "+ total_load[k]);
            System.out.println("number of average load from floor 1: "+ total_load[k]/load_count[k]);

        }
    }

    public void run() {
        goTo("handleFirstAvailableCustomer");

        // step 4
        while (TIME <= TOTAL_TIME_OF_SIMULATION) {
            try {
                if (method == null) {
                    System.out.println("Program shut down prematurely");
                    return;
                }
                method.invoke(this);
            } catch (Exception e) {
                System.out.println("EXCEPTION IN METHOD INVOKE: " + methodString + ". CODE WILL EXIT");
                System.out.println(e);
                System.exit(1);

            }
        }
        log();
    }

    @Override
    public String toString() {
        return "Simulator{" +
                "TOTAL_MAX_CUSTOMERS=" + TOTAL_MAX_CUSTOMERS +
                ", \nTOTAL_ELEVATORS=" + TOTAL_ELEVATORS +
                ", \nTOTAL_FLOORS=" + TOTAL_FLOORS +
                ", \nDELTIME=" + DELTIME +
                ", \nELEVTIME=" + ELEVTIME +
                ", \nMAXDEL=" + MAXDEL +
                ", \nMAXELEV=" + MAXELEV +
                ", \nQUELEN=" + QUELEN +
                ", \nQUETIME=" + QUETIME +
                ", \nMAXQUE=" + MAXQUE +
                ", \nquetotal=" + quetotal +
                ", \nremain=" + remain +
                ", \nbetween=" + Arrays.toString(between) +
                ", \nfloor=" + Arrays.toString(floor) +
                ", \ndelivery=" + Arrays.toString(delivery) +
                ", \nfirst=" + Arrays.toString(first) +
                ", \noccup=" + Arrays.toString(occup) +
                ", \nselvec=" + Arrays.toString(selvec) +
                ", \nflrvec=" + Arrays.toString(flrvec) +
                ", \nelevator=" + Arrays.toString(elevator) +
                ", \neldel=" + Arrays.toString(eldel) +
                ", \narrive=" + Arrays.toString(arrive) +
                ", \ni=" + i +
                ", \nj=" + j +
                ", \nTIME=" + TIME +
                ", \nreturn_to_ground=" + Arrays.toString(return_to_ground) +
                ", \nstop=" + Arrays.toString(stop) +
                ", \noperate=" + Arrays.toString(operate) +
                ", \nwait=" + Arrays.toString(wait) +
                '}';
    }
}
