public class Main {
    public static void main(String[] args) {
        for(int i  = 0; i < 10; i++){
            System.out.println("__________SIMULATOR RUN "+i+"__________");
            Simulator simulator = new Simulator(i);
            simulator.run();
        }
    }
}
