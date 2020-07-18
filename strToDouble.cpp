
#include <iostream>

#include <cstring>


using namespace std;


double strToDouble(const char* str){
	double id = 0;
	double fd = 0;

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
			id *= 10;
			id += i;
		}else{
			id += i * fractionalPosition;
			fractionalPosition *= 0.1;
		}		
	}

	return id;
}

int main(){

	const char* str = "3.142";

	double d = strToDouble(str);

	cout << d << endl;

	return 0;
}