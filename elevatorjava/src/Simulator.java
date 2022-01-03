import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.concurrent.ThreadLocalRandom;

public class Simulator {
    public final int TOTAL_MAX_CUSTOMERS = 400;
    public final int TOTAL_ELEVATORS = 4;
    public final int TOTAL_FLOORS = 12;

    public int DELTIME = 0;
    public int ELEVTIME = 0;
    public int MAXDEL = 0;
    public int MAXELEV = 0;
    public int QUELEN = 0;
    public int QUETIME = 0;
    public int MAXQUE = 0;
    public int quetotal = 0;
    public int remain = 0;

    public int[] between;
    public int[] floor;
    public int[] delivery;
    public int[] first;
    public int[] occup;
    public int[][] selvec;
    public int[][] flrvec;
    public int[] elevator;
    public int[] eldel;
    public int[] arrive;

    public int i = 0;
    public int j = 0;
    public int k = 0;

    public int TIME;
    public int[] return_to_ground;
    public int[] stop;
    public int[] operate;
    public int[] wait;

    public int limit = 0;

    public int quecust = 0;
    public int startque = 0;
    public int queue = 0;
    public int R;


    //variables outside algorithm
    public Method method;
    public boolean jumpedFrom30 = false;
    public static int randInt(int min, int max) {
        return ThreadLocalRandom.current().nextInt(min, max + 1);
    }
    public Simulator() {
        // step 1
        between = new int[TOTAL_MAX_CUSTOMERS+1];
        floor = new int[TOTAL_MAX_CUSTOMERS+1];
        delivery = new int[TOTAL_MAX_CUSTOMERS+1];

        for(int c = 0 ; c < TOTAL_MAX_CUSTOMERS+1; c++){
            between[c] = randInt(1,100);
            floor[c] = randInt(2, TOTAL_FLOORS);
            delivery[c] =  randInt(1,100);
        }

        first = new int[TOTAL_ELEVATORS+1];
        occup = new int[TOTAL_ELEVATORS+1];
        selvec = new int[TOTAL_ELEVATORS+1][];
        for(int c = 0 ; c <TOTAL_ELEVATORS+1;c++){
            selvec[c] = new int[TOTAL_FLOORS+1];
        }
        flrvec = new int[TOTAL_ELEVATORS+1][];
        for(int c = 0 ; c < TOTAL_ELEVATORS+1; c++){
            flrvec[c] = new int[TOTAL_FLOORS+1];
        }
        elevator = new int[TOTAL_MAX_CUSTOMERS+1];
        eldel = new int[TOTAL_ELEVATORS+1];
        arrive = new int[TOTAL_MAX_CUSTOMERS+1];

        // step 2
        i = 1;
        delivery[i] = 15;

        // step 3
        TIME = between[i];
        return_to_ground = new int[TOTAL_ELEVATORS+1];
        for(int c = 0 ; c < TOTAL_ELEVATORS+1; c++){
            return_to_ground[c] = TIME;
        }
        stop = new int[TOTAL_ELEVATORS+1];
        operate = new int[TOTAL_ELEVATORS+1];
        wait = new int[TOTAL_MAX_CUSTOMERS+1];
        j = 0;


    }
    public void block2(){
        // step 5
        j = 0;
        if(TIME >= return_to_ground[1]){
            j=1;
        }else if(TIME >= return_to_ground[2]){
            j=2;
        }else if(TIME >= return_to_ground[3]){
            j=3;
        }else if(TIME >= return_to_ground[4]){
            j=4;
        }

        if(j == 0){
            goTo("block10");
        }else {
            goTo("block3");
        }
    }
    public void block3(){

        // step 6
        first[j] = i;
        occup[j] = 0;
        for(int k = 1; k <= TOTAL_ELEVATORS; k++){
            selvec[j][k] = 0;
            flrvec[j][k] = 0;
        }

        goTo("block4");
    }
    public void block4(){
        // step 7
        selvec[j][floor[i]]=1;
        flrvec[j][floor[i]] = flrvec[j][floor[i]]+1;
        occup[j]= occup[j]+1;
        goTo("block5");
    }
    public void block5(){
        // step 8
        i = i+1;
        TIME = TIME+between[i];
        delivery[i]=15;

        // step 9
        for(int k = 1; k <=4; k++){
            if(TIME>=return_to_ground[k]){
                return_to_ground[k] = TIME;
            }
        }
        // step 10
        if(between[i]<=15 && occup[j]<12){
            for(int k = first[j]; k<=i-1; k++){
                delivery[k] = delivery[k] + between[i];
            }
            goTo("block4");
        }else{
            limit = i-1;

            goTo("block6");
        }
    }
    public void block6(){
        // step 11
        k = first[j];
        jumpedFrom30 = false;
        goTo("block7");
    }
    public void block7(){
        // step 12
        int N = floor[k] -1;

        int sumFlrVec = 0;
        for(int m = 1; m <= N; m++){
            sumFlrVec+=flrvec[j][m];
        }
        int sumSelVec = 0;
        for(int m = 1; m <=N; m++){
            sumSelVec+= selvec[j][m];
        }
        elevator[k] = 10*N + 3*sumFlrVec + 3 + 10*sumSelVec + 5;

        // stem 13
        delivery[k]= delivery[k]+elevator[k];

        // step 14
        DELTIME = DELTIME + delivery[k];

        // step 15
        if(delivery[k] > MAXDEL){
            MAXDEL = delivery[k];
        }
        // step 16
        if(elevator[k] > MAXELEV){
            MAXELEV = elevator[k];
        }

        k++;
        if(k<=limit){
            goTo("block7");
        }else {
            goTo("block8");
        }
    }
    public void block8(){
        // step 17
        int selvecSum = 0;
        for(int m = 1; m <= 12; m++){
            selvecSum += selvec[j][m];
        }
        stop[j] = stop[j] + selvecSum;

        int Max = -1;
        int maxValue = -99999;

        for(int m = 1; m<=12; m++){
            if(selvec[j][m]>maxValue){
                maxValue = selvec[j][m];
                Max = m;
            }
        }

        int flrvecSum = 0;
        for(int m = 1; m <=12; m++){
            flrvecSum+= flrvec[j][m];
        }

        eldel[j] = 20*(Max - 1)+3*flrvecSum+10*selvecSum;

        return_to_ground[j] = TIME + eldel[j];
        operate[j] = operate[j] + eldel[j];

        if(jumpedFrom30){
            goTo("block12");
            return;
        }
        goTo("block9");

    }
    public void block9(){
        //step 18
        goTo("block2");
    }

    public void block10(){
        // step 19
        quecust = i;
        startque = TIME;
        queue = 1;
        arrive[i] = TIME;
        goTo("block11");
    }
    public void block11(){
        while(true){
            // step 20
            i = i + 1;
            TIME = TIME + between[i];;
            arrive[i] = TIME;
            queue = queue + 1;
            // step 21
            if(TIME >= return_to_ground[1]){
                j = 1;
                break;
            }else if(TIME >= return_to_ground[2]){
                j = 2;
                break;
            }else if(TIME >= return_to_ground[3]){
                j = 3;
                break;
            }else if(TIME >= return_to_ground[4]){
                j = 4;
                break;
            }
        }

        // step 22
        for(int k = 1; k <=12; k++){
            selvec[j][k] = 0;
            flrvec[j][k] = 0;
        }
        remain = queue - 12;
        // step 23

        if(remain<=0){
            R = i;
            occup[j] = queue;
        }else {
            R = quecust + 11;
            occup[j] = 12;
        }
        // step 24
        for(int k = quecust; k<= R; k++){
            selvec[j][floor[k]] = 1;
            flrvec[j][floor[k]] = flrvec[j][floor[k]] + 1;
        }
        // step 25
        if(queue>=QUELEN){
            QUELEN = queue;
        }
        // step 26
        int sumOfTimeSubArrive = 0;
        for(int m = quecust; m <= R; m++){
            sumOfTimeSubArrive+=(TIME-arrive[m]);
        }
        quetotal = quetotal + occup[j];
        QUETIME = QUETIME + sumOfTimeSubArrive;

        // step 27
        if(TIME -startque>=MAXQUE){
            MAXQUE = TIME - startque;
        }
        // step 28
        first[j] = quecust;
        // step 29
        for(int k = first[j]; k<= R; k++){
            delivery[k] = 15+ (TIME-arrive[k]);
            wait[k] = TIME-arrive[k];
        }
        // step 30
        if(remain <=0){
            queue = 0;
            goTo("block5");
        }else{
            limit = R;
            jumpedFrom30 = true;
        }
    }
    public void block12(){
        // step 31
        queue = remain;
        quecust = R + 1;
        startque = arrive[R+1];
        // step 32
        goTo("block11");
    }
    public void goTo(String methodName){
        try {
            method = Simulator.class.getMethod(methodName);
        } catch (NoSuchMethodException e) {
            System.out.println("METHOD NOT FOUND");
        }
    }
    public void log(){
        // step 33
        float N = (i-queue);
        System.out.println("The total number of customers served: "+ N);
        System.out.println("Average delivery time: "+(DELTIME/N));
        System.out.println("Maximum delivery time of a customer: "+MAXDEL);
        // step 34
        ELEVTIME = 0;
        for(int m = 1; m <= limit; m++){
            ELEVTIME += (elevator[m]/limit);
        }
        System.out.println("The average time spent in an elevator: "+ELEVTIME);
        System.out.println("The maximum time spent in an elevator: "+MAXELEV);

        // step 35
        System.out.println("The number of customers waiting in the longest queue: "+ QUELEN);
        System.out.println("The average time a customer who waits in line spends in a queue: "+ (QUETIME/quetotal));
        System.out.println("The longest time spent in a queue: "+ MAXQUE);

        // step 36
        System.out.println("The total number of stops for each elevator and the percentage time each elevator is in transport: ");
        for(int k = 1; k <=4; k++){
            System.out.println("k="+k+": stop count: "+stop[k]+", in transport portion: "+((operate[k]/4800.0)*100)+"%");
        }
    }
    public void run(){
        goTo("block2");

        // step 4
        while(TIME <=4800){
            try {
                if(method==null){
                    System.out.println("Program shut down prematurely");
                    return;
                }
                method.invoke(this);
            } catch (Exception e) {
                System.out.println("EXCEPTION IN METHOD INVOKE");
            }
        }
        System.out.println("Simulation ended");
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
