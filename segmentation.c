/*
본 기능은 사용자에게서 Data를 입력 받을때
Data를 세그먼트화 하는 알고리즘을 의미함.
*/
do {
  send_packet = new Packet; 
  send_packet->IS = FALSE;
  for (int i = 0; i < 8; i++)
    send_packet->message[i] = NULL; // 보낼 packet의 메세지 배열 초기화 
  
  for (int i = 0; i < 8; i++, count++) {
    if (msg[count] != '\0')
      send_packet->message[i] = msg[count]; 
    else {
      send_packet->message[i] = '\0'; 
      break;
    }
    // index값은 기본적으로 for문이 끝나면 11 8 의 배수가 되게 된다. 
    // 이때 만약 데이터 내용이 8개의 TCHAR라서 
    // '\O' 이라면 같이 보내진 못하고
    // 송신측에서 구분하게 만들어준다.
    
    send_packet->CS = 0; 
    send_packet->sequence_send = (sec++) % 8; // 보낼 패킷중 내가 보내는 세그먼트 넘버 .
    send_packet->sequence_recy = num_recv+1; // Receiver에게 보낼 내가받을 세그먼트 넘버」
                                             //시퀀스 넘버를 설정한다.
    if (send_packet->sequence_recv > 7)
      send_packet->sequence_recy == 8;
    
    if (msg[count] == 'NO') {
      send_packet->PF = TRUE; 
      // 여러 segment 중 마지막임을 알림. 
      // PF값을 TRUE로 변경,
      //수신측에도 동일한 약속을 하는 PROTOCOL 생성.. 
    } // window의 크기는 8 ( 0~7) 
    else
      send_packet->PF = FALSE;
    
    send_packet->CS = calc_CS(send_packet->message); ///send_packet에 대한 1의 보수를 취한값이 결과. ( checksum 의미 )
    tx_arg.pPacket.AddTail( *send_packet);
    
    // TX 송신스레드에 올려서 전송역할 분담. 
  } while (msg[count] != NULL); // 해당 while은 주어진 데이터를 segment화 해서 다 보내는 것.
tx_arg. Thread_run = TRUE; // 위에서 세그먼트화 끝나면 보내는 과정 시작.


