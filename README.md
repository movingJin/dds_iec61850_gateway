# DDS_IEC61850_Gateway
DDS to IEC 61850 gateway for microgrid network.

###Configuration
1. Set up mmslite API.
2. Set up OpenDDS ver.3.13 with Xerces and security module on mmslite API's root directory.
3. Paste this project to OpenDDS API's root directory.
4. go to the $(DDS_ROOT)/PulicIOT/ and typing command make.

###How To Run
Server side: #./DDS_Server -DCPSConfigFile rtps_DDS_Server.ini
Client side: #./DDS_Client -DCPSConfigFile rtps_DDS_Client.ini (if you use IEC61850 server, add -m option.)
