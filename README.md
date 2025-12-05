# Environment Setup Guide

This README provides step-by-step instructions for setting up the environme>

- AMQP broker (Apache Artemis or ActiveMQ)
- Proton-CPP 0.40
- S-LDM build instructions
- Common debugging fixes
- ns3 build instructions
- Testing the setup

---
## Start the AMQP Broker

### Create a broker instance
```bash
cd ~/DAT295/apache-artemis-2.33.0/bin
./artemis create ~/DAT295/<BROKER_DIR>
```
When asked enter log-in info
- Username: admin
- Password: admin
- Y for anonymous login
### Run the broker
```bash
cd ~/DAT295/<DIR>/bin/
./artemis run
```
## Install Proton-CPP 0.40 and Build S-LDM

### Install Proton 0.40.0
Download and unpack the Proton tar file and then run the following:
```bash
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
sudo ldconfig
```
### Build the S-LDM
Follow the instructions, taken from the official repository, but the files are edited upon, so do not replace them:
```bash
rm -rf build
mkdir build && cd build
cmake ..
make -j$(nproc)
```
### Debugging & Common Errors
Below are common issues with Proton/S-LDM and their fixes.

### ❗ undefined symbol: `_ZTVN6proton6senderE`

**Cause:**  
Multiple Proton versions installed (system proton conflicts with custom proton).

**Fix:**
1. Remove old proton versions:
```bash
sudo apt remove libqpid-proton*
```
2. Reinstall Proton 0.40  
3. Validate with:
```bash
ldconfig -p | grep proton
```
4. Ensure S-LDM links against the correct Proton library.
---


### ❗ undefined reference to `CONF_modules_unload@@OPENSSL_3.0.0`


**Fix:**  
Edit the S-LDM `Makefile` and replace:

```bash
$(EXECNAME): $(OBJ_CC)
```

with:

```bash
$(CXX) $(LDFLAGS) $^ -o $@ -Wl,--copy-dt-needed-entries $(LDLIBS) $(CXXFLAGS) $(CFLAGS)
```

---
## 4. Verification Checklist

- Broker starts without errors  
- `./SLDM --help` works  
- Only Proton 0.40 appears in:
```bash
ldconfig -p | grep proton
```
- No undefined reference errors during S-LDM build

---
## Build NS3
### Set up the ns3 config to build it correctly
```bash
cd ~/DAT295/autonomous-vehicles/van3t/ns-3-dev
./ns3 configure --enable-tests --enable-example
./ns3 build -j 32
```
The build depends on number of avaialabe cores 

Step 10 would be something like 

```bash
sudo ip netns exec ns1 ./ns3 run --gdb "v2x-emulator --sumo-config=src/automotive/examples/sumo_files_test_map/new_map.sumo.cfg  --interface=veth1ns --subnet=10.10.7.0 --gateway=10.10.7.254 --udp=10.10.7.254:20000 --sumo-netns=ns1 --sim-time=800"
```
### ❗Getting errors:
**Fix:**
add to the config parameters the following
```bash
./ns3 configure --enable-tests --enable-example -d debug --disable-werror
```
**Cause:**  
This is due to some inconsstancy in the code inside the directory maybe using wrong libraries, dividing by zero, etc.

### ❗Permisison denied:
**Fix:**
Not recommanded, but go the lines that is complaining and comment them out, it is an if statment saying if not root stop the procecss and print a message, just remove it.


## Testing SUMO
Installing SUMO is not included in this README
### Testing the maps

