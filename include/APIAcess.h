#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "Demarcacao.h"

#pragma once

#define SERVER_URL = "https://saed-ihm.onrender.com/"
#define API_KEY = "k)]p}A:t!s[Mu5V!T2niRrL8@Lrbc@%s\"(tR)]#c2o8nw#n5N3X%b6%hl\"XIs[<L"

// Caso necessário, atualizar periodicamente
#define SECUR_CERT =                                                                        \
                       "-----BEGIN CERTIFICATE-----\n"                                      \
                       "MIIDqDCCA02gAwIBAgIQHwW5BOFxLYcTv4Fo/5eH/jAKBggqhkjOPQQDAjA7MQsw\n" \
                       "CQYDVQQGEwJVUzEeMBwGA1UEChMVR29vZ2xlIFRydXN0IFNlcnZpY2VzMQwwCgYD\n" \
                       "VQQDEwNXRTEwHhcNMjYwNTI2MjEwMjE1WhcNMjYwODI0MjIwMTUwWjAXMRUwEwYD\n" \
                       "VQQDEwxvbnJlbmRlci5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARze/Dk\n" \
                       "nzLX5Gits31q8yugeD2IW5TvewNwlD6v4slY+ff2SLhJzSGP4Y3mGH0W6QeoxA1d\n" \
                       "59oH2CCTIJU3CiPAo4ICVTCCAlEwDgYDVR0PAQH/BAQDAgeAMBMGA1UdJQQMMAoG\n" \
                       "CCsGAQUFBwMBMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYEFJOrm5yVzMc8+p9kmyPk\n" \
                       "MoJwajZeMB8GA1UdIwQYMBaAFJB3kjVnxP+ozKnme9mAeXvMk/k4MF4GCCsGAQUF\n" \
                       "BwEBBFIwUDAnBggrBgEFBQcwAYYbaHR0cDovL28ucGtpLmdvb2cvcy93ZTEvSHdV\n" \
                       "MCUGCCsGAQUFBzAChhlodHRwOi8vaS5wa2kuZ29vZy93ZTEuY3J0MCcGA1UdEQQg\n" \
                       "MB6CDG9ucmVuZGVyLmNvbYIOKi5vbnJlbmRlci5jb20wEwYDVR0gBAwwCjAIBgZn\n" \
                       "gQwBAgEwNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2MucGtpLmdvb2cvd2UxL1R0\n" \
                       "T1RUckM4dlJRLmNybDCCAQQGCisGAQQB1nkCBAIEgfUEgfIA8AB2AMIxfldFGaNF\n" \
                       "7n843rKQQevHwiFaIr9/1bWtdprZDlLNAAABnmZPVMIAAAQDAEcwRQIhANdM4lux\n" \
                       "ihfZYLswGy/rcG+G/WssuL7Bx58RpV4uS7EiAiBWxX98jyypkZJ17acyRAiiE3iy\n" \
                       "GHTC6A7NQKUSz6T/owB2ANdtfRDRp/V3wsfpX9cAv/mCyTNaZeHQswFzF8DIxWl3\n" \
                       "AAABnmZPVLEAAAQDAEcwRQIgXM5fiMciAnSY04omLAYed2U87bYQMLz1VAivC+n2\n" \
                       "4dQCIQDhjTO34pBjjKk0Lzhxr9ht3VMGXGQe56YvJ5CFBFcgPTAKBggqhkjOPQQD\n" \
                       "AgNJADBGAiEA4k/ujjcK0+A2zhU2QjJA4hEJu7SBuTYO2r8/3zQ1Vl8CIQCP4tiz\n" \
                       "m8J0zqPNEwwOaEdMnNiBPO2knJt40nhi84c21g==\n"                         \
                       "-----END CERTIFICATE-----\n"

namespace SAeD
{
    class APIAcess
    {
    private:
        WiFiClient client;
        HTTPClient https;

    public:
        APIAcess() {}

        void begin();

        bool pubNewItem(area area);
    };
}