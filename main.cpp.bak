#include<iostream>
#include<string>
#include<math.h>
#include<vector>
using namespace std;

long time = 0;
long stop_time;
int TTRT = 0;
class Stacja;

class FC_field {
public:
  bool l;
	char type; // a, s, n, c, b, t
	short pryiorytet;
	int length;
	FC_field(bool , char , short );
};

FC_field::FC_field(bool l1, char type1, short pryiorytet1){
	l = l1;
	type = type1;
	pryiorytet = pryiorytet1;
	length = 2*5;
}

// klasa pakietu
class Pakiet {
public:
	int PA_l;
	int SD_l;
	FC_field* fc;
	Stacja* DA;
	Stacja* SA;
	int DA_l;
	int SA_l;
	int INFO_l;
	Pakiet(bool , char, short, Stacja*, Stacja*, int);
	int FCS_l;
	int ED_l;
	int FS_l;
	bool FS_E;
	bool FS_A;
	bool FS_c;

	int length();
};

Pakiet::Pakiet(bool l, char t, short p, Stacja* DA1, Stacja* SA1, int DATA_l){
	PA_l = 16*5;
	SD_l = 4*20;
	fc = new FC_field(l, t, p);
	if (t != 't') {
		DA = DA1;
		SA = SA1;
		if (l == false){
			DA_l = 20;
			SA_l = 20;
		}else{
			DA_l = 60;
			SA_l = 60;
		}
		INFO_l = ceil((float)DATA_l*5/4);
		FCS_l = 40;

		FS_l = 5 * 3;
		FS_E = false;
		FS_A = false;
		FS_c = false;
	}
	else {
		DA_l = 0;
		SA_l = 0;
		INFO_l = 0;
		FCS_l = 0;
		FS_l = 0;
	}

	ED_l = 5;

	FS_l = 5 * 3;
	FS_E = false;
	FS_A = false;
	FS_c = false;
}

int Pakiet::length() {
	return PA_l + SD_l + fc->length + DA_l + SA_l + INFO_l + FCS_l + FS_l;
}


// klasa stacji
class Stacja {
	public:
		string label;

		int THT;
		int TRT;
		int syncTime;

		int syncData;
		int asyncData;

/*		int asyncData7;
		int asyncData6;
		int asyncData5;
		int asyncData4;
		int asyncData3;
		int asyncData2;
		int asyncData1;
		int asyncData0; */

		bool ct_late;

		bool monitor;
		bool token;

		Pakiet* pakiet;

		Stacja* nastepna;
		Stacja* poprzednia;

		Stacja(string, int);
		//newData(); tworzenie zmiennych;
		void send();

		bool receiving;
		bool sending;
		long send_counter;
};

Stacja::Stacja(string str,int isyncTime) {
	label = str;
	
	syncTime = isyncTime;
	THT = 0;
	TRT = 0;

	syncData = 0;
/*	asyncData7 = 0;
	asyncData6 = 0;
	asyncData5 = 0;
	asyncData4 = 0;
	asyncData3 = 0;
	asyncData2 = 0;
	asyncData1 = 0;
	asyncData0 = 0;*/
	ct_late = false;
	token = false;
	monitor = false;
	
	pakiet = NULL;
	receiving = false;
	sending = false;
	send_counter = 0;

}

void Stacja::send(){

	if (pakiet != NULL) {
		if (sending == false){
			cout << label << " rozpoczol nadawanie pakietu." << endl;
			
			nastepna->receiving = true;
			sending = true;
			send_counter = pakiet->length();
		}
		else if (sending == true && send_counter > 0) {
			send_counter--;
			if (pakiet->length() - pakiet->DA_l - pakiet->PA_l - pakiet->FS_l  == send_counter) {
				cout<< "Pakiet wskazuje na: " << pakiet->DA->label << endl;
				cout<< "Pakiet jest odbierany przez: " << nastepna->label << endl;
				if  (pakiet->DA == nastepna){
					cout << nastepna->label << " dostalem pakiet! ;)" << endl;
					cout <<  "Pakiet przyszedl od " << pakiet->SA->label << endl;
				}else{
					nastepna->pakiet=pakiet;
					nastepna->send();
					cout << nastepna->label << " przesyla dalej pakiet do " << nastepna->nastepna->label << endl;
				}
			}
		}
		else if (sending == true) {
			sending = false;
			pakiet = NULL;
			cout << label << " zakonczyl nadawanie pakietu." << endl;
		}
	}
}








int main() {
	vector<Stacja*> stacje;
	int ilosc_stacji;
	cout << "Podaj czas symulacji:" << endl;
	cin >> stop_time;
	cout << "Podaj liczbe stacji:" << endl;
	cin >> ilosc_stacji;
	for (int i = 0; i<ilosc_stacji;i++){
		string label;
		int syncTime;
		cout << "Podaj nazwe stacji:" << endl;
		cin >> label;
		cout << "Podaj syncTime stacji:" << endl;
		cin >> syncTime;
		stacje.push_back(new Stacja(label, syncTime));
	}
	for (int i = 0; i<ilosc_stacji;i++){
		if(i != 0){
			stacje[i]->poprzednia = stacje[i-1];
		}else{
			stacje[i]->poprzednia = stacje[ilosc_stacji-1];
		}
		if(i != ilosc_stacji-1){
			stacje[i]->nastepna = stacje[i+1];
		}else{
			stacje[i]->nastepna = stacje[0];
		}
		TTRT += stacje[i]->syncTime;
	}
	//sztywny pakiet
	stacje[0]->pakiet = new Pakiet(true, 's', 0, stacje[2], stacje[0], 300);

	for (time; time<stop_time; time++){
		for (int i = 0; i<ilosc_stacji;i++){
			stacje[i]->send();
		}
	}
	system("pause");
}
