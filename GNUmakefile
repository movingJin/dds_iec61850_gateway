# -*- makefile -*-
#----------------------------------------------------------------------------
#       GNU ACE Workspace
#
# $Id: GNUACEWorkspaceCreator.pm 2928 2017-10-13 20:27:16Z mitza $
#
# This file was generated by MPC.  Any changes made directly to
# this file will be lost the next time it is generated.
#
# MPC Command:
# /home/ngn/OpenDDS-3.13/ACE_wrappers/bin/mwc.pl -type gnuace /home/ngn/OpenDDS-3.13/DDS_TAOv2_all.mwc
#
#----------------------------------------------------------------------------

MAKEFILE = GNUmakefile

ifeq ($(findstring k,$(MAKEFLAGS)),k)
  KEEP_GOING = -
endif

include $(ACE_ROOT)/include/makeinclude/macros.GNU

all: DDS_Messenger_Idl DDS_Client DDS_Server

depend: DDS_Messenger_Idl-depend DDS_Client-depend DDS_Server-depend

REMAINING_TARGETS := $(filter-out all depend,$(TARGETS_NESTED:.nested=)) $(CUSTOM_TARGETS)
.PHONY: $(REMAINING_TARGETS)

$(REMAINING_TARGETS):
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Messenger_Idl $(@)
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Client $(@)
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Server $(@)

.PHONY: DDS_Messenger_Idl
DDS_Messenger_Idl:
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Messenger_Idl all

.PHONY: DDS_Messenger_Idl-depend
DDS_Messenger_Idl-depend:
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Messenger_Idl depend

.PHONY: DDS_Client
DDS_Client: DDS_Messenger_Idl
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Client all

.PHONY: DDS_Client-depend
DDS_Client-depend:
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Client depend

.PHONY: DDS_Server
DDS_Server: DDS_Messenger_Idl 
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Server all

.PHONY: DDS_Server-depend
DDS_Server-depend:
	$(KEEP_GOING)@$(MAKE) -f GNUmakefile.DDS_Server depend

project_name_list:
	@echo DDS_Messenger_Idl
	@echo DDS_Client
	@echo DDS_Server
