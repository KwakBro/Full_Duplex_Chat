# Socket_chatting
MFC기반 Socket Communication을 이용한 양방향 데이터 통신 프로젝트

개요
===
MFC를 이용하여 간단한 Socket 통신 프로그램에  
+ TCP/UDP이용한 Full-Duplex Multi-thread 
+ 사진 전송
+ Timeout
+ ARQ ( REJ, ACK, etc.. )
+ Checksum
+ Piggyback
+ Sliding Window
+ Segmentation ~ Reassemble  

기능 추가하여 데이터통신의 개념 실습.  

구조 
===
![image](https://user-images.githubusercontent.com/68212288/87344185-f089ac80-c588-11ea-8375-e524832ca675.png)  
![image](https://user-images.githubusercontent.com/68212288/87344304-2595ff00-c589-11ea-97cd-333168593f6b.png)  

동작원리
===
+ Segmentation  
---
![image](https://user-images.githubusercontent.com/68212288/87447759-ebcf0200-c635-11ea-875b-5b41362c55b0.png)  
+ Reassembly  
---
![image](https://user-images.githubusercontent.com/68212288/87447832-ff7a6880-c635-11ea-88d5-365efceccee7.png)  
+ CheckSum  
---
![image](https://user-images.githubusercontent.com/68212288/87447886-115c0b80-c636-11ea-9cf2-490470807d98.png)  
+ Sliding Window 
---
![image](https://user-images.githubusercontent.com/68212288/87447955-2afd5300-c636-11ea-96b1-4a5b161b5ac1.png)  

    RR 을 받는 경우 정상적으로 송신받은 데이터는 제거 후 윈도우를 동기화. 이때의 동작이 Sliding Window  
    
![image](https://user-images.githubusercontent.com/68212288/87448062-4bc5a880-c636-11ea-8652-b513aa0ad472.png)  

    Error가 발생하여 REJ를 받거나, RR(p=1) 을 받는 경우 가장 마지막으로 성공한 상황까지 n을 옮긴다.  
    Window를 n까지 옮긴듯한 동작이므로 Go-Back-n 이다.  
    


결과물
===
![image](https://user-images.githubusercontent.com/68212288/87344697-d43a3f80-c589-11ea-8a13-36c5cbc09ad3.png)
![image](https://user-images.githubusercontent.com/68212288/87344859-1c596200-c58a-11ea-9388-37161d09b212.png)
