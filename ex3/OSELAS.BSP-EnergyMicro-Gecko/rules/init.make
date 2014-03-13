# -*-makefile-*-
#
# Copyright (C) 2011 by Marc Kleine-Budde <mkl@pengutronix.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_INIT) += init

INIT_VERSION	:= 1.0.0

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/init.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/init.targetinstall:
	@$(call targetinfo)

	@$(call install_init, init)
	@$(call install_fixup,init,PRIORITY,optional)
	@$(call install_fixup,init,SECTION,base)
	@$(call install_fixup,init,AUTHOR,"Marc Kleine-Budde <mkl@pengutronix.de>")
	@$(call install_fixup,init,DESCRIPTION,missing)

	@$(call install_alternative, init, 0, 0, 0755, /linuxrc)

	@$(call install_finish,init)

	@$(call touch)

# vim: syntax=make
