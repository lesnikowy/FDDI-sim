#include<iostream>
#include<string>
#include<math.h>
#include<vector>
using namespace std;

long it_time = 0;
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
		INFO_l = ceil((float)DATA_l*5.0/4.0);
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
        int pasmo;
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

		Stacja(string, int, int);
		//newData(); tworzenie zmiennych;
		void send();
        void send_token();
        void trt_dec();
        void zapakuj();
		bool receiving;
		bool sending;
		long send_counter;
};

Stacja::Stacja(string str,int isyncTime, int ipasmo) {
	label = str;
	pasmo = ipasmo;
	syncTime = isyncTime;
	THT = 0;
	TRT = 0;
    pasmo = 0;
	syncData = 0;
	asyncData = 0;
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
void Stacja::trt_dec(){
     if(TRT>0)
        TRT--;
     else
       TRT=TTRT;
}

void Stacja::send_token(){
     pakiet = new Pakiet(true, 't', 1, nastepna, this, 0 );
     send();
}

void Stacja::zapakuj(){
     if(syncData > 0){
           if(asyncData > 9000){   
              pakiet = new Pakiet(true, 's', 1, nastepna, this, 9000 );
              syncData -= 9000;
           }else{
              pakiet = new Pakiet(true, 's', 1, nastepna, this, syncData );
              syncData = 0;   
           } 		   
     }else{
           if(asyncData > 9000){   
              pakiet = new Pakiet(true, 'a', 1, nastepna, this, 9000 );
              asyncData -= 9000;
           }else{
              pakiet = new Pakiet(true, 'a', 1, nastepna, this, asyncData );
              asyncData = 0;   
           }     
     }
     send();
}
void Stacja::send(){
     //dodac linijke, pakowania danych do pakietu, jezeli THT > 0 i istniej¹ dane do nadania. 
	if (pakiet != NULL and (token == true or receiving == true)) {
		if (sending == false){
			cout << label << " rozpoczal nadawanie pakietu." << endl;
			nastepna->receiving = true;
			sending = true;
			send_counter = pakiet->length();
		}
		else if (sending == true && send_counter > 0) {
			if ((pakiet->length() - pakiet->PA_l - pakiet->FS_l  == send_counter) && pakiet->fc->type == 't') {
               token = false;
               nastepna->token = true;
               if(nastepna->TRT > 0)
                  nastepna->THT = TRT;
               else
                  nastepna->THT = 0; 
               nastepna->TRT = TTRT;
            }
            if (pakiet->length() - pakiet->DA_l - pakiet->PA_l - pakiet->FS_l  == send_counter) {                
                if  (pakiet->DA == nastepna){
					cout << nastepna->label << " dostalem pakiet! ;)" << endl;
					cout <<  "Pakiet przyszedl od " << pakiet->SA->label << endl;
				}else{
					nastepna->pakiet=pakiet;
					nastepna->send();
					cout << nastepna->label << " przesyla dalej pakiet do " << nastepna->nastepna->label << endl;
				}
			}
			send_counter--;
		}
		else if (sending == true) {
			sending = false;
			nastepna->receiving = false;
			pakiet = NULL;
			cout << label << " zakonczyl nadawanie pakietu." << endl;
			if(asyncData == 0){
			   send_token();
            }else{
               if(asyncData > 9000){   
                  pakiet = new Pakiet(true, 'a', 1, nastepna, this, 9000 );
                  asyncData -= 9000;
               }else{
                  pakiet = new Pakiet(true, 'a', 1, nastepna, this, asyncData );
                  asyncData = 0;   
               }
               send();
            }
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
		int pasmo;
		cout << "Podaj nazwe stacji:" << endl;
		cin >> label;
		cout << "Podaj syncTime stacji:" << endl;
		cin >> syncTime;
		cout << "Podaj pasmo stacji:" << endl;
		cin >> syncTime;
		stacje.push_back(new Stacja(label, syncTime, pasmo));
	}
	TTRT = stacje[i]->syncTime;
	
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
		if(stacje[i]->syncTime < TTRT)
            TTRT = stacje[i]->syncTime;
	}
	for (int i = 0; i<ilosc_stacji;i++)
	    stacje[i]->pasmo = (stacje[i]->pasmo * TTRT)/stacje[i]->syncTime;
	int suma_pasm = 0; 
    for (int i = 0; i<ilosc_stacji;i++)
        suma_pasm += (stacje[i]->pasmo*stacje);
    if(suma_pasm > TTRT){
        cout << "Nie mo¿na utworzyc sieci o takich parametrach"
        return 0;
        }        
	//sztywny pakiet i token
	stacje[0]->pakiet = new Pakiet(true, 's', 0, stacje[2], stacje[0], 300);
    stacje[0]->token = true;
	for (it_time; it_time<stop_time; it_time++){
		for (int i = 0; i<ilosc_stacji;i++){
			stacje[i]->send();
		}
	}
	system("pause");
}
