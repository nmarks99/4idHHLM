all:
	export CAQTDM_DISPLAY_PATH=$(CAQTDM_DISPLAY_PATH):/APSshare/uisys/sr/id
	/APSshare/bin/adl2ui hhlm.adl
	/APSshare/bin/adl2ui convex_plot.adl
	/APSshare/bin/adl2ui concave_plot.adl
	mv hhlm.ui ../ui/autoconvert/
	mv convex_plot.ui ../ui/autoconvert/
	mv concave_plot.ui ../ui/autoconvert/
clean:
	rm ../ui/autoconvert/hhlm.ui
	rm ../ui/autoconvert/convex_plot.ui
	rm ../ui/autoconvert/concave_plot.ui
	
# CAQTDM_DISPLAY_PATH=$CAQTDM_DISPLAY_PATH:/APSshare/uisys/sr/id
