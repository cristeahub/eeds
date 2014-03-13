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
PACKAGES-$(PTXCONF_GAME) += game

#
# Paths and names
#
GAME_VERSION	:= 1.0
GAME_MD5		:=
GAME		:= game
GAME_URL		:= file://local_src/$(GAME)-$(GAME_VERSION)
GAME_DIR		:= $(BUILDDIR)/$(GAME)
GAME_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/game.extract:
#	@$(call targetinfo)
#	@$(call clean, $(GAME_DIR))
#	@$(call extract, GAME)
#	@$(call patchin, GAME)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#GAME_PATH	:= PATH=$(CROSS_PATH)
GAME_CONF_TOOL	:= NO
GAME_MAKE_ENV	:= $(CROSS_ENV)

#$(STATEDIR)/game.prepare:
#	@$(call targetinfo)
#	@$(call world/prepare, GAME)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/game.compile:
#	@$(call targetinfo)
#	@$(call world/compile, GAME)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/game.install:
#	@$(call targetinfo)
#	@$(call world/install, GAME)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/game.targetinstall:
	@$(call targetinfo)

	@$(call install_init, game)
	@$(call install_fixup, game,PRIORITY,optional)
	@$(call install_fixup, game,SECTION,base)
	@$(call install_fixup, game,AUTHOR,"")
	@$(call install_fixup, game,DESCRIPTION,missing)

#
# TODO: Add here all files that should be copied to the target
# Note: Add everything before(!) call to macro install_finish
#
	@$(call install_copy, game, 0, 0, 0755, $(GAME_DIR)/game, /usr/bin/game)

	@$(call install_finish, game)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/game.clean:
	@$(call targetinfo)
	@-cd $(GAME_DIR) && \
		$(GAME_ENV) $(GAME_PATH) $(MAKE) clean
	@$(call clean_pkg, GAME)

# vim: syntax=make
