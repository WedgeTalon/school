//
// Name : Matthew Kwiecien
// Date : 09/04/2017
// Course : CISP1010-70
// Instructor : Dr. Brown
// Program : Week 2 - MPG Calculator
// Description:
// Program to calculate miles per gallon. The user inputs
// total miles traveled and gallons of gas required. The
// program then outputs the calculated miles per gallon.
//

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
using namespace std;

int main(int argc, char *argv[]){
	float miles;
	float gallons;
	float mpg;
	std::string mpgColor;

	cout << "\n\033[0;36m*** Welcome to MPG Converter ***\n";
	cout << "\n\033[0mTotal miles traveled: \033[0;34m";
	if(argc>=2 && argv[1]){
		miles = strtof(argv[1],NULL);
		cout << miles << endl;
	}else{
		cin >> miles;
	}
	cout << "\033[0mGallons of gas required: \033[0;35m";
	if(argc >=3 && argv[2]){
		gallons = strtof(argv[2],NULL);
		cout << gallons << endl;
	}else{
		cin >> gallons;
	}
	mpg = miles/gallons;
	if(mpg<=15) mpgColor = "\033[1;31m"; //red
	if(mpg>15)  mpgColor = "\033[1;33m"; //yellow
	if(mpg>30)  mpgColor = "\033[1;32m"; //green
	cout << "\033[0m\nYou traveled \033[0;34m" << miles;
	cout << "\033[0m miles on \033[0;35m" << gallons;
	cout << "\033[0m gallons of gas and got " << mpgColor;
	cout << fixed << setprecision(2) << mpg << "\033[0m miles per gallon.";
	cout << "\033[0;36m\n\n*** Thank you for using MPG Converter ***\n\033[0m\n";
	return 0;
}
