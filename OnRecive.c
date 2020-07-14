/*
본 파일은 OnReceive 함수의 오버라이딩을 행한다.
Control flag에 대한 전체적인 제어를 수행한다.
*/
void CDataSocket::OnReceive(int nError Code)
{
  start = clock(); 
  EnterCriticalSection(&(pDlg->CC)); 
  TCHAR temp[8]; 
  unsigned short Checksum = 0; 
  int In_win = 0; 
  Packet receive;
  
  SYSTEMT IME cur_time; 
  GetLocalTime(&cur_time);
  
  LARGE_INTEGER stt, end, freq;
  
  QueryPerformanceFrequency(&freg);
  
  CString tt; 
  tt.Format( _T(" ^ ^2+ %02d : %02d : %02d.%03d"), cur_time.wHour ,cur_time.wMinute, cur_time. Second, cur_time.wMilliseconds); 
  pDlg->Process. AddString(tt);
  
  QueryPerformanceCounter(&stt); 
  if (ReceiveFromEx(&receive. 22, addr, pDlg->por tnum) < 0)
    pDlg->Process. AddString T( "Error in Receive")); 
  QueryPerformanceCounter(&end);
  
  CString t;
  t.Format( T("Receive Time : %lf us"), (((double)end.QuadPart - (double)stt.QuadPart / (double)freq.QuadPart) *1000000.0));
  pDlg->Process.AddString(t); // 
  
  // TCHAR 8*2 바이트 + Sequncenum 1바이트
  // CheckSum 4 바이트
  if( last_SQ > 7)
    last_SQ = (last_SQ) % 8;
  
  int count = receive sequence_send + 1;
  //////////////////////////////////////////////////////////////////////////
  if (receive. IS == TRUE) { // 받은 데이터가 단순한 ACK 일 경우
  //////////////////////////////////////////////////////////////////////////
    
    if (receive. RR_REJ == true) {  // RR 일때 
      if (receive.PF == FALSE) { ///////////////// 보통 RR
        int SQnum = receive.sequence_send; 
        int countREJ = 0;
        
        CString tt; 
        tt.Format(_T("RR %d received"), receive. sequence_send); 
        pDlg->Process.AddString(tt); // 위의Segment 이전까진 다 받았다. 
        CA. Lock();
        for (int i = 0; i < 4; i++) { // window size 는 4라서
          if(check_for_SW > 7)
            check_for_SW = check_for_SW % 8;  // window 한계 초과 --> 다시 0으로 회귀
          
          SQnum--;
          
          if (SQnum < 0) {
            SQnum += 8;
            if (pDlg->pData->send_window[SQnum] == SEND_NOT_ACKRECV) {
              pDlg->pData->send_window[SQnum]= NOT SEND_NOT_WIN; //송신확인, window 탈출 
              
              pDlg->pData->send_window[check_for_SW++] =
                pDlg->pData->NOT_SEND_IN_WIN; // CS IS
            }
          }
          else { 
            if (pDlg->pData->send_window[SQnum] == SEND_NOT_ACKRECV) { 
              pDlg->pData->send_window[SQnum] = NOT_SEND_NOT_WIN; //송신확인, window 탈출
              pDlg->pData->send_window[check_for_SW++] = pDig->pData->NOT_SEND_IN_WIN; // 다음 window 설정
            }
          }
        }
        CA.Unlock(); 
        pDlg->Send_Process.SetWindowTextW(pDlg->pData->GetCurrentState(pDig->pData->send_window));
        pDlg->Recv_process.SetWindowTextW(pDig->pData->GetCurrentState(pDlg->pData->recv_window));
      }
      else { ///////////////// 확인용 RR
        // Last SQ 프레임 요구.
        CString tt; 
        tt.Format(T("time out RR is received"));
        Packet RRP; 
        RRP.IS = TRUE; 
        RRP.PF = TRUE; 
        RRP. RR_REJ = false;
        RRP. sequence_send = last_SQ;
         
        pDig->tx_arg.pPacket.AddTail(RRP);
        pDig->tx_arg.Thread_run = TRUE;
        
        tt.Format(_T("time out RR %d send."), RRP. sequence_send); 
        pDlg->Process.AddString(tt); // 위의 segment 이전까진 다 받았다는 뜻
      }
      
    }
    /////////////////////////////////////////////////////////////////////////////////////////
    else if (receive. RR_REJ == false) { // 받은 데이터가 REJ 일 경우 sequence_send 부터 다시 전송
    /////////////////////////////////////////////////////////////////////////////////////////
      int SQnum;
      
      SQnum = receive. sequence_send;
      
      CA.Lock(); 
      
      for (int i = 0; i < 4; i++) {
        
        if (check_for_SW > 7)
          check_for_SW %= 8;
        
        SQnum--; 
        
        if (SQnum < 0) { // 이전 값들은 잘 받았다는 뜻
          SQnum += 8; 
          if (pDlg->pData->send_window[SQnum] == SEND_NOT_ACKRECV) {
            pDlg->pData->send_window[SQnum] = NOT SEND_NOT_WIN; //송신확인 window 탈출
            pDig->pData->send_window[check_for_SW++] = pDlg->pData->NOT_SEND_IN_WIN; // 다음 window 설정
          }
        }
        else { 
          if (pDlg->pData->send_window[SQnum] == SEND_NOT_ACKRECV) { 
            pDlg->pData->send_window[SQnum] = NOT_SEND_NOT_WIN; // 송신확인. window 탈출
            pDlg->pData->send_window[check_for_sW++] = pDig->pData->NOT_SEND_IN_WIN; // 다음 윈도우 설정
          }
        }
      }
      CA.Unlock(); 
      pDlg->tx_arg.ForceStop = TRUE;
      
      CString tt; 
      ///////////// PF는 단순히 time out 인지 진짜 REJ인지 구분 위함. 
      if (receive.PF == FALSE) {
        tt.Format(_T("REJ %d received"), receive. sequence_send);
        pDlg->Process.AddString(tt); // 위의 segment 이전까진 다 받았다는 뜻
      }
      else {
        tt.Format(_T("Checking RR %d is received"), receive.sequence_send); 
        pDlg->Process. AddString(tt); // 위의 segment 이전까진 다 받았다는 뜻
      }
      pDig->Send_Process. SetWindowTextW(pDig->pData->GetCurrentState(pDlg->pData->send_window)); 
      pDig->Recv_process. SetWindowTextW(pDig->pData->GetCurrentState(pDlg->pData->recv_window));
    }
    //////////////////////////////////////////////////////////////////////////////////////
    if (receive. RR_REJ == 2) {// 받은 데이터가 파일 데이터를 암시하는 protocol control 비트일때.
    //////////////////////////////////////////////////////////////////////////////////////
      if (receive.CS == 20) { // 20 : 파일전송 요청 보내고 ACK 받을때.
        pDlg->Process.AddString(_T("File Check Bit Receive..."));
        pDlg->File_OK = true; 
        pDlg->OnBnclickedButton3();
      }
      else if (receive.CS == 0) { // 0 : 파일 전송을 위해 요청하는 경우.
        File_len = receive.message[0] + (receive.message[1] « 16 ); 
        Co = File_len; 
        Packet OK_bit; 
        OK_bit.IS = TRUE; 
        OK_bit.cs = 20;
        OK_bit. RR_REJ = 2;
        pDlg->fDig->DoModal(); 
        str = pDlg->fD1g->GetPathName();
        target.Open( str, CFile:: modeCreate CFile:: modeWrite | CFile:: typeBinary);
        pDig->Process.AddString(_I("File Check Bit send....")); 
        pDlg->tx_arg.pPacket.AddTail(OK_bit); 
        pDig->tx_arg.Thread_run = TRUE; 
        pDlg->Process.AddString(T( "######## File Recy Start ! ########"));
      }
    }
  }
  ///////////////////////////////////////////////////////////////// 
  if (receive. IS == FALSE) { // 수신 데이터가 information 인 경우.
  /////////////////////////////////////////////////////////////////
    for (int i = 0; i < 8; i++) {
      temps [i] = receive.messages[i]; //strcpy는 chracter 형 아니면 사용불가.--> chracterising
    }
    CheckSum = calc_CS( temp, receive.CS); 
    CString CS;
    CS.Format(_T("Segment %d Receive : Checksum result : %d"), receive.sequence_send, CheckSum); 
    pDlg->Process.AddString(CS);
      
    ///////////////////////////////////////////////// Check Sum 체크 과정 
    if (CheckSum != 0) { // Checksum 비정상
      pDlg->Process.AddString(_T( "@@@ Error Detected in CheckSum.@@@ ")); 
      CString a; 
      a.Format(_T( "@@@ Segment %d is DISCARD @@@"),receive.sequence_send);
      pDlg->Process.AddString(a);
      
      if (switch_REJ) { // Checksum 에러인 경우 REJ 전송.
        Packet ack; 
        ack. IS = TRUE; 
        ack.PF = FALSE; 
        ack.sequence_send = receive.sequence_send;
        ack. RR_REJ = false;  // false = REJ;
                              // Checksum으로 확인된 Frame에 대해서만 내용 받음.
        pDig->tx_arg.pPacket.AddTail(ack); 
        pDlg->tx_arg.Thread_run = TRUE; 
        // 데이터 수신에 Error발생했다고 ACK 전달. ( TX thread 동작 ) 
        switch_REJ = FALSE;
        
        CString CS; 
        CS.Format(_T("REJ %d now sending."), ack.sequence_send); 
        pDlg->Process.AddString(CS);
        
        return;
      }
      else {
        return;
      }
    } 
    // 수신측에서 받은 Checksum을 포함한
    // 데이터의 내용을 모두 더한 값에 
    // 1의 보수를 취한 new checksum을 만든다.
    // 해당 값이 0이어야만 Error 없다고 판별, 아닐시 데이터 discard 후 REJ 전송하는 과정이다.
     //////////////////////////////////////////////////도착 packet sequence 정상인 경우
    if (receive. sequence_send == (last_SQ)) {
      int init_num = receive. sequence_recv;
      for (int i = 0; i < 4; i++, init_num++) {
        send window[init_num % 8] = NOT SEND_IN_WIN;
        send window[( init_num + 4) % 8] = NOT SEND_NOT_WIN; 
      }//// 상대측의 RR n 신호에 따라 내 send window 동기화.
      
      sec = receive.sequence_recv;
      // Segment 과정에서 sequence 넘버를 붙이기 위한 변수를 동기화시킴.
      // ( 해당 과정에서 extern 변수인 sec는 새로이 바뀜.)
      
      if (receive.PF == TRUE) {             ///////////// 받은것이 마지막 Sequence 데이터인 경우
        Packet ack; 
        ack. IS = TRUE; 
        ack.PF = FALSE; 
        ack. sequence_send = receive. sequence_send + 1; 
        if (ack.sequence_send > 7)
          ack.sequence_send -= 8; 
        ack. RR_REJ = true;  // true = RR;
        // Checksum으로 확인된 frame에 대해서만 내용을 받음.
        
        pDlg->tx_arg.pPacket.AddTail(ack); 
        pDig->tx_arg.Thread_run = TRUE;
        // 데이터를 잘 받았다고 ACK 전달. ( TX 동작 )
        //////////////////////////////////////////////////
        if (receive.is_F == FALSE) {    ///////// 데이터를 받으면
          pDlg->rx_arg.pList->AddTail(receive.message); 
          if (receive message[7] != T('\O'));
          pDlg->rx_arg.pList->AddTail( T("\O")); 
          pDlg->rx_arg.Thread_run = TRUE;
        }
        else if (receive. is_F == TRUE) { ///////// File을 받으면
          target.Write(receive message, Filelen); 
          target.Close(); 
          pDlg->Process.AddString(_T( "######## File Recy End ########")); 
          CString k; 
          k.Format(_T(" 100 percent complete ! ")); 
          pDlg->Edit_File.SetWindowText(k); 
          stack_leng = 0;
        }
        //////////////////////////////////////////////
        pDlg->num_recv = receive.sequence_send;
    
        CString CS;
        CS.Format(_T( "RR %d now sending."), ack.sequence_send); 
        pDlg->Process.AddString(CS);
      }
      
      else {                             ///////////// 받은것이 마지막 Sequence 데이터가 아닐경우
        if (((RR_bit++) % 2) == 1) {
          RR_bit = 0; // Overflow 방지
          Packet ack; 
          ack.IS = TRUE;
          ack.PF = FALSE; 
          ack.sequence_send = receive.sequence_send + 1; 
          if (ack.sequence_send > 7)
            ack.sequence_send -= 8;
          ack. RR_REJ = true; // true = RR;
                              //Checksum으로 확인된 frame에 대해서만 내용을 받음.
          pDig->tx_arg.pPacket.AddTail(ack); 
          pDlg->tx_arg.Thread_run = TRUE; // 데이터 잘 받았다는 ACK 전송 ( TX 동작 )
          CString CS;
          CS.Format(_T("RR %d now sending."), ack.sequence_send); 
          pDig->Process.AddString(CS);
        }
        //////////////////////////////////////////////////////// 
        if (receive.is_F == FALSE) // 받은 것이 data 라는 뜻
          pDlg->rx_arg.pList->AddTail(receive.message); 
        else {   // 받은 것이 file 인 경우
          target.Write(receive.message, 16); 
          File_len -= 16; 
          CString k; // 현재 전송상태 표기 위한 String k.
          stack_leng += 16; 
          k.Format(_T(" %lf percent complete ! "), (double)((double) (stack_leng + 100) / co)); 
          pDlg->Edit_file.SetWindowText(k); // k 를 Processing
        }
        //////////////////////////////////////////////////////// 
      }
      switch_REJ = TRUE; 
      Tast SQ = receive.sequence_send + 1;
    }
    //////////////////////////////////////////////////도착 packet sequence 비정상
    else { 
      if (switch_REJ) {
         Packet ack;
        ack.PF = FALSE; 
        ack.IS = TRUE; 
        ack.sequence_send = last_SQ; 
        ack. RR_REJ = false; // false = REJ;
                           // Checksum으로 확인된 frame에 대해서만 내용을 받음. 
        pDlg->tx_arg.pPacket.AddTail(ack); 
        pDlg->tx_arg.Thread_run = TRUE; // 데이터 잘못 받았다는 ACK 전달 ( TX 전송 )
        switch_REJ = FALSE;
      
        CString CS; 
        CS.Format(_T("REJ %d now sending."), ack.sequence_send); 
        pDlg->Process.AddString(CS);
      
        return;
      }
      else {
        return;
      }
    }
    
    
    int recv_num = receive. sequence_send;
    for (int i = 0; i < 4; i++) {
      recv_window[(recv_num + 4) % 8] = NOT_RECV_NOT_WIN;
      recv_window[recv_num++] = NOT_RECV_IN_WIN; } //받은 것 따라 윈도우 동기화
    
    recv_window[receive. sequence_send] = RECV_NOT_ACKSEND;
    
    pDlg->Recv_process.SetWindowTextW(pDig->pData->GetCurrentState(pDlg->pData->recv_window));
    //필요한건 앞의 1바이트 이므로 1바이트만 전달하면 됨. 
    recv_window[receive.sequence_send] = NOT_RECV_NOT_WIN; 
    recv_window[(receive.sequence_send + 4) % 8] = NOT_RECV_IN_WIN;
    
    pDig->Recv_process.SetWindowTextW(pDlg->pData->GetCurrentStates, pDlg->pData->recv_window)); 
    // ACK 보냈는데 에러가 나는 경우는 제외 
    // ACK 전송에서는 타임아웃 기능이 없기 때문.. 
    
    pDig->rx_arg. Thread_run = TRUE;
  }
  
  CA.Unlock(); 
  CSocket::OnReceive(nError Code);
}
