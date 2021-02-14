// Heather Toomey
// CIT-260
// Assignment W9dot1
// June 18, 2020

/**
 * The MyPoint Class
 * Purpose: Make a class to represent a point on a two-dimensional graph.
 * @param x and y
 * @return void
 */

package Week09;

import java.lang.Math;

// Design a class named MyPoint.
public class MyPoint {

    // 1. Two integer data fields x and y, that represent the x-coordinate
    // and the y-coordinate of the point.
    // Declare x and y.
    private int x;
    private int y;

    // 2. Getter and setter methods for x and y.
    // Method to call from outside the class to get private variable x.
    /**
     * The getX Method
     * Purpose: Return the value for the x coordinate
     * @param The user prompt as a String
     * @return the int value entered by the user
     */

    // Method to call from outside the class to get private variable x.
    public int getX() {
        return x;
    }

    // Method to call from outside the class to change private variable x.
    public void setX(int x) {
        this.x = x;
    }

    // Method to call from outside the class to get private variable y.
    public int getY() {
        return y;
    }

    // Method to call from outside the class to change private variable y.
    public void setY(int y) {
        this.y = y;
    }

    // 3. A no-arg constructor that creates a default point at (0,0).
    MyPoint() {
        this.x = 0;
        this.y = 0;
    }

    // 4. A parameterized constructor that creates a point at the designated x and y coordinate.
    MyPoint(int x, int y) {
        this.x = x;
        this.y = y;
    }

    // 5. A member method named distance that calculates and returns the distance between this
    // MyPoint object and another point that is specified by its x- and y-coordinates. The method
    // header looks like this:  public double distance(int xCoord, int yCoord);
    public double distance(int xCoord, int yCoord) {
        return Math.sqrt(Math.pow(this.x - xCoord,2) + Math.pow(this.y - yCoord,2));
    }

    // 6. A member method named distance that calculates and returns the distance between this
    // MyPoint object and another object of the MyPoint class. The method header looks like
    // this:  public double distance(MyPoint p)
    public double distance(MyPoint p) {
        return Math.sqrt(Math.pow(this.x - p.x,2) + Math.pow(this.y - p.y,2));
    }

    // 7. A static, non-member method named distance that takes two objects of the MyPoint
    // class as parameters, and calculates and returns the distance between the two of
    // them. The method header looks like this:      public static double distance (
    // MyPoint mp1, MyPoint mp2)
    public static double distance (MyPoint mp1, MyPoint mp2) {
        return Math.sqrt(Math.pow(mp1.x - mp2.x,2) + Math.pow(mp1.y - mp2.y,2));
    }
}

// 8. Create a UML diagram that documents your class design.

