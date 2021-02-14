#include <iostream>
using namespace std;

int main(){

	int width;
  int height;

  cout << "Input width: ";
  cin >> width;

	cout << "Input height: ";
  cin >> height;

  cout << endl << "Width: " << width << endl;
  cout << "Height: " << height << endl;

  if (width - height < width/2){
		// formula for trapezoid shape
		cout << "Impossible shape!" << endl;
    // exit program
    return 1;
  } else {

    cout << endl << "Shape: " << endl;

    for (int row = 0; row < height; row++){
      for (int column = 0; column < width; column++){
        if (row - column > 0 || column > (width - 1) - row){
          cout << ' ';
        }
        else {
          cout << '*';
        }
      }

      cout << endl;
	  }
  }

return 0;

}