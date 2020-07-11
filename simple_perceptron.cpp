#include<iostream>
#include<fstream>//ファイル読み込み用
#include<sstream>//文字列を数値へ変換する
//#include<stdio.h>
//#include<string>
#include<map>//map用
//#include<stdlib.h>
using namespace std;

class Iris{
private:
  static const double p = 0.25;//学習率
  static const int articles = 150;//全データ
  static const int feature = 4;//菖蒲の特徴数
  static const int species = 3;//分類する菖蒲の種類
public:
  //アヤメの特徴を格納する配列
  string Iris_data[articles][feature+1];
  double Iris_setosa[articles/species][feature];
  double Iris_versicolor[articles/species][feature];
  double Iris_virginica[articles/species][feature];

  //拡張重みベクトル
  double w[5];

  void readIris();//ファイルの読み込みを行う
  bool simple_perceptron();//setosaとversicolorを比較
  bool simple_perceptron2();//setosaとvirginiaを比較
  bool simple_perceptron3();//versicolorとvirginicaを比較

  Iris(){  
    for(int i=0; i < feature+1; i++){ w[i] = 0.1;}//重みの初期化:全て0.1
  };

};

//ファイルの読み込み////////////////////////////////////////////////////////////
void Iris::readIris(){
  //データファイルを読み込む
  ifstream ifs("bezdekIris.data");
 
  // 開かなかったらエラー
  if (!ifs){
    cout << "Error! File can not be opened" << endl;
    return;
  }
    
  string str = "";
  int i = 0;  // Iris[i][ ]のカウンタ。
  int j = 0;  // Iris[ ][j]のカウンタ。
 
  // ファイルの中身を一行ずつ読み取る
  while(getline(ifs, str)){
    string tmp = "";
    istringstream stream(str);
 
    // 区切り文字がなくなるまで文字を区切っていく
    while (getline(stream, tmp, ',')){
      // 区切られた文字がtmpに入る
      Iris_data[i][j] = tmp;
      j++;
    }	
    j = 0;
    i++;
  }
  //stringで読み込んでいるので,stringstreamでstring型をdouble型に変換
  for(int i=0; i < articles; i++){//菖蒲の数
    for(int j=0; j < feature; j++){//特徴の数
      stringstream buf;
      buf << Iris_data[i][j];
      double d;
      buf >> d; 
      if(i<articles/species) Iris_setosa[i][j] =d;//sretosa
      else if(i<(articles/species)*2)Iris_versicolor[i%50][j] =d;//versicolor
      else if(i<articles)Iris_virginica[i%100][j] =d;//virginica
    }
  }
  
  
  return;
}
////////////////////////////////////////////////////////////////////////////



//setosaとversicolorを識別////////////////////////////////////////
bool Iris::simple_perceptron(){

  double sum1;
  double sum2;
  int complete = 0;//比較回数のカウンター（正しく分離できて入れば++）
  double w1[5];//setosaの重み
  double w2[5];//versicolorの重み
  for(int i=0; i<5;i++){
    w1[i] = w[i];
    w2[i] = w[i]-0.1;
  }

  int times = 0;

  while(times< 10000){ 
    complete = 0;
    // setosaの学習データに関する修正///////////////////////////////////////
    for(int i=0; i<50; i++){
      sum1 = 0;
      sum2 = 0;
      //setosa
      for(int j=0; j<5; j++){
	if(j==0)sum1 += 1*w1[j];//係数用
	else sum1 += Iris_setosa[i][j-1]*w1[j];
      }
      //versicolor
      for(int j=0; j<5; j++){
	if(j==0)sum2 += 1*w2[j];
	else sum2 += Iris_setosa[i][j-1]*w2[j];
      }    
      //cout << sum1 << "-" << sum2 << endl; 
      //重みベクトルの修正
      if(sum1-sum2 > 0)complete++;
      else{
	for(int j=0;j<5;j++){
	  if(j==0){
	    w1[j] = w1[j]+p*1;
	    w2[j] = w2[j]-p*1;
	  }else{ 
	    w1[j] = w1[j]+(p*Iris_setosa[i][j-1]);
	    w2[j] = w2[j]-(p*Iris_setosa[i][j-1]);
	  }
	}
      }

    }
    // versicolorの学習データに関する修正///////////////////////////////////
    for(int i=0; i<50; i++){
      sum1 = 0;
      sum2 = 0;
      //setosa
      for(int j=0; j<5; j++){
	if(j==0)sum1 += 1*w1[j];//係数用
	else sum1 += Iris_versicolor[i][j-1]*w1[j];
      } 
      //versicolor
      for(int j=0; j<5; j++){
	if(j==0)sum2 += 1*w2[j];
	else sum2 += Iris_versicolor[i][j-1]*w2[j];
      }   
      //cout << sum1 << "-" << sum2 << endl; 
      //重みベクトルの修正
      if(sum1-sum2 < 0)complete++;
      else{
	for(int j=0;j<5;j++){
	  if(j==0){
	    w1[j] = w1[j]-p*1;
	    w2[j] = w2[j]+p*1;
	  }else{ 
	    w1[j] = w1[j]-(p*Iris_versicolor[i][j-1]);
	    w2[j] = w2[j]+(p*Iris_versicolor[i][j-1]);
	  }
	}
      }
    }

    
    if(complete == 100){
      cout << times << "回繰り返し" << endl;
      cout << "setosa:";
      for(int i = 0; i<5; i++)
	cout << w1[i] << ", ";  
      cout << endl;
      cout << "versicolor:";
      for(int i = 0; i<5; i++)
	cout  << w2[i] << ", ";  
      cout << endl;
      for(int i = 0; i< 5; i++){
	cout << w1[i]-w2[i] <<", ";
      }
      cout << endl;
      return true;
    }
    
  times++;
  }
  cout <<times << "回繰り返し" << endl;
  return false;
}
////////////////////////////////////////////////////////////////////////


//setosaとvirginicaを識別する///////////////////////////////////////////
bool Iris::simple_perceptron2(){

  double sum1;  
  double sum2;
  int complete = 0;
  double w1[5];//setosaの重み
  double w2[5];//virginicaの重み
  for(int i=0; i<5;i++){
    w1[i] = w[i];
    w2[i] = w[i]-0.1;
  }

  int times = 0;

  while(times< 10000){ 
    complete = 0;
    // setosaの学習データに関する修正///////////////////////////////////////
    for(int i=0; i<50; i++){
      sum1 = 0;
      sum2 = 0;
      //setosa
      for(int j=0; j<5; j++){
	if(j==0)sum1 += 1*w1[j];//係数用
	else sum1 += Iris_setosa[i][j-1]*w1[j];
      }
      //virginica
      for(int j=0; j<5; j++){
	if(j==0)sum2 += 1*w2[j];
	else sum2 += Iris_setosa[i][j-1]*w2[j];
      }    
      //cout << sum1 << "-" << sum2 << endl; 
      //重みベクトルの修正
      if(sum1-sum2 > 0)complete++;
      else{
	for(int j=0;j<5;j++){
	  if(j==0){
	    w1[j] = w1[j]+p*1;
	    w2[j] = w2[j]-p*1;
	  }else{ 
	    w1[j] = w1[j]+(p*Iris_setosa[i][j-1]);
	    w2[j] = w2[j]-(p*Iris_setosa[i][j-1]);
	  }
	}
      }

    }
    // virginicaの学習データに関する修正///////////////////////////////////
    for(int i=0; i<50; i++){
      sum1 = 0;
      sum2 = 0;
      //setosa
      for(int j=0; j<5; j++){
	if(j==0)sum1 += 1*w1[j];//係数用
	else sum1 += Iris_virginica[i][j-1]*w1[j];
      } 
      //virginica
      for(int j=0; j<5; j++){
	if(j==0)sum2 += 1*w2[j];
	else sum2 += Iris_virginica[i][j-1]*w2[j];
      }   
      //cout << sum1 << "-" << sum2 << endl; 
      //重みベクトルの修正
      if(sum1-sum2 < 0)complete++;
      else{
	for(int j=0;j<5;j++){
	  if(j==0){
	    w1[j] = w1[j]-p*1;
	    w2[j] = w2[j]+p*1;
	  }else{ 
	    w1[j] = w1[j]-(p*Iris_virginica[i][j-1]);
	    w2[j] = w2[j]+(p*Iris_virginica[i][j-1]);
	  }
	}
      }
    }

    
    if(complete == 100){
      cout << times << "回繰り返し" << endl;
      cout << "setosa:";
      for(int i = 0; i<5; i++)
	cout << w1[i] << ", ";  
      cout << endl;
      cout << "virginica:";
      for(int i = 0; i<5; i++)
	cout  << w2[i] << ", ";  
      cout << endl;
      for(int i = 0; i< 5; i++){
	cout << w1[i]-w2[i] <<", ";
      }
      cout << endl;
      return true;
    }
    
  times++;
  }
  cout <<times << "回繰り返し" << endl;
  return false;
}
/////////////////////////////////////////////////////////////////////////


//versicolorとvirginicaを識別する///////////////////////////////////////
bool Iris::simple_perceptron3(){
 
  double sum1;  
  double sum2;
  int complete = 0;
  double w1[5];//versicolorの重み
  double w2[5];//virginicaの重み
  for(int i=0; i<5;i++){
    w1[i] = w[i];
    w2[i] = w[i]-0.1;
  }

  int times = 0;

  while(times< 10000){ 
    complete = 0;
    // vergicolorの学習データに関する修正///////////////////////////////////////
    for(int i=0; i<50; i++){
      sum1 = 0;
      sum2 = 0;
      //versicolor
      for(int j=0; j<5; j++){
	if(j==0)sum1 += 1*w1[j];//係数用？
	else sum1 += Iris_versicolor[i][j-1]*w1[j];
      }
      //virginica
      for(int j=0; j<5; j++){
	if(j==0)sum2 += 1*w2[j];
	else sum2 += Iris_versicolor[i][j-1]*w2[j];
      }    
      //cout << sum1 << "-" << sum2 << endl; 
      //重みベクトルの修正
      if(sum1-sum2 > 0)complete++;
      else{
	for(int j=0;j<5;j++){
	  if(j==0){
	    w1[j] = w1[j]+p*1;
	    w2[j] = w2[j]-p*1;
	  }else{ 
	    w1[j] = w1[j]+(p*Iris_versicolor[i][j-1]);
	    w2[j] = w2[j]-(p*Iris_versicolor[i][j-1]);
	  }
	}
      }

    }
    // virginicaの学習データに関する修正///////////////////////////////////
    for(int i=0; i<50; i++){
      sum1 = 0;
      sum2 = 0;
      //versicolor
      for(int j=0; j<5; j++){
	if(j==0)sum1 += 1*w1[j];//係数用？
	else sum1 += Iris_virginica[i][j-1]*w1[j];
      } 
      //virginica
      for(int j=0; j<5; j++){
	if(j==0)sum2 += 1*w2[j];
	else sum2 += Iris_virginica[i][j-1]*w2[j];
      }   
      //cout << sum1 << "-" << sum2 << endl; 
      //重みベクトルの修正
      if(sum1-sum2 < 0)complete++;
      else{
	for(int j=0;j<5;j++){
	  if(j==0){
	    w1[j] = w1[j]-p*1;
	    w2[j] = w2[j]+p*1;
	  }else{ 
	    w1[j] = w1[j]-(p*Iris_virginica[i][j-1]);
	    w2[j] = w2[j]+(p*Iris_virginica[i][j-1]);
	  }
	}
      }
    }

    
    if(complete == 100){
      cout << times << "回繰り返し" << endl;
      cout << "versicolor:";
      for(int i = 0; i<5; i++)
	cout << w1[i] << ", ";  
      cout << endl;
      cout << "virginica:";
      for(int i = 0; i<5; i++)
	cout  << w2[i] << ", ";  
      cout << endl;
      for(int i = 0; i< 5; i++){
	cout << w1[i]-w2[i] <<", ";
      }
      cout << endl;
      return true;
    }
    
  times++;
  }
  cout << times << "回繰り返し" << endl;
  return false;
}
//////////////////////////////////////////////////////////////////



//main文/////////////////////////////////////////////////////////
int main(void){
  Iris q;
  q.readIris();

  //特徴を出力する
  /*  
    for(int i=0; i<150; i++){
      for(int j=0; j<4; j++){
	if(i < 50)cout << q.Iris_setosa[i][j] << ", ";
	else if(i<100)cout << q.Iris_versicolor[i%50][j] << ", ";
	else if(i<150)cout << q.Iris_virginica[i%100][j] << ", ";
      }
      cout << endl;
    }
    cout << endl;
  */


  cout << "重みの初期値 : ";
  for(int i = 0; i<5; i++)
    cout << q.w[i] << ", ";  
  cout << endl;
  for(int i=0;i<60;i++) cout <<"-";
  cout <<endl;

  if(q.simple_perceptron()){
    cout << "線形分離可能である" <<endl;
  }else{cout << "線形分離可能でないと判断" << endl;}
 
  for(int i=0;i<60;i++) cout <<"-";
  cout <<endl;

  if(q.simple_perceptron2()){
    cout << "線形分離可能である" <<endl;
  }else{cout << "線形分離可能でないと判断" << endl;}

  for(int i=0;i<60;i++) cout <<"-";
  cout << endl;
  
  if(q.simple_perceptron3()){
    cout << "線形分離可能である" <<endl;
  }else{cout << "線形分離可能でないと判断" << endl;}
 

  return 0;
}
