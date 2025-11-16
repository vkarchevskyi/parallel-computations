class MinMatrixScalarProductFinder implements Runnable {
    private double[][] matrix;
    private int fromN;
    private int toN;
    private int M;
    private double result = Double.MAX_VALUE;

    public MinMatrixScalarProductFinder(double[][] matrix, int fromN, int toN, int M) {
        this.matrix = matrix;
        this.fromN = fromN;
        this.toN = toN;
        this.M = M;
    }

    public void run() {
        double temp = 0;

        for (int i = this.fromN; i < this.toN; i++) {
            for (int j = this.fromN; j < this.toN; j++) {
                if (i == j) {
                    continue;
                }

                for (int k = 0; k < this.M; k++) {
                    temp += matrix[i][k] * matrix[j][k];
                }

                this.result = Math.min(this.result, temp);
                temp = 0;
            }
        }
    }

    public double getResult() {
        return this.result;
    }
}

public class MatrixMinScalarProductBenchmark {
    public static void main(String[] args) {
        final int M = 1000;
        final int N = 1000;

        final int ThreadCount = 4;
        double[][] matrix = new double[M][N];

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = Math.random();
            }
        }

        int portionSizeN = N / ThreadCount;
        MinMatrixScalarProductFinder[] finders = new MinMatrixScalarProductFinder[ThreadCount];
        Thread[] threads = new Thread[ThreadCount];

        long start = System.nanoTime();
        for (int i = 0; i < ThreadCount; i++) {
            finders[i] = new MinMatrixScalarProductFinder(matrix, i * portionSizeN, (i + 1) * portionSizeN, M);
            threads[i] = new Thread(finders[i]);
            threads[i].start();
        }

        double result = Double.MAX_VALUE;
        for (int i = 0; i < ThreadCount; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                System.out.println("an error occured");
            }
        }

        for (int i = 0; i < ThreadCount; i++) {
            result = Math.min(result, finders[i].getResult());
        }

        long duration1 = System.nanoTime() - start;
        System.out.printf("Length is %g. Duration in parallel mode is %d", result, duration1);

        start = System.nanoTime();
        result = Double.MAX_VALUE;
        double temp = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i == j) {
                    continue;
                }

                for (int k = 0; k < M; k++) {
                    temp += matrix[i][k] * matrix[j][k];
                }

                result = Math.min(result, temp);
                temp = 0;
            }
        }

        long duration2 = System.nanoTime() - start;
        System.out.printf("\nLength is %g. Duration in serial mode is %d\n", result, duration2);
        System.out.printf("Speed-up is %g\n", (double) duration2 / duration1);
    }
}