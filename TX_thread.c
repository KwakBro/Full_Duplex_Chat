UINT TX(LPVOID ARG) {
  srand(200); 
  ThreadArg* arg = (ThreadArg*) ARG;
  
  Packet* Send;

  int sec = 0;
  CWinThread *timer [8]; 
  Arr_arg arrg[8]; 
  LARGE_INTEGER stt, end, freq; 
  QueryPerformanceFrequency(&freg);
  POSITION Head; 
  POSITION curr;
  
  for (int i = 0; i < 8; i++) {
    arrg[i].pData = arg->pDlg->pData; 
    arrg[i].pDlg = arg->pDig;
    arrgli].run_st = i; 
    arrg[i].pList = &(arg->pPacket); 
    arrg[i].ForceStop = FALSE;
  }
  bool check_data = false; 
  while (true) { ////// 해당 while은 주어진 데이터를 Segmentation 해서 보냄 //////
    if(arg->Thread_run == TRUE) { //무엇인가 버퍼에 존재,ㅡ 동작 시작
      Send = new Packet;
      Head = arg->pPacket.GetHeadPosition();
      while (Head != NULL) { //순차적으로 현재 TX 버퍼의 값 전송
        Sleep(50); 
        CR.Lock(); 
        curr = Head; 
        *Send = arg->pPacket.GetNext(Head);

        if (flag_for_timeout[Send->sequence_send] == TRUE) { // 보내려는 데이터의 타이머가 동작상태면
          check_data = true; // 이 데이터는 이미 보내지는중, TImer가 마킹한다는 뜻
        }
        if (check_data == true) { // 해당 데이터의 검증
          check_data = false; 
          continue; // 타이머가 동작하고 있다면 해당 데이터 건들지 않고, 패스하기 위함.
        }
        if (Send->IS == FALSE) { // Information인 경우
          while (count_for_Timer >= 4) { //Flow control, Timer갯수가 4개 이상일시 전송 방지
            CR.Unlock(); // 주기적인 체크 방식
            
            Sleep(1000); 
            CR.Lock();
            if (arg->ForceStop == TRUE) { // 만약 REJ,RR(p=1) 도착으로 무한 루프중 강제종료 시에는
              for (int i = 0; i < 8; i++) { // 해당 ACK의 도착은 OnReceive 단계에서 성공적 전송된 데이터에 대해서는
                                            // 슬라이딩 윈도우 상태를 바꿈으로 정상종료
                if (arg->pDig->pData->send_window[ 1 ] ==arg->pDlg->pData->SEND_NOT_ACKRECV) {
                  arrg[i].ForceStop = TRUE; // 타이머는 동작중이지만, 전송에 실패한 데이터는 강제종료
                  CR.Unlock(); 
                  Sleep(500); // 다른 스레드의 성공적인 강제종료를 위함
                  CR.Lock(); 
                } //강제종료의 경우에는  TX버퍼의 데이터를 삭제하지 않음. 
                  //이후 TX 재실행은 보내지지 않은 데이터에 대한 재전송 의미
              }
              arg->ForceStop = FALSE; 
              Head = arg->pPacket.GetHeadPosition(); // 이후 다시 처음부터 데이터를 전송 시작 
              curr = Head; 
              *Send = arg->pPacket.GetNext(Head); 
              CR.Unlock();
            }
            // 만일 현재 송신윈도우가 가득 찬 상태일시, ACK를 처리하기 위해 while로 구성
          }
        }
        if (Send->is F == TRUE) 
          if (Send->PF == TRUE) {
            arg->pDlg->Process.AddString(_T("######## File Send End ! ########")); 
            CString k;
            k.Format(_T(" 100 percent complete ! "); 
            arg->pDig->Edit_File.SetWindowText(k); 
            stack_length = 0;
          }
          else {
            CString k; 
            stack_length += 16; 
            k.Format( I"_%If percent complete ! "), (double) (( double) (stack_length * 100) / copy));
            arg->pDig->Edit_File. SetWindowText(k):
          }
          SYSTEMTIME cur_time; 
          GetLocalTime(&cur_time);
          CString tt; 
          tt.Format(_T(" 송신 시각: %02d : %02d : %02d.%03d"), cur_time. wHour ,
                        cur_time. WMinute, cur_time.Second, cur_time.wMilliseconds); 
          arg->pDlg->Process.AddString(tt); 
          QueryPerformanceCounter(&stt);
          if(arg->pDlg->pData->SendTo( Send, 22, arg->Dlg->portnum, arg->pDlg->pData->addr) < 0)
            arg->pDlg->Process.AddString(_T("Error in SendToEx")); 
            QueryPerformanceCounter(&end);
          CString t; 
          t.Format(_T("Transmission Time : %lf us"), (((double)end.QuadPart-(double)stt.QuadPart) / (double)freq.QuadPart)*1000000.0);
          arg->pDlg->Process.AddString(t);
          if (Send->IS =TRUE) {   // ACK면 window 건들 필요 없이 보낸 Data 바로 삭제
            arg->pPacket. RemoveAt(curr);
          }
          else {   // Send 패킷이 데이터일때. 
            arrg[Send->sequence_send].element_pos = curr;
            
            arg->pDlg->pData->send_window[Send->sequence_send] = arg->pDlg->pData->SEND_NOT_ACKRECV;  // 현재 보내고 ACK 못받은 상태로 설정
            arg->pDlg->Send_Process.SetWindowTextW(arg->pDlg->pData->GetCurrentState(arg->pDlg->pData->send_window)); 
            arg->pDlg->Recv_process.SetWindowTextW(arg->pDlg->pData->GetCurrentState(arg->pDlg->pData->recv_window));
          CString str; 
          str.Format(_T( "Segment %d (1, %d, %d ) is now Sending"), Send->sequence_send, Send->sequence_send, Send->sequence_recv);
          arg->pDlg->Process.AddString(str);
          
          flag_for_timeout [Send->sequence_send] = TRUE; 
          arrg Send->sequence_send].run_st = Send->sequence_send;
          timer [Send->sequence_send] = AfxBeginThread( time, &arrg[Send->sequence_send]); 
          //송신 시작 후 타이머 동작
          
          ++count_for_Timer; //현재 타이머 수 업데이트
        }
        CR.Unlock();
        
      }
      arg->Thread_run = FALSE;    // TX의 동작 종료로 조작
    }   
    else if (arg->Thread_run == FALSE)
      Sleep(10); // busy waiting.은 피하기 위함
        
    if (arg->Thread_run == -1)
      return 1; // 종료하는 경우
        
    if (arg->ForceStop == TRUE) {   // TX 대기중인 상태에서 강제 종료
      for (int i = 0; i < 8; i++) {
        if (arg->pDlg->pData->send_window[i] == arg->pDlg->pData->SEND_NOT_ACKRECV)
          arrg[i].ForceStop = TRUE; // REJ, RR(p=1) 에 의한 업데이트 이후에도
                                    // 살아있는 타이머 에 대한 강제종료
          Sleep(100);
      }
        
      arg->ForceStop = FALSE; 
      arg->Thread_run = TRUE; // 모든 Timer 강제종료 후 TX 재시작
      Sleep(500); // 스레드 종료를 기다리기 위함.
    }
  }
}


