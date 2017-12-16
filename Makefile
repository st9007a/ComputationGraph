TOPTARGETS = all clean
SUBDIRS = src tests

.PHONY: $(TOPTARGETS) $(SUBDIRS) check
all: $(SUBDIRS)

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

check:
	@$(MAKE) -C test check
