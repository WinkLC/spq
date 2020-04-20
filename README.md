# Strict Priority Queuing (SPQ)

## Purpose
The project is used to achieve a more real SPQ.
The project also comes with ns-3 applications that allow for testing and validation of the SPQ  implementations. This can be found in `$ cd ns-3-allinone/ns-3-dev/scratch` in `myspq.cc`. There are n inputServers (--inputCount=n) and m outputServers(--outputCount=m) and a router connected to inputservers and outputservers.8 priority queues are configured on the ingress and egress ports of the router.

## Installation
The best way to install this NS3 project is to clone this repository, and then go through the [NS3 installation guide](https://www.nsnam.org/wiki/Installation)

The only supported platform for this project in Linux, Ubuntu LTS versions 16.04 or 18.04 are the reccomended distribution.
Running Ubuntu on a virtual machine installed on a macOS or Windows machine is also a viable option.

## How to Run
Once properly installed, follow these steps to run the program.
- Navigate to the ns-3-dev directory `$ cd ns-3-allinone/ns-3-dev`
- Configure the waf `$ ./waf -d debug configure`
- Build the executable `$ ./waf build`
- Run `./waf --run "myspq --inputCount=4 --outputCount=2" 2>log.log`
