UINT RX(LPVOID ARG) {
  ThreadArg* arg = (ThreadArg*) ARG; 
  CString temp; 
  int index = 0;
  
  while (true) {
    if (arg->Thread_run == TRUE) {
      POSITION pos = arg->pList->GetHeadPosition(); 
      POSITION curr; 
      while (pos != NULL) {
        curr = pos; 
        CString msg = arg->pList->GetNext(pos); 
        index = 0;
        
        do { 
          if (msg[index] != 'Wo') { //만약 받은 데이터가 문자의 끝이 아닐시
            temp.Insert( temp.GetLength(), msg[index]); //temp에 insert
          } 
          else {
            temp.Insert( temp.GetLength(), msg[index]); //끝이라면 일단 temp 뒤에다가 insert 이후
            break;                                      // 해당 do-while 종료
          }
        } while (( index++ % 8) != 7);      // 한 msg의 최대 내용은 8개의 TCHAR이므로, index가 7이 되면 이는 한 내용의 끝, 종료.

        if (temp[ temp.GetLength() - 1] == '\0') { // 만약 위에서 종료된 상황( index가 over든, \0을 만나든 ) 일때의 \0 처리
          arg->pList->RemoveAt(curr);              // 일단 해당 포지션의 내용 지움 ( 이미 추가했으니 )
          temp.Insert(0, _T("Client : ")); 
          arg->pDlg->List_Recieved.AddString(temp);   // 완성을 의미하므로 temp를 받은 내용에 출력
          temp. Empty();                              // 다음 작업을 위해 temp 초기화
          arg->Thread_run = FALSE;
        }
        else {
          arg->pList->RemoveAt(curr); //해당 포지션 지움. 다음번 작업을 위해 tmep 남김.
        }
      }
      Sleep(100);
    }
    else
      Sleep(100);
  }
  return 1;
}
 
