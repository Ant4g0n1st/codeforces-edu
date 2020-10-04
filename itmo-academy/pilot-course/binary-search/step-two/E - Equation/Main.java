import java.util.function.DoublePredicate;
import java.util.Scanner;

import java.io.IOException;
import java.io.PrintWriter;

public class Main {

    private static final double EPSILON = 5E-7;

    private static double findSolution(double min, double max, double eps, DoublePredicate f) {
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
        Scanner input = new Scanner(System.in);
        double c = input.nextDouble();
        DoublePredicate f = (double x) -> x * x + Math.sqrt(x) < c;
        double x = findSolution(0, Math.sqrt(c), EPSILON, f);
        writer.println(x);
        writer.flush();
        input.close();
    }

    public static void main(String[] args) throws IOException {
        solveProblem();
    }

}