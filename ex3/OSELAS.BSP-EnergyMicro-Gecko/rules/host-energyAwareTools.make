# -*-makefile-*-
#
# Copyright (C) 2012 by Uwe Kleine-Koenig <u.kleine-koenig@pengutronix.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
HOST_PACKAGES-$(PTXCONF_HOST_ENERGYAWARETOOLS) += host-energyawaretools

#
# Paths and names
#
HOST_ENERGYAWARETOOLS_VERSION	:= 25052012
HOST_ENERGYAWARETOOLS_MD5	:= e6e6c9b52a5e41235242bdc04e5faa02
HOST_ENERGYAWARETOOLS		:= energyAwareTools_$(HOST_ENERGYAWARETOOLS_VERSION)
HOST_ENERGYAWARETOOLS_SUFFIX	:= tgz
HOST_ENERGYAWARETOOLS_URL	:= http://cdn.energymicro.com/dl/packages/$(HOST_ENERGYAWARETOOLS).$(HOST_ENERGYAWARETOOLS_SUFFIX)
HOST_ENERGYAWARETOOLS_SOURCE	:= $(SRCDIR)/$(HOST_ENERGYAWARETOOLS).$(HOST_ENERGYAWARETOOLS_SUFFIX)
HOST_ENERGYAWARETOOLS_DIR	:= $(HOST_BUILDDIR)/$(HOST_ENERGYAWARETOOLS)

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(HOST_ENERGYAWARETOOLS_SOURCE):
#	@$(call targetinfo)
#	@$(call get, HOST_ENERGYAWARETOOLS)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#HOST_ENERGYAWARETOOLS_CONF_ENV	:= $(HOST_ENV)

#
# autoconf
#
#HOST_ENERGYAWARETOOLS_CONF_TOOL	:= autoconf
#HOST_ENERGYAWARETOOLS_CONF_OPT	:= $(HOST_AUTOCONF)

#$(STATEDIR)/host-energyawaretools.prepare:
#	@$(call targetinfo)
#	@$(call clean, $(HOST_ENERGYAWARETOOLS_DIR)/config.cache)
#	cd $(HOST_ENERGYAWARETOOLS_DIR) && \
#		$(HOST_ENERGYAWARETOOLS_PATH) $(HOST_ENERGYAWARETOOLS_ENV) \
#		./configure $(HOST_ENERGYAWARETOOLS_CONF_OPT)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/host-energyawaretools.compile:
	@$(call targetinfo)
#	void
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/host-energyawaretools.install:
	@$(call targetinfo)
	for f in eACommander nandflashloader.bin; do \
		install -D "$(HOST_ENERGYAWARETOOLS_DIR)/bin/$$f" "$(HOST_ENERGYAWARETOOLS_PKGDIR)/lib/energyAwareTools/$$f"; \
	done

	for f in libjlinkarm.so.4; do \
		install -D "$(HOST_ENERGYAWARETOOLS_DIR)/lib/$$f" "$(HOST_ENERGYAWARETOOLS_PKGDIR)/lib/energyAwareTools/$$f"; \
	done

	install -D "$(HOST_ENERGYAWARETOOLS_DIR)/eACommander.sh" "$(HOST_ENERGYAWARETOOLS_PKGDIR)/bin/eACommander"

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/host-energyawaretools.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, HOST_ENERGYAWARETOOLS)

# vim: syntax=make
