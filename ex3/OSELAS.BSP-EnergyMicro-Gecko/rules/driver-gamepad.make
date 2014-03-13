# -*-makefile-*-
#
# Copyright (C) 2014 by 
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_DRIVER_GAMEPAD) += driver-gamepad

#
# Paths and names and versions
#
DRIVER_GAMEPAD_VERSION	:= 1.0
DRIVER_GAMEPAD		:= driver-gamepad-$(DRIVER_GAMEPAD_VERSION)
DRIVER_GAMEPAD_URL		:= lndir://$(PTXDIST_WORKSPACE)/local_src/$(DRIVER_GAMEPAD)
DRIVER_GAMEPAD_DIR		:= $(BUILDDIR)/$(DRIVER_GAMEPAD)
DRIVER_GAMEPAD_LICENSE	:= unknown

ifdef PTXCONF_DRIVER_GAMEPAD
$(STATEDIR)/kernel.targetinstall.post: $(STATEDIR)/driver-gamepad.targetinstall
endif

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

$(STATEDIR)/driver-gamepad.prepare:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/driver-gamepad.compile:
	@$(call targetinfo)
	$(KERNEL_PATH) $(KERNEL_ENV) $(MAKE) $(KERNEL_MAKEVARS) \
		-C $(KERNEL_DIR) \
		M=$(DRIVER_GAMEPAD_DIR) \
		modules
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/driver-gamepad.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/driver-gamepad.targetinstall:
	@$(call targetinfo)
	$(KERNEL_PATH) $(KERNEL_ENV) $(MAKE) $(KERNEL_MAKEVARS) \
		-C $(KERNEL_DIR) \
		M=$(DRIVER_GAMEPAD_DIR) \
		modules_install
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/driver-gamepad.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, DRIVER_GAMEPAD)

# vim: syntax=make
