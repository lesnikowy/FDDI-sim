#include<iostream>
#include<string>
#include<math.h>
#include<vector>
#include<time.h>

using namespace std;

float rand_exp(float from, float to) {
      from*=100;
      to*=100;
      vector<float> v;
      int l=0;
      for(float i=from; i<to; i++){
          for( float j=0;j<exp(i);j++)
               v.push_back(j);  
               l++;              
      }
      return 1.0/v[rand()];
}

main() {
 for(int i=0; i<100; i++)
         cout << rand_exp(1.0,10.0) << endl;
 system("pause");      
}
