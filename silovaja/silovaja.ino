//error=1   превышение оборотов
//error=2   нет вращения
//error=3   показания датчика температуры за пределами допустимого
//error=4   за 200 секунд не сработал датчик заполнения бака
//error=5   за 180 секунд не слилась вода
//error=6   проблемы с блокировкой двери

//error=9  переполнение бака
//--------------------------------------------------------------
#include <Arduino


  
//---------------для вращения двигателя--------------------------------
volatile int dimming = 140;  // время задержки от нуля   7 = максимально, 130 = минимально
volatile unsigned long time; // время в микросекундах срабатывания датчика нуля
unsigned long tims;           // переменная показаний времени

int obMin = 300;          //ввести минимальные обороты
int kImp = 70;          //ввести кол-во импульсов на 10 оборотов
int holl = 0;    //переменная  срабатываня датчика
int pR;                  // показания регулятора
int pRR;                // переменная для расчёта.
int ogr ;                 //переменная ограничений симистора на текущих оборотах
volatile int sp = 0;           //переменная суммы срабатываний датчика
volatile int prOb ;   //предвар реальн обороты
volatile int rOb ;    // реальные обороты

volatile unsigned int int_tic;    //переменные для подсчёта времени между импульсами.
volatile unsigned long tic;
volatile int t2 = 0;               //минимальное время импульсов +1
int val=0;

long totgm=0;
long e2;

             

int obMax = 1200;            //ввести максимальные обороты
int minzn = 115;            //  минимальное значение симмистора на котором начинается вращение.
int ogrmin =  70 ;          // ограничение симистора на минимальных оборотах.
int mindimming = 80;       //значение симистора при закллинившем станке (первоначальный импульс)
int dopusk =  1000 ;         //допуск оборотов в минус и плюс
int razgon = 1;             //переменная разгона 1 - 100
int zaschita = 300;

//-----------------------------------------------------------------------------------------------




String StringtoSend;
int etap;
int oetap=0;            //для этапов отжима
int pusk=0;              // 1 - цикл запущен, 0 - нет  (при подключенной плате индикации pusk = 0) 
int stirka=0;            // статус стирки
int poloskanije=0;       // статус полоскания
int sliv=0;              // статус слива
int otgim=0;             // статус отжима
int start=0;


int timeHour=0;           // время всего цикла стирки (часы) (при подключенной плате индикации равно 0)
int timeMin=0;            // время всего цикла стирки (минуты) (при подключенной плате индикации равно 0)  
int polosk=2;             // колличество полосканий (при подключенной плате индикации равно 0) 
int temperatureUst=20;    // переменная установленной температуры - при подключенной плате индикации считывается с нее


long interval,ms8;
long ms,mszap,mszaliv,mssls,mssliv,mstime,timerotation;
bool zap=0;
bool startzaliv=0;
bool sl=0;
bool timerun=0;           //разрешить отсчёт времени

bool pu=0;
bool otgimsmal=0;         //короткий отжим
bool slivst=0;
bool rotation,rotation1;
bool zah;


int ksum=0;
int t=0;
int temperature=20;
int error=0;              // код ошибки
int tst,tpol,kolotg;         
int tpol1=0;
int totg=0;
int izamok=0;
int zamok;
int tst1=0;
//----------------------------------------------------------------------------------------


void setup() {
  
  Serial.begin(9600);
 

  pinMode(A0, INPUT);      // вход от датчика температуры
  
  pinMode(A1, OUTPUT);     // клапан залива 1
  pinMode(A2, OUTPUT);     // клапан залива 2
  pinMode(A3, OUTPUT);     // ТЭН
  pinMode(A4, OUTPUT);     // реле реверса 1
  pinMode(A5, OUTPUT);     // реле реверса 2
  
  pinMode(4, OUTPUT);       // питание замка
  pinMode(5, OUTPUT);       // двигатель ШИМ

  pinMode(6, INPUT);       // бак заполнен 
  pinMode(7, INPUT);       // переполнение бака
  pinMode(8, INPUT);       // прерывание по таймеру
  pinMode(9, INPUT);       // вход подтверждение закрытия замка
  
  
  pinMode(11, OUTPUT);     // сливной насос
  
  attachInterrupt(0, zero_crosss_int, RISING);  // прерывание по пину 2

 
  
  //настройка 16 бит таймера-счётчика 1
  TCCR1B = 0; TCCR1A = 0; TCNT1 = 0;
  TIMSK1 = (1 << ICIE1) | (1 << TOIE1); //создавать прерывание от сигнала на пине ICP1
  TCCR1B = (1 << ICNC1) | (1 << ICES1) | (1 << CS10); //div 1



 
 
}




void loop() {

if (millis()-interval>500) {SerialREAD(); interval=millis();}   //----------приём данных---------------




//--------------------отправка данных в плату индикации----------------------------------------
 if ( millis()-ms8>500) {
if(digitalRead(9)==1) zamok=0;else zamok=1;
if (stirka==1) etap=1;
if (poloskanije==1 && stirka==0) etap=2;
if (poloskanije==0 && stirka==0 && otgim==1) etap=4;
if (pusk==0 && pu==1) {etap=5;pu=0;}
ksum=timeHour+timeMin+temperature+pusk+error+etap+zamok;
StringtoSend=String(timeHour)+";"+String(timeMin)+";"+String(temperature)+";"+String(pusk)+";"+String(error)+";"+String(etap)+";"+String(zamok)+";"+String(ksum)+":";
Serial.println(StringtoSend);

 ms8=millis();
                }

//--------------------------------------------------------------------------------------------- 

  
  



//-------------------опрос датчика температуры -------------------------------------------------

  if (millis()-ms>1000) {
         t=analogRead(A0);
         if (t<50 || t>990) error=3;        // датчик температуры за пределами нормы 
         temperature=datchik(t);

            ms=millis();            
                        }
//----------------------------------------------------------------------------------------------------------









//-------------------отсчет времени---------------------------------------------------------------------------


if (millis()-mstime>=60000 && timerun==1) {
  
   timeMin=timeMin-1;
   
   if (stirka==1) tst=tst-1;
   if (poloskanije==1 && stirka==0 && otgimsmal==0) tpol1=tpol1-1;
   if (otgimsmal==1 && sl==1) totg=totg-1;
   if (otgim==1 && sl==1) totg=totg-1;
   

   
                                       if (timeMin<0) {
                                        
                                        timeHour=timeHour-1;
                                        if (timeHour<0) {                //время истекло
                                          timeMin=1;
                                          timeHour=0;
                                                        
                                          
                                          } else timeMin=59;
                                        }
                      if  (timeHour==0 && timeMin==0)  {                 //время истекло
                                          
                                          timeMin=1;
                                          timeHour=0;
                                          
                                             }
                                          
  mstime=millis();
  }
//-----------------------------------------------------------------------------








// время цикла стирки = время полученное с платы индикации - (3мин* кол-во полосканий) - 8 мин отжим - (3 мин * кол - во промежуточных отжимов) 
// кол-во промежуточных отжимов = стирка + (кол-во полосканий -1)                        

//------------------- цикл запущен--------------------------------------------

if (pusk==1 && error==0 && pu==0) {
  tpol1=3;
  tpol=polosk*3;
  
  kolotg=polosk-1;
  totg=1;                                             // 1мин +2мин+4мин время основного отжима - 3 этапа
  if (stirka==1) kolotg=kolotg+1;
  tst=(timeMin+timeHour*60)-tpol-8-3*kolotg;       //вычисление времени для этапа стирки  
  if (tst<5) tst=5;                                   
 zah=0;
  
  



  while(digitalRead(9)==1 && izamok<=3){
  
  digitalWrite(4,1);                      // подать импульс на замок
  delay(5);
  digitalWrite(4,0);
  delay(300);
  izamok++;
  }
  if (digitalRead(9)==1) error=6; else {
  rotation=1;                             // разрешить вращение двигателя
  mstime=millis();
  izamok=0;
   
  }
 pu=1;                                   // чтобы сюда больше не заходить 
}   
//----------------------------------------------------------------------------




if (pusk==0 && pu==1 ) {etap=5;stopr();}


  
//-----------------------замок закрылся---------------------------------------

if (pusk==1 && digitalRead(9)==LOW) {
  




//-----------------------------------------этап стирки------------------------  
  
  if (stirka==1) {              



   
//------------------------------параметры для мотора-------------------------------
    
 obMin = 300;
 obMax = 1200;         //ввести максимальные обороты
 minzn = 115;           //  минимальное значение симмистора на котором начинается вращение.
 ogrmin =  60 ;         // ограничение симистора на минимальных оборотах.
 mindimming = 80;        //значение симистора при закллинившем станке (первоначальный импульс)
 dopusk =  1000 ;         //допуск оборотов в минус и плюс
 razgon = 1;            //переменная разгона 1 - 100
 zaschita = 300;
 pRR = obMin;
 t2 = (15000 / ( obMin * (kImp / 10))) * 2; 
    //-------------------------------------------------------------------------------
    
    
    
    
    
    zapolnenie();  //------залив воды--------


    //----------------контроль температуры--------------------------------
    if (digitalRead(6)==0) if (temperature<temperatureUst-2) digitalWrite(A3,1);
    if (digitalRead(6)==1) digitalWrite(A3,0);
    if (temperature>=temperatureUst)  digitalWrite(A3,0);
   //---------------------------------------------------------------------




    
//----------вращение барабана во время стирки----------------------------------------------------------------------------------

if (rotation==0) {digitalWrite(A4,0); digitalWrite(A5,0);}

if (rotation==1 && rotation1==0) {timerotation=millis();rotation1=1;}

if (millis()-timerotation>2000 && millis()-timerotation<=2500 && rotation==1) digitalWrite(A4,0);  // установка направления вращения
if (millis()-timerotation>2500 && millis()-timerotation<=4000 && rotation==1) digitalWrite(A5,1);

if (millis()-timerotation>4000 && millis()-timerotation<=24000 && rotation==1) {val=400;motor1();}  //подать ШИМ на симмистр
if (millis()-timerotation>24000 && millis()-timerotation<=26000 && rotation==1) {val=0;motor1();}  //снять ШИМ с симмистора


if (millis()-timerotation>26000 && millis()-timerotation<=27000 && rotation==1) digitalWrite(A5,0);  // установка направления вращения
if (millis()-timerotation>27000 && millis()-timerotation<=27500 && rotation==1) digitalWrite(A4,1);

if (millis()-timerotation>28000 && millis()-timerotation<=48000 && rotation==1) {val=400;motor1();}  //подать ШИМ на симмистр
if (millis()-timerotation>48000 && millis()-timerotation<=49000 && rotation==1) {val=0;motor1();}  //снять ШИМ с симмистора
if (millis()-timerotation>49000 && rotation==1) rotation1=0;

//-----------------------------------------------------------------------------------------------------------------------------    




    if (tst<=0) {     // время стирки истекло 
       tst=0;
       tpol1=3;
       totg=3;
       val=0;
       digitalWrite(A3,0);
       digitalWrite(A4,0);
       digitalWrite(A5,0);
       stirka=0;
       rotation=0;
       otgimsmal=1;
       slivst=0;
       sl=0;
       zah=0;
       startzaliv=0;
       timerun=0;
         }
  
    }
 //------------------------------------------------------------------------------------------------------------------------------









  //------------------------ процесс полоскания----------------------------------------------------------------------------------
 
 if (poloskanije==1 && otgimsmal==0 && stirka==0 && otgim==0 && sliv==0) {   
  
 obMin = 300;
 obMax = 1200;         //ввести максимальные обороты
 minzn = 115;           //  минимальное значение симмистора на котором начинается вращение.
 ogrmin =  60 ;         // ограничение симистора на минимальных оборотах.
 mindimming = 80;        //значение симистора при закллинившем станке (первоначальный импульс)
 dopusk =  1000 ;         //допуск оборотов в минус и плюс
 razgon = 1;            //переменная разгона 1 - 100
 zaschita = 300;
 pRR = obMin;
 t2 = (15000 / ( obMin * (kImp / 10))) * 2;

  
     zapolnenie();  //---залив воды----

     
     
//----------вращение барабана во время полоскания-------------

if (rotation==0) {digitalWrite(A4,0); digitalWrite(A5,0);}

if (rotation==1 && rotation1==0) {timerotation=millis();rotation1=1;}

if (millis()-timerotation>2000 && millis()-timerotation<=2500 && rotation==1) digitalWrite(A4,0);  // установка направления вращения
if (millis()-timerotation>2500 && millis()-timerotation<=4000 && rotation==1) digitalWrite(A5,1);

if (millis()-timerotation>4000 && millis()-timerotation<=24000 && rotation==1) {val=500;motor1();}  //подать ШИМ на симмистр
if (millis()-timerotation>24000 && millis()-timerotation<=26000 && rotation==1) {val=0;motor1();}  //снять ШИМ с симмистора


if (millis()-timerotation>26000 && millis()-timerotation<=27000 && rotation==1) digitalWrite(A5,0);  // установка направления вращения
if (millis()-timerotation>27000 && millis()-timerotation<=27500 && rotation==1) digitalWrite(A4,1);

if (millis()-timerotation>28000 && millis()-timerotation<=48000 && rotation==1) {val=500;motor1();}  //подать ШИМ на симмистр
if (millis()-timerotation>48000 && millis()-timerotation<=49000 && rotation==1) {val=0;motor1();}  //снять ШИМ с симмистора
if (millis()-timerotation>49000 && rotation==1) rotation1=0;


//--------------------------------------------------------    



     
     if (tpol1<=0) {     // время полоскания истекло 
     polosk=polosk-1;
     val=0;
     rotation=0;
     digitalWrite(A4,0);
     digitalWrite(A5,0);
     
     if (polosk<=0) {    //перейти на основной отжим
     poloskanije=0;
     tpol1=0;
     startzaliv=0;
     slivst=0;
     sl=0;
     timerun=0;
     totg=1;  //время первого этапа отжима
     zah=0;
     timeMin=7;
     timeHour=0;
     oetap=0;
     otgim=1;
     
   
     } 
  if (polosk>0) {    //перейти на короткий отжим
     poloskanije=1;
     val=0;
     rotation=0;
     tpol1=3;
     totg=3;
     startzaliv=0;
     otgimsmal=1;
     slivst=0;
     sl=0;
     timerun=0;
     zah=0;
       
     }     
      }
     
    }
//-------------------------------------- полоскание --------------------------------------------------











//--------------------------программа просто слив воды-------------------------------------------------
if (sliv==1) {            
    digitalWrite(11,1);
if (digitalRead(6)==0 && slivst==0) {slivst=1;mssls=millis();}
if (digitalRead(6)==1 && sl==0) {sl=1;timerotation=millis();}
if (millis()-mssls>180000 && digitalRead(6)==0) error=5;
if (millis()-timerotation>30000 && digitalRead(6)==1) {
                                 digitalWrite(11,0);
                                 sliv=0;
                                  }
   
       }
//-----------------------------------------------------------------------------------------------------------------






//------------------------- короткий отжим ---------------------------------------- 
  
if  (otgimsmal==1) {

 obMin = 300;
 obMax = 9000;         //ввести максимальные обороты
 minzn = 120;           //  минимальное значение симмистора на котором начинается вращение.
 ogrmin =  60 ;         // ограничение симистора на минимальных оборотах.
 mindimming = 80;        //значение симистора при закллинившем станке (первоначальный импульс)
 dopusk =  3000 ;         //допуск оборотов в минус и плюс
 razgon = 1;            //переменная разгона 1 - 100
 zaschita = 300;
 pRR = obMin;
 t2 = (15000 / ( obMin * (kImp / 10))) * 2; 

    
    
    digitalWrite(11,1);
if (digitalRead(6)==0 && slivst==0) {slivst=1;mssls=millis();}
if (digitalRead(6)==1 && sl==0) {timerun=1;sl=1;timerotation=millis();}
if (millis()-mssls>180000 && digitalRead(6)==0) error=5;

//--------------------вращение барабана при коротком отжиме---------------------
if (millis()-timerotation>20000 && millis()-timerotation<=22000 && digitalRead(6)==1) {digitalWrite(A4,0);digitalWrite(A5,1);}     //установить направление вращения
if (millis()-timerotation>22000 && digitalRead(6)==1 && zah==0)  {zah=1;totgm=millis();val=15;}  // подать ШИМ на двигатель
motor2();
if (millis()-totgm>500 && val>0 && otgimsmal==1 && totg>0) {
val=val+2;

if (val>600) val=600;

  totgm=millis();
  }
 

if (totg<=0){          // время истекло
  
  val=0; digitalWrite(A4,0);digitalWrite(A5,0);
  
  if (pulseIn(8,LOW)<100) {otgimsmal=0; zah=0;rotation=1;digitalWrite(11,0);}     //------выполнить после полного останова двигателя--------- 
  
  }

} 
//--------------------------Конец короткий отжим----------------------------------------------------------












//------------------------- основной отжим ----------------------------------------------------------- 
  
if  (otgim==1 && stirka==0 && sliv==0 && poloskanije==0 && otgimsmal==0) {    
 
 obMax = 25000;         //ввести максимальные обороты
 minzn = 115;           //  минимальное значение симмистора на котором начинается вращение.
 ogrmin =  60 ;         // ограничение симистора на минимальных оборотах.
 mindimming = 90;        //значение симистора при закллинившем станке (первоначальный импульс)
 dopusk =  3000 ;         //допуск оборотов в минус и плюс
 razgon = 1;            //переменная разгона 1 - 100
 zaschita = 300;
 obMin=300;
 pRR = obMin;
 t2 = (15000 / ( obMin * (kImp / 10))) * 2; 
  
    
    
    digitalWrite(11,1);
if (digitalRead(6)==0 && slivst==0) {slivst=1;mssls=millis();}
if (digitalRead(6)==1 && sl==0) {timerun=1;sl=1;timerotation=millis();}
if (millis()-mssls>180000 && digitalRead(6)==0) error=5;

//--------------------вращение барабана при основном отжиме  три этапа---------------------

if (millis()-timerotation>20000 && millis()-timerotation<=22000 && digitalRead(6)==1) { if (oetap==0) {digitalWrite(A4,0);digitalWrite(A5,1);} else {digitalWrite(A4,1);digitalWrite(A5,0);}}     //установить направление вращения
if (millis()-timerotation>22000 &&  digitalRead(6)==1 && zah==0)  {
                                                                   zah=1;
                                                                   if (oetap==0) totg=1; 
                                                                   val=10;
                                                                   totgm=millis();
                                                                  
                                                                   e2=millis();
                                                                   }  // подать ШИМ на двигатель



if (millis()-totgm>500 && val>0 && otgim==1 && totg>0) {
if (val<60) val=val+3; else val=val+2;

if (oetap==0 && val>600) val=600;
if (oetap==1 && val>600) val=600;

if (oetap==2 && val>1000) val=1000;

  totgm=millis();
  }
motor2(); 






  
if (totg<=0 && oetap==0){          // время первого этапа истекло
 
  val=0;digitalWrite(A4,0);digitalWrite(A5,0);
  
    if (pulseIn(8,LOW)<100 ) {oetap=1;zah=0;totg=2;timerotation=millis();}     //------выполнить после полного останова двигателя--------- 
  
  }

if (totg<=0 && oetap==1){          // время второго этапа истекло
 
  val=0;digitalWrite(A4,0);digitalWrite(A5,0);
  
    if (pulseIn(8,LOW)<100 ) {oetap=2;totg=4;zah=0;timerotation=millis();}     //------выполнить после полного останова двигателя--------- 
  
  }

  if (totg<=0 && oetap==2){          // время третьего этапа истекло
  
  val=0; digitalWrite(A4,0);digitalWrite(A5,0);
  
  if (pulseIn(8,LOW)<100 ) {otgim=0;rotation=0;totg=0;digitalWrite(11,0);}     //------выполнить после полного останова двигателя--------- 
  
  }


}



 
  }


//-----------------------замок закрылся----------------------------------------



if (error>0) stopr();
if (digitalRead(7)==0) error=9;         //сработал перелив






//--------------------------------цикл завершен----------------------------------------
if (stirka==0 && poloskanije==0 && otgim==0 && sliv==0 && otgimsmal==0) {
  
 
 while(digitalRead(9)==0 && izamok<=3){
  
  digitalWrite(4,1);                      // подать импульс на замок
  delay(5);
  digitalWrite(4,0);
  delay(300);
  izamok++;
  }
  if (digitalRead(9)==0) error=6; else {
                                         
  izamok=0;
   
  }
//-----------тут обнулить все переменные------------   
  pusk=0;
  val=0;
digitalWrite(A1,0);
digitalWrite(A2,0);
digitalWrite(A3,0);
digitalWrite(A4,0);
digitalWrite(A5,0);

  obnulenie();
  }

//-------------------------------------------------------------------------------------  
}









void stopr(){

val=0;
digitalWrite(A1,0);
digitalWrite(A2,0);
digitalWrite(A3,0);
digitalWrite(A4,0);
digitalWrite(A5,0);
//digitalWrite(4,0);
if (error==9 && digitalRead(7)==0) {digitalWrite(11,1);}
                                                         else {
                                                         digitalWrite(11,0);
                                                         
                                           //------открыть замок-------------------
                                           
                                            while(digitalRead(9)==0 && izamok<=5){
  
                                           digitalWrite(4,1);                      // подать импульс на замок
                                           delay(5);
                                           digitalWrite(4,0);
                                           delay(300);
                                           izamok++;
                                            }  
                                           izamok=0;obnulenie();
                                                            }
}

void obnulenie(){   //обнуление переменных
 rotation=0;
 timeHour=0;         
 timeMin=0;           
 polosk=0;           
 temperatureUst=20; 
 zap=0;
 startzaliv=0;
 sl=0;
 timerun=0;
 otgimsmal=0;
 slivst=0;
 rotation1=0;
 zah=0;
 oetap=0;
t=0;
temperature=20;
tst=0;
tpol=0;
kolotg=0;         
tpol1=0;
totg=0;

  }
  

  
void zapolnenie(){
if (digitalRead(6)==1 && error==0) {digitalWrite(A2,1);timerun=0;if (stirka==0 && polosk==1) digitalWrite(A1,1);}
if (digitalRead(6)==0 ) startzaliv=0;
if (digitalRead(6)==1 && startzaliv==0) {digitalWrite(A2,1);mszaliv=millis();startzaliv=1;digitalWrite(A3,0);}      // бак не заполнен - включить клапан 2, выключить ТЭН
if (digitalRead(6)==0 && zap==0) {zap=1;mszap=millis();startzaliv=0;timerun=1;}                                                  // бак заполнен - дать добрать воды ещё 10 секунд   
if (zap==1 && millis()-mszap>10000) {digitalWrite(A2,0); digitalWrite(A1,0);zap=0;}
if (digitalRead(6)==1 && startzaliv==1 && millis()-mszaliv>200000) error=4;                                                       // за 200 секунд бак не заполнился , уйти в ошибку
}








//--------------------------------------------------------------------------------------------
ISR (TIMER1_CAPT_vect) { //прерывание захвата сигнала на входе ICP1
  tic = ((uint32_t)int_tic << 16) | ICR1 ; //подсчёт тиков
  ICR1 = 0; int_tic = 0; TCNT1 = 0;
  sp = sp + 1 ; // для подсчёта оборотов в минуту.
  holl = holl + 1;
}   // после каждого срабатывания датчика холл+1


ISR (TIMER1_OVF_vect) { //прерывание для счёта по переполнению uint
  int_tic++; //считать переполнения через 65536 тактов
  if (int_tic > t2) {
    tic = 0;  //если на входе пусто более минимального времени то обнулить счётчики
    int_tic = 0;
  }
  if (int_tic > 500) {
    dimming = 140; // если стоим 2 секунды, то сбрасываем напряжение.
    
  }
}

void zero_crosss_int()  // детектор нуля
{
  time = micros();

}
//-----------------------------------------------------------------------------------




//---------вращение мотора при стирке-----------------------
void motor1(){

   
  pR = map(val, 0, 1023, obMin, obMax); //Приводим показания регулятора к минимальным и максимальным оборотам
  if ((val > 0 && stirka==1) || (val > 0 && poloskanije==1) ) {                                       //   если регулятор больше 0

    
    if ( holl >= 1) {                                  // если сработал датчик
      prOb = 60000000 / ((tic * 0.0625 ) * kImp / 10); //Высчитываем обороты по показаниям датчика
      
      if ( prOb >= 0) {                                //проверяем на соответствие.
        rOb = prOb ;                                   //если нормально, записываем в реальные обороты
      }
   /*   
        //--------------защита от превышения оборотов---------------------------
      if ( rOb > (pR + 2000) && rOb < (pR + 5000) ) {                           
          dimming = 140;                                   
          pRR = obMin;
          error=1;
          digitalWrite(A4, 0);
          digitalWrite(A5, 0);    
          
          
        }
      //-----------------------------------------------------------------------    
*/
        
      if ( rOb < pR ) {                                //сверяем показания регулятора и реальные обороты
        int fff = pR  - rOb;                           //узнаём разницу между оборотами
        int pRu = map(fff, 1, obMax, 1, razgon);       //исходя из разницы и разгона высчитываем на сколько увеличить переменную для расчёта
        pRR = pRR + pRu  ;                             //увеличиваем переменную расчёта
      }
      
      if ( pR < (rOb - 20) ) {                         //сверяем показания регулятора и реальные обороты
        int fff = rOb - 20  - pR;                      //узнаём разницу между оборотами
        int pRu = map(fff, 1, obMax, 1, razgon)+100;       //исходя из разницы и разгона высчитываем на сколько уменьшить переменную для расчёта
        pRR = pRR - pRu ;                              //увеличиваем переменную расчёта
      }
      pRR = constrain(pRR, (pR / 2), obMax);           //задаём пределы переменной для расчёта.
      ogr = map(val, 0, 1023, ogrmin, 7);              //исходя из показаний регулятора узнаём на сколько может быть открыт симистор.
      
      dimming = map(rOb, (pRR - dopusk), (pRR + dopusk), ogr, minzn); //рассчитываем управление симистором.
      holl = 0;                                                       // обнуляем срабатывание датчика
    }
    
    if (tic == 0) {                                    // если двигатель не вращается
      dimming = mindimming ;                          //  время задержки равно первоначальному импульсу
    }
    dimming = constrain(dimming, ogr, minzn) ;       //  Следим чтоб время задержки было не меньше ограничения и не больше минимального значения
  }
  else {
    dimming = 140;                                   //Если регулятор на 0 то время задержки 130
    pRR = obMin;
  }


  int dimtime = (75 * dimming);               // For 60Hz =>65
  tims = micros();                            // считываем время, прошедшее с момента запуска программы
  if (tims >= (time + dimtime)) {             //если время больше или равно времени срабатывания нуля + время задержки

    digitalWrite(5, HIGH);              // открываем симистор
    delayMicroseconds(10);                   // задержка 10 микросекунд (для 60Hz = 8.33)
    digitalWrite(5, LOW);              // выключаем сигнал на симистор.

  }
    
}
//----------------------------------------------------------------------------








//------------------------вращение на отжиме--------------------------

void motor2(){

   
pR = map(val, 0, 1023, obMin, obMax); //Приводим показания регулятора к минимальным и максимальным оборотам
  if ((val > 0 && otgimsmal==1)||(val > 0 && otgim==1)) {                                      
  /*
 if (millis()-e2>1000) {
  if (pulseIn(8,LOW)<100) error=2;
 e2=millis();
 }   
 */
    if ( holl >= 1) {                                  // если сработал датчик
      prOb = 60000000 / ((tic * 0.0625 ) * kImp / 10); //Высчитываем обороты по показаниям датчика
      
      if ( prOb >= 0) {                                //проверяем на соответствие.
        rOb = prOb ;                                   //если нормально, записываем в реальные обороты
      }
      
        /*
          //--------------защита от превышения оборотов---------------------------
      if ( rOb > (pR + 1000) && rOb < (pR + 5000) ) {                           
          dimming = 140;                                   
          pRR = obMin;
          error=1;
          digitalWrite(A4, 0);
          digitalWrite(A5, 0);    
          
          
        }
      //-----------------------------------------------------------------------    

    */

        
      if ( rOb < pR ) {                                //сверяем показания регулятора и реальные обороты
        int fff = pR  - rOb;                           //узнаём разницу между оборотами
        int pRu = map(fff, 1, obMax, 1, razgon);       //исходя из разницы и разгона высчитываем на сколько увеличить переменную для расчёта
        pRR = pRR + pRu  ;                             //увеличиваем переменную расчёта
      }
      
      if ( pR < (rOb - 20) ) {                         //сверяем показания регулятора и реальные обороты
        int fff = rOb - 20  - pR;                      //узнаём разницу между оборотами
        int pRu = map(fff, 1, obMax, 1, razgon);       //исходя из разницы и разгона высчитываем на сколько уменьшить переменную для расчёта
        pRR = pRR - pRu ;                              //увеличиваем переменную расчёта
      }
      pRR = constrain(pRR, (pR / 2), obMax);           //задаём пределы переменной для расчёта.
      ogr = map(val, 0, 1023, ogrmin, 7);              //исходя из показаний регулятора узнаём на сколько может быть открыт симистор.
      
      dimming = map(rOb, (pRR - dopusk), (pRR + dopusk), ogr, minzn); //рассчитываем управление симистором.
      holl = 0;                                                       // обнуляем срабатывание датчика
    }
    
    if (tic == 0) {                                    // если двигатель не вращается
      dimming = mindimming ;                          //  время задержки равно первоначальному импульсу
    }
    dimming = constrain(dimming, ogr, minzn) ;       //  Следим чтоб время задержки было не меньше ограничения и не больше минимального значения
  }
  else {
    dimming = 140;                                   //Если регулятор на 0 то время задержки 140
    pRR = obMin;
  }


  int dimtime = (75 * dimming);               // For 60Hz =>65
  tims = micros();                            // считываем время, прошедшее с момента запуска программы
  if (tims >= (time + dimtime)) {             //если время больше или равно времени срабатывания нуля + время задержки

    digitalWrite(5, HIGH);              // открываем симистор
    delayMicroseconds(10);                   // задержка 10 микросекунд (для 60Hz = 8.33)
    digitalWrite(5, LOW);              // выключаем сигнал на симистор.
  }
    
}
//---------------------------------------------------------

void SerialREAD (){

if (!Serial.available()) return;

ksum= Serial.parseInt ();
timeHour = Serial.parseInt ();
timeMin = Serial.parseInt ();
temperatureUst= Serial.parseInt ();
stirka = Serial.parseInt ();
polosk = Serial.parseInt ();
poloskanije = Serial.parseInt ();
sliv = Serial.parseInt ();
otgim = Serial.parseInt ();
start = Serial.parseInt ();
if(ksum==(timeHour+timeMin+temperatureUst+stirka+polosk+poloskanije+sliv+otgim+start)) pusk=start;
                                                                                        else {
                                                                                          pusk=0;
                                                                                          timeMin=0;
                                                                                          timeHour=0;
                                                                                          polosk=0;
                                                                                          poloskanije=0;
                                                                                          sliv=0;
                                                                                          otgim=0;
                                                                                          start=0;
                                                                                          oetap=0;
                                                                                          }

}










int datchik (int t){   //калибровка датчика
  int temper;
       
         if (t>=935) temper=91;
         if (t>=933 && t < 935) temper=90;
         if (t>=931 && t < 933) temper=89;
         if (t>=929 && t < 931) temper=88;
         if (t>=926 && t < 929) temper=87;
         if (t>=923 && t < 926) temper=86;
         if (t>=920 && t < 923) temper=85;
         
         if (t>=917 && t < 920) temper=84;
         if (t>=914 && t < 917) temper=83;
         if (t>=911 && t < 914) temper=82;
         if (t>=908 && t < 911) temper=81;
         if (t>=904 && t < 908) temper=80;
         if (t>=901 && t < 904) temper=79;
         if (t>=897 && t < 901) temper=78;
         if (t>=894 && t < 897) temper=77;
         if (t>=890 && t < 894) temper=76;
         if (t>=886 && t < 890) temper=75;
         if (t>=882 && t < 886) temper=74;
         if (t>=877 && t < 882) temper=73;
         if (t>=873 && t < 877) temper=72;
         if (t>=868 && t < 873) temper=71;
         if (t>=866 && t < 868) temper=70;
         if (t>=862 && t < 866) temper=69;
         if (t>=857 && t < 862) temper=68;
         if (t>=852 && t < 857) temper=67;
         if (t>=847 && t < 852) temper=66;
         if (t>=842 && t < 847) temper=65;
         if (t>=836 && t < 842) temper=64;
         if (t>=832 && t < 836) temper=63;
         if (t>=826 && t < 832) temper=62;
         if (t>=819 && t < 826) temper=61;
         if (t>=813 && t < 819) temper=60;
         if (t>=807 && t < 813) temper=59;
         if (t>=802 && t < 807) temper=58;
         if (t>=795 && t < 802) temper=57;
         if (t>=788 && t < 795) temper=56;
         if (t>=782 && t < 788) temper=55;
         if (t>=775 && t < 782) temper=54;
         if (t>=768 && t < 775) temper=53;
         if (t>=761 && t < 768) temper=52;
         if (t>=754 && t < 761) temper=51;
         if (t>=746 && t < 754) temper=50;
         if (t>=738 && t < 746) temper=49;
         if (t>=730 && t < 738) temper=48;
         if (t>=722 && t < 730) temper=47;
         if (t>=714 && t < 722) temper=46;
         if (t>=705 && t < 714) temper=45;
         if (t>=697 && t < 705) temper=44;
         if (t>=688 && t < 697) temper=43;
         if (t>=680 && t < 688) temper=42;
         if (t>=671 && t < 680) temper=41;
         if (t>=661 && t < 671) temper=40;
         if (t>=652 && t < 661) temper=39;
         if (t>=642 && t < 652) temper=38;
         if (t>=632 && t < 642) temper=37;
         if (t>=622 && t < 632) temper=36;
         if (t>=612 && t < 622) temper=35;
         if (t>=602 && t < 612) temper=34;
         if (t>=592 && t < 602) temper=33;
         if (t>=582 && t < 592) temper=32;
         if (t>=572 && t < 582) temper=31;
         if (t>=563 && t < 572) temper=30;
         if (t>=552 && t < 563) temper=29;
         if (t>=542 && t < 552) temper=28;
         if (t>=532 && t < 542) temper=27;
         if (t>=522 && t < 532) temper=26;
         if (t>=512 && t < 522) temper=25;
         if (t>=501 && t < 512) temper=24;
         if (t>=491 && t < 501) temper=23;
         if (t>=480 && t < 491) temper=22;
         if (t>=470 && t < 480) temper=21;
         if (t>=460 && t < 470) temper=20;
         if (t>=450 && t < 460) temper=19;
         if (t>=440 && t < 450) temper=18;
         if (t>=430 && t < 440) temper=17;
         if (t>=420 && t < 430) temper=16;
         if (t>=410 && t < 420) temper=15;
         if (t>=400 && t < 410) temper=14;
         if (t>=395 && t < 400) temper=13;
         if (t>=385 && t < 395) temper=12;
         if (t>=379 && t < 385) temper=11;
         if (t>=375 && t < 379) temper=10;
         if (t>=365 && t < 375) temper=9;
         if (t>=353 && t < 365) temper=8;
         if (t>=342 && t < 353) temper=7;
         if (t>=331 && t < 342) temper=6;
         if (t>=320 && t < 331) temper=5;
         if (t>=309 && t < 320) temper=4;
         if (t>=295 && t < 309) temper=3;
         if (t>=282 && t < 295) temper=2;
         if (t>=270 && t < 282 ) temper=1;
         
         if (t < 270) temper=0;
         
  return temper;
  }
  







 
