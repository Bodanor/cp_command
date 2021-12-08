all:
	+$(MAKE) -C cp
	+$(MAKE) -C cp-dev-gui

cp-cli:
	+$(MAKE) -C cp

cp-gui:
	+$(MAKE) -C cp-dev-gui