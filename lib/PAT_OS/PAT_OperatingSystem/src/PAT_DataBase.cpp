#include "PAT_JsonStorage.h"
#include "PAT_DataBase.h"

//===============================================================================================================================================================================================
//                                                                                         / dbuser
//===============================================================================================================================================================================================
const char *dbuser_json = R"(
[
  {
    "id": 0,
    "userId": 1,
    "username": "NovaVendor",
    "password": "Vendor123",
    "role": "Vendor"
  },
  {
    "id": 1,
    "userId": 2,
    "username": "NovaSupperAdmin",
    "password": "NovaSupperAdmin123",
    "role": "SupperAdmin"
  },
  {
    "id": 2,
    "userId": 3,
    "username": "NovaAdmin",
    "password": "Admin123",
    "role": "Admin"
  },
  {
    "id": 3,
    "userId": 4,
    "username": "NovaUser",
    "password": "NovaUser123",
    "role": "User"
  }
]
)";
JsonStorage dbuser("/dbuser.json", dbuser_json, strlen(dbuser_json));
//===============================================================================================================================================================================================
//                                                                                         / dbaccount
//===============================================================================================================================================================================================
const char *dbaccount_json = R"(
[
]
)";
JsonStorage dbaccount("/dbaccount.json", dbaccount_json, 1024);
//===============================================================================================================================================================================================
//                                                                                         / dbsetting;
//===============================================================================================================================================================================================
const char *dbsetting_json = R"({
    "Settings": {
        "WiFiNetwork": {
            "Enable": true,
            "SSIDs": [
                {
                    "Name": "PAT_IOT",
                    "MAC": "B6:11:74:86:64:A1",
                    "RSSI_db": -59,
                    "RSSI_Percentage": "82%",
                    "RSSI_Distance": 3
                },
                {
                    "Name": "B7tb-dXNlcjI5MzA3Nzg2ODc5",
                    "MAC": "7E:1C:68:6E:DA:AB",
                    "RSSI_db": -89,
                    "RSSI_Percentage": "22%",
                    "RSSI_Distance": 25
                }
            ],
            "Connections": [
                {
                    "Enable": true,
                    "Name": "PAT_IOT",
                    "Password": "Password_1"
                },
                {
                    "Enable": false,
                    "Name": "mahsa",
                    "Password": "1123456789"
                }
            ]
        },
        "HotspotNetwork": {
            "Enable": false,
            "Name": "NAR_14285576",
            "Password": "NAR_14285576",
            "Status": "Disconnected",
            "IP": "192.168.80.200",
            "GW": "192.168.80.1",
            "SN": "255.255.255.0",
            "DNS": "8.8.8.8"
        },
        "LANNetwork": {
            "Enable": true,
            "Status": "Disconnected",
            "Static": {
                "Enable": false,
                "mac": "DEADBEEFFE01",
                "IP": "192.168.88.220",
                "GW": "192.168.88.1",
                "SN": "255.255.255.0",
                "DNS": "8.8.8.8"
            }
        },
        "InputsOutputs": {
            "ChannelNumber": 12,
            "Signal": [
                "Relay",
                "Relay",
                "Relay",
                "Relay",
                "Relay",
                "Relay",
                "Relay",
                "Relay",
                "Relay",
                "Relay",
                "Relay",
                "Relay"
            ]
        },
        "MQTTBroker": {
            "Enable": false
        },
        "DateAndTime": "xx:xx",
        "Password": {
            "SuperAdmin": "**********",
            "Admin": "**********",
            "User": "**********"
        },
        "ModelName": "Novaday IOT Controller",
        "SerialNumber": "NAR_xxxxxxxx",
        "LastSyncWithServer": "14xx-xx-xx xx:xx",
        "Version": "1.0.0"
    }
})";

JsonStorage dbsetting("/dbsetting.json", dbsetting_json, strlen(dbsetting_json) + 1024); // Use strlen instead of sizeof
//===============================================================================================================================================================================================
//                                                                                         / db_webServer_relay
//===============================================================================================================================================================================================
const char *db_Home_json = R"({
"Relay": [0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1],
"Name": ["Light1", "Light2", "Light3", "Light4", "Light5", "Light6", "Light7", "Light8", "Light9", "Light10", "Light11", "Light12"],
"Description": ["Room1", "Room2", "Room3", "Room4", "Room5", "Room6", "Room7", "Room8", "Room9", "Room10", "Room11", "Room12"],
"Icon": ["SVG1", "SVG2", "SVG3", "SVG4", "SVG5", "SVG6", "SVG7", "SVG8", "SVG9", "SVG10", "SVG11", "SVG12"]
})";

JsonStorage db_Home("/db_Home.json", db_Home_json, strlen(db_Home_json) + 500); // Use strlen instead of sizeof
