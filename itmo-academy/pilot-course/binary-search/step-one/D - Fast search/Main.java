import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.Closeable;

import java.util.StringTokenizer;
import java.util.Arrays;

public class Main {

    public static class FastReader implements Closeable {

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

        public int nextInteger() throws IOException {
            return Integer.parseInt(this.nextToken());
        }

        @Override
        public void close() throws IOException {
            reader.close();
        }

    }

    private static int firstNotSmaller(int elements[], int find) {
        int end = elements.length - 1;
        int begin = 0;
        int index = 0;
        while (begin <= end) {
            int middle = (begin + end) / 2;
            if (elements[middle] < find) {
                begin = middle + 1;
                continue;
            }
            end = middle - 1;
            index = middle;
        }
        return index;
    }

    private static int lastNotGreater(int elements[], int find) {
        int index = elements.length - 1;
        int end = elements.length - 1;
        int begin = 0;
        while (begin <= end) {
            int middle = (begin + end) / 2;
            if (elements[middle] > find) {
                end = middle - 1;
                continue;
            }
            begin = middle + 1;
            index = middle;
        }
        return index;
    }

    public static void main(String[] args) throws IOException {
        PrintWriter writer = new PrintWriter(System.out);
        FastReader reader = new FastReader(System.in);
        int n = reader.nextInteger();
        int[] elements = new int[n];
        for (int i = 0; i < n; i++) {
            elements[i] = reader.nextInteger();
        }
        int k = reader.nextInteger();
        Arrays.sort(elements);
        int last = elements[n - 1];
        int first = elements[0];
        for (int i = 0; i < k; i++) {
            int l = reader.nextInteger();
            int r = reader.nextInteger();
            if (r < first || last < l) {
                writer.print("0 ");
                continue;
            }
            int a = firstNotSmaller(elements, l);
            int b = lastNotGreater(elements, r);
            writer.print(b - a + 1 + " ");
        }
        writer.println();
        writer.flush();
        reader.close();
    }

}