import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.Closeable;

import java.util.StringTokenizer;

import java.math.BigInteger;

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

        public long nextLong() throws IOException {
            return Long.parseLong(this.nextToken());
        }

        @Override
        public void close() throws IOException {
            reader.close();
        }

    }

    private static class Rectangle {

        private BigInteger height = BigInteger.ZERO;
        private BigInteger width = BigInteger.ZERO;

        Rectangle(long width, long height) {
            this.height = BigInteger.valueOf(height);
            this.width = BigInteger.valueOf(width);
        }

        public BigInteger getHeight() {
            return height;
        }

        public BigInteger getWidth() {
            return width;
        }

    }

    private static long minBoundingSquare(Rectangle r, long n) {
        BigInteger end = BigInteger.valueOf(Long.MAX_VALUE);
        BigInteger begin = BigInteger.ZERO;
        BigInteger side = BigInteger.ZERO;
        BigInteger middle = null;
        BigInteger m = BigInteger.valueOf(n);
        while (begin.compareTo(end) <= 0) {
            middle = begin.add(end).divide(BigInteger.TWO);
            BigInteger x = middle.divide(r.getHeight());
            BigInteger y = middle.divide(r.getWidth());
            if (x.multiply(y).compareTo(m) >= 0) {
                end = middle.subtract(BigInteger.ONE);
                side = middle;
            } else {
                begin = middle.add(BigInteger.ONE);
            }
        }
        return side.longValue();
    }

    public static void main(String[] args) throws IOException {
        PrintWriter writer = new PrintWriter(System.out);
        FastReader reader = new FastReader(System.in);
        long w = reader.nextLong();
        long h = reader.nextLong();
        long n = reader.nextLong();
        Rectangle rectangle = new Rectangle(w, h);
        long s = minBoundingSquare(rectangle, n);
        writer.println(s);
        writer.flush();
        reader.close();
    }

}