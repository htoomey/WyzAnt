// ----------------------------
// Heather Toomey
// CIT-260
// Problem W8dot1 Class
// June 13, 2020
// ----------------------------

/*
Using the example of the Circle class from section 9.2 of the textbook
design a Rectangle class. Create a UML class diagram that shows your design.

UML Class Diagram for Rectangle ---
=======================================
Rectangle
=======================================
-height: double
-width: double
---------------------------------------
+Rectangle()
+Rectangle(newHeight: double, newWidth: double)
+getArea(): double
+getPerimeter(): double
+getHeight(): double
+getWidth(): double
+setHeight(newHeight: double): void
+setWidth(newWidth: double): void
=======================================

UML Object Diagram for Rectangle1 ---
=======================================
Rectangle1
=======================================
+height = 4
+width = 40
=======================================

UML Object Diagram for Rectangle2 ---
=======================================
Rectangle2
=======================================
+height = 3.5
+width = 5
=======================================
*/

package Week08;

class Rectangle {

    // 1. Two data fields named width and height, that specify the width and height of the rectangle.
    // The default values for both width and height should be 1.
    double height = 1;
    double width = 1;

    // 2. A no-arg constructor that creates a default rectangle object.
    Rectangle() {
    }

    // 3. A parameterized constructor that creates a rectangle object with the specified height and width.
    Rectangle(double newHeight, double newWidth) {
        height = newHeight;
        width = newWidth;
    }

    // 4. Getter and setter methods for height and width.
    public double getHeight() {
        return height;
    }

    public void setHeight(double newHeight) {
        height = newHeight;
    }

    public double getWidth() {
        return width;
    }

    public void setWidth(double newWidth) {
        width = newWidth;
    }

    // 5. A method named getArea( ) that computes and returns the area of the rectangle.
    public double getArea() {
        return height * width;
    }

    // 6. A method names getPerimeter( ) that computes and returns the perimeter of the rectangle.
    public double getPerimeter() {
         return (2 * width) + (2 * height);
    }
}


