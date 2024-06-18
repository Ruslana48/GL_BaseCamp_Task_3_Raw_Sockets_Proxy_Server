# GL_BaseCamp_Task_3_Raw_Sockets_Proxy_Server

Requirements:
- Use Linux for implementation. (or virtual machine with Linux)
- Use GCC for compilation.
- Use C/C++ for implementation.
- Console application. No UI is required.
- Use Make file for build/link/clean instructions.
- Things to google: "RAW socket"

3. Implement Packets checker application.
- Create simple TCP based client-server applications.
- Create a proxy application (that is in the middle).
- Application logic:
	- Client sends data to Proxy. 
	- Option1: Proxy just forwards it to the server.
	- Option2: Proxy does modifications in payload and resend them to server.

For Option2: Server must figure out that data has been changed in the middle (calculating packet checksum).

# Run application

// mkdir build && cd build

`cmake CMakeLists.txt`

`make`

Run (sudo usage is mandatory since raw sockets require root privileges)

`sudo ./RawSocketsProxyServer`
