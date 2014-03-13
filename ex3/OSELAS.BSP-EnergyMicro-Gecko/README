Build instructions
==================

These instructions mostly apply only if you want to set up the development toolchain on your own (Linux-based) computer.

If you are working from a lab computer, the toolchain has already been set up, you can skip to the "Building the Linux distribution" part, or start reading the compendium instead. 


Installing Prerequisite Packages
--------------------------------

build-essential expect dialog gawk flex bison texinfo gettext libncurses-dev automake autoconf libtool pkg-config


Building the Toolchain
----------------------

This BSP is created for OSELAS.Toolchain-2012.12. You can build it
yourself using the following commands:

	wget http://www.pengutronix.de/oselas/toolchain/download/OSELAS.Toolchain-2012.12.0.tar.bz2
	wget http://www.ptxdist.org/software/ptxdist/download/ptxdist-2012.12.0.tar.bz2

Then install ptxdist:

	tar xjf ptxdist-2012.12.0.tar.bz2
	cd ptxdist-2012.12.0
	./configure

if ptxdist is missing some packages on your system, install them and restart configure

	make
	sudo make install

This installs ptxdist to /usr/local. This particular version of ptxdist is only needed for building the toolchain, so you don't have to add it to your PATH.

For building the toolchain, fakeroot is needed:

	sudo apt-get install fakeroot

Extract the toolchain and add configuration fix:

	tar xf OSELAS.Toolchain-2012.12.0.tar.bz2
	cd OSELAS.Toolchain-2012.12.0
	add the following lines at the end of config/uclibc-0.9.33.2-arm-cortexm3-uclinuxeabi.config :
	UCLIBC_SUSV3_LEGACY=y
	UCLIBC_SUSV4_LEGACY=y

Then build the toolchain:

	ln -sf /usr/local/bin/ptxdist-2012.12.0 ptxdist
	./ptxdist select ptxconfigs/arm-cortexm3-uclinuxeabi_gcc-4.7.2_uclibc-0.9.33.2_binutils-2.22_kernel-3.6-sanitized.ptxconfig
	./ptxdist go

Now go and get yourself a cup of coffee, as this'll take some time. This will install the toolchain under /opt/OSELAS.Toolchain-2012.12.0

install ptxdist 2013.07.1
-------------------------

	wget http://ptxdist.de/software/ptxdist/download/ptxdist-2013.01.0.tar.bz2
	tar xjf ptxdist-2013.07.1.tar.bz2
	cd ptxdist-2013.07.1
	./configure && make
	sudo make install
	
It is recommended to add this version of ptxdist to your PATH, since you will be invoking it frequently.

Building the Linux distribution
--------------------------------

First, speficy the platform and toolchain configurations for the project:

	cd /path/to/OSELAS.BSP-EnergyMicro-Gecko
	ptxdist select configs/ptxconfig
	ptxdist platform configs/platform-energymicro-efm32gg-dk3750/platformconfig
	ptxdist toolchain <path_to_toolchain_bin_dir> 

	
where <path_to_toolchain_bin_dir> is, for example:
* for the lab: /opt/ex3/OSELAS.Toolchain-2012.12.0/arm-cortexm3-uclinuxeabi/gcc-4.7.2-uclibc-0.9.33.2-binutils-2.22-kernel-3.6-sanitized/bin
* on your own machine: /opt/OSELAS.Toolchain-2012.12.0/arm-cortexm3-uclinuxeabi/gcc-4.7.2-uclibc-0.9.33.2-binutils-2.22-kernel-3.6-sanitized/bin

Finally, to build the flashable images, issue:

	ptxdist images

This takes some time now. After ptxdist is completed and you plugged your Gecko
board to your machine, first make sure that the debug mode is set to "MCU".
(Press the "CFG" button under the display, then select "MCU" for "Debug
Control" and save.) Then you can do

    ptxdist test flash-all

to write the built images into the Gecko's flash and boot into Linux.
Note that this uses Energymicro's eACommander which needs 32-bit Qt
installed. See the compendium for more detailed information.
   
Install board firmware
----------------------

(This applies only if you would like to use ethernet for your project)

The only board firmware currently known to support ethernet is version
0v9p9b909 available from:

	http://forum.energymicro.com/topic/151-dk3550-dk3650-dk3750-firmware-099-beta/

The newer firmware 0v9p10b929 does not work.   
