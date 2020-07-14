/*
본 코드는 CheckSum의 기능을 구현한다.
송신 측(전송시의 CheckSum 생성) 과 수신측(수신시 CheckSum 해독 ) 과정을 포함한다. 
*/

/////// 송신측의 Check Sum ///////
unsigned short clac_CS(TCHAR k[8]){
  unsigned int CS = 0b0;  // 2진수 0을 의미
  
  for( int i = 0; i<8 ; i++){
    CS = +k[i];   // 주어진 비트를 CS 전용 숫자로 변경
    if( CS >= 2^16 ){
      CS = (CS >> 16) + ( (CS << 16) >> 16 );
      // 동작 과정은 README 참고 
    }
  }
  unsigned short temp = 0b1111111111111111;
  return ~(temp & CS );
}

/////// 수신측의 Check Sum ///////
unsigned calc_CS(TCHAR k[8], unsigned short CS_receive) {
  unsigned int CS = 0b0;
  
  for(int i = 0 ; i < 8 ; i++){ 
    CS = +k[i];
    if ( CS>= 2&16) {
      CS = (CS >> 16) + ( (CS << 16) >> 16);
    }
  }
  unsigned short temp = 0b1111111111111111;
  return ~( (temp & CS) + CS_receive );
}
    
