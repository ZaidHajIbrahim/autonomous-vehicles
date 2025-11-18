#!/bin/bash

# Define commands for each terminal
AMQP_BROKER_CMD="~/sim/apache-activemq-5.18.6/bin/activemq console ; exec bash"
S_LDM_CMD="sleep 5 && ~/sim/S-LDM/SLDM -A 46.020128:11.027979-47.034540:11.686929 --ext-area-lat-factor 0.0002 --ext-area-lon-factor 0.0002 -L stdout --enable-on-demand-requests; exec bash"
NAMESPACE_CMD="~/sim/S-LDM/tester/ms-van3t-namespace/ms-van3t-namespace-creator.sh ; exec bash"
REST_SERVER_CMD="sleep 10 && python3 ~/sim/S-LDM/tester/Python-REST-server/RESTserver.py ; exec bash"
AMQP_RELAYER_CMD="sleep 10 &&~/sim/S-LDM/tester/UDP-AMQP-relayer/UDPAMQPrelayer --url 127.0.0.1:5672 ; exec bash"
MS_VAN3T_CMD="sleep 10 && sudo ip netns exec ns1 sudo -u $(id -un) ~/sim/ms-van3t/ns-3-dev/ns3 run "v2i-trafficManager-80211p"; exec bash"

# Open a terminal for AMQP-Broker
gnome-terminal --title="AMQP-Broker" -- bash -c "$AMQP_BROKER_CMD"

# Open a terminal for S-LDM
gnome-terminal --title="S-LDM" -- bash -c "$S_LDM_CMD"

# Open a terminal for Namespace
gnome-terminal --title="Namespace" -- bash -c "$NAMESPACE_CMD"

# Open a terminal for REST-Server
gnome-terminal --title="REST-Server" -- bash -c "$REST_SERVER_CMD"

# Open a terminal for AMQP-Relayer
gnome-terminal --title="AMQP-Relayer" -- bash -c "$AMQP_RELAYER_CMD"

# Open a terminal for ms-van3t
gnome-terminal --title="ms-van3t" -- bash -c "$MS_VAN3T_CMD"