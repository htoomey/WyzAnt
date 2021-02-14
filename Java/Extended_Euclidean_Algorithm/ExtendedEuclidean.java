package EEA;

public class ExtendedEuclidean
{
    public void ExtendedEuclideanFunction(long a, long b)
    {
        long firsta = a;
        long firstb = b;
        long x = 0, y = 1;
        long lastx = 1, lasty = 0, prev;
        long gcd;

        while (b != 0)
        {
            // Calculate new quotient and remainder
            long q = a / b;
            long r = a % b;

            // Shift a, b, and r before next iteration
            a = b;
            b = r;

            // x = x_n, lastx = x_n-1
            prev = x;
            x = lastx - q * x;
            lastx = prev;

            // y = y_n, lasty = y_n-1
            prev = y;
            y = lasty - q * y;
            lasty = prev;
        }

        // Caclulate the greatest common divisor GCD
        gcd = (firsta*lastx + firstb*lasty);

        System.out.println("x = " + lastx);
        System.out.println("y = " + lasty);
        System.out.println("GCD = "+ gcd + "\n");

        System.out.println("Equation: ax + by = gcd(a, b)");
        System.out.println("Equation: (" + firsta +
                ")(" + lastx + ") " + "+ (" + firstb +
                ")(" + lasty + ") = gcd(" + firsta + ", " + firstb +
                ") = " + gcd);
    }
}