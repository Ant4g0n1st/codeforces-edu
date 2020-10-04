import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.Closeable;

import java.util.function.DoublePredicate;
import java.util.StringTokenizer;
import java.util.Arrays;

public class Main {

    private static class FastReader implements Closeable {

        private StringTokenizer tokenizer = null;
        private BufferedReader reader = null;

        FastReader(InputStream is) {
            reader = new BufferedReader(new InputStreamReader(is));
        }

        private String nextToken() throws IOException {
            if (tokenizer == null || !tokenizer.hasMoreTokens()) {
                tokenizer = new StringTokenizer(reader.readLine());
            }
            return tokenizer.nextToken();
        }

        public double nextDouble() throws IOException {
            return Double.parseDouble(this.nextToken());
        }

        public int nextInt() throws IOException {
            return Integer.parseInt(this.nextToken());
        }

        @Override
        public void close() throws IOException {
            reader.close();
        }

    }

    private static final double EPSILON = 5E-7;

    private static double binarySearch(double min, double max, double eps, DoublePredicate f) {
        final int SEARCH_ITERATIONS = 100;
        double solution = max;
        double begin = min;
        double end = max;
        for (int i = 0; i < SEARCH_ITERATIONS; i++) {
            double middle = (begin + end) / 2.0;
            if (f.test(middle)) {
                begin = middle + eps;
            } else {
                end = middle - eps;
                solution = middle;
            }
        }
        return solution;
    }

    private static void solveProblem() throws IOException {
        PrintWriter writer = new PrintWriter(System.out);
        FastReader reader = new FastReader(System.in);
        final int n = reader.nextInt();
        double k = reader.nextDouble();
        double[] ropes = new double[n];
        double max = Double.MIN_VALUE;
        for (int i = 0; i < n; i++) {
            ropes[i] = reader.nextDouble();
            max = Math.max(ropes[i], max);
        }
        Arrays.sort(ropes);
        DoublePredicate f = (length) -> {
            double ropeCount = 0.0;
            for (int i = n - 1; i >= 0; i--) {
                ropeCount += Math.floor(ropes[i] / length);
                if (!(ropeCount < k)) {
                    return true;
                }
            }
            return false;
        };
        double length = binarySearch(0, max, EPSILON, f);
        writer.printf("%.12f\n", length);
        writer.flush();
        reader.close();
    }

    public static void main(String[] args) throws IOException {
        solveProblem();
    }

}