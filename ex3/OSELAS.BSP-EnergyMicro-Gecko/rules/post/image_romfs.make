# -*-makefile-*-
#
# Copyright (C) 2003-2010 by the ptxdist project <ptxdist@pengutronix.de>
#               2011 by Marc Kleine-Budde <mkl@pengutronix.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

ifdef PTXCONF_IMAGE_ROMFS
images: $(IMAGEDIR)/root.romfs
endif

$(IMAGEDIR)/root.romfs: $(STATEDIR)/image_working_dir $(STATEDIR)/host-genromfs.install.post
	@echo -n "Creating $(notdir $(@)) from $(notdir $(<))..."
	@cd $(image/work_dir);						\
	(awk -F: $(DOPERMISSIONS) $(image/permissions) &&		\
	(								\
		echo -n "genromfs ";					\
		echo -n $(PTXCONF_IMAGE_ROMFS_EXTRA_ARGS);		\
		echo -n "-d $(image/work_dir) ";			\
		echo -n "-f $@" )					\
	) | $(FAKEROOT) --
	@echo "done."

# vim: syntax=make
