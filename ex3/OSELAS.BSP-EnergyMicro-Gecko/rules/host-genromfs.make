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
HOST_PACKAGES-$(PTXCONF_HOST_GENROMFS) += host-genromfs

#
# Paths and names
#
HOST_GENROMFS_VERSION	:= 0.5.2
HOST_GENROMFS_MD5	:= 2a91463c56f9e042edc330c063a0cf5a
HOST_GENROMFS		:= genromfs-$(HOST_GENROMFS_VERSION)
HOST_GENROMFS_SUFFIX	:= tar.gz
HOST_GENROMFS_URL	:= $(PTXCONF_SETUP_SFMIRROR)/romfs/$(HOST_GENROMFS).$(HOST_GENROMFS_SUFFIX)
HOST_GENROMFS_SOURCE	:= $(SRCDIR)/$(HOST_GENROMFS).$(HOST_GENROMFS_SUFFIX)
HOST_GENROMFS_DIR	:= $(HOST_BUILDDIR)/$(HOST_GENROMFS)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

HOST_GENROMFS_CONF_TOOL := NO
HOST_GENROMFS_MAKE_OPT := $(HOST_ENV_CC)
HOST_GENROMFS_INSTALL_OPT := prefix= PREFIX=$(HOST_GENROMFS_PKGDIR) install

# vim: syntax=make
