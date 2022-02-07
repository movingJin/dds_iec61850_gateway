# DDS_IEC61850_Gateway
The DDS to IEC 61850 gateway for microgrid network. This project necessary to MMS-Lite and OpenDDS API. Please install in advance above APIs.  
마이크로그리드 네트워크를 위한 DDS와 IEC61850 게이트웨이 구성을 다루며, 이 프로젝트는 MMS-Lite와 OpenDDS API를 필요로 합니다. 아래의 API를 설치 바랍니다.

### Configuration
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


![통신architecture](https://user-images.githubusercontent.com/23163982/81475936-0f4a8a80-924a-11ea-87f1-446c97646c00.png)
