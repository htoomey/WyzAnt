// Heather Toomey
// CIT-260
// Assignment W9dot1
// June 17, 2020

package Week09;

public class Main {

    /**
     * The MyPoint Class
     * Purpose: Make a class to represent a point on a two-dimensional graph.
     * @param x and y
     * @return void
     */
    /* The MyPoint Class
    Purpose: represent a point on a two-dimensional graph
    @param - x: double; - y: double
    @return void
     */

    // Design a class named MyPoint.
    public class MyPoint {
        // 1. Two integer data fields x and y, that represent the x-coordinate
        // and the y-coordinate of the point.
        // Declare x and y.
        private double x;
        private double y;

        // 2. Getter and setter methods for x and y.
            // Method to call from outside the class to get private variable x.
            /**
             * The getX Method
             * Purpose: Return the value for the x coordinate
             * @param The user prompt as a String
             * @return the double value entered by the user
             */
            public double getX() {
                return x;
            }

            // Method to call from outside the class to change private variable x.
            public void setX(double x) {
                this.x = x;
            }

            // Method to call from outside the class to get private variable y.
            public double getY() {
                return y;
            }

            // Method to call from outside the class to change private variable y.
            public void setY(double y) {
                this.y = y;
            }

        // 3. A no-arg constructor that creates a default point at (0,0).

    }
    }
