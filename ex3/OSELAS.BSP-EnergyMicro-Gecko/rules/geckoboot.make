# -*-makefile-*-
#
# Copyright (C) 2013 by Uwe Kleine-Koenig <u.kleine-koenig@pengutronix.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_GECKOBOOT) += geckoboot

#
# Paths and names
#
GECKOBOOT_VERSION	:= 2013.01.0
GECKOBOOT		:= geckoboot-$(GECKOBOOT_VERSION)
GECKOBOOT_DIR		:= $(BUILDDIR)/$(GECKOBOOT)
GECKOBOOT_SUFFIX	:= 
GECKOBOOT_URL		:= file://$(PTXDIST_WORKSPACE)/local_src/$(GECKOBOOT)
GECKOBOOT_LICENSE	:= unknown

GECKOBOOT_CONF_TOOL	:= NO
GECKOBOOT_MAKE_ENV	:= $(CROSS_ENV)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/geckoboot.targetinstall:
	@$(call targetinfo)

	@$(call install_init, geckoboot)
	@$(call install_fixup, geckoboot,PRIORITY,optional)
	@$(call install_fixup, geckoboot,SECTION,base)
	@$(call install_fixup, geckoboot,AUTHOR,"Uwe Kleine-Koenig <u.kleine-koenig@pengutronix.de>")
	@$(call install_fixup, geckoboot,DESCRIPTION,missing)
	
#	@$(call install_copy, geckoboot, 0, 0, 0755, $(GECKOBOOT_DIR)/foobar, /dev/null)

	@$(call install_finish, geckoboot)

	@install -D -m 644 $(GECKOBOOT_DIR)/geckoboot.bin $(IMAGEDIR)/geckoboot-image

	@$(call touch)

