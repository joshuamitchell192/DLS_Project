
#include <iostream>

#include <cstring>


using namespace std;


double strToDouble(const char* str){
	double d = 0;
	double fractionalPosition = 0.1;
	bool intOrFrac = true;
	for (int s = 0; s < strlen(str); s++){
		char c = str[s];
		if (c == '.'){
			intOrFrac = false;
			continue;
		}
		int i = c - 48;

		if(intOrFrac){
			d *= 10;
			d += i;
		}else{
			d += i * fractionalPosition;
			fractionalPosition *= 0.1;
		}		
	}
	return d;
}

int main(){

	const char* str = "3.142";

	double d = strToDouble(str);

	cout << d << endl;

	return 0;
}