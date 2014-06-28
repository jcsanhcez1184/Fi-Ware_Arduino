#!/bin/bash
AUTH_TOKEN=lBiR8Pztrng2TMJdSlALgNt8KtHSrO3AFiHmPs3V5UYksZLbMbRher6S4qViBtKpzSe4r21Vo49lGBi4SoO4WQ
ID=1000000
curl orion.lab.fi-ware.org:1026/ngsi10/contextEntities/$ID -X GET -s -S --header 'Content-Type: application/json' --header 'Accept: application/json' --header "X-Auth-Token: $AUTH_TOKEN"
