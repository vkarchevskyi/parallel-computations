class Adder implements Runnable {
    double[] array;
    int from;
    int to;
    private double result;

    public Adder(double[] array, int from,
            int to) {
        this.array = array;
        this.from = from;
        this.to = to;
    }

    public void run() {
        result = 0;
        for (int i = from; i < to; i++) {
            result += array[i] * array[i];
        }
    }

    public double getResult() {
        return result;
    }
}

public class AdderBenchmark {
    public static void main(String[] args) {
        final int N = 60_000_000;
        final int ThreadCount = 4;
        double[] array = new double[N];
        for (int i = 0; i < N; i++) {
            array[i] = Math.random();
        }
        int portionSize = N / ThreadCount;
        Adder[] adders = new Adder[ThreadCount];
        Thread[] threads = new Thread[ThreadCount];
        long start = System.nanoTime();
        for (int i = 0; i < ThreadCount; i++) {
            adders[i] = new Adder(array, i * portionSize,
                    (i + 1) * portionSize);
            threads[i] = new Thread(adders[i]);
            threads[i].start();
        }
        double result = 0;
        for (int i = ThreadCount * portionSize; i < N; i++) {
            result += array[i];
        }
        for (int i = 0; i < ThreadCount; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                System.out.println("an error occured");
            }

        }
        for (int i = 0; i < ThreadCount; i++) {
            result += adders[i].getResult();
        }
        long duration1 = System.nanoTime() - start;
        System.out.printf("Length is %g. Duration in parallel mode is %d", result, duration1);
        start = System.nanoTime();
        result = 0;
        for (int i = 0; i < N; i++) {
            result += array[i] * array[i];

        }
        long duration2 = System.nanoTime() - start;
        System.out.printf("\nLength is %g. Duration in serial mode is %d\n", result, duration2);
        System.out.printf("Speed-up is %g\n", (double) duration2 / duration1);
    }
}