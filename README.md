## STEPS ON HOW TO SET UP THE ENVIRONEMTNS:

**STEP1**
*Create a broker instance*
- Go to ~/DAT295/apache-artemis-2.33.0/bin
- ./artemis create ~/DAT295/<DIR>
- Enter log in info 
	- Username: admin
	- Password: admin
	- Y for anonymous login
- cd ~/DAT295/<DIR>/bin/ && ./artemis run

**STEP2**
*Set up proton0.40 and the SLDM
- unpack the proton-tar
- follow the commands in the git repo of the SLDM
- rm -rf build
- mkdir build && cd build
- cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
- make -j$(nproc)
- sudo make install
- sudo ldconfig
- in case of getting this error undefined symbol: _ZTVN6proton6senderE
	- DEBUGGING: <check for eventual missmatch proton version on the machine, make sure to link the SLDM to the correct proton version >
	- remove all different proton versions on the machine and Clean Install of Proton 0.40.0
	- check with ldconfig -p | grep proton to make sure it is linked correctly
	- against apt show libqpid-proton-cpp12
- when doing makefile in the SLDM and getting undefined reference to 'CONF_modules_unload@@OPENSSL_3.0.0' do this:
	- in the make file of the SLDM after the "$(EXECNAME): $(OBJ_CC)" replace with <$(CXX) $(LDFLAGS) $^ -o $@ -Wl,--copy-dt-needed-entries $(LDLIBS) $(CXXFLAGS) $(CFLAGS)>
	- to make sure the linker pulled in dependent symbols from libcrypto.
