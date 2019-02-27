#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
const byte LINHAS = 4;
const byte COLUNAS = 4;
char matriz_teclas[LINHAS][COLUNAS] = 
{
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte PinosLinhas[LINHAS] = {30, 32, 34, 36};
byte PinosColunas[COLUNAS] = {38, 40, 42, 45};
Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosLinhas, 
                            PinosColunas, LINHAS, COLUNAS); 
void(* resetFunc) (void) = 0;
float desu;
int lembrar,nrpm, ponto=250, trava=0;
float A=0, B=0;
float calib[256];
float tempo;
float x=0,x2=0,y=0,yx=0; 
float soma=0;
float crono;
float jubileu;
int passo=5;
int i, menu=0;;
int t=0;
int s, demo=1, pwm=0;
int v=0;
int tomate;
float calc;
float num=255;
float rpm=0;
int h=0;
char dado;
int dadu=0;
float cuck=100;
char tecla_pressionada = meuteclado.getKey();
LiquidCrystal lcd(12, 13, 14, 15, 16, 17);

struct MyObject {
  float field1;
  float field2;
};









void calibragem(){
  A=0;
  B=0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RPM selecionado");
  lcd.setCursor(3, 1);
  lcd.print(rpm,0);
  lcd.print("RPM");
  delay(3000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("EQ. LINEAR");
  lcd.setCursor(2, 1);
  lcd.print("Y = A*X + B");
  delay(5000);
   lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("INICIANDO");
  lcd.setCursor(6, 1);
  lcd.print("..."); 
  while(demo==1){

  if(trava==0){analogWrite(4,ponto); trava=1; soma=0;delay(5000);  }
    desu=pulseIn(11,HIGH);
  if(desu>100){
  
  desu=14900000/desu;
  if(desu<8000){
    if(i<30){
  soma=(soma+desu);
  i++
  ;}else{
    soma=soma/30;
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("RPM:");
    lcd.print(soma,0);
    lcd.setCursor(3, 1);
    lcd.print("PWM: ");
    lcd.print(ponto);
    Serial.print(soma);
    Serial.print(" , ");
    Serial.println(ponto);
      if(rpm<=soma)demo=1; else{ demo=0;pwm=ponto;}
      calib[ponto]=soma;
      trava=0;
      i=0;
      ponto=ponto-passo;
     
      desu=0;
        }
}
  }
 
}
 analogWrite(4,0);
Serial.println("mandando valor");
ponto=250;
i=0;
while((ponto)>=pwm){
//A=A+passo/(calib[ponto]-calib[ponto-passo]);

x=x+calib[ponto];
x2=x2+calib[ponto]*calib[ponto];
y=y+ponto;
yx=yx+calib[ponto]*ponto;
  ponto=ponto-passo;
  i++;
}
A=(y-yx*(i/x)) / (x-x2*(i/x));
B=(y-x*A)/i;

/*A=A/i;
ponto=250;
i=0;
while((ponto-passo)>=pwm){

  B=B+(ponto-A*calib[ponto]);

  ponto=ponto-passo;
  i++;
}
B=B/i;
*/

A=A*1000;
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Calibracao");
  lcd.setCursor(3, 1);
  lcd.print("concluida!");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A=");
  lcd.print(A/1000,4);
  lcd.setCursor(0, 1);
  lcd.print("B=");
  lcd.print(B);
  delay(5000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Reiniciando");
  lcd.setCursor(6, 1);
  lcd.print("...");
  delay(2000);
Serial.println(A/1000,3);
Serial.println(B);
  float f = 123.456f;  //Variable to store in EEPROM.
  int eeAddress = 0;   //Location we want the data to be put.
  EEPROM.put(eeAddress, f);
  MyObject customVar = {A, B,};
  eeAddress += sizeof(float); //Move address to the next byte after float 'f'.
  EEPROM.put(eeAddress, customVar);


h=0;
trava=0;
i=0;
ponto=255;


} //final da calibragem
void setup() {
 
 lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DIGITE O RPM");
  lcd.setCursor(0, 1);
  lcd.print("DEPOIS O TEMPO");
  pinMode(11, INPUT);
  pinMode(4, OUTPUT);
  analogWrite(4,0);
  
  float f = 0.00f;   //Variable to store data read from EEPROM.
  int eeAddress = 0; //EEPROM address to start reading from
  EEPROM.get(eeAddress, f);
  eeAddress = sizeof(float); //Move address to the next byte after float 'f'.
  MyObject customVar; //Variable to store custom object read from EEPROM.
  EEPROM.get(eeAddress, customVar);
  Serial.println("Read custom object from EEPROM: ");
  A=customVar.field1/1000;
  B=customVar.field2;
  Serial.println(A, 3);
  Serial.println(B);
}


void loop() {
 
  tecla_pressionada = meuteclado.getKey();
  if(h==1)desu=pulseIn(11,HIGH);
  if(desu>100){
  
  desu=14900000/desu;
  if(desu<8000){
    if(i<5){
  soma=(soma+desu);
  i++
  ;}else{
    soma=soma/5;
    tomate=soma;
    calc=(rpm-tomate)/rpm;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RPM:");
      lcd.print(soma, 0);
      lcd.print(" Dif:");
      lcd.print(abs(calc*100), 0);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print(" Falta: ");
      lcd.print((crono-(millis()-jubileu))/1000 , 0); //ARRUMEI AAA
      lcd.print(" seg");
     //Serial.print("seg");
     Serial.print((millis()-jubileu),0);
     Serial.print(" , ");
     Serial.println(soma, 0);
      soma=0;
      s=1;
      i=0;
        }
}
                            }

 

 if(tecla_pressionada){ // LAYOUT COMEÇA AQUI
   dado=tecla_pressionada;
    if(dado=='1'){rpm=rpm*10+1;lembrar=1;}
    if(dado=='2'){rpm=rpm*10+2;lembrar=2;}
    if(dado=='3'){rpm=rpm*10+3;lembrar=3;}
    if(dado=='4'){rpm=rpm*10+4;lembrar=4;}
    if(dado=='5'){rpm=rpm*10+5;lembrar=5; }
    if(dado=='6'){rpm=rpm*10+6;lembrar=6; }
    if(dado=='7'){rpm=rpm*10+7;lembrar=7; }
    if(dado=='8'){rpm=rpm*10+8;lembrar=8; }
    if(dado=='9'){rpm=rpm*10+9;lembrar=9; }
    if(dado=='0'){rpm=rpm*10+0;lembrar=0; }
    if(dado=='A'){nrpm=3000; crono=60000; t=2;}  //AQUI ESTA O MACROS (MACRO A)
    if(dado=='B'){nrpm=2500; crono=60000; t=2;} //MACRO B
    if(dado=='C'){nrpm=2000; crono=60000; t=2;}; //MACRO C
    if(dado=='D'){ if(menu==0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MODO CALIBRAGEM");
    lcd.setCursor(6, 1);
    lcd.print("...");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DIGITE A ROTACAO");
    lcd.setCursor(5, 1);
    lcd.print("LIMITE");
    delay(5000);
      menu=1;
    }
      
      
      
      
      
    
    } // AQUI TERMINA DOS MACROS (MACRO D)
    if(dado=='#'){rpm=(rpm-lembrar)/10; if(rpm<=0)rpm=0;}
    if(rpm>=8050){ rpm=0;
       
    }  
    
    if(t==0 && menu==0){
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("RPM:");
    lcd.print(rpm,0);
    lcd.setCursor(0, 1);
     lcd.print("* para continuar");
    
    }

    if(t==0 && menu==1){
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("RPM:");
    lcd.print(rpm,0);
    lcd.setCursor(0, 1);
     lcd.print("* para calibrar");
    }
  /* if(t==1 && menu==1){
   calibragem(); rpm=0;
    }   */
    if(t==1 && menu==0){
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Tempo:");
  lcd.setCursor(4, 1);
  lcd.print(rpm,0);
  lcd.print(" seg");
              }

    
    if(dado=='D'){ num=0; analogWrite(4,num); digitalWrite(A0,0);digitalWrite(A0,1);}



if(dado=='*' && rpm!=0 || dado=='A' || dado=='B' || dado=='C'){ 
if(menu==1){calibragem(); resetFunc();}
if(t==0 && menu==0){
 
nrpm=rpm;
rpm=0;
lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Tempo:");
  lcd.setCursor(4, 1);
  lcd.print(rpm,0);
  lcd.print(" seg");
  
}
if(t==1 && menu==0){
    crono=rpm*1000;
    lcd.clear();
     lcd.setCursor(2, 0);
  lcd.print("Aperte '*'");
  lcd.setCursor(2, 1);
  lcd.print("para iniciar");
}





if(t==2 && menu==0){
 rpm=nrpm;  

  num=rpm*A+B; 
  analogWrite(4,255);
  h=1; tempo=millis(); dado='i'; jubileu=millis();  
  while(soma<=0.9*rpm){
  desu=pulseIn(11,HIGH);
  if(desu>100){
  
  desu=14900000/desu;
  if(desu<8000){
    if(i<1){
  soma=desu;
  i++;}else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RPM:");
      lcd.print(soma, 0);
      lcd.print(" Dif:");
      lcd.print(abs(calc*100), 0);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print(" Falta: ");
      lcd.print((crono-(millis()-jubileu))/1000 , 0); //ARRUMEI AAA
      lcd.print(" seg");
     Serial.print((millis()-jubileu),0);
     Serial.print(" , ");
     Serial.println(soma, 0);
      soma=0;
      i=0;
        }
  }}}
  
  
    
    if(num<=255 && num>=0){
        analogWrite(4,num);
            }if(num>255){num=255; analogWrite(4,num);}
                 if(num<0){num=0; analogWrite(4,num);}
                          
                           

     
               }
    t++; 
               }
            
}



if(h==1 && (millis()-tempo)>=cuck && s==1 ){ // CORREÇÃO DE VEL COMEÇA AQUI
  
  //Serial.print(calc);
  //Serial.println("%");
 if(abs(calc)>0.01 ){cuck=50;}
 if(abs(calc)<=0.01){cuck=100;}
if( calc>=0.01 && num<252 && calc<0.1 ){
 num=num+3;
  analogWrite(4,num); 
}
if( calc>=0.005 && num<255){
 num++;  
  analogWrite(4,num); 
}


if( calc<=-0.005 && num>0 && calc>-0.1 ){
  num--;
  analogWrite(4,num);
}
if( calc<=-0.1 && num>2){
  num=num-3;
  analogWrite(4,num); 
}

if(calc<=0.01 && calc>=-0.01)v=1;
tempo=millis();
s=0;  

}


if((crono-(millis()-jubileu)<=0) && h==1){resetFunc();}
  
}
                                            



                         
