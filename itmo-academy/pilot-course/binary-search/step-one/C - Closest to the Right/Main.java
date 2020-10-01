import java.util.StringTokenizer;

import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.Closeable;

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

    static final int InvalidIndex = Integer.MAX_VALUE;

    private static int firstNotSmaller(int elements[], int find) {
        int end = elements.length - 1;
        int index = InvalidIndex;
        int begin = 0;
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

    public static void main(String[] args) throws IOException {
        PrintWriter writer = new PrintWriter(System.out);
        FastReader reader = new FastReader(System.in);
        int n = reader.nextInteger();
        int k = reader.nextInteger();
        int[] elements = new int[n];
        for (int i = 0; i < n; i++) {
            elements[i] = reader.nextInteger();
        }
        for (int i = 0; i < k; i++) {
            int find = reader.nextInteger();
            int index = firstNotSmaller(elements, find);
            if (index != InvalidIndex) {
                writer.println(index + 1);
            } else {
                writer.println(n + 1);
            }
        }
        writer.flush();
        reader.close();
    }

}