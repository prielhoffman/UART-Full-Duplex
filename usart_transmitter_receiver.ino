#include<math.h>

#define TX 7
#define RX 6
#define BIT_TIME 20
#define BAUD_RATE 9600

static int t_state=0;
static int t_data = 0b00011100;
static int t_bit;
static int t_parity;
 static int t_counter = 0;
static int t_flag = 0;
static unsigned long  t_time_ref = 0;

static int r_state=0;
static int r_counter = 0;
static int r_bit;
static int r_data = 0b00000000;
static int r_parity;
static int r_flag = 0;
static unsigned long  r_time_ref = 0;
static int bit_check[5];
static int i = 0;


//int to_send=0;
static int one_counter;
unsigned static long currTime = 0;


void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);
}



void loop()
{
  currTime = millis();
  usart_tx();
  usart_rx();
}

int check_samp(){
  if(bit_check[1] == bit_check[2] && bit_check[1] == bit_check[3]){
    return 1;
  }
  else{
    return 0;
  }
}


void usart_tx(){
  
  if(r_flag == 0){
    
	switch(t_state){
      
      case 0: //IDLE
      	digitalWrite(TX,HIGH);
      	t_state=1;
    	break;
    
      case 1: //START
      	digitalWrite(TX,LOW);
      	if(currTime - t_time_ref >= BIT_TIME){
          t_time_ref = currTime;
          t_state=2;
      	}
    	break;
 
      case 2: //DATA
        
        if(currTime - t_time_ref >= BIT_TIME){
          t_time_ref = currTime;
          t_bit = bitRead(t_data, t_counter);
          //Serial.print(t_bit); //test***************
          digitalWrite(TX,t_bit);
          t_counter++;
          if (t_bit == 1){
            one_counter++;
          }
          if(t_counter == 8){
            t_state = 3;
            t_counter = 0;
          }
        }
    	break;
     	
      case 3: //PARITY
      	
      	t_parity = one_counter%2;
      	one_counter = 0;
      	if(currTime - t_time_ref >= BIT_TIME){
        	t_time_ref = currTime;
        	digitalWrite(TX,t_parity);
          	t_state = 4;
        }     	            		  
    	break;

      case 4: //STOP
      	
      	digitalWrite(TX,HIGH);
        if(currTime - t_time_ref >= BIT_TIME){
        	t_state = 0;
          	t_flag = 1;
        }
    	break; 
	}
  }
  else{
    if(currTime - t_time_ref >= (random(1,50)*BIT_TIME)){
      t_time_ref = currTime;
      t_flag = 0;
    }
  }   
}

void usart_rx(){
  
  if(r_flag == 0){
  
	switch(r_state){
       
      case 0: //IDLE
     
      	if(digitalRead(RX) == 0){
          r_state = 1;
        }
      break;

      case 1: //START
      	
      	if(currTime - r_time_ref >= (BIT_TIME/5) ){
          r_time_ref = currTime;
          bit_check[i] = digitalRead(RX);
          i++;
          if(i == 5){
            i = 0;
            if(check_samp()){
              r_state = 2;
            }
            else{
              r_flag = 1;
            }
          }
        }
    	break;
 
    	
      case 2: //DATA
            
      	if(currTime - r_time_ref >= (BIT_TIME/5) ){
        	r_time_ref = currTime;
          	bit_check[i] = digitalRead(RX);
          	i++;
          	if(i == 5){
              i = 0;
              if(check_samp()){
                bitWrite(r_data,r_counter,bit_check[2]);
                //Serial.print(bit_check[2]);//test***************
                r_counter++;
                if(bit_check[2] == 1){
                  one_counter++;
                }
                if(r_counter == 8){
                  r_counter = 0;
                  r_state = 3;
                }   
              }
              else{
              	r_flag = 2;
              }
            }
    	break;
 
    	
    	case 3: //PARITY
          
            if(currTime - r_time_ref >= (BIT_TIME/5) ){
        		r_time_ref = currTime;
          		bit_check[i] = digitalRead(RX);
          		i++;
              	if(i == 5){
              		i = 0;
              		if(check_samp()){
                      r_parity = bit_check[2];
                      if(!(one_counter%2) == r_parity){
                        //Serial.print(r_parity);//test***************
                        one_counter = 0;
                        r_state = 4;
                      }
                      else{
                        one_counter = 0;
                        r_flag = 3;
                      }
                    }
                    else{
                      one_counter = 0;
                      r_flag = 3;
                  }
                }
            }
          break;

        
    	case 4: //STOP
          
          if(currTime - r_time_ref >= (BIT_TIME/5) ){
          	r_time_ref = currTime;
          	bit_check[i] = digitalRead(RX);
          	i++;
          	if(i == 5){
            	i = 0;
            	if(check_samp()){
              		r_state = 0;
            	}
            	else{
              		r_flag = 4;
            	}
          	}
          }
          break;
        }
    }
  }
      
      
      
      else if(r_flag == 1){
        r_state=0;
		r_counter = 0;
		r_data = 0b00000000;
        if(currTime - r_time_ref >= (10*BIT_TIME) ){
          r_flag = 0;
        }
      }
      else if(r_flag == 2){
        r_state=0;
		r_counter = 0;
		r_data = 0b00000000;
        if(currTime - r_time_ref >= ((8-r_counter)*BIT_TIME) ){
          r_flag = 0;
        }
      }
              
      else if(r_flag == 3){
        r_state=0;
		r_counter = 0;
		r_data = 0b00000000;
        if(currTime - r_time_ref >= (2*BIT_TIME) ){
          r_flag = 0;
        }
      }
      else if(r_flag == 4){
        r_state=0;
		r_counter = 0;
		r_data = 0b00000000;
        if(currTime - r_time_ref >= BIT_TIME ){
          r_flag = 0;
        }
      } 
}
  
                 
    
  


  
    
    
    
  
  
  


