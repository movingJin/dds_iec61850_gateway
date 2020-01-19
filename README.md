# DDS_IEC61850_Gateway
The DDS to IEC 61850 gateway for microgrid network. This project necessary to MMS-Lite and OpenDDS API. Please install in advance above APIs.

### Configuration
1. Set up MMS-Lite API.
2. Set up OpenDDS ver.3.13 with Xerces and security module on MMS-Lite API's root directory.
3. Paste this project to OpenDDS API's root directory.
4. Go to the $(DDS_ROOT)/PulicIOT/ and typing command make.

### How To Run
Server side: #./DDS_Server -DCPSConfigFile rtps_DDS_Server.ini
Client side: #./DDS_Client -DCPSConfigFile rtps_DDS_Client.ini (if you use IEC61850 server, add -m option.)
