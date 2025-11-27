-------------------------------------------------------------------------
------- How to test the S-LDM with an ms-van3t simulated scenario -------
-------------------------------------------------------------------------

First of all, both the S-LDM and ms-van3t are designed to run on Linux. Officially supported versions: Ubuntu 20 LTS and Ubuntu 22 LTS.

1) If you do not already have it, download the S-LDM code from GitHub (https://github.com/DriveX-devs/S-LDM) and compile it with `´make -j10`

2) If you do not already have ms-van3t, you have now two options: 

    1) \[Easiest option to start with; recommended]  Use the pre-prepared version of ms-van3t with a lane merge scenario already included (`ms-van3t-lanemerge-gntsprop`); this is not the latest version of ms-van3t, but it comes already patched and configured with a specific emulation mode scenario for the S-LDM (this scenario can be used as-is to test the S-LDM setup for the first time, and then customized); if you use this version, after extracting it from the .zip archive, you need (just once, the first time you extract the files) to configure and build it with the following two commands, to be launched from inside the `ns-3.33` directory:
        `./waf configure --build-profile=optimized --enable-examples --enable-tests --disable-python` and then `./waf build` The build process may take some minutes to complete (even up to 30-40 minutes on some older and slower devices). You can then build your own scenarios, besides the lane merge one, directly on this version. Your own scenarios should be build so that they support UDP emulation (following the logic of the sample V2X emulator already available; see also here: https://ms-van3ts-documentation.readthedocs.io/en/master/Emulation.html#udp-mode)

    2) \[More complex, to have the latest version of ms-van3t] Download the latest version of ms-van3t from GitHub: https://github.com/ms-van3t-devs/ms-van3t; compile then ms-van3t (following the instructions on GitHub); patch it with the patch files included inside the repository of the S-LDM, in `./tester/ms-van3t-patch`; if the patch does not work with the latest version of ms-van3t, you can manually copy the `metadata-header.cc/.h` files inside `./src/automotive/model/GeoNet` of the ms-van3t repository, and manually patch the `./src/automotive/model/GeoNet/geonet.cc` file in the ms-van3t source tree with the modifications highlighted inside the file `./tester/ms-van3t-patch/0001-patch-ms-van3t-sldm.patch` of the S-LDM repository; you need then to create your own emulation mode scenario, and run it with UDP emulation (following the sample V2X emulator already available; see also here: https://ms-van3ts-documentation.readthedocs.io/en/master/Emulation.html#udp-mode)

3) If you do not already have an AMQP 1.0 broker to connect to, or if you want to use a locally hosted messaging broker, you can download ActiveMQ classic (version 5.xy.z is the officially supported one) from: https://activemq.apache.org/components/classic/download/; after you have downloaded the `apache-activemq-5.xy.z-bin.tar.gz` archive, extract the content in a folder of your choice (that will be referred to as <activemq_dir> from now on). ActiveMQ is the officially supported broker and it supports the AMQP 1.0 protocol, i.e., the AMQP version supported by the S-LDM and standardized by the C-Roads project for vehicle-to-infrastructure communications. All the rest of the steps refer to a locally hosted AMQP 1.0 broker (if you want to use a remote broker you will need to replace `127.0.0.1` with the IP address of the remote broker).

4) Open now six terminal tabs/windows.

5) On terminal tab/window 1 move to `<activemq_dir>/apache-activemq-5.xy.z/bin` and run the following command to start the AMQP 1.0 broker: `./activemq console`; keep this terminal window open; you will be able to close the broker with a simple `Ctrl+C` on this terminal tab/window.

6) On terminal tab/window 2 move to the directory where you downloaded and compiled the S-LDM, and run the following command to start the S-LDM with a basic set of options (you can then run any combination of options that you need): ./SLDM -A 46.020128:11.027979-47.034540:11.686929 --ext-area-lat-factor 0.0002 --ext-area-lon-factor 0.0002 -L stdout --enable-on-demand-requests. Instead of `46.020128:11.027979-47.034540:11.686929` you should insert the coverage area of the S-LDM, in terms of `minumum_lat:minimum_lon-maximum_lat:maximum_lon`. `46.020128:11.027979-47.034540:11.686929`, is, however, the right set of coordinates designed for the default lane merge scenario of `ms-van3t-lanemerge`.
Running `./SLDM --help` will visualize, instead, all the available options.

7) On terminal tab/window 3 move to the directory `./S-LDM/tester/ms-van3t-namespace` (inside the S-LDM folder), and run: `./ms-van3t-namespace-creator.sh`
This will create a Linux network namespace to enable the communication between ms-van3t, the UDP-AMQP-relayer and the broker, to which the S-LDM should subscribe. You will be able to delete the namespace, after doing your tests, by simply pressing ENTER (or Ctrl+C) on this terminal tab/window.

8) On terminal tab/window 4 move to the directory `./S-LDM/tester/Python-REST-server` (inside the S-LDM folder) and run: `python3 ./RESTserver.py`
This is not mandatory, but it will start a `dummy` Python REST server, to receive data from the S-LDM when the lane merge occurs, if you choose to run the lane merge scenario inside `ms-van3t-lanemerge`.

9) On terminal tab/window 5 move to the directory `./S-LDM/tester/UDP-AMQP-relayer` (inside the S-LDM folder) and run: `./UDPAMQPrelayer --url 127.0.0.1:5672`
This will enable the UDP-AMQP-relayer to receive messages from the simulated vehicles in ms-van3t and route them to the broker, to which the S-LDM should subscribe; you will be able to close the relayer with a simple `Ctrl+C` on this terminal tab/window.

10) On terminal tab/window 6 move inside the ms-van3t `ns-3` directory and run your scenario in UDP emulation mode. The `ns-3` directory is called `ns-3.33` in `ms-van3t-lanemerge-gntsprop`, or `ns-3-dev` in the latest version of ms-van3t.

    1) If you are using `ms-van3t-lanemerge`, you can easily run the pre-pre-prepared lane merge scenario with:  sudo ip netns exec ns1 ./waf --run `v2x-emulator-lanemerge --interface=veth1ns --subnet=10.10.7.0 --gateway=10.10.7.254 --udp=10.10.7.254:20000 --sumo-netns=ns1 --sim-time=800`

    2) If you are using the latest version of ms-van3t with a custom scenario, run it with the options that you need, with a command that should look like `sudo ip netns exec ns1 ./ns3 run `<\simulation name> \<command line options>`

11) After running the scenario, you should see that the S-LDM is receiving the CAM messages from the simulated vehicles, via the AMQP 1.0 broker. You can then visualize the vehicles moving in the S-LDM GUI by opening a browser and pointing it to `127.0.0.1:8080` (8080 is the default port for the GUI but can be changed with the --vehviz-web-port option of the S-LDM). You can also query the S-LDM database, requesting the data of all vehicles located inside a given circular area, by writing a simple JSON client as described in the `Retrieving data from the S-LDM on demand` section of the README on GitHub. A sample Python JSON client can be found inside (in the S-LDM folder): `./S-LDM/tester/Sample-JSON-client`