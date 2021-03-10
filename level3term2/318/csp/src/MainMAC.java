public class MainMAC {
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
            System.out.println("----Forward check in file " + fileName + "-----");
            MACGame g = new MACGame("data/" + fileName);
            g.initialStateFC.print();

            boolean valid=g.MACSolve();

            System.out.println("Forward check possible: "+valid);
            g.finishedStateFC.print();
            System.out.println("consistency check: "+g.numberOfConsistencyChecking);
            System.out.println("fails: "+g.numberOfFails);
        }
    }
}
