// ----------------------------
// Heather Toomey
// CIT-260
// Problem W8dot1 Main
// June 13, 2020
// ----------------------------

package Week08;

public class TestRectangle {

    public static void main(String[] args) {
        // 1. Tells the user what the program does.
        System.out.print("This program creates two rectangle " +
                "objects and displays their width, height, area and " +
                "perimeter.\n\n");

        // 2. Creates two Rectangle objects,
        // the first with a height of 4 and a width of 40,
        // the second with a height of 3.5 and a width of 5.

        // Create rectangle1
        Rectangle rectangle1 = new Rectangle(4, 40);

        // Create rectangle 2
        Rectangle rectangle2 = new Rectangle(3.5, 5);

        // 3. Call the methods in your Rectangle class to output the width,
        // height, area, and perimeter of each rectangle, with 2 digits
        // after the decimal point.
        System.out.print("Rectangle 1:\n");
        System.out.print("height = " + String.format("%.2f", rectangle1.getHeight()) + "\n");
        System.out.print("width = " + String.format("%.2f", rectangle1.getWidth()) + "\n");
        System.out.print("area = " + String.format("%.2f", rectangle1.getArea()) + "\n");
        System.out.print("perimeter = " + String.format("%.2f", rectangle1.getPerimeter()) + "\n\n");

        System.out.print("Rectangle 2:\n");
        System.out.print("height = " + String.format("%.2f", rectangle2.getHeight()) + "\n");
        System.out.print("width = " + String.format("%.2f", rectangle2.getWidth()) + "\n");
        System.out.print("area = " + String.format("%.2f", rectangle2.getArea()) + "\n");
        System.out.print("perimeter = " + String.format("%.2f", rectangle2.getPerimeter()) + "\n\n");

        // 4. Output a goodbye message.
        System.out.print("goodbye ...\n");

    }
}
