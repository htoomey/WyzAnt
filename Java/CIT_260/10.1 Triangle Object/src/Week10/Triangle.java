// Heather Toomey
// CIT-260
// Assignment W10dot1
// June 27, 2020

/*
 * The Triangle Class
 * Purpose: Make a triangle given side lengths, color, and filled attributes
 * @param side1, side2, side3
 * @return void
 */

package Week10;

public class Triangle extends GeometricObject {
    // 1. Three data fields side1, side2, and side3 that represent sides of a triangle.
    private double side1;
    private double side2;
    private double side3;

    // 2. Getter and setter methods for side1, side2, and side3.

    /*
     * The getSide1 class
     * Purpose: give length of side 1
     * @param none
     * @return side1
     */
    public double getSide1() {
        return side1;
    }

    /*
     * The setSide1 Class
     * Purpose: set value of side1
     * @param side1
     * @return void
     */
    public void setSide1(double side1) {
        this.side1 = side1;
    }

    /*
     * The setSide2 Class
     * Purpose: give length of side 2
     * @param none
     * @return side2
     */
    public double getSide2() {
        return side2;
    }

    /*
     * The setSide2 Class
     * Purpose: set side2 value
     * @param side2
     * @return void
     */
    public void setSide2(double side2) {
        this.side2 = side2;
    }

    /*
     * The getSide3 Class
     * Purpose: give length of side 3
     * @param side1, side2, side3
     * @return side3
     */
    public double getSide3() {
        return side3;
    }

    /*
     * The setSide3 Class
     * Purpose: set length of side 3
     * @param side3
     * @return void
     */
    public void setSide3(double side3) {
        this.side3 = side3;
    }

    /*
     * The Triangle Class
     * Purpose: Make a default triangle
     * @param s
     * @return void
     */
    // 3. A no-arg constructor that creates a default triangle with each side equal to 1.
    public Triangle() {
        this.side1 = 1.0;
        this.side2 = 1.0;
        this.side3 = 1.0;
    }

    /*
     * The Triangle Class
     * Purpose: Make a triangle given side lengths
     * @param side1, side2, side3
     * @return void
     */
    // 4. A parameterized constructor that creates a triangle with the specified values for side1, side2, and side3.
    public Triangle(double side1, double side2, double side3) {
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
    }

    /*
     * The getArea Class
     * Purpose: calculates and returns the area of the triangle
     * @param side1, side2, side3
     * @return area
     */
    // 5. A member method named getArea( ) that calculates and returns the area of the triangle. The formula to compute
    // the area of a Triangle, given three sides is called Heron's formula. Heron's formula is described in problem
    // 2.19 in the textbook. To function correctly, you must add a getArea() method to the GeometricObject class,
    // and the getArea( ) method in the Triangle class must override GeometricObject's  getArea() method.
    @Override
    public double getArea() {
        double semiPerimeter = (side1 + side2 + side3) / 2.0;
        return Math.sqrt(semiPerimeter * (semiPerimeter - side1) * (semiPerimeter - side2) * (semiPerimeter - side3));
     }

    /*
     * The getPerimeter Class
     * Purpose: calculates and returns the perimeter of the triangle
     * @param side1, side2, side3
     * @return perimeter
     */
    @Override
    public double getPerimeter() {
        return side1 + side2 + side3;
    }

    /*
     * The toString Class
     * Purpose: Make a triangle given side lengths, color, and filled attributes
     * @param
     * @return string representation of the triangle object
     */
    // 6. A member method named toString( ) that generates a String representation of the triangle object. This
    // toString( ) method must call GeometricObject's  toString() method to correctly display the String
    // representation of the Triangle object.
    @Override
    public String toString() {
        return (
        "\nTriangle: " +
                "side1 = " + this.getSide1() +
              ", side2 = " + this.getSide2() +
             ", side 3 = " + this.getSide3() +
        "\ncreated on " + this.getDateCreated() +
        "\ncolor: " + this.getColor() + " and filled = " + this.isFilled() +
        "\nArea = " + String.format("%.2f", this.getArea()));
    }
}

// 7. Create a UML diagram that documents your class design. Show both the GeometricObject class and the Triangle
// class in your diagram.
    /*
    _______________
    GeometricObject
    _______________
    -- color: string
    - filled: boolean
    - dateCreated: Date
    _______________
    # GeometricObject(): void
    # GeometricObject()(String color, boolean filled): void
    + setColor(): string
    + getColor(): void
    + isFilled(boolean filled): boolean
    + setFilled(boolean filled): void
    + getDateCreated(): Date
    + toString(): string
    + getArea(): abstract
    + getPerimeter(): abstract
    _______________
    /\
    \/   <--- colored in!!
    |
    3
    |
    _______________
    Triangle
    _______________
    - side1: double
    - side2: double
    - side3: double
    _______________
    + getSide1(): double
    + getSide2(): double
    + getSide3(): double
    + setSide1(double side1): void
    + setSide2(double side2): void
    + setSide3(double side3): void
    + Triangle(): void
    + Triangle(double side1, double side2, double side3): void
    + getArea(): double
    + toString(): string
    _______________
    |
    |
    is called by
    |
    |
    _______________
    Main
    _______________
    ~ MAX_TRIANGLES: int
    ~ triangleList: Triangle
    _______________
    none
    _______________
     */

