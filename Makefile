UTILS_DIR:=utils
SRC_DIR:=src

all :
	@/bin/mkdir -p build
	$(MAKE) -C $(SRC_DIR)
	$(MAKE) -C $(UTILS_DIR)

clean:
	$(MAKE) -C $(UTILS_DIR) clean
	$(MAKE) -C $(SRC_DIR) clean
	@/bin/rm -rf build
