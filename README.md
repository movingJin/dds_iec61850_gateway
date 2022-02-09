# DDS_IEC61850_Gateway
### Autor
Corresponding Author: Dept. of Computer Engineering, Sejong University, Korea  
E-mail: shdlehdwls@gmail.com  
ORCID: https://orcid.org/0000-0002-8521-9866  

### Configuration
The DDS to IEC 61850 gateway for microgrid network. This project necessary to MMS-Lite and OpenDDS API. Please install in advance above APIs.  
마이크로그리드 네트워크를 위한 DDS와 IEC61850 게이트웨이 구성을 다루며, 이 프로젝트는 MMS-Lite와 OpenDDS API를 필요로 합니다. 아래의 API를 설치 바랍니다.
1. Set up MMS-Lite API.
2. Set up OpenDDS ver.3.13 with Xerces and security module on MMS-Lite API's root directory.
3. Paste this project to $(DDS_ROOT)/PublicIOT directory.
4. Go to the $(DDS_ROOT)/PulicIOT/ and typing command make.  

=========================================================================
1. MMS-Lite API를 설치하십시오.
2. OpenDDS 3.13 version과 OpenDDS에서 제공하는 Xerces와 security 플러그인을 MMS-Lite가 설치된 root 디렉토리에 함께 설치 하십시오.
3. 이 프로젝트를 $(DDS_ROOT)/PublicIOT 디렉토리에 check out 하여 복사합니다.
4. $(DDS_ROOT)/PublicIOT 로 이동 후, make 명령어로 빌드합니다.  

### How To Run
Server side: #./DDS_Server -DCPSConfigFile rtps_DDS_Server.ini  
Client side: #./DDS_Client -DCPSConfigFile rtps_DDS_Client.ini (if you use IEC61850 server, add -m option.)

### Protocol Stack
Gateway server has dual protocol stacks, One is MMS protocol to communicating with relay, ESS, and other electrical devices.
Another is DDS protocol to communicating Gateway Client through public internet network.
Therefore, Client side has only one DDS protocol stack.  

게이트웨이 서버는 듀얼 프로토콜 스택을 가집니다. 한 쪽은 릴레이, ESS, 혹은 다른 전기장치와 통신하기 위한 MMS 프로토콜을 다루며,  
다른 한 쪽은 인터넷망환경에서 게이트웨이 클라이언트와 통신하기 위한 DDS 프로토콜을 다룹니다.  
때문에, 게이트웨이 클라이언트 측은 오직 DDS 프로토콜 스택을 다룹니다.

![image](https://user-images.githubusercontent.com/23163982/152809698-afb93010-6ffd-41ec-9305-14fe7f81b6e6.png)

### Implementation
* 게이트웨이는 3종류의 통신 주체가 있으며, 각각 DMS, MGMS (IEC 61850 클라이언트/서버), IEC 61850 서버로 구성.
* DMS와 IEC 61850 서버는 서로 다른 통신프로토콜을 사용하지만, MGMS멀티플랫폼이 이 둘의 통신이 가능하도록 중간자 역할을 수행

#### Gateway
* C프로젝트와 C++프로젝트를 통합하기 위해 MMS-lite 프로젝트를 정적라이브러리로 빌드한 후 OpenDDS 프로젝트에 포함.
* MMS-Lite의 클라이언트기능만을 필요로 하므로 시스템의 크기와 소비되는 자원을 줄이기 위해 불필요한 서버기능을 제거한 후에 오브젝트파일을 생성하여 libcositpxs0_ld.a 이름의 정적라이브러리를 생성.
* 이후 OpenDDS 프로젝트에서 libcositpxs0_ld.a 라이브러리를 링크하여 MMS의 클라이언트 기능을 수행.
* 기본적인 요청은 아래 시퀀스 다이어그램과 같이 DMS(End user)가 마이크로그리드 내, 장치로부터 데이터를 취득 요청시,  
MGMS(게이트웨이)를 통해 IEC61850서버로 요청한 뒤, IEC61850 서버가 MGMS로 응답, MGMS가 Message 변환후, DMS로 DDS프로토콜을 사용하여 응답하는 구조를 가짐.

![image](https://user-images.githubusercontent.com/23163982/152804943-2d70f9a3-2c5b-447e-9420-0ad008bdcb6e.png)

#### IEC61850 Server
* 게이트웨이가 기능적으로 올바르게 동작하는지 확인하기 위해 IEC 61850 서버를 개발.
* 서버는 분산에너지자원 중 태양광발전장치기능을 수행하며, MGMS의 요청에서 따라 response 혹은 report 기능을 수행.
* 실제 태양광발전장치의 계측데이터를 제공하는 IED 서버와 유사한 환경에서 실험을 진행하기 위해 태양광 패널과 라즈베리파이로 IED 서버를 개발.
* 라즈베리파이는 I2C 통신을 통해 ADC가 읽어온 아날로그 데이터를 수신하며, 이때 클럭 동기화 기능을 위한 SCL핀과 입출력데이터 전송을 위한 SDA (Serial Data) 핀이 사용.  
![image](https://user-images.githubusercontent.com/23163982/152803772-69e2823a-723e-4291-9f8d-254c0dba6941.png)

### Lab environment
* MGMS역할을 하는 노트북PC는 KT회선에 연결되어 있으며, DMS역할을 하는 PC는 세종대학교 내 서버 실에 위치.
* 서로 다른 네트워크에 위치한 두 호스트 간의 hop수는 13 hop.
* 태양광패널과 RaspberryPi3를 태양광 IED 서버로 구성.

![image](https://user-images.githubusercontent.com/23163982/152807252-1a412779-0ae1-4755-8dde-4c39e43ddfd2.png)
![image](https://user-images.githubusercontent.com/23163982/152807986-fd890935-666c-4864-a8f5-bd1f23a08c49.png)


### Result
![image](https://user-images.githubusercontent.com/23163982/152995182-2eb7c031-c33f-411a-bb80-9bcd36aba7a3.png)
![image](https://user-images.githubusercontent.com/23163982/152995281-ccc88004-da89-455a-9d4c-ca649cf4d0ea.png)
![image](https://user-images.githubusercontent.com/23163982/152995317-92ee025a-4aaf-4d76-967b-c888a71e434e.png)

* Reliability QoS를 RELIABLE로 설정한 경우 최대 1,700ms의 RTT와 0%의 손실률, 0.004%의 1초 이상의 응답률을 보임.
* BEST_EFFORT로 설정한 경우의 최대 61ms의 RTT와 0.001%의 손실률을 보임.
* 이는 Reliability QoS의 재전송 메커니즘으로 인한 결과이며 BEST_EFFORT로 설정하였을 때 수신 측이 메시지를 올바르게 수신하였는가를 확인하지 않기 때문에 빠른 전송이 가능한 것.
* ELIABLE의 1초 이상 응답률과 지연시간, BEST_EFFORT의 패킷손실률을 비교하였을 때 BEST_EFFORT를 사용하여 지연시간을 줄이고, 일정 시간 내에 수신되지 않으면 관리자에게 메시지가 전달되지 않았음을 알리는 것이 적당할 것으로 보임.
