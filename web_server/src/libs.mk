SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

config/libconfig.a:
	$(MAKE) -C $(SRC_DIR)config

daemon/libdaemon.a:
	$(MAKE) -C $(SRC_DIR)daemon

server/libserver.a:
	$(MAKE) -C $(SRC_DIR)server

