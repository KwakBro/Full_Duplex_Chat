critical Section CR, CT; 
int count_for_Timer = 0; 
int flag_for_timeout [8] = { FALSE, }; 
POSITION Tast_pos = NULL;

HUINT time(LPVOID ag) {
  Arr_arg *arg = (Arr_arg*)ag; 
  Packet ToSend; 
  LARGE_INTEGER start, end, freq; 
  int result;
  
  ToSend = arg->pList->GetAt(arg->element_pos);
  
  QueryPerformanceFrequency(&freq); 
  QueryPerformanceCounter(&start);
  
  while (true) {
    Sleep(1); 
    QueryPer formanceCounter (&end); 
    result = ((double)(end. QuadPart - start. QuadPart) / (double)freq. QuadPart) * 1000;
    
    if (((Arr_arg*)ag) ->ForceStop == TRUE) { 
      CT.Lock(); 
        // 강제 종료는 REJ, Check RR에 의한 활동임
        // 즉 강제종료의 경우에는 TX 데이터를 지우지 않고 끝냄으로써
        // TX의 재시작으로 해당 데이터의 재전송을 의미함
        // 해당방법으로는 순차적이 아닌 쓰레드 스케쥴링에 대한 문제점인
        // 재전송시의 Sequence 비순차적 전송이 발생하지 않게 됨.
      arg->pDlg->tx_arg. Thread_run = TRUE; 
      arg->ForceStop = FALSE; 
      CString str; 
      str.Format(T("timer %d force die."), arg->run_st); 
      arg->pDig->Process.AddString(str); 
      flag_for_timeout[arg->run_st] = FALSE;
      --count_for_Timer; // 현재 활동중인 타이머 개수 조절
      CT.Unlock(); 
      return 1;
    }
    if (arg->pData->send_window[arg->run_st] == arg->pData->SEND_NOT_ACKRECV) { 
      if (result < 2500) {
        continue;
      }
      CT.Lock(); 
      CString kk; 
      kk.Format(_T( "time out in timer %d"), arg->run_st); 
      arg->pDlg->Process.AddString(kk);
      // 아직 ACK를 못 받은 경우에
      // Timeout RR 보내기.
      Packet RRP; 
      RRP. IS = TRUE; 
      RRP.PF = TRUE; 
      RRP.RR_REJ = true; 
      CString str;
      str.Format(LI("time out at Segment_%d, checking RR sending.."), ToSend. sequence_send); 
      arg->pDlg->Process. AddString(str); 
      /////////////////RR ( p = 1 ) Send/////////////////////////// 
      if (arg->pDlg->pData->SendToEx(&RRP, 22, arg->pDlg->por tnum,arg->pDlg->pData->addr) < 0)
        AfxMessageBox(_T("err")); 
      //////////////////////////////////////////////////////////// 
      CT.Unlock(); 
      Sleep(1000); // REJ 받고 정상종료/ 강제 종료 범주를 나눌때 까지 기다림.
      CT.Lock();
      
      QueryPerformanceCounter(&start); 
      CT.Unlock();
    }
    else { // ACK를 받은 경우 타이머 종료
      CT.Lock(); 
      arg->pList->RemoveAt(arg->element_pos); 
      CString ff; 
      ff.Format(_T("%d_timer success end"), arg->run_st); 
      arg->pDlg->Process.AddString(ff);
      flag_for_timeouts[arg->run_st] = FALSE; // 정상종료
      --count_for_Timer; 
      CT.Unlock(); 
      return 1;
    }
  }
}
