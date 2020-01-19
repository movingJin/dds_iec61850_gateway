
SRCDIR = $(DDS_ROOT)/../mvl/usr/client
MMSOPDIR = $(DDS_ROOT)/../mvl/usr/client
MMSSRCDIR = $(DDS_ROOT)/../src
MVLSRCDIR = $(DDS_ROOT)/../mvl/src
FOUNDRYDIR = $(DDS_ROOT)/../bin
USRSRCDIR = $(DDS_ROOT)/../mvl/usr
ACSESRCDIR = $(DDS_ROOT)/../mvl/src/acse
INCDIR = $(DDS_ROOT)/../inc

CPPFLAGS     += \
	-DDEBUG_SISCO -DMMS_LITE -DMOSI -DLEAN_T \
	-DGOOSE_RX_SUPP -DGSSE_RX_SUPP -DGSE_MGMT_RX_SUPP -DSMPVAL_RX_SUPP -DSUBNET_THREADS \
	-DMLOG_ENABLE \
	-DUSE_DIB_LIST \
	-D_THREAD_SAFE

CPPFLAGS += -I$(INCDIR) -I$(SRCDIR) 
LDFLAGS  += -L../../lib
LDLIBS = \
	 -l:libcositpxs0_ld.a \
	 -l:smpval_ld.a \
	 -l:mvlu_ld.a \
	 -l:mlogl_ld.a \
	 -l:mmsle_ld.a \
	 -l:mmsl_ld.a \
	 -l:asn1l_ld.a \
	 -l:gse_mgmt_ld.a \
	 -l:mem_ld.a \
	 -l:slog_ld.a \
	 -l:util_ld.a \
	 -l:ositpxs_ld.a

VPATH = $(SRCDIR) $(MVLSRCDIR) $(MVLSRCDIR)/acse $(MMSSRCDIR) $(USRSRCDIR) \
	$(INCDIR)

FILES = \
	MMS_client.c \
	cli_rpt.c \
	mmsop_en.c \
	mvlop_en.c \
	mvl_acse.c \
	event2.c \
	logcfgx.c \
	clntobj.c \
	reject.c \
	$(MMSSRCDIR)/slogipc.c \
	subnet.c \
	dib_list.c \
	scl_dib.c

